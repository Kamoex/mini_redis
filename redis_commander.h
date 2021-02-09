#pragma once
#include "hiredis.h"
#include "redis_define.h"
    
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

        template<class T>
        void parse(T&& t)
        {
            T t1 = t;
            int a = 1;
        }

        template<class... Args >
        redisReply* default_excute(redisContext* p_redis_context, const char* str_cmd, Args ... args)
        {
            REDIS_ERR_CHECK(p_redis_context, , nullptr);

            int arr[100] = { (parse(args), 0)... };

            redisReply* p_reply = (redisReply*)redisCommand(p_redis_context, str_cmd, std::forward<Args>(args)...);

            if (!check_result(p_redis_context, &p_reply))
                return nullptr;

            return p_reply;
        }

        // key commands
        redisReply* set_key(redisContext* p_redis_context, const char* str_key, const char* str_val, ESET_KEY_MODE u_mod, unsigned long long u_expire_time);

    private:
        redis* m_predis;
    };
}


