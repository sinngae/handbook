/*************************************************************************
	> File Name: zookeeper_client_safe.h
	> Author: renzq
	> Mail: zqren@foxmail.com
	> Created Time: Wed 09 May 2018 10:25:19 PM HKT
 ************************************************************************/

#ifndef _ZOOKEEPER_CLIENT_SAFE_H
#define _ZOOKEEPER_CLIENT_SAFE_H
#include "zookeeper_client.h"

#include <boost/serialization/singleton.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>

#include <list>
#include <utility>

typedef std::pair<boost::posix_time::time_duration, boost::posix_time::time_duration> time_pair;
typedef std::list<time_pair> time_pairs;

typedef boost::unique_lock<boost::shared_mutex> write_lock;
typedef boost::shared_lock<boost::shared_mutex> read_lock;

class zookeeper_client_safe : public boost::noncopyable
{
public:
    zookeeper_client_safe();
    virtual ~zookeeper_client_safe();

    void init(
        log4cplus::Logger &logger,
        const char *addrs,
        int sessiontimeout_ms = 30000,
        int servicetimeout_ms = 3000,
        SWITCH_CALLBACK switchtomain_cb = NULL,
        SWITCH_CALLBACK switchtobak_cb = NULL,
        ZooLogLevel log_level = ZOO_LOG_LEVEL_WARN,
        FILE *log_stream = NULL);
    
    /* @brief 注册服务
     * @param [in] parent_path: 服务结点注册在ZK上的parent_path下
     * @param [in] prefix: 服务结点注册在ZK上的名字前缀
     * */
    int login(const char *parent_path, const char *prefix = "");

    /* @brief 心跳
     * @return 0-not;1-yes;
     * */
    int heartbeat_safe();

    /* @brief 判断服务结点是否被选举为主服务结点
     * @return 0-not;1-yes;
     * */
    int is_elected() { return m_zk_client_ptr->is_elected(); }

    /* @brief 注销服务
     * @return
     * */
    int logout();

    /* @brief 增加主服务活跃时间段
     *  活跃时间段之外，zk_client来提供心跳
     * @param [in] _time_pair
     * */
    void add(const time_pair &_time_pair);

    /* @brief 清空活跃时间段
     * */
    void clear_timepairs();

    /* @brief 启动
     *      启动zk_client的辅助心跳
     * */
    void start(boost::uint32_t interval_seconds);

    /* @brief 停止辅助心跳
     * */
    void stop();
    
private:
    log4cplus::Logger *m_logger;
    zookeeper_client_ptr m_zk_client_ptr;               // zk_client_ptr
    boost::mutex m_heartbeat_mutex;                     // 心跳安全锁

    time_pairs m_time_pairs;                            // 主服务活跃时间段
    boost::shared_mutex m_timepairs_rwlock;             // 时间段读写锁

    boost::asio::io_service m_io_service;               // io_service服务
    boost::asio::io_service::strand m_strand;           // 线性服务
    boost::asio::io_service::work m_work;               // 守护
    boost::shared_ptr<boost::thread> m_thread;          // 服务线程
    boost::asio::deadline_timer m_heartbeat_timer;      // 辅助心跳定时器

    boost::uint32_t m_interval_seconds;                 // 时间间隔

    template<class handler>
    void start_timer(boost::posix_time::seconds interval,
                     boost::asio::deadline_timer &_timer, 
                    handler _handler)
    {
        // 启动定时器
        _timer.expires_at(_timer.expires_at() + interval);
        _timer.async_wait(_handler);
    }

    void handle_heartbeat_timer(const boost::system::error_code &error);
};

typedef boost::serialization::singleton<zookeeper_client_safe> zk_client_safe_singleton;
#define g_zk_client_safe zk_client_safe_singleton::get_mutable_instance();

#endif
