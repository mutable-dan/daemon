#pragma once

#include <string>
#include <semaphore.h>
#include <filesystem>

namespace tools {

///
/// \brief ...library provides a way to daemonize application
/// drop in your main, call runDaemon, start your app async and call wait
///
///
class Daemon
{
    private:
        pid_t       	      m_pid                    = -1;      // pid (proc id) of daemon
        std::filesystem::path m_path                   = "/tmp/";
        std::string		      m_strApplicationName     = "";      // name of app and thus pid file
        bool                  m_bInit                  = false;

    public:
        static sem_t   m_sem;

    private:
        Daemon( const Daemon& );

        bool                         recoverPid( );
        bool                         savePid( );
        const std::filesystem::path& makePidPath();

        static void	signal_handler( const int32_t a_nSignal );

    public:
        Daemon();
        Daemon( std::string a_strAppName );
        void setName( const std::string a_strAppName );
        bool runForeground();
        bool runDaemon( const bool a_bChroot = false, const bool a_bDebug = false );
        bool stop();
        bool wait();
};

}
