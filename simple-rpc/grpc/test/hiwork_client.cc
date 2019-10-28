#include "log_print.h"

#include "hiwork_client.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
//using namespace google::protobuf::util;
using namespace hiwork;

int HiworkClient::test(hiwork::TestReply &reply, const std::string& testmsg) 
{
    int retcode = Status::OK.error_code();

    // Data we are sending to the server.
    TestRequest request;
    request.set_testmsg(testmsg);
    
    // Container for the data we expect from the server.
    //TestReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    std::chrono::system_clock::time_point deadline = std::chrono::system_clock::now() + std::chrono::seconds(10);
    context.set_deadline(deadline);

    // The actual RPC.
    Status status = stub_->Test(&context, request, &reply);

    // Act upon its status.
    if (! status.ok()) {
        LOG_PRINT(EN_LOG_ERROR, "call hiwork failed, error:%d, msg:%s", status.error_code(), status.error_message().data());
        retcode = status.error_code();
        goto __end;
    }

__end:
    return retcode;
}

