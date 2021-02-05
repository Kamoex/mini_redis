#pragma once
#include <chrono>

#define REDIS_ERR_CHECK(check_val, op, return_val) if(!(check_val)) { op; return (return_val); }

namespace mini_redis
{
    enum ECONNECTION_TYPE
    {
        E_CON_TCP,
        E_CON_UNIX
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
