#include "redis.h"
#include <iostream>
#include <stdlib.h>

namespace mini_redis
{
    redis::redis() 
    {
        clear();
    }

    redis::~redis()
    {
        clear();
    }

    bool redis::init(const redis_config& conf)
    {
        clear();
        memcpy_s(&m_config, sizeof(redis_config), &conf, sizeof(redis_config));
        m_connector.init(this);
        m_commander.init(this);
        m_result.init(this);
        return true;
    }

    void redis::clear()
    {
        m_pcontext = nullptr;
        m_strerr = nullptr;
        m_err = 0;
        m_connector.clear();
        m_commander.clear();
        m_result.clear();
        m_config.clear();
    }
    
    int redis::connect()
    {
        m_pcontext = m_connector.real_connect(m_config);
        if (!m_pcontext)
            return REDIS_ERR;
        
        REDIS_ERR_CHECK(m_pcontext->err == REDIS_OK, free_context(), false);
        return m_pcontext->err;
    }

    bool redis::disconnect()
    {
        m_connector.disconnect();
        free_context();
        return true;
    }

    void redis::free_context()
    {
        if (!m_pcontext)
            return;

        set_error(m_pcontext->err, m_pcontext->errstr);

        redisFree(m_pcontext);
        m_pcontext = nullptr;
    }

    int redis::auth(const char* str_user, const char* str_password)
    {
        REDIS_ERR_CHECK(m_pcontext, set_error(REDIS_ERR, const_cast<char*>("redisContext is null!")), REDIS_ERR);

        redisReply* p_reply = m_commander.auth(str_user, str_password, m_pcontext);
        return m_pcontext->err;
    }

    int redis::echo(const char* str_echo)
    {
        REDIS_ERR_CHECK(m_pcontext, , REDIS_ERR);

        redisReply* p_reply = m_commander.echo(str_echo, m_pcontext);
        return m_pcontext->err;
    }

    int redis::ping(const char* str_ping_msg)
    {
        REDIS_ERR_CHECK(m_pcontext, , REDIS_ERR);

        redisReply* p_reply = m_commander.ping(str_ping_msg, m_pcontext);
        return m_pcontext->err;
    }

    int redis::quit()
    {
        REDIS_ERR_CHECK(m_pcontext, , REDIS_ERR);

        redisReply* p_reply = m_commander.quit(m_pcontext);
        return m_pcontext->err;
    }

    int redis::select(const char* str_db)
    {
        REDIS_ERR_CHECK(m_pcontext, , REDIS_ERR);

        redisReply* p_reply = m_commander.select(str_db, m_pcontext);
        return m_pcontext->err;
    }

}


int main()
{
    //m_context = redisConnect("192.168.42.128", 6379);

    mini_redis::redis_config conf;
    conf.url.m_host = const_cast<char*>("192.168.42.128");
    conf.m_con_type = mini_redis::E_CON_TCP;
    conf.m_port = 6379;
    mini_redis::redis r;
    r.init(conf);
    r.connect();
    r.auth("1", "123");
    r.select("");
    r.ping("");
    r.echo("1111");
    r.quit();
    return 0;
}

