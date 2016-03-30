#include "FTP_Functions.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h> 
#include <unistd.h>
#include <netdb.h>  
#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

//function prototypes
void serverReceiveFile(char* msg, int sockid);
void serverSendFile(char* msg, int sockid);

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
	char buffer[4096]; // 
	memset(buffer, '\0', 4096); // Clear the buffer.
	
	if((msgSize = recv(sockid, buffer, 4095, 0)) < 0) 
	{
		cerr << "Receive error.";
	}

	char* response=(char*)malloc(sizeof(buffer));
	sprintf(response, "%s", buffer);

	memset(buffer, '\0', 4096); // Clear the buffer.

	return response;


}

int sendMessage(int sockid, char* message){
	int msgSize = 0;

	//cout << "server sending " << message << endl;
	
	if((msgSize = send(sockid, message, strlen(message), 0)) < 0) 
	{
		cerr << "Send error.";
		return 1;
	}

	return 0;

}

void serverParseMessage(char* msg, int sockid){
	

	if(strstr(msg, "STOR")){
		serverReceiveFile(msg, sockid);
	}
	else{
		serverSendFile(msg, sockid);
	}
}

void serverReceiveFile(char* msg, int sockid){

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


  	ofstream outfile;
 	outfile.open(filename);

 	if(!outfile.is_open()){
 		sendMessage(sockid, "ERR:Cannot write to file");
 		outfile.close();
 		return;
 	}

 	char clear_msg[sizeof filename + 4];
	strcpy(clear_msg, "CTS:");
	strncpy(clear_msg + 4, filename, sizeof filename);
	//cout << "sending CTS" << endl;
 	
 	sendMessage(sockid, clear_msg);

 	char* server_recv_file = receiveMessage(sockid);
  	cout << server_recv_file << endl << endl;
  	cout << endl;

  	
 	outfile << server_recv_file + 5;
 	outfile.close();

 	memset(server_recv_file, '\0', sizeof(server_recv_file));

}

void serverSendFile(char* msg, int sockid){

	
	char * pch;
	char* filename;
  	pch = strtok (msg,":");
  	while (pch != NULL)
  	{	
    	pch = strtok (NULL, ":");
    	if(pch != NULL)
    		filename = pch;
  	}


  	ifstream is;
 	is.open(filename);

 	if(!is.is_open()){
 		sendMessage(sockid, "ERR:Cannot read from file");
 		is.close();
 		return;
 	}


 	char clear_msg[sizeof filename + 4];
 	memset(clear_msg, '\0', sizeof clear_msg);
	strcpy(clear_msg, "RTS:");
	strncpy(clear_msg + 4, filename, sizeof filename);
 	
 	sendMessage(sockid, clear_msg);

 	
 	is.seekg (0, is.end);
    int length = is.tellg();
    is.seekg (0, is.beg);
	char buf[length];
	is.read(buf, length);
	strcat(buf, "\0");

	char file[sizeof buf + 5]; 
	strcpy(file, "CONT:");
	strncpy(file+5, buf, sizeof buf);	
  	
 	is.close();

 	cout << "sending file" << endl;
 	sendMessage(sockid, file);


 	memset(file, '\0', sizeof(file));








	/*char temp_msg[strlen(msg)];
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

  	is.seekg (0, is.end);
    int length = is.tellg();
    is.seekg (0, is.beg);
	char buf[length];
	is.read(buf, length);
	strcat(buf, "\0");

	char* retval = (char*)malloc(sizeof(buf)); //return a pointer to the message
	sprintf(retval, "%s", buf);


	cout << "sending: " << filename << endl;

	sendMessage(sockid, retval);*/

}
