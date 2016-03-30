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
void clientSendFile(char* msg, int sockid);
void clientReceiveFile(char* msg, int sockid);


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

char* receiveMessage(int sockid){

	int msgSize;
	char buffer[4096]; // 
	memset(buffer, '\0', 4096); // Clear the buffer.
	//cout << "client receiving" << endl;
	
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

		char* cmdptr = (char*)malloc(sizeof(cmd)); //return a pointer to the message
		memset(cmdptr, '\0', sizeof(cmdptr));
		sprintf(cmdptr, "%s", cmd);	

		//cout << "command: " << cmdptr << endl;

		sendMessage(sockid, cmdptr);
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
		memset(retval, '\0', sizeof(retval));
		sprintf(retval, "%s", cmd);	

		//cout << "command: " << retval << endl;

		sendMessage(sockid, retval);
		clientReceiveFile(msg, sockid);	

	}
}

void clientSendFile(char* msg, int sockid){
	//check file exists try to open it
	
	char* servmsg = receiveMessage(sockid);
	//cout << servmsg << endl;

	if(strstr(servmsg, "ERR")){
		char error[sizeof servmsg];
		//strcpy(error, servmsg + 4);
		cout << "Error: " << servmsg + 4 << endl;
		return; 
	}
	
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

  	//cout << "Sending: " << filename << endl;


	ifstream is;
  	is.open(filename);
  	if(!is.is_open()){
  		cout << "file not available" << endl;	
  		return;
  		is.close();
  	}

  	is.seekg (0, is.end);
    int length = is.tellg();
    is.seekg (0, is.beg);
	char buf[length];
	is.read(buf, length);
	strcat(buf, "\0");

	char file[sizeof buf + 5]; 
	strcpy(file, "CONT:");
	strncpy(file+5, buf, sizeof buf);	

	//cout << "buf" << endl << buf << endl;

	//cout << "sending: " << file << endl;

	sendMessage(sockid, file);
	//sendMessage(sockid, "Goodbye");
	
	is.close();

}

void clientReceiveFile(char* msg, int sockid){

	char* servmsg = receiveMessage(sockid);
	//cout << servmsg << endl;

	if(strstr(servmsg, "ERR")){
		char error[sizeof servmsg];
		//strcpy(error, servmsg + 4);
		cout << "Error: " << servmsg + 4 << endl;
		return; 
	}

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

  	cout << "Receiving: " << filename << endl;

 	ofstream outfile;
 	outfile.open(filename);
 	cout << servmsg + 9 + strlen(filename) << endl;


  	//char* clientFile = receiveMessage(sockid);

  // 	cout << clientFile << endl;
  // 	cout << endl;

  	//outfile << clientFile + 5;
  	outfile << servmsg 9 + strlen(filename);

 	outfile.close();	

 	// memset(clientFile, '\0', sizeof(clientFile));	

 	outfile.close();

}


