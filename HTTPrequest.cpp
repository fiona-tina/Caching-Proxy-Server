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


int HTTPrequest::set_fields(){

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
	request.erase(0, position_two + 1);
	position_two = request.find(" ");
	if(!(position_two == string::npos)){
		this->http_type = request.substr(position_two + 1);
	}
	else{
		cerr << "Request format is incorrect." << endl;
		return -1;
	}

	if((this->http_type.compare("HTTP/1.0") == 0) || (this->http_type.compare("HTTP/1.1") == 0)){
		cerr << "HTTP type is incorrect." << endl;
		return -1;
	}
	
	//Get server name
    string helper_request(request_buffer.data());
    string host;
    helper_request = helper_request.substr(helper_request.find("Host: ")+6);
    host = helper_request.substr(0, helper.find("\r\n"));
    size_t position_three = host.find(":");
    if(!(position_three != string::npos)){
    	cerr << "Request format is incorrect." << endl;
    	return -1;
    }
    else{
    	host = host.substr(0,position_three); 
    	this->server = host;
    }


    //The case where port num is different than 443 or 80
    string actual_request(request_buffer.data());
    string temporary;
 	actual_request = actual_request.substr(actual_request.find("Host:")+6);
 	size_t position_four = actual_request.find("\r\n");
    actual_request = actual_request.substr(0, position_four);

    if(actual_request.find(":")!= string::npos){
      actual_request = actual_request.substr(actual_request.find(":")+1);
      temporary = actual_request.substr(0,actual_request.find("\r\n"));
      this->server_port_num = std::stoi(temporary);
    }
    else
    {	
    	cerr << "Request format is incorrect." << endl;
    	return -1;
    }
   

	return 0;


}


int main(){
return 0;

}


