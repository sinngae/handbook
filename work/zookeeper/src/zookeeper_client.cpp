#include "zookeeper_client.h"

#include <log4cplus/loggingmacros.h>

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

std::string get_local_ipv4(std::string &error)
{
    try
    {
        int sock_get_ip;
        char ip_addr[50];
        struct sockaddr_in *sin;
        struct ifreq ifr_ip;

        if ((sock_get_ip = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            error = "get_local_ipv4 create socket error";
            return "";
        }
        memset(&ifr_ip, 0, sizeof(ifr_ip));
        strncpy(ifr_ip.ifr_name, "eth0", sizeof(ifr_ip.ifr_name) - 1);
        if (ioctl(sock_get_ip, SIOCGIFADDR, &ifr_ip) < 0)
        {
            error = "get_local_ipv4 ioctl error";
            return "";
        }
        sin = (struct sockaddr_in*)&ifr_ip.ifr_addr;
        strcpy(ipaddr, inet_ntoa(sin->sin_addr));
        close(sock_get_ip);
        return std::string(ipaddr);
    }
    catch (const std::exception &e)
    {
        std::string err = e.what();
        error = "get_local_ipv4 exception error=" + err;
        return "";
    }
    catch (...)
    {
        error = "get_local_ipv4 exception!";
        return "";
    }
}

namespace zk_client{
zookeeper_client::zookeeper_client(
		log4cplus::Logger &logger,
		const char *addrs,
		int sessiontimeout_ms,
		int servicetimeout_ms,
		SWITCH_CALLBACK switchtomain_cb,
		SWITCH_CALLBACK switchtobak_cb,
		ZooLogLevel log_level,
		FILE *log_stream)
    : m_logger(logger)
    , m_addrs(addrs)
    , m_sessiontimeout_ms(sesssiontimeout_ms)
    , m_servicetimeout_ms(servicetimeout_ms)
    , m_switchtomain_cb(switchtomain_cb)
    , m_switchtobak_cb(switchtobak_cb)
    , m_zoologlevel(log_level)
    , m_logstream(log_stream)
    , m_zhandle(NULL)
    , m_is_elected(ZKCLI_NOTELECTED)
{
    if (m_switchtobak_cb != NULL)
        (*m_switchtobak_cb)();

    std::string errmsg;
    m_local_ipv4 = get_local_ipv4(errmsg);
    if (! errmsg.empty())
    {
        LOG4CPLUS_DEBUG(m_logger, "zkcli::zkcli " << errmsg.c_str());
        m_local_ipv4 = "unknown";
    }

    connect();
}

zookeeper_client::~zookeeper_client()
{
    disconnect();
}

void zk_watcher_default(zhandle_t *zh, int tyep, int state,
			const char *path, void *watcherCtx)
{
    zookeeper_client *zk_client = (zookeeper_client *)watcherCtx;
    if (zk_client)
    {
        LOG4CPLUS_DEBUG(zk_client->m_logger, "zoo_watcher:[type:" << type
                << ",state:" << state
                << ",path:" << path << "]");
        switch (type)
        {
        case -1/*ZOO_SESSION_EVENT*/:
            if (state == ZOO_EXPIRED_SESSION_STATE)
            {
                zk_client->reconnect();
            }
            break;
        }
    }
}

void zk_watcher_elected_node(zhandle_t *zh, int tyep, int state,
			const char *path, void *watcherCtx)
{
    zookeeper_client *zk_client = (zookeeper_client *)watcherCtx;
    if (zk_client)
    {
        LOG4CPLUS_DEBUG(zk_client->m_logger, "zoo_watcher:[type:" << type
                << ",state:" << state
                << ",path:" << path << "]");
        switch (type)
        {
        case 2/*ZOO_DELETE_EVENT*/:
            zk_client->elect();
            break;
        }
    }
}

int zookeeper_client::login(const char *parent_path, const char *prefix)
{
    m_parent_path = parent_path;
    m_prefix = prefix;

    if (m_zhandle == NULL)
        return ZK_CLIENT_FAIL_CONNECTZK;

    if (! m_path_to_create.empty())
        return ZK_CLIENT_LOGINED;

    int ret = ZK_CLIENT_OK;
    // 检查父节点路径是否存在
    if ((ret = parent_path_exists(parent_path)) != ZK_CLIENT_OK)
        return ret;

    // 创建服务结点对应的zk上的临时有序结点
    if ((ret = recreate_node()) != ZK_CLIENT_OK)
        return ret;

    // 选举
    if ((ret = elect()) != ZK_CLIENT_OK)
        return ret;
    
    return ret;
}

int zookeeper_client::heartbeat()
{
    /*// 检测连接状态
    int state = ZOO_CONNECTED_STATE;
    state = zoo_state(m_zhandle);
    if (state == ZOO_EXPIRED_SESSION_STATE)
    {
        reconnect();
        return;
    }*/

    // 更新自己的时间戳
    boost::posix_time::ptime now(boost::posix_time::microsec_clock::local_time());
    char zknode_data[64];
    snprintf(zknode_data, 64, "%s|%s", 
            boost::posix_time::to_iso_string(now).c_str(),
            m_local_ipv4.c_str());
    int ret = ZOK;
    ret = zoo_set(m_handle, m_path_to_create.c_str(), zknode_data, strlen(zknode_data), -1);
    if (ret != ZOK)
    {
        if (ret != ZNONODE)
        {
            LOG4CPLUS_ERROR(m_logger, "zk_client::heartbeat set the created node: " << m_path_to_create.c_str()
                           << ", error: " << ret << zerror(ret));
            return ZK_CLIENT_ERROR;
        }
        ret = ZK_CLIENT_OK;
        ret = recreate_node();
        if (ret == ZK_CLIENT_OK)
            ret = elect();
        return ret;
    }

    if (m_is_elected == ZK_CLI_NOTELECTED)
    {
        // 检查上一优先结点时间戳是否超时
        ret = ZOK;
        char buffer[1024] = {0};
        int buffer_len = 1024;
        struct Stat stat;
        ret = zoo_get(m_zhandle, m_path_to_watch.c_str(), 0, buffer, &buffer_len, &stat);
        if (ret != ZOK)
        {
            if (ret != ZNONODE)
            {
                LOG4CPLUS_ERROR(m_logger, "zk_client::heartbeat get the watched node: " << m_path_to_watch.c_str
                               << ", error: " << ret << zerror(ret));
                return ZK_CLIENT_ERROR;
            }
            return elect();
        }
        //LOG4CPLUS_DEBUG(m_logger, "zkcli_test " << buffer);
        std::vector<std::string> items;
        boost::split(items, buffer, boost::is_any_of("|"));
        boost::posix_time::ptime ptime_to_watch(boost::posix_time::from_iso_string(items[0]));

        do
        {
            if (m_ptime_to_watch.is_not_a_date_time())
                break;
            if (ptime_to_watch < m_ptime_to_watch || ptime_to_watch > m_ptime_to_watch)
                break;
            if (now <= m_last_watch_ptime)
                break;
            boost::posix_time::time_duration timespan = now - m_last_watch_ptime;
            if (timespan.total_milliseconds() < m_servicetimeout_ms)
                break;

            ret = ZOK;
            ret = zoo_delete(m_zhandle, m_path_to_watch.c_str(), -1);
            if (ret != ZOK && ret != ZNONODE)
            {
                LOG4CPLUS_ERROR(m_logger, "Failed to delete the watched node: " << items[1]
                               << ", error: " << ret << zerror(ret));
                return ZK_CLIENT_ERROR;
            }
            LOG4CPLUS_INFO(m_logger, "Suceed to kick out the expired service node: " << items[1]);
            return elect();
        } while (false);

        m_ptime_to_watch = ptime_to_watch;
        m_last_watch_ptime = now;
    }

    return ret;
}

int zookeeper_client::logout()
{
    int ret = ZOK;
    ret = zoo_delete(m_zhandle, m_path_to_create.c_str(), -1);
    if (ret != ZOK && ret != ZNONODE)
    {
        LOG4CPLUS_ERROR(m_logger, "Failed to delete the created node: " << m_path_to_create.c_str()
                       << ", error: " << ret << zerror(ret));
        return ZK_CLIENT_ERROR;
    }
    m_path_to_create.clear();
    return ZK_CLIENT_OK;
}

int zookeeper_client::connect()
{
    LOG4CPLUS_METHOD(m_logger, "zk_client::connect");
    zoo_set_debug_level(m_zoologlevel);
    if (m_logstream)
        zoo_set_log_stream(m_logstream);

    // 创建会话
    m_zhandle = zookeeper_init(m_addrs.c_str(), zk_watcher_default,
                              m_sessiontimeout_ms, 0, NULL, 0);
    if (m_zhandle == NULL)
    {
        LOG4CPLUS_ERROR(m_logger, "Failed to connect to zookeeper servers: " << m_addr.c_str(),
                       << ", state: " << zoo_state(m_zhandle));
        return ZK_CLIENT_FAIL_CONNECTZK;
    }
    zoo_set_context(m_zhandle, this);
    return ZK_CLIENT_OK;
}

void zookeeper_client::disconnect()
{
    LOG4CPLUS_METHOD(m_logger, "zk_client::disconnect");
    if (m_zhandle)
    {
        int ret = ZOK;
        if ((ret = zookeeper_close(m_zhandle)) != ZOK)
        {
            LOG4CPLUS_ERROR(m_logger, "Failed to close zookeeper: " << ret << zerror(ret));
            return ;
        }
        m_zhandle = NULL;
    }
}

int zookeeper_client::parent_path_exists(const char *sz_parent_path)
{
    int ret = ZOK;
    // 根节点 “/”
    if (strcmp(sz_parent_path, "/") == 0)
        return ZK_CLIENT_OK;
    
    struct Stat stat;
    ret = zoo_exists(m_zhandle, sz_parent_path, 0, &stat);
    if (ret == ZOK)
        return ZK_CLIENT_OK;

    if (ret == ZNONODE)
    {
        ret = ZOK;
        char parent_path[1024] = {0};
        size_t path_len = strlen(sz_parent_path);
        strncpy(parent_path, sz_parent_path, 1024);
        size_t i = path_len - 1;
        while (i > 0)
        {
            if (parent_path[i] ==  '/')
            {
                parent_path[i] = '\0';
                break;
            }
            i--;
        }
        // if (i == 0) { sz_parent_path的父节点是根节点 } else
        if (i != 0) // sz_parent_path的父节点不是根节点
        {
            ret = parent_path_exists(parent_path);
        }
        if (ret == ZOK)
        {
            char buffer[1024] = {0};
            int buffer_len = 0;
            ret = zoo_create(m_zhandle, sz_parent_path, "hi, zk",  6,
                            &ZOO_OPENT_ACL_UNSAFE, 0,
                            buffer, buffer_len);
            if (ret == ZOK || ret == ZNODEEXISTS) 
                return ZK_CLIENT_OK;
        }
    }
    // 异常
    LOG4CPLUS_ERROR(m_logger, "zk_client::parent_path_exists error: " << ret << zerror(ret));
    return ZK_CLIENT_ERROR;
}

int zookeeper_client::_is_elected()
{
    int ret = ZOK;
    struct String_vector nodes_list;
    ret = zoo_get_children(m_zhandle, m_parent_path.c_str(), 0, &node_list);
    if (ret != ZOK)
    {
        LOG4CPLUS_ERROR(m_logger, "error: " << ret << zerror(ret));
        return ZK_CLIENT_ERROR;
    }
    if (nodes_list.count <= 0)
    {
        LOG4CPLUS_ERROR(m_logger, "error: no service node on zookeeper " << m_parent_path.c_str());
        return ZK_CLIENT_NOSERVICENODE;
    }

    std::string node_to_watch;
    std::string node_to_create = m_node_name;
    for (int32_t i = 0; i < nodes_list.count; i++)
    {
        if (strcmp(nodes_list.data[i], node_to_create.c_str()) < 0)
        {
            if (node_to_watch.empty())
                node_to_watch = nodes_list.data[i];
            else
                if (strcmp(nodes_list.data[i], node_to_watch.c_str()) > 0)
                    node_to_watch = node_list.data[i];
        }
    }

    if (node_to_watch.empty())
        return ZK_CLIENT_OK;

    m_path_to_watch = m_parent_path;
    m_path_to_watch += "/"
    m_path_to_watch += node_to_watch;

    return ZK_CLIENT_LOSE_ELECTION;
}

int zookeeper_client::elect()
{
    int ret = ZK_CLIENT_OK;
    // 判断是否中举
    ret = _is_elected();
    if (ret == ZK_CLIENT_OK)
    {
        if (m_is_elected == ZKCLI_NOTELECTED && m_switchtomain_cb != NULL)
            (*m_switchtomain_cb)();
        m_is_elected = ZKCLI_ISELECTED;
    }
    else if (ret == ZK_CLIENT_LOSE_ELECTION)
    {
        if (m_is_elected == ZKCLI_ISELECTED && m_switchtobak_cb != NULL)
            (*m_switchtobak_cb)();
        m_is_elected = ZKCLI_NOTELECTED;

        // 监控上一优先结点
        ret = ZOK;
        struct Stat stat;
        ret = zoo_wexists(m_zhandle, m_path_to_watch.c_str(), 
                         zk_watcher_elected_node, this, &stat);
        if (ret != ZOK)
        {
            if (ret == ZNONODE)
                return elect();
            LOG4CPLUS_ERROR(m_logger, "Failed to watch node: " << ret << zerror(ret));
            ret = ZK_CLIENT_FAIL_WATCHPRIOR;
        }
    }
    return ret;
}

int zookeeper_client::reconnect()
{
    if (m_zhandle)
        disconnect();

    int ret = ZK_CLIENT_OK;
    ret = connect();
    if (ret != ZK_CLIENT_OK)
        return ret;

    if (! m_path_to_create.empty())
    {
        ret = ZK_CLIENT_OK;
        ret = login(m_parent_path.c_str(), m_prefix.c_str());
        if (ret != ZK_CLIENT_OK)
            return ret;
    }

    return ret;
}

int zookeeper_client::recreate_node()
{
    m_path_to_create = m_parent_path;
    m_path_to_create += "/";
    m_path_to_create += m_prefix;

    char buffer[1024] = {0};
    int buffer_len = 1024;
    boost::posix_time::ptime now(boost::posix_time::microsec_clock::local_time());
    char zknode_data[64];
    snprintf(zknode_data, 64, "%s|%s",
             boost::posix_time::to_iso_string(now).c_str(),
            m_local_ipv4.c_str());
    int ret = ZOK:
    // 创建临时有序结点
    ret = zoo_create(m_zhandle, m_path_to_create.c_str(), zknode_data, strlen(zknode_data),
                    &ZOO_OPENT_ACL_UNSAFE, ZOO_EPHEMERAL|ZOO_SEQUENCE,
                    buffer, buffer_len);
    if (ret != ZOK || strlen(buffer) == 0)
    {
        LOG4CPLUS_ERROR(m_logger, "recreate_node " << m_path_to_create.c_str()
                       << " error: " << ret << zerror(ret));
        return ZK_CLIENT_FAIL_CREATENODE;
    }

    m_path_to_create = buffer;
    m_node_name = m_path_to_create.substr(m_parent_path.length() + 1);
    
    LOG4CPLUS_DEBUG(m_logger, "recreate_node: parent_path: " << m_parent_path.c_str()
                   << ", path_to_create: " << m_path_to_create.c_str()
                   << ", node_name: " << m_node_name.c_str());
    return ZK_CLIENT_OK;
}

}// end of namspace zk_client
