#include "redis_commander.h"
#include "redis_define.h"
#include "redis.h"
#include <string.h>

namespace mini_redis
{
    redis_commander::redis_commander()
    {
    }

    redis_commander::~redis_commander()
    {
    }

    bool redis_commander::init(redis* predis)
    {
        REDIS_ERR_CHECK(predis, , false)
        m_predis = predis;
        return true;
    }

    void redis_commander::clear()
    {
        m_predis = nullptr;
    }   

    void redis_commander::free_reply(redisReply** p_reply)
    {
        freeReplyObject(*p_reply);
        *p_reply = nullptr;
    }

    bool redis_commander::check_result(redisContext* p_redis_context, redisReply** p_reply)
    {
        if (p_redis_context->err != REDIS_OK)
        {
            m_predis->set_error(p_redis_context->err, p_redis_context->errstr);

            free_reply(p_reply);
            return false;
        }

        if ((*p_reply)->type == REDIS_REPLY_ERROR)
        {
            m_predis->set_error(REDIS_ERR, (*p_reply)->str);
        }
        return true;
    }

    redisReply* redis_commander::set_key(redisContext* p_redis_context, const char* str_key, const char* str_val, ESET_KEY_MODE u_mod, unsigned long long u_expire_time)
    {
        REDIS_ERR_CHECK(p_redis_context, , nullptr);

        std::string str_cmd = "SET %s %b";
        // KEEPTTL | EX | PX
        if(u_mod & E_KEEP_TTL)
            str_cmd.append(" KEEPTTL");
        else if (u_mod & E_EXPIRE_SEC)
            str_cmd.append(" EX ").append(std::to_string(u_expire_time));
        else if(u_mod & E_EXPIRE_MSEC)
            str_cmd.append(" PX ").append(std::to_string(u_expire_time));

        // NX | XX
        if(u_mod & E_NX)
            str_cmd.append("NX");
        else if(u_mod & E_XX)
            str_cmd.append("XX");

        redisReply* p_reply = (redisReply*)redisCommand(p_redis_context, str_cmd.c_str(), str_key, str_val, strlen(str_val));

        if (!check_result(p_redis_context, &p_reply))
            return nullptr;

        return p_reply;
    }

}


