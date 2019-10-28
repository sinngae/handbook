#ifndef __HIWORK_CLIENT_H__
#define __HIWORK_CLIENT_H__

#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "hiwork.grpc.pb.h"
#else
#include "hiwork.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class HiworkClient {
    public:
        HiworkClient(const char *sockstr)
            : stub_(hiwork::Hiwork::NewStub(grpc::CreateChannel(sockstr, grpc::InsecureChannelCredentials()))) {}

        int test(hiwork::TestReply &reply, const std::string& testmsg);
        
    private:
        std::unique_ptr<hiwork::Hiwork::Stub> stub_;
};

#endif //end of __HIWORK_CLIENT_H__
