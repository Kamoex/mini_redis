#pragma once
#include "hiredis.h"
    
namespace mini_redis
{
    class redis;

    class redis_commander
    {
    public:
        redis_commander();
        ~redis_commander();

        bool init(redis* predis);
        void clear();

        void free_reply(redisReply** p_reply);
        bool check_result(redisContext* p_redis_context, redisReply** p_reply);

        /*template<class... Args >
        void excute(EREDIS_COMMENDS e_cmd, Args... args);*/

        // connection commands
        redisReply* auth(const char* str_user, const char* str_password, redisContext* p_redis_context);
        redisReply* echo(const char* str_echo, redisContext* p_redis_context);
        redisReply* ping(const char* str_ping_msg, redisContext* p_redis_context);
        redisReply* quit(redisContext* p_redis_context);
        redisReply* select(const char* str_db, redisContext* p_redis_context);

    private:
        redis* m_predis;
    };
}


