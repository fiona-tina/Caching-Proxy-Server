/**
 * Caching Proxy Server in C++
 * February 18, 2019
 * Prathikshaa Rangarajan (pr109), Rijish Ganguly (rj???)
 */
#define HTTP_PORT "12345"
#define LISTEN_BACKLOG 1000

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <vector>

#include "cache.h"
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

using namespace std;
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
  if (status != 0) {
    perror("getaddrinfo:");
    return -1;
  }

  for (rm_it = addrlist; rm_it != NULL; rm_it = rm_it->ai_next) {
    fd = socket(rm_it->ai_family, rm_it->ai_socktype, rm_it->ai_protocol);

    if (fd == -1) {
      continue;
    }

    int yes = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
      perror("setsockopt:");
      return -1;
    }

    // bind
    int status = ::bind(fd, rm_it->ai_addr, rm_it->ai_addrlen);

    if (status == 0) {
      std::cout << "Successfully bound to port" << port << std::endl; // log
      break;
    }

    // bind failed
    close(fd);
  }
  if (rm_it == NULL) {
    // bind failed
    fprintf(stderr, "Error: socket bind failed\n");
    return -1;
    // exit(EXIT_FAILURE);
  }

  if (listen(fd, LISTEN_BACKLOG) == -1) {
    perror("listen:");
    return -1;
  }

  freeaddrinfo(addrlist);

  return fd;
}

int open_client_socket(const char *hostname, const char *port) {
  cout << hostname << " " << port << endl;
  int fd;
  int status;
  struct addrinfo hints;
  struct addrinfo *addrlist, *rm_it;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  status = getaddrinfo(hostname, port, &hints, &addrlist);
  if (status != 0) {
    perror("getaddrinfo:");
    return -1;
  }

  for (rm_it = addrlist; rm_it != NULL; rm_it = rm_it->ai_next) {
    fd = socket(rm_it->ai_family, rm_it->ai_socktype, rm_it->ai_protocol);
    // cout << rm_it->ai_addr << endl;
    if (fd == -1) {
      continue;
    }

    int yes = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
      perror("setsockopt:");
      return -1;
    }

    if (connect(fd, rm_it->ai_addr, rm_it->ai_addrlen) != -1) {
      // close(fd);
      // perror("client: connect");
      break;
    }
    // bind failed
    close(fd);
  }

  if (rm_it == NULL) {
    // bind failed
    fprintf(stderr, "Error: connect failed\n");
    return -1;
    // exit(EXIT_FAILURE);
  }

  freeaddrinfo(addrlist);

  return fd;
}

std::vector<char> forward_request(const char *hostname, const char *port,
                                  const char *request) {
  int serverfd = open_client_socket(hostname, port);
  cout << "client connection successful attempting to send #bytes : "
       << strlen(request) << " " << request << endl;
  int num_to_send = strlen(request);
  while (num_to_send > 0) {
    cout << "bytes left to send : " << num_to_send << endl;
    int num_sent = send(serverfd, request, num_to_send,
                        0); // while loop to send everything
    request += num_sent;
    num_to_send -= num_sent;
  }
  char buffer[1];
  std::vector<char> response;
  size_t line_break_count = 0;
  while (1) {
    // print_vec(response);
    recv(serverfd, buffer, 1, MSG_WAITALL); // while loop to receive everything
    response.push_back(buffer[0]);
    if (buffer[0] == '\n') {
      // cout << "newline" << endl;
      // print_vec(response);
      string str(response.end() - 4, response.end());
      // cout << "\"" << str << "\"" << endl;
      // break;
      if (str == "\r\n\r\n") {
        if (line_break_count == 0) {
          break;
        }
        line_break_count++;
      }
    }
  }
  cout << "receive successful" << endl;
  // error checking
  // print_vec(response);
  //  cout << response << endl;
  return response;
}

int main(void) {
  Cache s_cache;

  printf("hello! now in main\n\n"); // remove

  // server socket -- bind to port (80)?
  char port[10] = HTTP_PORT;
  int listener_fd = open_server_socket(NULL, port);

  struct sockaddr_storage user_addr;
  socklen_t user_addr_len = sizeof(user_addr);

  // become a daemon
  // ref: http://www.netzmafia.de/skripten/unix/linux-daemon-howto.html
  int user_fd =
      accept(listener_fd, (struct sockaddr *)&user_addr, &user_addr_len);
  if (user_fd == -1) {
    perror("Error: failed to accept connection on socket\n");
    exit(EXIT_FAILURE);
  }

  printf("connected to client");

  // /* Our process ID and Session ID */
  // pid_t pid, sid;

  // /* Fork off the parent process */
  // pid = fork();
  // if (pid < 0) {
  //   exit(EXIT_FAILURE);
  // }
  // /* If we got a good PID, then
  //    we can exit the parent process. */
  // if (pid > 0) {
  //   exit(EXIT_SUCCESS);
  // }

  // /* Change the file mode mask */
  // umask(0);

  // /* Open any logs here */

  // /* Create a new SID for the child process */
  // sid = setsid();
  // if (sid < 0) {
  //   /* Log the failure */
  //   exit(EXIT_FAILURE);
  // }

  // /* Change the current working directory */
  // if ((chdir("/")) < 0) {
  //   /* Log the failure */
  //   exit(EXIT_FAILURE);
  // }

  // /* Close out the standard file descriptors */
  // close(STDIN_FILENO);
  // close(STDOUT_FILENO);
  // close(STDERR_FILENO);

  // /* Daemon-specific initialization goes here */

  /* The Big Loop */
  for (;;) { // daemon loop

    // accept connection here

    // spawn thread to handle request from user_fd

    // recv the HTTP REQ
    char buffer[512];
    memset(&buffer, 0, sizeof buffer);

    ssize_t read_id;

    read_id = recv(user_fd, buffer, sizeof(buffer), 0); // MSG_WAITALL
    buffer[read_id] = '\0';
    printf("msg recvd:%s", buffer);
    //    send(user_fd, buffer, sizeof buffer, 0);

    // pre-spawn threads to handle requests

    // listen to HTTP requests

    // parse HTTP requests -- ref RFC

    // look in cache -- using files?

    // build HTTP response -- ref RFC

    // send back HTTP response -- html/js/css/txt, etc. files stored in cache
    // plus status code
    string host = "www.google.com";
    string port = "80";
    string request = "GET / HTTP/1.1\r\nHost: google.com\r\n\r\n";
    cout << request << endl;

    // cache lookup

    std::vector<char> response =
        forward_request(host.c_str(), port.c_str(), request.c_str());
    std::string resp_str(response.begin(), response.end());
    print_vec(response);
    // cout << response << endl;

    // adding to cache
    s_cache.insert(request, response);
    cout << "Response has been cached." << endl;
    s_cache.print();

    send(user_fd, resp_str.c_str(), resp_str.length(), 0);
    // sleep(30); /* wait 30 seconds */
  } // end for(;;)

  return EXIT_SUCCESS;
}
