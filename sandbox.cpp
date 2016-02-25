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

	//char str[] = "PUT:file.txt";
	// char str[] = "GET:filename.txt";
	// char* tok;
	// tok = strtok(str, ":");

	// while(tok != NULL) {
	// 	cout << tok << endl;
	// 	tok = strtok(str, ":");
	// }

	char buffer[] = "Hello, World";
	cout << "buffer " << buffer<<endl;
	char* response=(char*)malloc(sizeof(buffer));
	sprintf(response, "%s", buffer);

	cout << response << endl;


	return 0;
}
