#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <arpa/inet.h>
#include <iostream>


#define SIZE 4096
using namespace std;

void send_file_bin(int sockfd) {
    // *** hard-coded, change if needed
    char* filename = "./input/d.jpg";
    // open the file in binary format
    ifstream fp;
    fp.open(filename, ios::binary|ios::in);
    if (!fp.is_open()) {
        perror("Error in reading file.\n");
        exit(-1);
    }

    // find out the size of the file
    fp.seekg(0, fp.end);
    int length = fp.tellg();
    fp.seekg(0, fp.beg);

    // allocate the same amount of memory, this should match
    // the size of the file.
    char *buffer = new char[length];
    printf("Allocate: %d bytes of the data\n",length);

    // read in the whole file, then sending the data piece by piece
    fp.read(buffer, length);
    printf("client read file\n");


    int sent_byte;
    // counter, starts at different point.
    int total_sent = 0;
    // send buffer via socket
    while(length > 0){
        int tmp = total_sent;
        // send data chunk by chunk
        // if(length > SIZE){ sent_byte = SIZE;} else{sent_byte = length;}
        sent_byte = length > SIZE ? SIZE : length;
        // printf("sent_byte: %d\n",sent_byte);
        // EX: first time send the 0-4096 byte of the data, second time send  the
        // 4096-8192 fo the data.
        if(send(sockfd,buffer+total_sent,sent_byte,0) == -1){
            perror("Error in sending data.\n");
            exit(-1);
        }
        length -= sent_byte;
        // reset counter after each loop
        total_sent += sent_byte;
        // cout<<tmp<< "-" << total_sent<<endl;

    }
    printf("Client send buffer\n");
    fp.close();
    printf("file closed\n");

    delete[] buffer;
}

int main(){
    // ** change ip for connection with different machines
    char* ip = "127.0.0.1";
    int port = 8080;
    int e;

    int sockfd;
    struct sockaddr_in server_addr;


    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0){
        perror("Error in socket.");
        exit(-1);
    }
    printf("Server socket created\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    e = connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(e == -1){
        perror("Error in connecting.");
        exit(-1);
    }
    printf("connecting successful\n");


    // hard coded the name of the file we want to transfer

    send_file_bin(sockfd);
    printf("file data sent.\n");
    close(sockfd);
    printf("Disconnect from the server.\n");

}
