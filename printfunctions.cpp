#include <fstream>
#include <iostream>
using namespace std;

void printError(int &status, int &id){

ofstream file;
file.open("/var/log/erss/proxy.log", ios::app);

   if(status==400){
      file<< ID << " : " << "ERROR invalid HTTP request\n";
      file<< ID << " : " << "Responding HTTP/1.1 400 Bad Request\n";
      file.close();
      return;
    }
    else if(status==-100){
      file << ID << " : " << "ERROR recv() fail\n";
      file.close();
      return;
    }
    else if(status==-200){
      file << ID << " : " << "ERROR send() fail\n";
      file.close();
      return;
    }
    else if(status==-300){
      file << ID << " : "<< "ERROR select() fail\n";
      file.close();
      return;
    }
    else if(status==-400){
      file << ID << " : " << "ERROR connect() fail\n";
      file.close();
      return;
    }
     else if(status==411){
      file<< ID << " : " << "ERROR invalid POST Require header\n";
      file << ID << " : " << "Responding HTTP/1.1 411 Length Required\n";
      file.close();
      return;
    }
    else if(status==502){
      file << ID << " : " << "ERROR invalid Response header\n";
      file<< ID << " : " << "Responding HTTP/1.1 502 Bad Gateway\n";
      file.close();
      return;
    }
    else{
      file.close();
      return;
    }

}