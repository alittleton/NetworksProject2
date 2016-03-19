#include <iostream>
#include <string.h>

using namespace std;

int main(){

	char buf[] = "Hello, World";
	strcat(buf, "\0");

	cout << buf << endl;
	cout << strstr(buf, "\0") << endl;

	return 0;
}