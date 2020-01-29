#include "daemon.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <unistd.h>
#include <signal.h>


using namespace std;

sem_t tools::Daemon::m_sem;


///
/// \brief glib::Daemon::Daemon
///
tools::Daemon::Daemon(  )
{
}


///
/// \brief glib::Daemon::Daemon
/// \param a_strApplicationName
///
tools::Daemon::Daemon( string a_strApplicationName ) : m_strApplicationName( a_strApplicationName )
{
}

///
/// \brief glib::Daemon::Daemon
///
tools::Daemon::Daemon( const tools::Daemon& )
{
    // todo
}

///
/// \brief the app name is used for the pid file name. the pid file is stored in /tmp/ and uses the supplied name .pid
/// \param a_strApplicationName
///
void tools::Daemon::setName( string a_strApplicationName )
{
    auto pos = a_strApplicationName.find_last_of( '/' );  // no trailing '/'
    m_strApplicationName = a_strApplicationName.substr( ++pos );
}




///
/// \brief brief  call to run as a daemon
///  sets up semaphore
///  fork this process and return as daemon
///  setup signal handler for TERM, INT
///  write pid file
///
///  on return, the proc is the forked daemon
///  debug: Do not close stdout, stderr & stdin
///
/// \param a_bChroot ...run as chroot if true (not implimented)
/// \param a_bDebug ...run  debug - does not redirect stdin, stderr, stdout
/// \return bool true if suceeds
///
///
bool tools::Daemon::runDaemon( const bool a_bChroot, const bool a_bDebug )
{
    stringstream sstr;
    cout << "start" << endl;

    if( 0 == ::daemon( !a_bChroot, a_bDebug ) )
    {
        if( -1 == sem_init( &Daemon::m_sem, 0, 0 ) )
        {
            char* pszError = strerror( errno );
            stringstream sstrInit;
            sstrInit << "Failed on semaphore initialize while starting daemon: " << pszError;
            cerr << sstrInit.str() << endl;
            return false;
        }

        // setup signals
        signal( SIGPIPE, SIG_IGN );
        signal( SIGTSTP, SIG_IGN );
        signal( SIGTTOU, SIG_IGN );
        signal( SIGTTIN, SIG_IGN );
        signal( SIGPIPE, SIG_IGN );
        signal( SIGINT , signal_handler );
        signal( SIGTERM, signal_handler );
        signal( SIGHUP,  SIG_IGN );


        // this is forked daemon
        m_pid = getpid();
        savePid();
        return true;
    } else
    {
        char* pszError = strerror( errno );
        stringstream sstrErr;
        sstrErr << "Daemon startup failed: " << pszError;
        cerr << sstrErr.str();
        return false;
    }
}


///
/// \brief ...call to run in foreground.  process can be stopped with a TERM or INT
///      async call your worker and then this function
///
/// \return bool
///
bool tools::Daemon::runForeground()
{
    if( -1 == sem_init( &Daemon::m_sem, 0, 0 ) )
    {
        char* pszError = strerror( errno );
        cout << "Failed on semaphore initialize while starting foreground: " << pszError << endl;
        return false;
    }

    signal( SIGINT , signal_handler );
    signal( SIGTERM, signal_handler );
    sem_wait( &Daemon::m_sem );
    return true;
}


///
/// \brief used to stiop ma daemon  this is called from a different process, but gets the procid from the pid file of the daemon to stop.
/// reads the pid from the /tmp/ pid file and issues a TERM to that pid
/// signals the wait (wait on sem) with a TERM
///
/// \return bool
///
bool tools::Daemon::stop()
{

    if( true == recoverPid() )
    {
        if( 0 != unlink( makePidPath().c_str() ) )
        {
            cerr << "pid not removed" << endl;
        }

        return kill( m_pid, SIGTERM ) == 0? true: false;
    } else
    {
        cerr << "no process found to stop" << endl;
        return false;
    }
}


///
/// \brief after you call  after you call runDaemon, call wait
///
///        do your work with async call to your work
///        then wait
///        when wait returns, signal your job to end and when it does the daemon can end
///
/// \return bool
///
bool tools::Daemon::wait()
{
    if( 0 == sem_wait( &Daemon::m_sem ) )
    {
        return true;
    }
    return false;
}


///
/// \brief ...private function to mke pid path
///
/// \return std::string
///
const filesystem::path& tools::Daemon::makePidPath()
{
	 if( false == m_bHavePidPath )
	 {
	    m_path /= m_strApplicationName + ".pid";
		 m_bHavePidPath = true;
	 }
	 return m_path;
}


///
/// \brief read the pid file and return contents, there is no error recovery except return false if it can not open
///
/// \param a_pid ...
/// \return bool
///
bool tools::Daemon::recoverPid()
{
    ifstream inf( makePidPath() );
    if( inf.is_open() )
    {
        inf >> m_pid;
        cout << "pid open:" << m_pid << endl;
    } else
    {
        cout << "pid not found" << endl;
        return false;
    }
    return true;
}


///
/// \brief ..write a pid file to tmp/appname.pid
///          pid file is txt file containing proc id
///
/// \param a_pid ...
/// \return bool
///
bool tools::Daemon::savePid( )
{
    ofstream of( makePidPath() );
    if( of.is_open() )
    {
        of << m_pid;
    } else
    {
        return false;
    }
    return true;
}



///
/// \brief callback for signal TERM
///        when stop is called, it generates a sig term, this handles the term and posts on the sem
///
/// \param a_nSignal ...
/// \return void
///
void tools::Daemon::signal_handler( const int32_t a_nSignal )
{
    if( (SIGTERM == a_nSignal) || (SIGINT == a_nSignal) )
    {
        sem_post( &Daemon::m_sem );
    }
}

