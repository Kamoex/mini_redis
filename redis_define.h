#pragma once
#include <chrono>

namespace mini_redis
{

#define REDIS_ERR_CHECK(check_val, op, return_val) if(!(check_val)) { op; return (return_val); }

    enum ECONNECTION_TYPE
    {
        E_CON_TCP,
        E_CON_UNIX
    };

    enum ESET_KEY_MODE
    {
        E_NONE        = 0x00000000,
        E_EXPIRE_SEC  = 0x00000001,
        E_EXPIRE_MSEC = 0x00000002,
        E_KEEP_TTL    = 0x00000004,
        E_NX          = 0x00000008,
        E_XX          = 0x00000010,
    };

    struct redis_config 
    {   
        ECONNECTION_TYPE m_con_type = E_CON_TCP;

        union url
        {
            char* m_host;
            char* m_path;
        } url;
        int m_port = 6379;
        std::chrono::milliseconds m_connect_timeout{ 0 };
        std::chrono::milliseconds m_socket_timeout{ 0 };
       
        void clear()
        {
            m_con_type = E_CON_TCP;
            url.m_host = nullptr;
            url.m_path = nullptr;
            m_port = 6379;
        }
    }; 
}
