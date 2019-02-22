

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


int main(void){
  Cache cache1; //init
  vector<char> str; 
  str.push_back('a');
  cache1.insert("1",str);
  cache1.insert("2",str);
  cache1.insert("3",str);
  cache1.insert("4",str);
  cache1.insert("5",str);
  cache1.insert("6",str);
  cache1.print();
  cout << "---------------------" << endl;
  cache1.evict("6");
  cache1.print();
  cout << "---------------------" << endl;
  cache1.insert("7",str);
  cache1.print();
  cout << "---------------------" << endl;
  return EXIT_SUCCESS;
}
