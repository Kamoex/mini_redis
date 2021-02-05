#pragma once    
namespace mini_redis
{
    class redis;
    class redis_result
    {
    public:
        redis_result();
        ~redis_result();

        void clear();
        bool init(redis* predis);

    };
}


