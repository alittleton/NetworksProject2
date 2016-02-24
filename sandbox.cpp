#include <iostream>
#include <string.h>
//#include <regex>


using namespace std;

int main(){

	//char str[] = "PUT:file.txt";
	char str[] = "GET:filename.txt";
	char* tok;
	tok = strtok(str, ":");

	while(tok != NULL) {
		cout << tok << endl;
		tok = strtok(str, ":");
	}
	//if(strstr(str, "GET") == NULL){cout << "none" << endl;}



	return 0;
}
