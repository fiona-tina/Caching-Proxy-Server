/**
 * Caching Proxy Server in C++
 * February 18, 2019
 * Prathikshaa Rangarajan (pr109), Rijish Ganguly (rj???)
 */

#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>


// need to use boost libraries -- see how/why

int main(void) {

  // server socket -- bind to port (80)?

  // become a daemon
  // ref: http://www.netzmafia.de/skripten/unix/linux-daemon-howto.html

  for (;;) { // daemon loop

    /* Our process ID and Session ID */
    pid_t pid, sid;
        
    /* Fork off the parent process */
    pid = fork();
    if (pid < 0) {
      exit(EXIT_FAILURE);
    }
    /* If we got a good PID, then
       we can exit the parent process. */
    if (pid > 0) {
      exit(EXIT_SUCCESS);
    }

    /* Change the file mode mask */
    umask(0);
                
    /* Open any logs here */        
                
    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
      /* Log the failure */
      exit(EXIT_FAILURE);
    }
        

        
    /* Change the current working directory */
    if ((chdir("/")) < 0) {
      /* Log the failure */
      exit(EXIT_FAILURE);
    }
        
    /* Close out the standard file descriptors */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
        
    /* Daemon-specific initialization goes here */
        
    /* The Big Loop */
    while (1) {
      /* Do some task here ... */
      
      sleep(30); /* wait 30 seconds */
    }
    exit(EXIT_SUCCESS);


    // pre-spawn threads to handle requests

    // listen to HTTP requests

    // parse HTTP requests -- ref RFC

    // look in cache -- using files?
    
    // build HTTP response -- ref RFC
    
    // send back HTTP response -- html/js/css/txt, etc. files stored in cache
    // plus status code

  } // end for(;;)
}
