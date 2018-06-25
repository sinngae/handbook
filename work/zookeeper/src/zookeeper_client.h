/* @file zookeeper_client.h
 * @brief
 * 	zookeeper_client
 *	// extern ZOOAPI const int ZOO_EXPIRED_SESSION_STATE;
 *	// extern ZOOAPI const int ZOO_AUTH_FAILED_STATE;
 *	// extern ZOOAPI const int ZOO_CONNECTING_STATE;
 *	// extern ZOOAPI const int ZOO_ASSOCIATING_STATE;
 *	// extern ZOOAPI const int ZOO_CONNECTED_STATE;
 * @author renzq
 * @date 20171109
 * @class zookeeper_client
 */
#ifndef __ZOOKEEPER_CLIENT_H__
#define __ZOOKEEPER_CLIENT_H__
#include <string.h>

#include <zookeeper/zookeeper.h>
#include <log4cplus/logger.h>

#include <boost/date_time.hpp>
#include <boost/timer.hpp>
#include <boost/thread.hpp>
#include <boost/integer.hpp>
#include <boost/shared_ptr.hpp>

enum ZK_CLIENT_ERRORS
{
	ZK_CLIENT_OK = 0,		// 正常
	ZK_CLIENT_ERROR = 1,		// 
	ZK_CLIENT_LOSE_ELECTION = 2,	// 
	ZK_CLIENT_NOSERVICENODE = 3,	// 
	ZK_CLIENT_FAIL_CREATENODE = 4,	//
	ZK_CLIENT_FAIL_CONNECTZK = 5,	//
	ZK_CLIENT_FAIL_WATCHPRIOR = 6,	//
	ZK_CLIENT_LOGINED = 7		// 
}zkclient_errors;

enum ZK_CLIENT_STATES
{
	ZKCLI_NOTELECTED = 0,		//
	ZKCLI_ISELECTED = 1		//
}zkclient_states;

typedef void (*SWITCH_CALLBACK)(void);

class zookeeper_client
{
public:
	/* @brief construct
	 * @param [in] logger 日志对象
	 * @param [in] addrs zk访问地址[ip:port,ip:port,...]
	 * @param [in] session_timeout_ms 会话超时时间，默认30000ms即30s
	 * @param [in] service_timeout_ms 服务结点超时时间，默认3000ms即3s
	 * @param [in] switchtomain_cb 切换为主的回调函数
	 * @param [in] switchtobak_cb 切换为备的回调函数
	 * @param [in] log_level zk定义的日志级别
	 * 	ZOO_LOG_LEVEL_ERROR = 1,
	 * 	ZOO_LOG_LEVEL_WARN = 2,
	 * 	ZOO_LOG_LEVEL_INFO = 3,
	 * 	ZOO_LOG_LEVEL_DEBUG = 4,
	 * @param [in] log_stream zk日志对象，确保可写，默认为stderr
	 */
	zookeeper_client(
		log4cplus::Logger &logger,
		const char *addrs,
		int sessiontimeout_ms = 30000,
		int servicetimeout_ms = 3000,
		SWITCH_CALLBACK switchtomain_cb = NULL,
		SWITCH_CALLBACK switchtobak_cb = NULL,
		ZooLogLevel log_level = ZOO_LOG_LEVEL_WARN,
		FILE *log_stream = NULL);
	virtual ~zookeeper_client();

	/* @brief 注册服务
	 */
	int login(const char *parent_path, const char *prefix = "");

	/* @brief
	 */
	int heartbeat();

	/* @brief
	 */
	int is_elected() { return m_is_elected; }
	/* @brief
	 */
	int logout();
private:
	log4cplus::Logger &m_logger;

	std::string m_addrs;
	int m_sessiontimeout_ms;
	int m_servicetimeout_ms;
	SWITCH_CALLBACK m_switchtomain_cb;
    SWITCH_CALLBACK m_switchtobak_cb;
    ZooLogLevel m_zoologlevel;
    FILE *m_logstream;

    std::string m_parent_path;
    std::string m_prefix;
    
    zhandle_t *m_zhandle;
    int m_is_elected;
    std::string m_node_name;
    std::string m_path_to_create;
    std::string m_path_to_watch;
    
    boost::posix_time::ptime m_ptime_to_watch;
    std::string m_local_ipv4;
    boost::posix_time::ptime m_last_watch_time;

protected:
    /* @brief 连接及断开
     * */
    int connect();
    void disconnect();
    
    /* brief 
     *      Check whether parent_path exists on zookeeper_client.h
     *      Create the path when it does not 
     * @param [in] parent_path parent path
     * @return ZK_CLIENT_OK, otherwise error
     * */
    int parent_path_exists(const char *parent_path);

    /* @brief
     *      Check whether the node m_path_to_create is elected to be main.
     * @return ZK_CLIENT_OK yes, ZK_CLIENT_LOSE_ELECTION no, otherwise error
     * */
    int _is_elected();

    /* @brief 选举
     * @return 
     * */
    int elect();
    
    
    /* @brief 重连
     * @return 
     * */
    int reconnect();

    /* @brief 重建 服务节点对应的zk上的结点
     * */
    int recreate_node();

    friend void zk_watcher_elected_node(zhandle_t* zk, int type, int state, 
                                       const char* path, void* watcherCtx);
    friend void zk_watcher_default(zhandle_t* zh, int type, int state, 
                                   const char* path, void* watcherCtx);
};

void zk_watcher_elected_node(zhandle_t* zk, int type, int state, 
                             const char* path, void* watcherCtx);
void zk_watcher_default(zhandle_t* zh, int type, int state, 
                        const char* path, void* watcherCtx);

typedef boost::shared_ptr<zookeeper_client> zookeeper_client_ptr;

#endif // end of __ZOOKEEPER_CLIENT_H__
