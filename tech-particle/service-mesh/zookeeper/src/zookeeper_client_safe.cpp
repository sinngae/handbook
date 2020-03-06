/*************************************************************************
	> File Name: zookeeper_client_safe.cpp
	> Author: renzq
	> Mail: zqren@foxmail.com
	> Created Time: Wed 09 May 2018 11:38:01 PM HKT
 ************************************************************************/
#include "zookeeper_client_safe.h"
#include <log4cplus/loggingmacros.h>

zookeeper_client_safe::zookeeper_client_safe()
    : m_strand(m_io_service)
    , m_work(m_io_service)
    , m_heartbeat_timer(m_io_service, boost::posix_time::seconds(1))
{        
}

zookeeper_client_safe::~zookeeper_client_safe()
{
}

void zookeeper_client_safe::init(
    log4cplus::Logger &logger,
    const char *addrs,
    int sessiontimeout_ms,
    int servicetimeout_ms,
    SWITCH_CALLBACK switchtomain_cb,
    SWITCH_CALLBACK switchtobak_cb,
    ZooLogLevel log_level,
    FILE *log_stream)
{
    m_zk_client_ptr.reset(new zookeeper_client(
        logger,
        addrs,
        sessiontimeout_ms,
        servicetimeout_ms,
        switchtomain_cb,
        switchtobak_cb,
        log_level,
        log_stream));
    m_p_logger = &logger;
}

int zookeeper_client_safe::login(const char *parent_path, const char *prefix)
{
    return m_zk_client_ptr->login(parent_path, prefix);
}

int zookeeper_client_safe::heartbeat_safe()
{
    boost::mutex::scoped_lock lock(m_heartbeat_mutex);
    return m_zk_client_ptr->heartbeat();
}

int zookeeper_client_safe::logout()
{
    return m_zk_client_ptr->logout();
}

int zookeeper_client_safe::add(const timepair &_time_pair)
{
    write_lock w_lock(m_timepairs_rwlock);
    m_time_pairs.push_back(_time_pair);
    LOG4CPLUS_DEBUG((*m_p_logger), "zookeeper_client_safe::add[first:"
                    << boost::posix_time::to_simple_string(_time_pair.fist).c_str()
                    << " seconds: " << boost::posix_time::to_simple_string(_time_pair.second).c_str() << "]");
}

void zookeeper_client_safe::clear_timepairs()
{
    write_lock w_lock(m_timepairs_rwlock);
    m_time_pairs.clear();
}

void zookeeper_client_safe::start(boost::uint32_t interval_seconds)
{
    m_interval_seconds = interval_seconds;
    m_thread.reset(new boost::thread(booost::bind(&boost::asio::io_service::run, boost::ref(m_io_service))));

    start_timer(boost::posix_time::seconds(m_interval_seconds), m_heartbeat_timer,
                boost::bind(&zookeeper_client_safe::handle_heartbeat_timer, this, 
                boost::asio::placeholders::error));
}

void zookeeper_client_safe::stop()
{
    boost::this_thread::disable_interruption di;
    boost::system::error_code error;

    m_heartbeat_timer.cancel(error);
    m_io_service.stop();
    m_thread->join();
    m_thread.reset();
}

void zookeeper_client_safe::handle_heartbeat_timer(const boost::system::error_code &error)
{
    if (error)
    {
        if (error != boost::system::operation_canceled)
        {
            LOG4CPLUS_ERROR((*m_p_logger), "zookeeper_client_safe::handle_heartbeat_timer error: " << error.message());
        }
        LOG4CPLUS_INFO((*m_p_logger), "zookeeper_client_safe::handle_heartbeat_timer timer exit");
        return ;
    }

    boost::posix_time::time_duration time_now(boost::posix_time::second_clock::local_time().time_of_day());
    bool not_beating = true;
    do
    {
        read_lock r_lock(m_timepairs_rwlock);
        for (time_pairs::iterator iter = m_time_pairs.begin();
            iter != m_time_pairs.end(); ++iter)
        {
            if (time_now > iter->first && time_now < iter->second)
            {
                not_beating = false;
                break;
            }
        }
    }
    while (false);

    if (not_beating)
        heartbeat_safe();

    start_timer(boost::posix_time::seconds(m_interval_seconds), m_heartbeat_timer,
                boost::bind(&zookeeper_client_safe::handle_heartbeat_timer, this,
                boost::asio::placeholders::error));
}

