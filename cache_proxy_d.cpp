/**
 * Caching Proxy Server in C++
 * February 18, 2019
 * Prathikshaa Rangarajan (pr109), Rijish Ganguly (rj???)
 */
#define HTTP_PORT 80
#define LISTEN_BACKLOG 1000

#include <cstdio>
#include <cstdlib>

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

// need to use boost libraries -- see how/why

int open_server_socket(char *hostname, char *port) {
  int fd;
  int status;
  struct addrinfo hints;
  struct addrinfo *addrlist, *rm_it;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  status = getaddrinfo(hostname, port, &hints, &addrlist);
  // if()

  for (rm_it = addrlist; rm_it != NULL; rm_it = rm_it->ai_next) {
    fd = socket(rm_it->ai_family, rm_it->ai_socktype, rm_it->ai_protocol);

    if (fd == -1) {
      continue;
    }

    int yes = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
      exit(EXIT_FAILURE);
    }

    // bind
    if (bind(fd, rm_it->ai_addr, rm_it->ai_addrlen) == 0) {
      break;
    }

    close(fd);
  }

  if (listen(fd, LISTEN_BACKLOG) == -1) {
    perror("ERROR");
  }

  return fd;
}

int main(void) {

  // server socket -- bind to port (80)?
  char port[10] = "80";
  int listener_fd = open_server_socket(NULL, port);

  struct sockaddr_storage user_addr;
  socklen_t user_addr_len = sizeof(user_addr);

  int user_fd =
      accept(listener_fd, (struct sockaddr *)&user_addr, &user_addr_len);

  // recv the HTTP REQ
  char buffer[512];
  ssize_t read_id;

  read_id = recv(user_fd, buffer, sizeof(buffer), 0); // MSG_WAITALL

  printf("msg recvd:%s", buffer);

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
