I was not able to implement the RTS, CTS, CONT, or ERR messages.
This program does use the RCRV and STOR messages.

The Client and Server files are in their own directory.

For the client, type "make -f makefile makeclient"
To run, type ./FTPclient <portnumber>, host is automatically localhost

For the server, type "make -f makefile makeserver"
To run, type ./FTPserver <portnumber>,host is automatically localhost

GET, PUT, and quit commands work as specified
