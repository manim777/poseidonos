/*
 *   BSD LICENSE
 *   Copyright (c) 2021 Samsung Electronics Corporation
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *     * Neither the name of Samsung Electronics Corporation nor the names of
 *       its contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "metafs_io_scheduler.h"

#include <string>
#include <thread>

#include "src/metafs/include/metafs_aiocb_cxt.h"
#include "src/metafs/lib/metafs_time_interval.h"
#include "src/metafs/mim/scalable_meta_io_worker.h"
#include "src/metafs/mvm/meta_volume_manager.h"
#include "src/telemetry/telemetry_client/telemetry_publisher.h"

namespace pos
{
MetaFsIoScheduler::MetaFsIoScheduler(const int threadId, const int coreId,
    const int totalCoreCount, const std::string& threadName,
    const cpu_set_t mioCoreSet, MetaFsConfigManager* config,
    TelemetryPublisher* tp, MetaFsTimeInterval* timeInterval)
: MetaFsIoHandlerBase(threadId, coreId, threadName),
  TOTAL_CORE_COUNT(totalCoreCount),
  MIO_CORE_SET(mioCoreSet),
  mioCoreCount_(CPU_COUNT(&mioCoreSet)),
  config_(config),
  tp_(tp),
  cpuStallCnt_(0),
  timeInterval_(timeInterval),
  currentReqMsg_(nullptr),
  chunkSize_(0),
  fileBaseLpn_(0),
  startLpn_(0),
  currentLpn_(0),
  requestCount_(0),
  remainCount_(0),
  extentsCount_(0),
  currentExtent_(0),
  extents_(nullptr),
  issueCount_(),
  metricNameForStorage_()
{
    metricNameForStorage_[0] = TEL40100_METAFS_SCHEDULER_ISSUE_COUNT_TO_SSD;
    metricNameForStorage_[1] = TEL40101_METAFS_SCHEDULER_ISSUE_COUNT_TO_NVRAM;
    metricNameForStorage_[2] = TEL40102_METAFS_SCHEDULER_ISSUE_COUNT_TO_JOURNAL_SSD;
}

MetaFsIoScheduler::~MetaFsIoScheduler(void)
{
    threadExit_ = true;

    if (timeInterval_)
    {
        delete timeInterval_;
        timeInterval_ = nullptr;
    }
}

void
MetaFsIoScheduler::ExitThread(void)
{
    for (auto metaIoWorker : metaIoWorkerList_)
    {
        POS_TRACE_INFO((int)POS_EVENT_ID::MFS_INFO_MESSAGE,
            "Exit MioHandler, " + metaIoWorker->GetLogString());

        metaIoWorker->ExitThread();
        delete metaIoWorker;
    }
    metaIoWorkerList_.clear();

    MetaFsIoHandlerBase::ExitThread();

    POS_TRACE_INFO((int)POS_EVENT_ID::MFS_INFO_MESSAGE,
        "Exit MetaIoScheduler, " + GetLogString());
}

void
MetaFsIoScheduler::_SetRequestCountOrCallbackCountOfCurrentRequest(int count)
{
    if (MetaIoMode::Async == currentReqMsg_->ioMode)
    {
        ((MetaFsAioCbCxt*)currentReqMsg_->aiocb)->SetCallbackCount(count);
    }
    else
    {
        currentReqMsg_->originalMsg->requestCount = count;
    }
}

void
MetaFsIoScheduler::_SetCurrentContextFrom(MetaFsIoRequest* reqMsg)
{
    currentReqMsg_ = reqMsg;

    chunkSize_ = currentReqMsg_->fileCtx->chunkSize;
    fileBaseLpn_ = currentReqMsg_->fileCtx->fileBaseLpn;
    startLpn_ = reqMsg->GetStartLpn();
    currentLpn_ = startLpn_;
    requestCount_ = reqMsg->GetRequestLpnCount();
    remainCount_ = requestCount_;
    extentsCount_ = currentReqMsg_->fileCtx->extentsCount;
    extents_ = currentReqMsg_->fileCtx->extents;

    // set the request count to process the callback count
    _SetRequestCountOrCallbackCountOfCurrentRequest(requestCount_);
}

void
MetaFsIoScheduler::_ClearCurrentContext(void)
{
    currentReqMsg_ = nullptr;
    chunkSize_ = 0;
    fileBaseLpn_ = 0;
    startLpn_ = 0;
    currentLpn_ = 0;
    requestCount_ = 0;
    remainCount_ = 0;
    extentsCount_ = 0;
    currentExtent_ = 0;
    extents_ = nullptr;
}

void
MetaFsIoScheduler::_UpdateCurrentLpnToNextExtent(void)
{
    if (currentLpn_ > extents_[currentExtent_].GetLast())
    {
        ++currentExtent_;
        currentLpn_ = extents_[currentExtent_].GetStartLpn();
    }
}

void
MetaFsIoScheduler::_UpdateCurrentLpnToNextExtentConditionally(void)
{
    while (currentExtent_ < extentsCount_)
    {
        if (currentLpn_ <= extents_[currentExtent_].GetLast())
        {
            break;
        }
        ++currentExtent_;
    }
}

const int64_t*
MetaFsIoScheduler::GetIssueCount(size_t& size /* output */) const
{
    size = NUM_STORAGE;
    return issueCount_;
}

void
MetaFsIoScheduler::IssueRequestAndDelete(MetaFsIoRequest* reqMsg)
{
    uint64_t byteOffset = 0;
    bool isFirstLpn = true;

    _SetCurrentContextFrom(reqMsg);
    _UpdateCurrentLpnToNextExtentConditionally();

    issueCount_[(int)currentReqMsg_->targetMediaType] += requestCount_;

    while (remainCount_)
    {
        // reqMsg     : only for meta scheduler, not meta handler thread
        // cloneReqMsg: new copy, sent to meta handler thread by scheduler
        // reqMsg->originalMsg: from a user thread
        MetaFsIoRequest* cloneReqMsg = new MetaFsIoRequest(*currentReqMsg_);

        // 1st
        if (isFirstLpn)
        {
            isFirstLpn = false;
            cloneReqMsg->buf = currentReqMsg_->buf;
            cloneReqMsg->byteOffsetInFile = currentReqMsg_->byteOffsetInFile;
            if (chunkSize_ < (currentReqMsg_->byteSize + (currentReqMsg_->byteOffsetInFile % chunkSize_)))
            {
                cloneReqMsg->byteSize = chunkSize_ - (currentReqMsg_->byteOffsetInFile % chunkSize_);
            }
            else
            {
                cloneReqMsg->byteSize = currentReqMsg_->byteSize;
            }
        }
        // last
        else if (remainCount_ == 1)
        {
            cloneReqMsg->buf = (FileBufType)((uint64_t)currentReqMsg_->buf + byteOffset);
            cloneReqMsg->byteOffsetInFile = currentReqMsg_->byteOffsetInFile + byteOffset;
            cloneReqMsg->byteSize = currentReqMsg_->byteSize - byteOffset;
        }
        else
        {
            cloneReqMsg->buf = (FileBufType)((uint64_t)currentReqMsg_->buf + byteOffset);
            cloneReqMsg->byteOffsetInFile = currentReqMsg_->byteOffsetInFile + byteOffset;
            cloneReqMsg->byteSize = chunkSize_;
        }

        byteOffset += cloneReqMsg->byteSize;
        cloneReqMsg->baseMetaLpn = currentLpn_;

        metaIoWorkerList_[currentLpn_ % mioCoreCount_]->EnqueueNewReq(cloneReqMsg);

        ++currentLpn_;
        --remainCount_;
        _UpdateCurrentLpnToNextExtent();
    }

    // delete msg instance, this instance was only for meta scheduler
    delete reqMsg;

    _ClearCurrentContext();
}

void
MetaFsIoScheduler::EnqueueNewReq(MetaFsIoRequest* reqMsg)
{
    ioMultiQ_.Enqueue(reqMsg, reqMsg->priority);
}

bool
MetaFsIoScheduler::AddArrayInfo(const int arrayId, const MaxMetaLpnMapPerMetaStorage& map)
{
    bool result = true;

    for (auto metaIoWorker : metaIoWorkerList_)
    {
        if (!metaIoWorker->AddArrayInfo(arrayId, map))
        {
            result = false;
            break;
        }
    }

    return result;
}

bool
MetaFsIoScheduler::RemoveArrayInfo(const int arrayId)
{
    bool result = true;

    for (auto metaIoWorker : metaIoWorkerList_)
    {
        if (!metaIoWorker->RemoveArrayInfo(arrayId))
        {
            result = false;
            break;
        }
    }

    return result;
}

void
MetaFsIoScheduler::StartThread(void)
{
    th_ = new std::thread(AsEntryPointNoParam(&MetaFsIoScheduler::Execute, this));

    POS_TRACE_INFO((int)POS_EVENT_ID::MFS_INFO_MESSAGE,
        "Start MetaIoScheduler, " + GetLogString());

    _CreateMioThread();
}

void
MetaFsIoScheduler::_CreateMioThread(void)
{
    const std::string fileName = "MioHandler";
    uint32_t handlerId = 0;
    int availableMioCoreCnt = CPU_COUNT(&MIO_CORE_SET);
    for (uint32_t coreId = 0; coreId < TOTAL_CORE_COUNT; ++coreId)
    {
        if (CPU_ISSET(coreId, &MIO_CORE_SET))
        {
            ScalableMetaIoWorker* mioHandler =
                new ScalableMetaIoWorker(handlerId++, coreId, fileName, config_, nullptr);
            mioHandler->StartThread();
            metaIoWorkerList_.emplace_back(mioHandler);
            availableMioCoreCnt--;

            POS_TRACE_INFO((int)POS_EVENT_ID::MFS_INFO_MESSAGE,
                "Create MioHandler, " + mioHandler->GetLogString());

            if (availableMioCoreCnt == 0)
            {
                break;
            }
        }
    }

    if (availableMioCoreCnt)
    {
        POS_TRACE_ERROR((int)POS_EVENT_ID::MFS_ERROR_MESSAGE,
            "The Count of created MioHandler: {}, expected count: {}",
            mioCoreCount_ - availableMioCoreCnt, mioCoreCount_);
    }
}

void
MetaFsIoScheduler::RegisterMetaIoWorkerForTest(ScalableMetaIoWorker* metaIoWorker)
{
    metaIoWorkerList_.emplace_back(metaIoWorker);
    mioCoreCount_ = metaIoWorkerList_.size();
}

void
MetaFsIoScheduler::Execute(void)
{
    PrepareThread();

    while (!threadExit_)
    {
        if (tp_ && timeInterval_ && timeInterval_->CheckInterval())
        {
            POSMetricVector* metricList = new POSMetricVector();
            for (uint32_t idx = 0; idx < NUM_STORAGE; ++idx)
            {
                POSMetric v(metricNameForStorage_[idx], POSMetricTypes::MT_GAUGE);
                v.SetGaugeValue(issueCount_[idx]);
                metricList->emplace_back(v);
                issueCount_[idx] = 0;
            }
            tp_->PublishMetricList(metricList);
        }

        MetaFsIoRequest* reqMsg = _FetchPendingNewReq();

        if (!reqMsg)
        {
            if (cpuStallCnt_++ > MAX_CPU_STALL_COUNT)
            {
                usleep(1);
                cpuStallCnt_ = 0;
            }
            continue;
        }
        cpuStallCnt_ = 0;

        IssueRequestAndDelete(reqMsg);
    }
}

MetaFsIoRequest*
MetaFsIoScheduler::_FetchPendingNewReq(void)
{
    return ioMultiQ_.Dequeue();
}
} // namespace pos
