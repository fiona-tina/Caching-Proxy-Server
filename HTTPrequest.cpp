#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "HTTPrequest.h"

using namespace std;


string HTTPrequest::return_header(){

	string temp(request_buffer.data());
	string header = temp.substr(0, temp.find("\r\n\r\n") + 4);
	return header;
}



bool HTTPrequest::header_receive_successful(){

	string temp(request_buffer.data());
	size_t end_position = temp.find("\r\n\r\n");
	this->header_length = end_position + 4;
	this->total_length = header_length;
	if(end_position != string::npos){
		return true;
	}
	else
		return false;
}



string HTTPrequest::return_etag(){

	string header(request_buffer.data(), header_length);
	size_t find_position = header.find("ETag");
	if(find_position == string::npos){
		return NULL;
	}
	else{
		size_t end_position = header.find("/r/n", find_position);
		string etag(header.substr(find_position, find_position - end_position ));
		return etag;
	}
}




int HTTPrequest::get_content_length(){
	string temp(request_buffer.data());
	size_t content_len_pos = temp.find("Content-Length: ");
	if(content_len_pos == string::npos){
		return -1;
	}
	else{
		string parse(temp.erase(0, content_len_pos));
		size_t position = parse.find("\r\n");
		if(position == string::npos){
			cerr << "Strange request." << endl;
			return -1;
		}

		stringstream ss;
		ss << parse.substr(content_len_pos + 2, content_len_pos - position);
		ss >> this->content_length;
		this->total_length = this->header_length + this->content_length;

		return 0;


	}

}


int set_fields(){

	string original_request(request_buffer.data());
	string request;
	
	//get firstline
	size_t position = original_request.find("\r\n");
	if(!(position == string::npos)){
		request = original_request.substr(0, position);
		this->request_line = request;
	}
	else{
		cerr << "Request format is incorrect." << endl;
		return -1;
	}

	//Get method
	string method;
	size_t position_two = request.find(" ");
	if(!(position_two == string::npos)){
		method = request.substr(0, position_two);
		this->http_method = method;
	}
	else{
		cerr << "Request format is incorrect." << endl;
		return -1;
	}

	if(this->http_method != "GET" || this->http_method != "POST" || this->http_method != "CONNECT"){
		cerr << "Method is not supported." << endl;
		return -1;
	}

	//Set the port numbers
	if((this->http_method.compare("GET") == 0) || (this->http_method.compare("POST") == 0))
		this->server_port_num = 80;
	else if (this->http_method.compare("CONNECT") == 0) 
		this->server_port_num = 443;

	//Get http type
	




}


