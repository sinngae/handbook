#pragma once

#include "hiredis3cpp.h"

namespace hiredis3cpp
{
	class redis_connect_pool;

	class HIREDIS3CPP_API redis_connect
	{
	public:
		redis_connect(const char* addr, int port);
		~redis_connect(void);

		bool is_alive();
	private:
		bool is_alive_;
		redis_connect_pool* connect_pool_;
	private:
		redis_connect(){}
	};

}