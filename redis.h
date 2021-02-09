#pragma once
#include <hiredis.h>
#include <string>
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

        inline void             set_error(int n_err, std::string str_err) { m_err = n_err; m_strerr = str_err; }
        inline const char*      get_error_str() { return m_strerr.c_str(); }
        inline int              get_error() { return m_err; }
        inline redisContext*    get_context() { return m_pcontext; }

        // connection commands
        redisReply*             auth(const char* str_user, const char* str_password);
        redisReply*             echo(const char* str_echo);
        redisReply*             ping(const char* str_ping_msg);
        redisReply*             quit();
        redisReply*             select(const char* str_db);

        // key commands
        redisReply*             get_key(const char* str_key);
        redisReply*             set_key(const char* str_key, const char* str_val, ESET_KEY_MODE u_mod = E_NONE, unsigned long long u_expire_time = 0); // u_mod ESET_KEY_MODE
        redisReply*             del_key(const char* str_key);
        redisReply*             dump_key(const char* str_key);
        redisReply*             exists_key(const char* str_key);
        redisReply*             expire_key( const char* str_key, unsigned int u_sec, unsigned long long u_msec = 0, unsigned long long u_timestamp = 0);
        redisReply*             keys(const char* str_pattern);
        redisReply*             move(const char* str_key, const char* str_db);
        redisReply*             persist(const char* str_key);
        redisReply*             ttl(const char* str_key, bool b_sec);
        redisReply*             random_key();
        redisReply*             rename_key(const char* str_key, const char* str_new_key, bool b_replace);
        redisReply*             type_key(const char* str_key);

            
            /*
            1	DEL key 该命令用于在 key 存在是删除 key。
            2	DUMP key 序列化给定 key ，并返回被序列化的值。
            3	EXISTS key 检查给定 key 是否存在。
            4	EXPIRE key seconds 为给定 key 设置过期时间。
            5	EXPIREAT key timestamp EXPIREAT 的作用和 EXPIRE 类似，都用于为 key 设置过期时间。 不同在于 EXPIREAT 命令接受的时间参数是 UNIX 时间戳(unix timestamp)。
            6	PEXPIRE key milliseconds 设置 key 的过期时间亿以毫秒计。
            7	PEXPIREAT key milliseconds - timestamp 设置 key 过期时间的时间戳(unix timestamp) 以毫秒计
            8	KEYS pattern 查找所有符合给定模式(pattern)的 key 。
            9	MOVE key db 将当前数据库的 key 移动到给定的数据库 db 当中。
            10	PERSIST key 移除 key 的过期时间，key 将持久保持。
            11	PTTL key 以毫秒为单位返回 key 的剩余的过期时间。
            12	TTL key 以秒为单位，返回给定 key 的剩余生存时间(TTL, time to live)。
            13	RANDOMKEY 从当前数据库中随机返回一个 key 。
            14	RENAME key newkey 修改 key 的名称
            15	RENAMENX key newkey 仅当 newkey 不存在时，将 key 改名为 newkey 。
            16	TYPE key 返回 key 所储存的值的类型。
            */
        
    private:
        redis_connector  m_connector;
        redis_commander  m_commander;
        redis_result     m_result;

        redisContext*    m_pcontext;
        redis_config     m_config;

        std::string      m_strerr;
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