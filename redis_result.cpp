#include "redis_result.h"
#include "redis.h"

namespace mini_redis
{
    redis_result::redis_result()
    {
    }


    redis_result::~redis_result()
    {
    }

    void redis_result::clear()
    {

    }

    bool redis_result::init(redis* predis)
    {
        return true;
    }
}