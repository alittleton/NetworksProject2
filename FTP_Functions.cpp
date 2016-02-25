#include "FTP_Functions.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <unistd.h>
#include <netdb.h>  
#include <cstdlib>
#include <iostream>

using namespace std;

int connect_to_server(int port){


	int sockfd;
	struct sockaddr_in server_addr;
	struct hostent* hent;

	// Check for correct commandline input.
	
	// Error check the server name.
	if((hent=gethostbyname("localhost")) == NULL) 
	{
		cerr << "Invalid host name.";
		exit(1);
	}
	
	// Create the client socket.
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		cerr << "Socket error.";
		exit(1);
	}
	
	memset((void *) &server_addr, 0, sizeof(server_addr)); // Clear the server address structure.
	
	// Set up the server address structure.
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr = *((struct in_addr *)hent->h_addr);
	server_addr.sin_port = htons(port);

	if(connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) 
	{
		cerr << "Connect error.";
		exit(1);
	}

	return sockfd;
}

int connect_to_client(int port){

	int sockfd, newsockfd;
	unsigned int clilen;
		
	// Structures for client and server addresses.
	struct sockaddr_in server_addr, cli_addr;
	
	// Create the server socket.
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		cerr << "Socket error.";
		exit(1);
	}
	
	memset((void *) &server_addr, 0, sizeof(server_addr)); // Clear the server address structure.
	
	// Set up the server address structure.
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);
	
	// Bind the socket to the server address and port.
	if(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) 
	{
		return -1;
	}
	
	// Listen on the socket, queue 5 incoming connections.
	listen(sockfd, 10);
	
	// Loop forever, handling connections.
	
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if(newsockfd < 0) 
	{
		cerr << "Accept error.";
		exit(1);
	}
		
	return newsockfd;
}

char* receiveMessage(int sockid){

	int msgSize;
	char buffer[1016]; // 
	memset(buffer, '\0', 1016); // Clear the buffer.
	
	if((msgSize = recv(sockid, buffer, 1015, 0)) < 0) 
	{
		cerr << "Receive error.";
	}
	
	//cout << "buffer" << buffer<<endl;
	char* response=(char*)malloc(sizeof(buffer));
	sprintf(response, "%s", buffer);

	return response;


}

int sendMessage(int sockid, char* message){
	int msgSize = 0;
	
	if((msgSize = send(sockid, message, strlen(message), 0)) < 0) 
	{
		cerr << "Send error.";
		return 1;
	}

	return 0;

}