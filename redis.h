#pragma once
#include <hiredis.h>
#include "redis_define.h"
#include "redis_connector.h"
#include "redis_commander.h"
#include "redis_result.h"

namespace mini_redis
{
    class redis
    {
    public:
        redis();
        ~redis();

        bool                    init(const redis_config& conf);
        void                    clear();

        int                     connect();
        bool                    disconnect();
        void                    free_context();

        inline void             set_error(int n_err, char* str_err) { m_err = n_err; m_strerr = str_err; }
        inline char*            get_error_str() { return m_strerr; }
        inline int              get_error() { return m_err; }
        inline redisContext*    get_context() { return m_pcontext; }

        // connection commands
        int                     auth(const char* str_user, const char* str_password);
        int                     echo(const char* str_echo);
        int                     ping(const char* str_ping_msg);
        int                     quit();
        int                     select(const char* str_db);
        
    private:
        redis_connector  m_connector;
        redis_commander  m_commander;
        redis_result     m_result;

        redisContext*    m_pcontext;
        redis_config     m_config;

        char*            m_strerr;
        int              m_err;
    };
}

/*
#define REDIS_REPLY_STRING 1
#define REDIS_REPLY_ARRAY 2
#define REDIS_REPLY_INTEGER 3
#define REDIS_REPLY_NIL 4
#define REDIS_REPLY_STATUS 5
#define REDIS_REPLY_ERROR 6
#define REDIS_REPLY_DOUBLE 7
#define REDIS_REPLY_BOOL 8
#define REDIS_REPLY_MAP 9
#define REDIS_REPLY_SET 10
#define REDIS_REPLY_ATTR 11
#define REDIS_REPLY_PUSH 12
#define REDIS_REPLY_BIGNUM 13
#define REDIS_REPLY_VERB 14
*/