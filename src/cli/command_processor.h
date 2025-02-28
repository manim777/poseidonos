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

#pragma once

#include "src/include/backend_event.h"
#include "proto/generated/cpp/cli.grpc.pb.h"
#include "proto/generated/cpp/cli.pb.h"
#include <string>

#define RESET_EVENT_WRR_DEFAULT_WEIGHT 20
#define DEFAULT_ARRAY_STATUS "Unmounted"
#define ARRAY_ERROR_INDEX -1

namespace pos
{
class IGCInfo;
} // namespace pos

using grpc::Status;
using grpc::StatusCode;
using grpc_cli::PosInfo;
using grpc_cli::SystemInfoRequest;
using grpc_cli::SystemInfoResponse;
using grpc_cli::SystemStopRequest;
using grpc_cli::SystemStopResponse;
using grpc_cli::GetSystemPropertyRequest;
using grpc_cli::GetSystemPropertyResponse;
using grpc_cli::SetSystemPropertyRequest;
using grpc_cli::SetSystemPropertyResponse;
using grpc_cli::StartTelemetryRequest;
using grpc_cli::StartTelemetryResponse;
using grpc_cli::StopTelemetryRequest;
using grpc_cli::StopTelemetryResponse;
using grpc_cli::ResetEventWrrRequest;
using grpc_cli::ResetEventWrrResponse;
using grpc_cli::ResetMbrRequest;
using grpc_cli::ResetMbrResponse;
using grpc_cli::StopRebuildingRequest;
using grpc_cli::StopRebuildingResponse;
using grpc_cli::UpdateEventWrrRequest;
using grpc_cli::UpdateEventWrrResponse;
using grpc_cli::AddSpareRequest;
using grpc_cli::AddSpareResponse;
using grpc_cli::RemoveSpareRequest;
using grpc_cli::RemoveSpareResponse;
using grpc_cli::CreateArrayRequest;
using grpc_cli::CreateArrayResponse;
using grpc_cli::AutocreateArrayRequest;
using grpc_cli::AutocreateArrayResponse;
using grpc_cli::DeleteArrayRequest;
using grpc_cli::DeleteArrayResponse;
using grpc_cli::MountArrayRequest;
using grpc_cli::MountArrayResponse;
using grpc_cli::UnmountArrayRequest;
using grpc_cli::UnmountArrayResponse;
using grpc_cli::ListArrayRequest;
using grpc_cli::ListArrayResponse;
using grpc_cli::ArrayInfoRequest;
using grpc_cli::ArrayInfoResponse;
using grpc_cli::SetLogLevelRequest;
using grpc_cli::SetLogLevelResponse;
using grpc_cli::SetLogPreferenceRequest;
using grpc_cli::SetLogPreferenceResponse;
using grpc_cli::LoggerInfoRequest;
using grpc_cli::LoggerInfoResponse;
using grpc_cli::GetLogLevelRequest;
using grpc_cli::GetLogLevelResponse;
using grpc_cli::ApplyLogFilterRequest;
using grpc_cli::ApplyLogFilterResponse;

class CommandProcessor
{
public:
    CommandProcessor(void);
    ~CommandProcessor(void);
    void FillHeader(const SystemInfoRequest* request, SystemInfoResponse* reply);

    // System Commands
    grpc::Status ExecuteSystemInfoCommand(const SystemInfoRequest* request, SystemInfoResponse* reply);
    grpc::Status ExecuteSystemStopCommand(const SystemStopRequest* request, SystemStopResponse* reply);
    grpc::Status ExecuteGetSystemPropertyCommand(const GetSystemPropertyRequest* request, GetSystemPropertyResponse* reply);
    grpc::Status ExecuteSetSystemPropertyCommand(const SetSystemPropertyRequest* request, SetSystemPropertyResponse* reply);

    // Telemetry Commands
    grpc::Status ExecuteStartTelemetryCommand(const StartTelemetryRequest* request, StartTelemetryResponse* reply);
    grpc::Status ExecuteStopTelemetryCommand(const StopTelemetryRequest* request, StopTelemetryResponse* reply);

    // Developer COmmands
    grpc::Status ExecuteResetEventWrrCommand(const ResetEventWrrRequest* request, ResetEventWrrResponse* reply);
    grpc::Status ExecuteResetMbrCommand(const ResetMbrRequest* request,ResetMbrResponse* reply);
    grpc::Status ExecuteStopRebuildingCommand(const StopRebuildingRequest* request, StopRebuildingResponse* reply);
    grpc::Status ExecuteUpdateEventWrrCommand(const UpdateEventWrrRequest* request, UpdateEventWrrResponse* reply);

    // Array Commands
    grpc::Status ExecuteAddSpareCommand(const AddSpareRequest* request, AddSpareResponse* reply);
    grpc::Status ExecuteRemoveSpareCommand(const RemoveSpareRequest* request, RemoveSpareResponse* reply);
    grpc::Status ExecuteCreateArrayCommand(const CreateArrayRequest* request, CreateArrayResponse* reply);
    grpc::Status ExecuteAutocreateArrayCommand(const AutocreateArrayRequest* request, AutocreateArrayResponse* reply);
    grpc::Status ExecuteDeleteArrayCommand(const DeleteArrayRequest* request, DeleteArrayResponse* reply);
    grpc::Status ExecuteMountArrayCommand(const MountArrayRequest* request, MountArrayResponse* reply);
    grpc::Status ExecuteUnmountArrayCommand(const UnmountArrayRequest* request, UnmountArrayResponse* reply);
    grpc::Status ExecuteListArrayCommand(const ListArrayRequest* request, ListArrayResponse* reply);
    grpc::Status ExecuteArrayInfoCommand(const ArrayInfoRequest* request, ArrayInfoResponse* reply);
    
    // Logger Commands
    grpc::Status ExecuteSetLogLevelCommand(const SetLogLevelRequest* request, SetLogLevelResponse* reply);
    grpc::Status ExecuteSetLogPreferenceCommand(const SetLogPreferenceRequest* request, SetLogPreferenceResponse* reply);
    grpc::Status ExecuteLoggerInfoCommand(const LoggerInfoRequest* request, LoggerInfoResponse* reply);
    grpc::Status ExecuteGetLogLevelCommand(const GetLogLevelRequest* request, GetLogLevelResponse* reply);
    grpc::Status ExecuteApplyLogFilterCommand(const ApplyLogFilterRequest* request, ApplyLogFilterResponse* reply);

private:
    bool _isPosTerminating;
    bool _IsPosTerminating(void) { return _isPosTerminating; }
    void _SetPosTerminating(bool input) { _isPosTerminating = input; }
    void _SetEventStatus(int eventId, grpc_cli::Status *status);
    void _SetPosInfo(grpc_cli::PosInfo *posInfo);
    std::string _GetRebuildImpactString(uint8_t impact);
    pos::BackendEvent _GetEventId(std::string eventName);
    std::string _GetGCMode(pos::IGCInfo* gc, std::string arrayName);
};
