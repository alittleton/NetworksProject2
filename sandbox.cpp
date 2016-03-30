#include <iostream>
#include <string.h>

using namespace std;

int main(){

	char buf[] = "RTS:file.txt";
	char newbuf[sizeof buf];

	strncpy(newbuf, buf + 4, strlen(buf));
	
	cout << newbuf << endl;

	return 0;
}