#include "uniform_ecode.h"
#include "log_print.h"

#include "hiwork_client.h"

using namespace hiwork;

const char *default_config_file = "mygrpc.conf";
const char *default_config_category = "grpc-test";

void usage()
{
    printf("usage: 1 - test;\n"
           "         - usage\n"
          );
}

void check_arg(int argc, /*char *argv[], */int count)
{
    if (argc <= count)
    {
        usage();
        printf("need %d parameters, but get %d parameters!\n", count, argc-1);
        exit(1);
    }
}

int main(int argc, char** argv) {
    int ret = dzlog_init(default_config_file, default_config_category);
    if (ret != 0) {
        fprintf(stderr, "[hiwork] dzlog init with file(%s) failed:%d", default_config_file, ret);
        return -1;
    }
    // Instantiate the client. It requires a channel, out of which the actual RPCs
    // are created. This channel models a connection to an endpoint (in this case,
    // localhost at port 50051). We indicate that the channel isn't authenticated
    // (use of InsecureChannelCredentials()).
    HiworkClient Hiwork("127.0.0.1:15000");

    check_arg(argc, 1);

    int retcode = UNE_OK;
    switch (atoi(argv[1]))
    {
        case 1:
            {
                std::string testmsg("world");
                TestReply reply;
                retcode = Hiwork.test(reply, testmsg);

                LOG_PRINT(EN_LOG_ERROR, "call hiwork test return:%d, reply.return:%d", retcode, reply.retcode());
                break;
            }
        default:
            usage();
            std::cout << "undefined option: " << argv[1] << std::endl;
            break;
    }

    return 0;
}
