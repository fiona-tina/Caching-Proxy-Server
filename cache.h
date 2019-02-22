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
