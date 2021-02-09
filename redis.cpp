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
        m_strerr = "";
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

    redisReply* redis::auth(const char* str_user, const char* str_password)
    {
        REDIS_ERR_CHECK(m_pcontext, set_error(REDIS_ERR, "redisContext is null!"), nullptr);
        if (str_user == '\0')
            return m_commander.default_excute<const char*>(m_pcontext, "AUTH %s", str_user);
        else
            return m_commander.default_excute<const char*, const char*>(m_pcontext, "AUTH %s %s", str_user, str_password);
    }

    redisReply* redis::echo(const char* str_echo)
    {
        REDIS_ERR_CHECK(m_pcontext, set_error(REDIS_ERR, "redisContext is null!"), nullptr);
        return m_commander.default_excute<const char*>(m_pcontext, "ECHO %s", str_echo);
    }

    redisReply* redis::ping(const char* str_ping_msg)
    {
        REDIS_ERR_CHECK(m_pcontext, set_error(REDIS_ERR, "redisContext is null!"), nullptr);
        return m_commander.default_excute<const char*>(m_pcontext, "PING %s", str_ping_msg);
    }

    redisReply* redis::quit()
    {
        REDIS_ERR_CHECK(m_pcontext, set_error(REDIS_ERR, "redisContext is null!"), nullptr);
        return m_commander.default_excute<>(m_pcontext, "QUIT");
    }

    redisReply* redis::select(const char* str_db)
    {
        REDIS_ERR_CHECK(m_pcontext, set_error(REDIS_ERR, "redisContext is null!"), nullptr);
        return m_commander.default_excute<const char*>(m_pcontext, "SELECT %s", str_db);
    }   


    redisReply* redis::get_key(const char* str_key)
    {
        REDIS_ERR_CHECK(m_pcontext, set_error(REDIS_ERR, "redisContext is null!"), nullptr);
        return m_commander.default_excute<const char*>(m_pcontext, "GET %s", str_key);
    }

    redisReply* redis::set_key(const char* str_key, const char* str_val, ESET_KEY_MODE u_mod/* = E_NONE*/, unsigned long long u_expire_time/* = 0*/)
    {
        REDIS_ERR_CHECK(m_pcontext, set_error(REDIS_ERR, "redisContext is null!"), nullptr);
        return m_commander.set_key(m_pcontext, str_key, str_val, u_mod, u_expire_time);
    }

    redisReply* redis::del_key(const char* str_key)
    {
        REDIS_ERR_CHECK(m_pcontext, set_error(REDIS_ERR, "redisContext is null!"), nullptr);
        return m_commander.default_excute<const char*>(m_pcontext, "DEL %s", str_key);
    }

    redisReply* redis::dump_key(const char* str_key)
    {
        REDIS_ERR_CHECK(m_pcontext, set_error(REDIS_ERR, "redisContext is null!"), nullptr);
        return m_commander.default_excute<const char*>(m_pcontext, "DUMP %s", str_key);
    }

    redisReply* redis::exists_key(const char* str_key)
    {
        REDIS_ERR_CHECK(m_pcontext, set_error(REDIS_ERR, "redisContext is null!"), nullptr);
        return m_commander.default_excute<const char*>(m_pcontext, "EXISTS %s", str_key);
    }

    redisReply* redis::expire_key(const char* str_key, unsigned int u_sec, unsigned long long u_msec /*= 0*/, unsigned long long u_timestamp /*= 0*/)
    {
        REDIS_ERR_CHECK(m_pcontext, set_error(REDIS_ERR, "redisContext is null!"), nullptr);
        std::string str_expire_time = "";
        if (u_sec >= 0)
            return m_commander.default_excute<const char*, const char*>(m_pcontext, "EXPIRE %s %s", str_key, std::to_string(u_sec).c_str());
        else if(u_msec >= 0)
            return m_commander.default_excute<const char*, const char*>(m_pcontext, "PEXPIRE %s %s", str_key, std::to_string(u_msec).c_str());
        else if (u_timestamp >= 0)
            return m_commander.default_excute<const char*, const char*>(m_pcontext, "PEXPIREAT %s %s", str_key, std::to_string(u_timestamp).c_str());
        return nullptr;
    }

    redisReply* redis::keys(const char* str_pattern)
    {
        REDIS_ERR_CHECK(m_pcontext, set_error(REDIS_ERR, "redisContext is null!"), nullptr);
        return m_commander.default_excute<const char*>(m_pcontext, "KEYS %s", str_pattern);
    }

    redisReply* redis::move(const char* str_key, const char* str_db)
    {
        REDIS_ERR_CHECK(m_pcontext, set_error(REDIS_ERR, "redisContext is null!"), nullptr);
        return m_commander.default_excute<const char*, const char*>(m_pcontext, "MOVE %s %s", str_key, str_db);
    }

    redisReply* redis::persist(const char* str_key)
    {
        REDIS_ERR_CHECK(m_pcontext, set_error(REDIS_ERR, "redisContext is null!"), nullptr);
        return m_commander.default_excute<const char*>(m_pcontext, "PERSIST %s", str_key);
    }

    redisReply* redis::ttl(const char* str_key, bool b_sec)
    {
        REDIS_ERR_CHECK(m_pcontext, set_error(REDIS_ERR, "redisContext is null!"), nullptr);
        if(b_sec)
            return m_commander.default_excute<const char*>(m_pcontext, "TTL %s", str_key);
        else
            return m_commander.default_excute<const char*>(m_pcontext, "PTTL %s", str_key);
    }

    redisReply* redis::random_key()
    {
        REDIS_ERR_CHECK(m_pcontext, set_error(REDIS_ERR, "redisContext is null!"), nullptr);
        return m_commander.default_excute(m_pcontext, "RANDOMKEY");
    }

    redisReply* redis::rename_key(const char* str_key, const char* str_new_key, bool b_replace)
    {
        REDIS_ERR_CHECK(m_pcontext, set_error(REDIS_ERR, "redisContext is null!"), nullptr);
        if(b_replace)
            return m_commander.default_excute<const char*, const char*>(m_pcontext, "RENAME %s %s", str_key, str_new_key);
        else
            return m_commander.default_excute<const char*, const char*>(m_pcontext, "RENAMENX %s %s", str_key, str_new_key);
    }

    redisReply* redis::type_key(const char* str_key)
    {
        REDIS_ERR_CHECK(m_pcontext, set_error(REDIS_ERR, "redisContext is null!"), nullptr);
        return m_commander.default_excute<const char*>(m_pcontext, "TYPE %s", str_key);

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

    // connection commands test
    /* 
    r.auth("1", "123");
    r.select("");
    r.ping("");
    r.echo("1111");
    r.quit();
    */

    // key commands
    std::string t = "t1";
    const char* test_key = t.c_str();
    std::string tv = "222";
    redisReply* res = nullptr;
    res = r.get_key(test_key);
    res = r.set_key(test_key, tv.c_str(), mini_redis::E_KEEP_TTL, 100);
    res = r.del_key(test_key);
    res = r.dump_key(test_key);
    res = r.exists_key(test_key); // ¥Ê‘⁄res.integer == 1 ∑Ò‘Úres.integer == 0
    res = r.expire_key(test_key, 100, 0, 0);
    res = r.keys("*");
    res = r.move(test_key, "0");
    res = r.persist(test_key);
    res = r.ttl(test_key, true);
    res = r.random_key();
    res = r.rename_key(test_key, "t99", false);
    res = r.type_key(test_key);
    return 0;
}

