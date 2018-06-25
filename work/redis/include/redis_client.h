#pragma once

#include "hiredis3cpp.h"
#include "redis_connect_pool.h"

namespace hiredis3cpp
{
	enum en_server_arch
	{
		server_arch_single = 0,
		server_arch_replication,
		server_arch_cluster
	};

	enum en_addrs_for
	{
		addrs_for_pool = 0,			// 192.168.0.1:6379,192.168.0.1:6380,192.168.0.1:6381,...
		addrs_for_link,				// 192.168.0.1:6379,192.168.0.1:6380,192.168.0.1:6381,...
		addrs_for_backup			
		// 192.168.0.1:6379,192.168.0.1:6380,192.168.0.1:6381;192.168.0.2:6379,192.168.0.2:6380,192.168.0.2:6381;...
	};

	class HIREDIS3CPP_API redis_client
	{
	public:
		redis_client(const char* addrs_, 
			en_server_arch server_arch = server_arch_single, 
			bool readonly = false, int max_conns = 1, int max_redirect = 0);
		~redis_client(void);

		void set_redis_service(en_server_arch server_arch){ server_arch_ = server_arch; }
		en_server_arch get_server_arch(){ return server_arch_; }
		
	protected:
		en_server_arch server_arch_;
		bool addrs_for_pool_;
		bool read_only_;
		int max_conns_;
		int max_redirect_;
		redis_connect_pool* redis_connect_pools_;

	private:
		redis_client(){}

	};


}