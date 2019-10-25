#include "hiworkserviceimpl.h"

extern "C"
{
#include "log_print.h"
#include "uniform_ecode.h"
}

using grpc::Server;
using grpc::ServerContext;
using grpc::Status;
using namespace hiwork;

//#ifdef HIWORK_TEST
Status HiworkServiceImpl::Test(
            ServerContext* context, 
            const TestRequest* request,
            TestReply* reply) {
    std::string prefix("This is hiwork server : ");

    reply->set_retcode(uniform_ecode(UNE_OK));
    reply->set_retmsg(prefix + request->testmsg() + uniform_ecode_info(UNE_OK));

    LOG_PRINT(EN_LOG_WARN, "test: %s ", request->testmsg().c_str());

    return Status::OK;
}
//#endif

