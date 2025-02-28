#include <iostream>
#include <memory>
#include <string>

#include "src/cli/grpc_cli_server.h"

#define MAX_NUM_CONCURRENT_CLIENTS 1

CommandProcessor* pc;

class PosCliServiceImpl final : public PosCli::Service {
  grpc::Status
  SystemInfo(ServerContext* context, const SystemInfoRequest* request,
                  SystemInfoResponse* reply) override
  {
    _LogCliRequest(request);

    grpc::Status status = pc->ExecuteSystemInfoCommand(request, reply);
    if (context->IsCancelled()) {
      _LogGrpcTimeout(request, reply);
      return Status(StatusCode::CANCELLED, GRPC_TIMEOUT_MESSAGE);
    }
    _LogCliResponse(reply, status);
    
    return status;
  }

  grpc::Status
  SystemStop(ServerContext* context, const SystemStopRequest* request,
                  SystemStopResponse* reply) override
  {
    _LogCliRequest(request);

    grpc::Status status = pc->ExecuteSystemStopCommand(request, reply);
    if (context->IsCancelled()) {
      _LogGrpcTimeout(request, reply);
      return Status(StatusCode::CANCELLED, GRPC_TIMEOUT_MESSAGE);
    }

    _LogCliResponse(reply, status);

    return status;
  }

  grpc::Status
  GetSystemProperty(ServerContext* context, const GetSystemPropertyRequest* request,
                  GetSystemPropertyResponse* reply) override
  {
    _LogCliRequest(request);

    grpc::Status status = pc->ExecuteGetSystemPropertyCommand(request, reply);
    if (context->IsCancelled()) {
      _LogGrpcTimeout(request, reply);
      return Status(StatusCode::CANCELLED, GRPC_TIMEOUT_MESSAGE);
    }

    _LogCliResponse(reply, status);

    return status;
  }

  grpc::Status
  SetSystemProperty(ServerContext* context, const SetSystemPropertyRequest* request,
                  SetSystemPropertyResponse* reply) override
  {
    _LogCliRequest(request);

    grpc::Status status = pc->ExecuteSetSystemPropertyCommand(request, reply);
    if (context->IsCancelled()) {
      _LogGrpcTimeout(request, reply);
      return Status(StatusCode::CANCELLED, GRPC_TIMEOUT_MESSAGE);
    }

    _LogCliResponse(reply, status);

    return status;
  }

  grpc::Status
  StartTelemetry(ServerContext* context, const StartTelemetryRequest* request,
                  StartTelemetryResponse* reply) override
  {
    _LogCliRequest(request);

    grpc::Status status = pc->ExecuteStartTelemetryCommand(request, reply);
    if (context->IsCancelled()) {
      _LogGrpcTimeout(request, reply);
      return Status(StatusCode::CANCELLED, GRPC_TIMEOUT_MESSAGE);
    }

    _LogCliResponse(reply, status);

    return status;
  }

  grpc::Status
  ResetEventWrr(ServerContext* context, const ResetEventWrrRequest* request,
                  ResetEventWrrResponse* reply) override
  {
    _LogCliRequest(request);

    grpc::Status status = pc->ExecuteResetEventWrrCommand(request, reply);
    if (context->IsCancelled()) {
      _LogGrpcTimeout(request, reply);
      return Status(StatusCode::CANCELLED, GRPC_TIMEOUT_MESSAGE);
    }

    _LogCliResponse(reply, status);
    
    return status;
  }

  grpc::Status
  ResetMbr(ServerContext* context, const ResetMbrRequest* request,
                  ResetMbrResponse* reply) override
  {
    _LogCliRequest(request);

    grpc::Status status = pc->ExecuteResetMbrCommand(request, reply);
    if (context->IsCancelled()) {
      _LogGrpcTimeout(request, reply);
      return Status(StatusCode::CANCELLED, GRPC_TIMEOUT_MESSAGE);
    }

    _LogCliResponse(reply, status);
    
    return status;
  }

  grpc::Status
  StopRebuilding(ServerContext* context, const StopRebuildingRequest* request,
                  StopRebuildingResponse* reply) override
  {
    _LogCliRequest(request);

    grpc::Status status = pc->ExecuteStopRebuildingCommand(request, reply);
    if (context->IsCancelled()) {
      _LogGrpcTimeout(request, reply);
      return Status(StatusCode::CANCELLED, GRPC_TIMEOUT_MESSAGE);
    }

    _LogCliResponse(reply, status);
    
    return status;
  }

  grpc::Status
  StopTelemetry(ServerContext* context, const StopTelemetryRequest* request,
                  StopTelemetryResponse* reply) override
  {
    _LogCliRequest(request);

    grpc::Status status = pc->ExecuteStopTelemetryCommand(request, reply);
    if (context->IsCancelled()) {
      _LogGrpcTimeout(request, reply);
      return Status(StatusCode::CANCELLED, GRPC_TIMEOUT_MESSAGE);
    }

    _LogCliResponse(reply, status);

    return status;
  }

  grpc::Status
  UpdateEventWrr(ServerContext* context, const UpdateEventWrrRequest* request,
                  UpdateEventWrrResponse* reply) override
  {
    _LogCliRequest(request);

    grpc::Status status = pc->ExecuteUpdateEventWrrCommand(request, reply);
    if (context->IsCancelled()) {
      _LogGrpcTimeout(request, reply);
      return Status(StatusCode::CANCELLED, GRPC_TIMEOUT_MESSAGE);
    }

    _LogCliResponse(reply, status);

    return status;
  }

  grpc::Status
  AddSpare(ServerContext* context, const AddSpareRequest* request,
                  AddSpareResponse* reply) override
  {
    _LogCliRequest(request);

    grpc::Status status = pc->ExecuteAddSpareCommand(request, reply);
    if (context->IsCancelled()) {
      _LogGrpcTimeout(request, reply);
      return Status(StatusCode::CANCELLED, GRPC_TIMEOUT_MESSAGE);
    }

    _LogCliResponse(reply, status);

    return status;
  }

  grpc::Status
  RemoveSpare(ServerContext* context, const RemoveSpareRequest* request,
                  RemoveSpareResponse* reply) override
  {
    _LogCliRequest(request);

    grpc::Status status = pc->ExecuteRemoveSpareCommand(request, reply);
    if (context->IsCancelled()) {
      _LogGrpcTimeout(request, reply);
      return Status(StatusCode::CANCELLED, GRPC_TIMEOUT_MESSAGE);
    }

    _LogCliResponse(reply, status);

    return status;
  }

  grpc::Status
  CreateArray(ServerContext* context, const CreateArrayRequest* request,
                  CreateArrayResponse* reply) override
  {
    _LogCliRequest(request);

    grpc::Status status = pc->ExecuteCreateArrayCommand(request, reply);
    if (context->IsCancelled()) {
      _LogGrpcTimeout(request, reply);
      return Status(StatusCode::CANCELLED, GRPC_TIMEOUT_MESSAGE);
    }

    _LogCliResponse(reply, status);

    return status;
  }

  grpc::Status
  AutocreateArray(ServerContext* context, const AutocreateArrayRequest* request,
                  AutocreateArrayResponse* reply) override
  {
    POS_TRACE_INFO(EID(CLI_MSG_RECEIVED), "message: {}", request->ShortDebugString());

    grpc::Status status = pc->ExecuteAutocreateArrayCommand(request, reply);
    
    POS_TRACE_INFO(EID(CLI_MSG_SENT), "message: {}", reply->ShortDebugString());

    return status;
  }

  grpc::Status
  DeleteArray(ServerContext* context, const DeleteArrayRequest* request,
                  DeleteArrayResponse* reply) override
  {
    _LogCliRequest(request);

    grpc::Status status = pc->ExecuteDeleteArrayCommand(request, reply);
    if (context->IsCancelled()) {
      _LogGrpcTimeout(request, reply);
      return Status(StatusCode::CANCELLED, GRPC_TIMEOUT_MESSAGE);
    }

    _LogCliResponse(reply, status);

    return status;
  }

  grpc::Status
  MountArray(ServerContext* context, const MountArrayRequest* request,
                  MountArrayResponse* reply) override
  {
    _LogCliRequest(request);

    grpc::Status status = pc->ExecuteMountArrayCommand(request, reply);
    if (context->IsCancelled()) {
      _LogGrpcTimeout(request, reply);
      return Status(StatusCode::CANCELLED, GRPC_TIMEOUT_MESSAGE);
    }

    _LogCliResponse(reply, status);

    return status;
  }

  grpc::Status
  UnmountArray(ServerContext* context, const UnmountArrayRequest* request,
                  UnmountArrayResponse* reply) override
  {
    _LogCliRequest(request);

    grpc::Status status = pc->ExecuteUnmountArrayCommand(request, reply);
    if (context->IsCancelled()) {
      _LogGrpcTimeout(request, reply);
      return Status(StatusCode::CANCELLED, GRPC_TIMEOUT_MESSAGE);
    }

    _LogCliResponse(reply, status);
    
    return status;
  }

  grpc::Status
  ListArray(ServerContext* context, const ListArrayRequest* request,
                  ListArrayResponse* reply) override
  {
    _LogCliRequest(request);

    grpc::Status status = pc->ExecuteListArrayCommand(request, reply);
    if (context->IsCancelled()) {
      _LogGrpcTimeout(request, reply);
      return Status(StatusCode::CANCELLED, GRPC_TIMEOUT_MESSAGE);
    }
    
    _LogCliResponse(reply, status);

    return status;
  }

  grpc::Status
  ArrayInfo(ServerContext* context, const ArrayInfoRequest* request,
                  ArrayInfoResponse* reply) override
  {
    _LogCliRequest(request);

    grpc::Status status = pc->ExecuteArrayInfoCommand(request, reply);
    if (context->IsCancelled()) {
      _LogGrpcTimeout(request, reply);
      return Status(StatusCode::CANCELLED, GRPC_TIMEOUT_MESSAGE);
    }
    
    _LogCliResponse(reply, status);

    return status;
  }

  grpc::Status
  SetLogLevel(ServerContext* context, const SetLogLevelRequest* request,
                  SetLogLevelResponse* reply) override
  {
    _LogCliRequest(request);

    grpc::Status status = pc->ExecuteSetLogLevelCommand(request, reply);
    if (context->IsCancelled()) {
      _LogGrpcTimeout(request, reply);
      return Status(StatusCode::CANCELLED, GRPC_TIMEOUT_MESSAGE);
    }
    
    _LogCliResponse(reply, status);

    return status;
  }

  grpc::Status
  SetLogPreference(ServerContext* context, const SetLogPreferenceRequest* request,
                  SetLogPreferenceResponse* reply) override
  {
    _LogCliRequest(request);

    grpc::Status status = pc->ExecuteSetLogPreferenceCommand(request, reply);
    if (context->IsCancelled()) {
      _LogGrpcTimeout(request, reply);
      return Status(StatusCode::CANCELLED, GRPC_TIMEOUT_MESSAGE);
    }
    
    _LogCliResponse(reply, status);

    return status;
  }

  grpc::Status
  LoggerInfo(ServerContext* context, const LoggerInfoRequest* request,
                  LoggerInfoResponse* reply) override
  {
    _LogCliRequest(request);

    grpc::Status status = pc->ExecuteLoggerInfoCommand(request, reply);
    if (context->IsCancelled()) {
      _LogGrpcTimeout(request, reply);
      return Status(StatusCode::CANCELLED, GRPC_TIMEOUT_MESSAGE);
    }
    
    _LogCliResponse(reply, status);

    return status;
  }

  grpc::Status
  GetLogLevel(ServerContext* context, const GetLogLevelRequest* request,
                  GetLogLevelResponse* reply) override
  {
    _LogCliRequest(request);

    grpc::Status status = pc->ExecuteGetLogLevelCommand(request, reply);
    if (context->IsCancelled()) {
      _LogGrpcTimeout(request, reply);
      return Status(StatusCode::CANCELLED, GRPC_TIMEOUT_MESSAGE);
    }
    
    _LogCliResponse(reply, status);

    return status;
  }

  grpc::Status
  ApplyLogFilter(ServerContext* context, const ApplyLogFilterRequest* request,
                  ApplyLogFilterResponse* reply) override
  {
    _LogCliRequest(request);

    grpc::Status status = pc->ExecuteApplyLogFilterCommand(request, reply);
    if (context->IsCancelled()) {
      _LogGrpcTimeout(request, reply);
      return Status(StatusCode::CANCELLED, GRPC_TIMEOUT_MESSAGE);
    }
    
    _LogCliResponse(reply, status);

    return status;
  }
};

void
_LogGrpcTimeout(const google::protobuf::Message* request, const google::protobuf::Message* reply)
{
  POS_TRACE_INFO(EID(CLI_TIMEOUT_OR_CANCELLED), "request: {}, reply: {}",
  request->ShortDebugString(), reply->ShortDebugString());
}

void
_LogCliRequest(const google::protobuf::Message* request)
{
  POS_TRACE_INFO(EID(CLI_MSG_RECEIVED), "request: {}", request->ShortDebugString());
}

void
_LogCliResponse(const google::protobuf::Message* reply, const grpc::Status status)
{
  POS_TRACE_INFO(EID(CLI_MSG_SENT), "response: {}, gRPC_error_code: {}, gRPC_error_details: {}, gRPC_error_essage: {}",
      reply->ShortDebugString(), status.error_code(), status.error_details(), status.error_message());
}

void
RunGrpcServer()
{
  pc = new CommandProcessor();

  std::string server_address(GRPC_CLI_SERVER_SOCKET_ADDRESS);
  PosCliServiceImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  grpc::ResourceQuota rq;
  
  rq.SetMaxThreads(MAX_NUM_CONCURRENT_CLIENTS + 1);
  builder.SetResourceQuota(rq);
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}