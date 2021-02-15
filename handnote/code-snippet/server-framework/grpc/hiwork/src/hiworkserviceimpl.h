/************************************************
  > File Name: hiworkserviceimpl.h
  > Author: renziqiang
  > Mail: 
  > Created Time: Tue 29 Jan 2019 01:34:32 AM CST
******************************************/
#ifndef __HIWORKSERVICEIMPL_H__
#define __HIWORKSERVICEIMPL_H__

#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "examples/protos/hiwork.grpc.pb.h"
#else
#include "hiwork.grpc.pb.h"
#endif

using grpc::Server;
using grpc::ServerContext;
using grpc::Status;
using namespace hiwork;

// Logic and data behind the server's behavior.
class HiworkServiceImpl final : public Hiwork::Service 
{
//#ifdef HIWORK_TEST
    Status Test(ServerContext* context, const TestRequest* request, TestReply* reply) override;
//#endif
};

#endif //end of __HIWORKSERVICEIMPL_H__
