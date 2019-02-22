#ifndef _CACHE_H
#define _CACHE_H
#include <iostream>
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
#include <vector>
#include <sstream>
#include <string>

#endif

using namespace std;

typedef std::unordered_map<string, std::vector<char> > map;

class Cache{

public:
  //vector <char> req;
  //vector <char> resource_name;
  map my_cache;
  map expiration_cache; 
  string MRU;
  size_t  size;
  size_t max_size;
  void print(void);
  int insert(string key, vector<char> val);
  int evictNMRU(void);
  int evict(string key);
  vector<char> lookup(string key);
  int update();

 Cache(int max_size): size(0), max_size(max_size){};
 Cache(): size(0), max_size(4){};
  ~Cache(){};

};

void print_vec(const vector<char> & vec){
  for (auto x: vec) {
    cout << x ;
  }
  cout << endl;
}

void Cache::print(void){
  for (auto it : this->my_cache) {
    cout << " " << it.first << ":" ;
    print_vec(it.second);
    cout << endl;
  }
  return;
}

int Cache::insert(string key, vector<char> val){
  if(this->size == this->max_size){
    evictNMRU();
  }
  this->my_cache[key] = val;
  this->size++;
  this->MRU = key;
  return 1;
}

int Cache::evictNMRU(void){
  for (auto it : this->my_cache) {
    if(it.first != this->MRU){
      this->my_cache.erase(it.first);
      break;
    }
  }
  this->size--;
  return 1;
}

int Cache::evict(string key){
  for (auto it : this->my_cache) {
    if(key == it.first){
      this->my_cache.erase(it.first);
      break;
    }
  }
  if(this->MRU == key){
    this->MRU = this->my_cache.begin()->first;
  }
  this->size--;
  return 1;
}
 
vector<char> Cache::lookup(string key){
  return this->my_cache[key];
}


int Cache::update(){
  return 1;
}

