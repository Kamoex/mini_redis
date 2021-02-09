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
            1	DEL key ������������ key ������ɾ�� key��
            2	DUMP key ���л����� key �������ر����л���ֵ��
            3	EXISTS key ������ key �Ƿ���ڡ�
            4	EXPIRE key seconds Ϊ���� key ���ù���ʱ�䡣
            5	EXPIREAT key timestamp EXPIREAT �����ú� EXPIRE ���ƣ�������Ϊ key ���ù���ʱ�䡣 ��ͬ���� EXPIREAT ������ܵ�ʱ������� UNIX ʱ���(unix timestamp)��
            6	PEXPIRE key milliseconds ���� key �Ĺ���ʱ�����Ժ���ơ�
            7	PEXPIREAT key milliseconds - timestamp ���� key ����ʱ���ʱ���(unix timestamp) �Ժ����
            8	KEYS pattern �������з��ϸ���ģʽ(pattern)�� key ��
            9	MOVE key db ����ǰ���ݿ�� key �ƶ������������ݿ� db ���С�
            10	PERSIST key �Ƴ� key �Ĺ���ʱ�䣬key ���־ñ��֡�
            11	PTTL key �Ժ���Ϊ��λ���� key ��ʣ��Ĺ���ʱ�䡣
            12	TTL key ����Ϊ��λ�����ظ��� key ��ʣ������ʱ��(TTL, time to live)��
            13	RANDOMKEY �ӵ�ǰ���ݿ����������һ�� key ��
            14	RENAME key newkey �޸� key ������
            15	RENAMENX key newkey ���� newkey ������ʱ���� key ����Ϊ newkey ��
            16	TYPE key ���� key �������ֵ�����͡�
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