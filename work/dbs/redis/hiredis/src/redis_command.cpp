#include "redis_command.h"
#include "redis_client.h"

using namespace hiredis3cpp;

redis_command::redis_command(void)
	:redis_cmd_(command_code_bgn)
{
}


redis_command::~redis_command(void)
{
}

void redis_command::hash_key()
{
	if (server_arch_cluster == redis_client_->get_server_arch())
		return ;
	unsigned short n = 
}