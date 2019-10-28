#include <sys/resource.h>

#include "hiworkserviceimpl.h"
#include "log_print.h"

using grpc::ServerBuilder;

const char *default_config_file = "mygrpc.conf";
const char *default_config_category = "mygrpc";

void RunServer() {
    LOG_PRINT(EN_LOG_WARN, "HIWORK start ...");

    std::string server_address("127.0.0.1:15000");
    HiworkServiceImpl service;
    
    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with 
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    LOG_PRINT(EN_LOG_INFO, "Server listening on %s", server_address.c_str());
    
    // Wait for the server to shutdown. Note that some other thread must be 
    // responsible for shutting down the server for this call to ever return.
    server->Wait();

    LOG_PRINT(EN_LOG_WARN, "HIWORK exit ...");
}

int main(int argc, char** argv) {
    int retcode = dzlog_init(default_config_file, default_config_category);
    if (retcode != 0) {
        fprintf(stderr, "[hiwork] dzlog init with file(%s) failed:%d", default_config_file, retcode);
        return -1;
    }

    //TODO:引入 ubus_app，并控制内存使用
    // 内存限制.
    struct rlimit rl = {600*1024*1024, 600*1024*1024};
    int err = setrlimit(RLIMIT_AS, &rl);
    if (err) {
        LOG_PRINT(EN_LOG_WARN, "set module(hiwork) memory limit(%lld) failed, errno(%d)", rl.rlim_cur, errno);
        return -1;
    }

    // 启动grpc服务
    RunServer();

    return 0;
}
