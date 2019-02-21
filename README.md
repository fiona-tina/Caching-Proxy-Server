# Caching Web Proxy Server in C++

## Current Progress:
* Listen on a socket for incoming requests
* print message to command line
* send() the message back to the user -- echo


## To Do:

* Put the incoming requests into a log file.

* Branch 1: Uncomment Daemon code and multi-threading
--  place accept inside the loop -- span a new thread for every new fd

* Branch 2: A simple proxy server -- caching aside
-- Parse the input request and forward the request and just pass back the response -- no caching yet.

* Step 3:
- Combine both of the above and check it works

* Branch 3:
-- Use the Branch 2 proxy server to build the cache model:
   Step 1: Just save the reponses as files pointed to by a Hash map
   