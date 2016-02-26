CC=g++
CFLAGS=-I.
DEPS = FTP_Functions.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ftpmake: FTP_Client.o FTP_Server.o FTP_Functions.o 
	g++ -o FTPclient FTP_Client.o FTP_Functions.o -I.
	g++ -o FTPserver FTP_Server.o FTP_Functions.o -I.
