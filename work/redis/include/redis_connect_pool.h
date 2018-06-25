#pragma once

#include "hiredis3cpp.h"
#include "redis_connect.h"

namespace hiredis3cpp
{
	class redis_client;

	class HIREDIS3CPP_API redis_connect_pool
	{
	public:
		redis_connect_pool(void);
		~redis_connect_pool(void);
	private:
		char addr_[30];
		int bgn_slot_;
		int end_slot_;
		redis_client* redis_client_;
		std::vector<redis_connect*> redis_connects_;
	};

}