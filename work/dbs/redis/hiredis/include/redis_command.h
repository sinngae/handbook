#pragma once

#include "hiredis3cpp.h"

namespace hiredis3cpp
{
	class redis_client;

	enum en_command_code
	{
		command_code_bgn		= -1,
		command_code_set		= 0,
		command_code_get		= 1,
		command_code_hset		= 2,
		command_code_hget		= 3,
		command_code_hvals		= 4,
		command_code_lpush		= 5,
		command_code_lrange		= 6,
		command_code_hmset		= 7,
		command_code_hmget		= 8,
		command_code_fushall	= 9,
		command_code_del		= 10,
		command_code_hdel		= 11,
		command_code_lpop		= 12,
		command_code_end		= 13
	};

	typedef std::pair<char*, int> redis_field;

	class HIREDIS3CPP_API redis_command
	{
	public:
		redis_command(void);
		~redis_command(void);
		
		void set_cmd_type(en_command_code redis_cmd){ redis_cmd = redis_cmd; }
		en_command_code get_cmd_type() { return redis_cmd_; }

		void add_field(const char* szfield);
		void add_field(const char* szfield, int len);

		void set_redis_client(redis_client* client);

	private: 
		en_command_code redis_cmd_;
		std::vector<redis_field> redis_fields_;
		int slot_;
		redis_client* redis_client_;

		int run();
		void hash_key();
	};


}