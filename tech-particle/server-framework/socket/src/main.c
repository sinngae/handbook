#include "server.h"

int main(int argc, char *argv[])
{
    server_t *server = new_server(8888);

    server_run(server);

	return 0;
}
