#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>


typedef std::unordered_map<std::string,std::string> map;

class Cache{

public:
  //vector <char> req;
  //vector <char> resource_name;
  map my_cache;
  map expiration_cache; 
  vector <char> MRU;
  size_t  size;
  size_t max_size;

  int add();
  int remove();
  int lookup();
  void update();

  Cache(){
    this.size = 0;
    this.content_length = 0;
    this.total_length = 0;
    request_buffer.resize(MAX_BUFFER);
  };
  ~Cache(){};

};


int main(void){
  cache cache1; //init
  cache1["GET REQUEST 1"] = "<html>"; //insert
  return EXIT_SUCCESS;
}
