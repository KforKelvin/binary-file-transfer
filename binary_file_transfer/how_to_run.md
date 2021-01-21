# What is this program about

This c++ program is transferring files from the client side to the server side via TCP/IP.
Client will read the files in the "input" folder, and server will create that file in the "output" folder.

## Command line to run this program

1.     make
2.     ./server
3.     ./client
  
## Flaws of  this program

Need to checked the name of the file manuanlly: **line 15 at client.cpp** and **line 15 at server.cpp**

IP address needs to be change if running this program on different machines **line 70 at client.cpp**. Default ip address is "127.0.0.1", which enable transfering in the same machine
