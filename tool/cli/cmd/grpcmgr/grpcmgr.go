package grpcmgr

import (
	"cli/cmd/globals"
	"context"
	"pnconnector/src/log"
	"time"

	pb "cli/api"

	"google.golang.org/grpc"
)

const timeout = 30

func SendSystemInfoRpc(req *pb.SystemInfoRequest) (*pb.SystemInfoResponse, error) {
	conn, err := grpc.Dial(globals.GrpcServerAddress, grpc.WithInsecure(), grpc.WithBlock())
	if err != nil {
		log.Error("cannot send a request to cli server: not connected")
		return nil, err
	}
	defer conn.Close()

	c := pb.NewPosCliClient(conn)
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*timeout)
	defer cancel()

	res, err := c.SystemInfo(ctx, req)
	if err != nil {
		log.Error("error: ", err.Error())
		return nil, err
	}

	return res, err
}

func SendSystemStopRpc(req *pb.SystemStopRequest) (*pb.SystemStopResponse, error) {
	conn, err := grpc.Dial(globals.GrpcServerAddress, grpc.WithInsecure(), grpc.WithBlock())
	if err != nil {
		log.Error("cannot send a request to cli server: not connected")
		return nil, err
	}
	defer conn.Close()

	c := pb.NewPosCliClient(conn)
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*timeout)
	defer cancel()

	res, err := c.SystemStop(ctx, req)

	if err != nil {
		log.Error("error: ", err.Error())
		return nil, err
	}

	return res, err
}

func SendGetSystemPropertyRpc(req *pb.GetSystemPropertyRequest) (*pb.GetSystemPropertyResponse, error) {
	conn, err := grpc.Dial(globals.GrpcServerAddress, grpc.WithInsecure(), grpc.WithBlock())
	if err != nil {
		log.Error("cannot send a request to cli server: not connected")
		return nil, err
	}
	defer conn.Close()

	c := pb.NewPosCliClient(conn)
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*timeout)
	defer cancel()

	res, err := c.GetSystemProperty(ctx, req)

	if err != nil {
		log.Error("error: ", err.Error())
		return nil, err
	}

	return res, err
}

func SendSetSystemPropertyRpc(req *pb.SetSystemPropertyRequest) (*pb.SetSystemPropertyResponse, error) {
	conn, err := grpc.Dial(globals.GrpcServerAddress, grpc.WithInsecure(), grpc.WithBlock())
	if err != nil {
		log.Error("cannot send a request to cli server: not connected")
		return nil, err
	}
	defer conn.Close()

	c := pb.NewPosCliClient(conn)
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*timeout)
	defer cancel()

	res, err := c.SetSystemProperty(ctx, req)
	if err != nil {
		log.Error("error: ", err.Error())
		return nil, err
	}

	return res, err
}

func SendStartTelemetryRpc(req *pb.StartTelemetryRequest) (*pb.StartTelemetryResponse, error) {
	conn, err := grpc.Dial(globals.GrpcServerAddress, grpc.WithInsecure(), grpc.WithBlock())
	if err != nil {
		log.Error("cannot send a request to cli server: not connected")
		return nil, err
	}
	defer conn.Close()

	c := pb.NewPosCliClient(conn)
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*timeout)
	defer cancel()

	res, err := c.StartTelemetry(ctx, req)

	if err != nil {
		log.Error("error: ", err.Error())
		return nil, err
	}

	return res, err
}

func SendStopTelemetryRpc(req *pb.StopTelemetryRequest) (*pb.StopTelemetryResponse, error) {
	conn, err := grpc.Dial(globals.GrpcServerAddress, grpc.WithInsecure(), grpc.WithBlock())
	if err != nil {
		log.Error("cannot send a request to cli server: not connected")
		return nil, err
	}
	defer conn.Close()

	c := pb.NewPosCliClient(conn)
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*timeout)
	defer cancel()

	res, err := c.StopTelemetry(ctx, req)

	if err != nil {
		log.Error("error: ", err.Error())
		return nil, err
	}

	return res, err
}

func SendResetEventWrrPolicyRpc(req *pb.ResetEventWrrRequest) (*pb.ResetEventWrrResponse, error) {
	conn, err := grpc.Dial(globals.GrpcServerAddress, grpc.WithInsecure(), grpc.WithBlock())
	if err != nil {
		log.Error("cannot send a request to cli server: not connected")
		return nil, err
	}
	defer conn.Close()

	c := pb.NewPosCliClient(conn)
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*timeout)
	defer cancel()

	res, err := c.ResetEventWrr(ctx, req)
	if err != nil {
		log.Error("error: ", err.Error())
		return nil, err
	}

	return res, err
}

func SendResetMbrRpc(req *pb.ResetMbrRequest) (*pb.ResetMbrResponse, error) {
	conn, err := grpc.Dial(globals.GrpcServerAddress, grpc.WithInsecure(), grpc.WithBlock())
	if err != nil {
		log.Error("cannot send a request to cli server: not connected")
		return nil, err
	}
	defer conn.Close()

	c := pb.NewPosCliClient(conn)
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*timeout)
	defer cancel()

	res, err := c.ResetMbr(ctx, req)
	if err != nil {
		log.Error("error: ", err.Error())
		return nil, err
	}

	return res, err
}

func SendStopRebuildingRpc(req *pb.StopRebuildingRequest) (*pb.StopRebuildingResponse, error) {
	conn, err := grpc.Dial(globals.GrpcServerAddress, grpc.WithInsecure(), grpc.WithBlock())
	if err != nil {
		log.Error("cannot send a request to cli server: not connected")
		return nil, err
	}
	defer conn.Close()

	c := pb.NewPosCliClient(conn)
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*timeout)
	defer cancel()

	res, err := c.StopRebuilding(ctx, req)
	if err != nil {
		log.Error("error: ", err.Error())
		return nil, err
	}

	return res, err
}

func SendUpdatEventWrr(req *pb.UpdateEventWrrRequest) (*pb.UpdateEventWrrResponse, error) {
	conn, err := grpc.Dial(globals.GrpcServerAddress, grpc.WithInsecure(), grpc.WithBlock())
	if err != nil {
		log.Error("cannot send a request to cli server: not connected")
		return nil, err
	}
	defer conn.Close()

	c := pb.NewPosCliClient(conn)
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*timeout)
	defer cancel()

	res, err := c.UpdateEventWrr(ctx, req)
	if err != nil {
		log.Error("error: ", err.Error())
		return nil, err
	}

	return res, err
}

func SendAddSpare(req *pb.AddSpareRequest) (*pb.AddSpareResponse, error) {
	conn, err := grpc.Dial(globals.GrpcServerAddress, grpc.WithInsecure(), grpc.WithBlock())
	if err != nil {
		log.Error("cannot send a request to cli server: not connected")
		return nil, err
	}
	defer conn.Close()

	c := pb.NewPosCliClient(conn)
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*timeout)
	defer cancel()

	res, err := c.AddSpare(ctx, req)
	if err != nil {
		log.Error("error: ", err.Error())
		return nil, err
	}

	return res, err
}

func SendRemoveSpare(req *pb.RemoveSpareRequest) (*pb.RemoveSpareResponse, error) {
	conn, err := grpc.Dial(globals.GrpcServerAddress, grpc.WithInsecure(), grpc.WithBlock())
	if err != nil {
		log.Error("cannot send a request to cli server: not connected")
		return nil, err
	}
	defer conn.Close()

	c := pb.NewPosCliClient(conn)
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*timeout)
	defer cancel()

	res, err := c.RemoveSpare(ctx, req)
	if err != nil {
		log.Error("error: ", err.Error())
		return nil, err
	}

	return res, err
}

func SendCreateArray(req *pb.CreateArrayRequest) (*pb.CreateArrayResponse, error) {
	conn, err := grpc.Dial(globals.GrpcServerAddress, grpc.WithInsecure(), grpc.WithBlock())
	if err != nil {
		log.Error("cannot send a request to cli server: not connected")
		return nil, err
	}
	defer conn.Close()

	c := pb.NewPosCliClient(conn)
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*timeout)
	defer cancel()

	res, err := c.CreateArray(ctx, req)
	if err != nil {
		log.Error("error: ", err.Error())
		return nil, err
	}

	return res, err
}

func SendAutocreateArray(req *pb.AutocreateArrayRequest) (*pb.AutocreateArrayResponse, error) {
	conn, err := grpc.Dial(globals.GrpcServerAddress, grpc.WithInsecure(), grpc.WithBlock())
	if err != nil {
		log.Error("cannot send a request to cli server: not connected")
		return nil, err
	}
	defer conn.Close()

	c := pb.NewPosCliClient(conn)
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*timeout)
	defer cancel()

	res, err := c.AutocreateArray(ctx, req)
	if err != nil {
		log.Error("error: ", err.Error())
		return nil, err
	}

	return res, err
}

func SendDeleteArray(req *pb.DeleteArrayRequest) (*pb.DeleteArrayResponse, error) {
	conn, err := grpc.Dial(globals.GrpcServerAddress, grpc.WithInsecure(), grpc.WithBlock())
	if err != nil {
		log.Error("cannot send a request to cli server: not connected")
		return nil, err
	}
	defer conn.Close()

	c := pb.NewPosCliClient(conn)
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*timeout)
	defer cancel()

	res, err := c.DeleteArray(ctx, req)
	if err != nil {
		log.Error("error: ", err.Error())
		return nil, err
	}

	return res, err
}

func SendMountArray(req *pb.MountArrayRequest) (*pb.MountArrayResponse, error) {
	conn, err := grpc.Dial(globals.GrpcServerAddress, grpc.WithInsecure(), grpc.WithBlock())
	if err != nil {
		log.Error("cannot send a request to cli server: not connected")
		return nil, err
	}
	defer conn.Close()

	c := pb.NewPosCliClient(conn)
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*timeout)
	defer cancel()

	res, err := c.MountArray(ctx, req)
	if err != nil {
		log.Error("error: ", err.Error())
		return nil, err
	}

	return res, err
}

func SendUnmountArray(req *pb.UnmountArrayRequest) (*pb.UnmountArrayResponse, error) {
	conn, err := grpc.Dial(globals.GrpcServerAddress, grpc.WithInsecure(), grpc.WithBlock())
	if err != nil {
		log.Error("cannot send a request to cli server: not connected")
		return nil, err
	}
	defer conn.Close()

	c := pb.NewPosCliClient(conn)
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*timeout)
	defer cancel()

	res, err := c.UnmountArray(ctx, req)
	if err != nil {
		log.Error("error: ", err.Error())
		return nil, err
	}

	return res, err
}

func SendArrayInfo(req *pb.ArrayInfoRequest) (*pb.ArrayInfoResponse, error) {
	conn, err := grpc.Dial(globals.GrpcServerAddress, grpc.WithInsecure(), grpc.WithBlock())
	if err != nil {
		log.Error("cannot send a request to cli server: not connected")
		return nil, err
	}
	defer conn.Close()

	c := pb.NewPosCliClient(conn)
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*timeout)
	defer cancel()

	res, err := c.ArrayInfo(ctx, req)
	if err != nil {
		log.Error("error: ", err.Error())
		return nil, err
	}

	return res, err
}

func SendListArray(req *pb.ListArrayRequest) (*pb.ListArrayResponse, error) {
	conn, err := grpc.Dial(globals.GrpcServerAddress, grpc.WithInsecure(), grpc.WithBlock())
	if err != nil {
		log.Error("cannot send a request to cli server: not connected")
		return nil, err
	}
	defer conn.Close()

	c := pb.NewPosCliClient(conn)
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*timeout)
	defer cancel()

	res, err := c.ListArray(ctx, req)
	if err != nil {
		log.Error("error: ", err.Error())
		return nil, err
	}

	return res, err
}

func SendSetLogPreference(req *pb.SetLogPreferenceRequest) (*pb.SetLogPreferenceResponse, error) {
	conn, err := grpc.Dial(globals.GrpcServerAddress, grpc.WithInsecure(), grpc.WithBlock())
	if err != nil {
		log.Error("cannot send a request to cli server: not connected")
		return nil, err
	}
	defer conn.Close()

	c := pb.NewPosCliClient(conn)
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*timeout)
	defer cancel()

	res, err := c.SetLogPreference(ctx, req)
	if err != nil {
		log.Error("error: ", err.Error())
		return nil, err
	}

	return res, err
}

func SendSetLogLevel(req *pb.SetLogLevelRequest) (*pb.SetLogLevelResponse, error) {
	conn, err := grpc.Dial(globals.GrpcServerAddress, grpc.WithInsecure(), grpc.WithBlock())
	if err != nil {
		log.Error("cannot send a request to cli server: not connected")
		return nil, err
	}
	defer conn.Close()

	c := pb.NewPosCliClient(conn)
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*timeout)
	defer cancel()

	res, err := c.SetLogLevel(ctx, req)
	if err != nil {
		log.Error("error: ", err.Error())
		return nil, err
	}

	return res, err
}

func SendLoggerInfo(req *pb.LoggerInfoRequest) (*pb.LoggerInfoResponse, error) {
	conn, err := grpc.Dial(globals.GrpcServerAddress, grpc.WithInsecure(), grpc.WithBlock())
	if err != nil {
		log.Error("cannot send a request to cli server: not connected")
		return nil, err
	}
	defer conn.Close()

	c := pb.NewPosCliClient(conn)
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*timeout)
	defer cancel()

	res, err := c.LoggerInfo(ctx, req)
	if err != nil {
		log.Error("error: ", err.Error())
		return nil, err
	}

	return res, err
}

func SendGetLogLevel(req *pb.GetLogLevelRequest) (*pb.GetLogLevelResponse, error) {
	conn, err := grpc.Dial(globals.GrpcServerAddress, grpc.WithInsecure(), grpc.WithBlock())
	if err != nil {
		log.Error("cannot send a request to cli server: not connected")
		return nil, err
	}
	defer conn.Close()

	c := pb.NewPosCliClient(conn)
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*timeout)
	defer cancel()

	res, err := c.GetLogLevel(ctx, req)
	if err != nil {
		log.Error("error: ", err.Error())
		return nil, err
	}

	return res, err
}

func SendApplyLogFilter(req *pb.ApplyLogFilterRequest) (*pb.ApplyLogFilterResponse, error) {
	conn, err := grpc.Dial(globals.GrpcServerAddress, grpc.WithInsecure(), grpc.WithBlock())
	if err != nil {
		log.Error("cannot send a request to cli server: not connected")
		return nil, err
	}
	defer conn.Close()

	c := pb.NewPosCliClient(conn)
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*timeout)
	defer cancel()

	res, err := c.ApplyLogFilter(ctx, req)
	if err != nil {
		log.Error("error: ", err.Error())
		return nil, err
	}

	return res, err
}
