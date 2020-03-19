#include "../daemon.h"
#include <iostream>
#include <queue>
#include <algorithm>


using namespace std;

void display();

///
/// \brief ...sample usesage of daemon library
///    appname start|stop [options]
///
///    start - start process
///       options - -f -d
///    stop  - stop running process
///
/// \note after 'start|stop [options]' you can put your options, such as a config file
///
///       app [-f] start
///          -f optional - run in foreground and do not redirect stdout and stderr to null device
///          -d optional - do not redirect stdout and stderr to null device, even for daemon
///          start - issue start command
///          cfg file
///
///          stop processes
///
int main( int argc, char* argv[] )
{
   queue<string> qCommands;
   tools::Daemon daemonApp;
   daemonApp.setName( string( argv[0] ) );
   
   if( 1 == argc )
   {
      display();
      return -1;
   }
   
   for( int32_t nIndex=1; nIndex<argc; ++nIndex )
   {
      string strCommand = argv[nIndex];
      transform( strCommand.begin(), strCommand.begin(), strCommand.end(), ::tolower );
      qCommands.push( strCommand );
   }    
   
   
   if( qCommands.front() == "start" )
   {
      qCommands.pop();
      bool bRunForeground( false ) ;
      bool bDebug( false );
      
      if( !qCommands.empty() ) 
      {
         if( qCommands.front() == "-f" )
         {
            bRunForeground = true;
            qCommands.pop();
         }
      }
      if( !qCommands.empty() ) 
      {
         if( qCommands.front() == "-d" )
         {
            bDebug = true;
            qCommands.pop();
         }
      }
      

      if( true == bRunForeground )
      {
         cout << "run in foreground" << endl << endl;
         // start prog body
         daemonApp.runForeground();
         cout << "stop in foreground" << endl << endl;
      } else
      {
         cout << "run as daemon" << endl << endl;
         if( true == bDebug )
         {
            cout << "stdio and srderr not redirected" << endl;
         }
         daemonApp.runDaemon( false, bDebug );
         cout << "start" << endl;

         // start prog body async
         daemonApp.wait();
         cout << "stop running" << endl;
         // issue a stop to any threads in prog body
      }
   } 
   else 
   if( qCommands.front() == "stop" )
   {
      cout << "stop" << endl;
      qCommands.pop();
      daemonApp.stop();
   }
   else
   {
      display();
   }
   
   return 0;
}



void display()
{
   cerr << "sample-daemon start|stop [options] " << endl;
   cerr << " start runs as a daemon" << endl;
   cerr << " stop signals daemon to shutdown, can use ctrl-c or stop to stop a foreground app" << endl;
   cerr << " where options:" << endl;
   cerr << "    -f  run app in foreground and do not redirect stdout and stderr" << endl;
   cerr << "    -d  do not redirect stdout and stderr - usefull for running app as daemon with console output" << endl;
}

