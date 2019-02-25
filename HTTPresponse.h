#ifndef _HTTPRESPONSE_H
#define _HTTPRESPONSE_H
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <stdlib.h>

#define MAX_BUFFER_SIZE 1024000

using namespace std;


class HTTPresponse{



public:

	int header_length;
	int content_length;
	int total_length;

	vector<char> response_buffer;
	string first_line;
	string code;

	HTTPresponse(){
		this->header_length = 0;
		this->content_length = 0;
		this->total_length = 0;
		this->response_buffer.resize(MAX_BUFFER_SIZE);

	}

	~HTTPresponse() {}
	string get_etag();
	bool receive_header_set_parameters();
	int get_content_length();
	string get_date();
	string get_last_modified();





};


string HTTPresponse::get_etag(){
	string response(response_buffer.begin(), response_buffer.end());
	string etag;
	size_t position = response.find("ETag:");
	if(position != string::npos){
		string temp = response.substr(position);
		etag = temp.substr(0,temp.find("\r\n"));
	}

	return etag;
}

bool HTTPresponse::receive_header_set_parameters(){
	
size_t position;	
string response(response_buffer.data());
position = response.find("\r\n\r\n");
this->header_length = position + 4;
this->total_length = header_length;

if(position ==  string::npos){
	return false;
}
else{

	this->first_line = response.substr(0,response.find("\r\n"));
	position = first_line.find(" ");
	string sub = first_line.substr(position + 1);
	position = sub.find(" ");
	this->code = sub.substr(0, position); //got the code

	return true;

}


}


int HTTPresponse::get_content_length(){
	string temp(response_buffer.data());
	size_t content_len_pos = temp.find("Content-Length: ");
	if(content_len_pos == string::npos){
		return -1;
	}
	else{
		
		string parse(temp.erase(0, content_len_pos));
		content_len_pos = parse.find(": ");
		size_t position = parse.find("\r\n");
		if(position == string::npos){
			cerr << "Strange request." << endl;
			return -1;
		}
		else{

			stringstream ss;
			ss << parse.substr(content_len_pos + 2, content_len_pos - position);
			ss >> this->content_length;
			this->total_length = this->header_length + this->content_length;
			if(content_length == 0){
				return -1;
			}
			
			return 0;

		}

	}

}

string HTTPresponse::get_date(){
	string date;
	string request(response_buffer.data());
	size_t position = request.find("Date:");
	if(position != string::npos){
		request = request.substr(position + 6);
		int position_two = request.find("\r\n");
		date = request.substr(0, position_two);
	}

	return date;
}

string HTTPresponse::get_expiry_time(){
	string request(response_buffer.data());
	string expire_time;
	size_t position = request.find("Expires:");
	if(position != string::npos){
		request = request.substr(position + 9);
		int position_two = request.find("\r\n");
		expire_time = request.substr(0,position_two);
	}

	return expire_time;
}

string HTTPresponse::get_last_modified(){
	
}







