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

int main(){

	char str[] ="- This, a sample string.";
	//char msg[] ="GET:filename.txt";
	//char* msg = "testing 1 2 3";
	char filename[] = "file.txt";

	ofstream fs;
	fs.open (filename);
	if(!fs.is_open())
		cout << "NOPE" << endl;

	fs << "Hello, World\nMy name is Andrew\n";

	fs.close();

	ifstream is;
	is.open(filename);
	if(!is.is_open()){
		cout << "NOPE" << endl;
	}
	    // get length of file:
    is.seekg (0, is.end);
    int length = is.tellg();
    is.seekg (0, is.beg);
	char buf[length];
	is.read(buf, length);

	cout << "file size: " << length << endl << buf << endl;

	is.close();




	/*fstream file_ops;

	file_ops.open("file.txt");
	if(!file_ops.is_open())
		cout << "NOPE" << endl;
	file_ops << msg;
	file_ops.close();*/
	


	return 0;
}
