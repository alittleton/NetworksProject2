//Andrew Littleton CSC4200 Spring 2016

//function prototypes
int connect_to_server(int port);
int runServer(int port);
char* receiveMessage(int sockid);
int sendMessage(int sockid, char* message);
void serverParseMessage(char* msg, int sockid);
void parseCommand(char* msg, int sockid);