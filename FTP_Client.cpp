//Andrew Littleton CSC 4200 Spring '16

/*
create, listen, and start accepting
on a socket, return socketid of accepted socket

get user input

send message to server
wait to receive message, put into a char*

call message parser, functions for each command?

store/send file as appropriate
*/

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "FTP_Functions.h"

using namespace std;



int main(int argc, char* argv[]){

	if(argc < 2 || atoi(argv[1]) <= 10000){
		cout << "Please provide a valid port number" << endl;
	}
	

	int sockid = connect_to_server(atoi(argv[1]));
	cout << "Ready to send commands" << endl;

	char* msg;
	char message[1024];

	bool done = false;

	while(!done){

		cin.getline(message, sizeof(message));

    	if(strstr(message, "quit")){
			done = true;
			sendMessage(sockid, message);
			break;
		}

		parseCommand(message, sockid);
	}
	
}



