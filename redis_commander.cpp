#include "redis_commander.h"
#include "redis_define.h"
#include "redis.h"

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

    redisReply* redis_commander::auth(const char* str_user, const char* str_password, redisContext* p_redis_context)
    {
        REDIS_ERR_CHECK(p_redis_context, , nullptr)

        redisReply* p_reply = nullptr;
        if(str_user == '\0')
            p_reply = (redisReply*)redisCommand(p_redis_context, "AUTH %s", str_password);
        else
            p_reply = (redisReply*)redisCommand(p_redis_context, "AUTH %s %s", str_user, str_password);

        if (!check_result(p_redis_context, &p_reply))
            return nullptr;

        return p_reply;
    }

    redisReply* redis_commander::echo(const char* str_echo, redisContext* p_redis_context)
    {
        REDIS_ERR_CHECK(p_redis_context, , nullptr);

        redisReply* p_reply = (redisReply*)redisCommand(p_redis_context, "ECHO %s", str_echo);

        if (!check_result(p_redis_context, &p_reply))
            return nullptr;

        return p_reply;
    }

    redisReply* redis_commander::ping(const char* str_ping_msg, redisContext* p_redis_context)
    {
        REDIS_ERR_CHECK(p_redis_context, , nullptr);

        redisReply* p_reply = (redisReply*)redisCommand(p_redis_context, "ECHO %s", str_ping_msg);

        if (!check_result(p_redis_context, &p_reply))
            return nullptr;

        return p_reply;

    }

    redisReply* redis_commander::quit(redisContext* p_redis_context)
    {
        REDIS_ERR_CHECK(p_redis_context, , nullptr);

        redisReply* p_reply = (redisReply*)redisCommand(p_redis_context, "QUIT");

        if (!check_result(p_redis_context, &p_reply))
            return nullptr;

        return p_reply;
    }

    redisReply* redis_commander::select(const char* str_db, redisContext* p_redis_context)
    {
        REDIS_ERR_CHECK(p_redis_context, , nullptr);

        redisReply* p_reply = (redisReply*)redisCommand(p_redis_context, "SELECT %s", str_db);

        if (!check_result(p_redis_context, &p_reply))
            return nullptr;

        return p_reply;
    }

}


