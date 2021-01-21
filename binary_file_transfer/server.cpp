#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <arpa/inet.h>
#include <iostream>

#define SIZE 4096
using namespace std;

// receive/write a file in binary format
void write_file_bin(int sockfd){
    // ** need to change the name of the file
    char* filename =  "./output/d2.jpg";
    // open/ create the file
    ofstream fp (filename, ios::binary| ios::out );

    if (!fp.is_open()){
        perror("Error in writing file.\n");
        exit(-1);
    }

    // read all the data, then write into the file
    char * buffer = (char*)malloc(SIZE);

    // size of the chuck of data needed to be read
    int n = 0;
    // location/pointer/counter, where the data start to be read
    int total_rec = 0;

    while(1){
        n = recv(sockfd,buffer + total_rec ,SIZE,0);

        // received all the send_byte
        if(n == 0){
            printf("finished reading\n");
            break;
        }

        if(n < 0){
            perror("Error in receiving data.");
            exit(-1);
        }

        total_rec += n;

        if (n == SIZE){
            buffer = (char*)realloc(buffer,total_rec + SIZE);
        }

        printf("total_rec: %d, n: %d\n",total_rec,n);
    }

    fp.write((char*)buffer,total_rec);
    printf("received\n");

//    fclose(fp);
    fp.close();
    printf("closed\n");


    return;
}

int main(){
//    char *ip = "10.254.149.23;
    char* ip = "127.0.0.1";
    int port = 8080;
    int e;

    int sockfd,new_sock;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[SIZE];

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0){
        perror("Error in socket.");
        exit(-1);
    }
    printf("Server socket created\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    e = bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(e < 0){
        perror("Error in binding.");
        exit(-1);
    }
    printf("Binding successful\n");

    e = listen(sockfd,10);
    if(e == 0){
        printf("Listening...\n");
    }else{
        perror("Error in listening");
        exit(-1);
    }
    addr_size = sizeof(new_addr);
    new_sock = accept(sockfd,(struct sockaddr*)&new_addr,&addr_size);

//   cout << recv_filename();
    write_file_bin(new_sock);
    printf("Data written in the text file\n");

}
