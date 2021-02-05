#pragma once
#include "hiredis.h"

namespace mini_redis
{
    struct redis_config;
    class redis;

    class redis_connector
    {
    public:
        redis_connector();
        ~redis_connector();

        bool            init(redis* p_redis);
        void            clear();

        redisContext*   real_connect(const redis_config& config);
        int             reconnect();
        bool            heart_beat();
        void            disconnect();

    private:
        redis*          m_predis;
        bool            m_bconnected;
    };

}

