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

	//char str[] ="- This, a sample string.";
	char str[] ="GET:filename.txt";
	
	char * pch;
	char* filename;
	  //printf ("Splitting string \"%s\" into tokens:\n",str);
	  pch = strtok (str,":");
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

	  cout << newstr << endl;

	/*char buffer[] = "Hello, World";
	cout << "buffer " << buffer<<endl;
	char* response=(char*)malloc(sizeof(buffer));
	sprintf(response, "%s", buffer);

	cout << response << endl;*/


	return 0;
}
