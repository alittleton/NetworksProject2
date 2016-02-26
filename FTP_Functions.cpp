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

//function prototypes
void sendFile(char* msg, int sockid);
void receiveFile(char* msg, int sockid);

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

int runServer(int port){

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

void parseCommand(char* msg, int sockid){

	//cases for GET and PUT

	if(strstr(msg, "PUT:")){

		sendFile(msg, sockid);

		//convert"PUT:" to "STOR:"
		char * pch;
		char* filename;
	  	pch = strtok (msg,":");
	  	while (pch != NULL)
	  	{
	   	 //printf ("%s\n",pch);
	    	pch = strtok (NULL, ":");
	    	if(pch != NULL)
	    		filename = pch;
	  	}

	  	char newstr[] = "STOR:";
		char file[sizeof(filename)];
		strcpy(file, filename);
		strcat(newstr, file);

		sendMessage(sockid, newstr);


	}
	else{
		//convert GET: to RTRV:
		receiveFile(msg, sockid);
		char * pch;
		char* filename;
	  	pch = strtok (msg,":");
	  	while (pch != NULL)
	  	{
	   	 //printf ("%s\n",pch);
	    	pch = strtok (NULL, ":");
	    	if(pch != NULL)
	    		filename = pch;
	  	}

	  	char newstr[] = "RCRV:";
		char file[sizeof(filename)];
		strcpy(file, filename);
		strcat(newstr, file);

		char* retval = (char*)malloc(sizeof(newstr)); //return a pointer to the message
		sprintf(retval, "%s", newstr);	

		sendMessage(sockid, retval);

	}
}

void serverParseMessage(char* msg, int sockid){

	if(strstr(msg, "STOR")){
		receiveFile(msg, sockid);
	}
	else{
		sendFile(msg, sockid);
	}
}

void sendFile(char* msg, int sockid){
	//check file exists try to open it
	//if successful, send RTS

	char * pch;
	char* filename;
  	pch = strtok (msg,":");
  	while (pch != NULL)
  	{
   	 //printf ("%s\n",pch);
    	pch = strtok (NULL, ":");
    	if(pch != NULL)
    		filename = pch;
  	}

  	cout << "sending file " << filename << endl;


	

}

void receiveFile(char* msg, int sockid){
	//check filename does not exist
	//if not, send CTS

	char * pch;
	char* filename;
  	pch = strtok (msg,":");
  	while (pch != NULL)
  	{
   	 //printf ("%s\n",pch);
    	pch = strtok (NULL, ":");
    	if(pch != NULL)
    		filename = pch;
  	}

  	cout << "Receiving file " << filename << endl;

}