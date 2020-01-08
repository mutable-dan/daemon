Daemon sample 

Shows how to use the libDaemon library

examples of running this sample

-start/stop as a daemon
./testdaemon start 
ps -ef|grep testd
./testdaemon stop
ps -ef|grep testd

-start/stop as a daemon in foreground (ctrl-c to stop)
./testdaemon start -f
ps -ef|grep test-daemond

-start/stop as a daemon alowing console out
./testdaemon start -d
ps -ef|grep test-daemond
./testdaemon stop
ps -ef|grep testd

