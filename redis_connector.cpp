#include "redis_connector.h"
#include "redis_define.h"
#include <hiredis.h>
#include <winsock2.h>
#include "redis.h"

namespace mini_redis
{
    redis_connector::redis_connector()
    {
        clear();
    }


    redis_connector::~redis_connector()
    {
        clear();
    }

    bool redis_connector::init(redis* p_redis)
    {
        if (!p_redis)
            return false;
        m_predis = p_redis;
        return true;
    }

    void redis_connector::clear()
    {
        m_predis = nullptr;
        m_bconnected = false;
    }

    redisContext* redis_connector::real_connect(const redis_config& config)
    {
        m_bconnected = false;
        redisContext* p_context = nullptr;
        //TODO CONN_FD,CONN_SSL
        timeval v;
        v.tv_sec = (long)std::chrono::duration_cast<std::chrono::seconds>(config.m_connect_timeout).count();
        v.tv_usec = (long)std::chrono::duration_cast<std::chrono::microseconds>(config.m_connect_timeout - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::seconds(v.tv_sec))).count();

        if (config.m_con_type == E_CON_TCP)
        {
            if (config.m_connect_timeout > std::chrono::milliseconds(0))
                p_context = redisConnectWithTimeout(config.url.m_host, config.m_port, v);
            else
                p_context = redisConnect(config.url.m_host, config.m_port);
        }
        else if(config.m_con_type == E_CON_UNIX)
        {
            if (config.m_connect_timeout > std::chrono::milliseconds(0))
                p_context = redisConnectUnixWithTimeout(config.url.m_path, v);
            else
                p_context = redisConnectUnix(config.url.m_path);
        }
        
        if (p_context && p_context->err == REDIS_OK)
            m_bconnected = true;

        return p_context;
    }

    int redis_connector::reconnect()
    {
        if (!m_predis)
            return REDIS_ERR;

        redisContext* p_context = m_predis->get_context();
        if (!p_context)
            return REDIS_ERR;

        int n_err = redisReconnect(p_context);
        if (n_err == REDIS_OK)
            m_bconnected = true;
        else
            m_bconnected = false;
        return n_err;
    }

    bool redis_connector::heart_beat()
    {
        return true;
    }

    void redis_connector::disconnect()
    {
        clear();
    }
}


