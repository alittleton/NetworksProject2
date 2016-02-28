//Andrew Littleton CSC 4200 Spring '16

/*
create, listen, and start accepting
on a socket, return socketid of accepted socket

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

//function prototypes


int main(int argc, char* argv[]){

	if(argc < 2 || atoi(argv[1]) <= 10000){
		cout << "Please provide a valid port number greater than 10000" << endl;
		return 1;
	}

	int sockid = runServer(atoi(argv[1]));

	if(sockid == -1){
		cerr << "Bind error, please try again" << endl;
		return 1;
	}

	bool done = false;
	char* msg;
	while(!done){

		msg = receiveMessage(sockid);

		if(strstr(msg, "quit")){
			done = true;
			break;
		}

		serverParseMessage(msg, sockid);

	}

}
