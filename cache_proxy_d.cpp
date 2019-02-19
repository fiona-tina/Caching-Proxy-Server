/**
 * Caching Proxy Server in C++
 * February 18, 2019
 * Prathikshaa Rangarajan (pr109), Rijish Ganguly (rj???)
 */

#include <cstdio>
#include <cstdlib>

// need to use boost libraries -- see how/why

int main(void) {

  // server socket -- bind to port (80)?

  // become a daemon
  // ref: http://www.netzmafia.de/skripten/unix/linux-daemon-howto.html

  for (;;) { // pretend to be daemon: temp

    // pre-spawn threads to handle requests

    // listen to HTTP requests

    // parse HTTP requests -- ref RFC

    // look in cache -- using files?

    // build HTTP response -- ref RFC

    // send back HTTP response -- html/js/css/txt, etc. files stored in cache
    // plus status code

  } // end for(;;)
}
