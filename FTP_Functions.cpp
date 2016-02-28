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
#include <fstream>

using namespace std;

//function prototypes
void clientSendFile(char* msg, int sockid);
void clientReceiveFile(char* msg, int sockid);
void serverReceiveFile(char* msg, int sockid);
void serverSendFile(char* msg, int sockid);

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
	char temp_msg[strlen(msg)];
	strcpy(temp_msg, msg);

	if(strstr(msg, "PUT:")){

		//convert"PUT:" to "STOR:"
		char * pch;
		char* filename;
	  	pch = strtok (temp_msg,":");
	  	while (pch != NULL)
	  	{
	   	 //printf ("%s\n",pch);
	    	pch = strtok (NULL, ":");
	    	if(pch != NULL)
	    		filename = pch;
	  	}

	  	char newstr[] = "STOR:";
		char cmd[strlen(newstr) + strlen(filename)];
		strcpy(cmd, newstr);
		strcat(cmd, filename);

		char* retval = (char*)malloc(sizeof(cmd)); //return a pointer to the message
		sprintf(retval, "%s", cmd);	

		sendMessage(sockid, retval);
		clientSendFile(msg, sockid);

	}
	else{
		//convert GET: to RTRV:
		
		char * pch;
		char* filename;
	  	pch = strtok (temp_msg,":");
	  	while (pch != NULL)
	  	{
	   	
	    	pch = strtok (NULL, ":");
	    	if(pch != NULL)
	    		filename = pch;
	  	}

	  	char newstr[] = "RTRV:";
		char cmd[strlen(newstr) + strlen(filename)];
		strcpy(cmd, newstr);
		strcat(cmd, filename);	

		char* retval = (char*)malloc(sizeof(cmd)); //return a pointer to the message
		sprintf(retval, "%s", cmd);	

		sendMessage(sockid, retval);
		clientReceiveFile(msg, sockid);	

	}
}

void serverParseMessage(char* msg, int sockid){

	if(strstr(msg, "STOR")){
		serverReceiveFile(msg, sockid);
	}
	else{
		serverSendFile(msg, sockid);
	}
}

void clientSendFile(char* msg, int sockid){
	//check file exists try to open it
	//if successful, send RTS

	char temp_msg[strlen(msg)];
	strcpy(temp_msg, msg);

	char * pch;
	char* filename;
  	pch = strtok (temp_msg,":");
  	while (pch != NULL)
  	{
   	 //printf ("%s\n",pch);
    	pch = strtok (NULL, ":");
    	if(pch != NULL)
    		filename = pch;
  	}


	ifstream is;
  	is.open(filename);
  	if(!is.is_open())
  		cout << "file not available" << endl;	

  	is.seekg (0, is.end);
    int length = is.tellg();
    is.seekg (0, is.beg);
	char buf[length];
	is.read(buf, length);

	char* file = (char*)malloc(length); //return a pointer to the message
	sprintf(file, "%s", buf);	

	sendMessage(sockid, buf);

}

void serverReceiveFile(char* msg, int sockid){
	//check filename does not exist
	//if not, send CTS

	char temp_msg[strlen(msg)];
	strcpy(temp_msg, msg);

	char * pch;
	char* filename;
  	pch = strtok (temp_msg,":");
  	while (pch != NULL)
  	{
   	 //printf ("%s\n",pch);
  		
    	pch = strtok (NULL, ":");
    	if(pch != NULL)
    		filename = pch;

  	}

  	char* file = receiveMessage(sockid);

  	cout << file << endl;

}

void clientReceiveFile(char* msg, int sockid){


	char temp_msg[strlen(msg)];
	strcpy(temp_msg, msg);

	char * pch;
	char* filename;
  	pch = strtok (temp_msg,":");
  	while (pch != NULL)
  	{
   	 //printf ("%s\n",pch);
  		
    	pch = strtok (NULL, ":");
    	if(pch != NULL)
    		filename = pch;

  	}

  	char* file = receiveMessage(sockid);

  	cout << file << endl;

}

void serverSendFile(char* msg, int sockid){

	char temp_msg[strlen(msg)];
	strcpy(temp_msg, msg);

	char * pch;
	char* filename;
  	pch = strtok (temp_msg,":");
  	while (pch != NULL)
  	{
   	 //printf ("%s\n",pch);
  		
    	pch = strtok (NULL, ":");
    	if(pch != NULL)
    		filename = pch;

  	}

  	cout << filename << endl;
  	
  	ifstream is;
  	is.open(filename);
  	if(!is.is_open())
  		cout << "file not available" << endl;	

  	is.seekg (0, is.end);
    int length = is.tellg();
    is.seekg (0, is.beg);
	char buf[length];
	is.read(buf, length);

	char* file = (char*)malloc(length); //return a pointer to the message
	sprintf(file, "%s", buf);	

	sendMessage(sockid, file);

}