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
//#include <regex>


using namespace std;

int main(){

	char str[] ="- This, a sample string.";
	//char msg[] ="GET:filename.txt";
	char* msg = "testing 1 2 3";
	char other_msg[strlen(msg)];
	strcpy(other_msg, msg);

	char thing[strlen(str) + strlen(msg)];
	strcpy(thing, other_msg);
	strcat(thing, str);

	cout << other_msg << endl;
	//delete(msg);
	cout << thing << endl;

	
	

	/*char buffer[] = "Hello, World";
	cout << "buffer " << buffer<<endl;
	char* response=(char*)malloc(sizeof(buffer));
	sprintf(response, "%s", buffer);

	cout << response << endl;*/


	return 0;
}
