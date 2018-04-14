#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>



void main(int argc, char *argv[]) {
    int sockfd = 0 /*listenfd*/, connfd = 0, n = 0;
    char recvBuff[1025];
    struct sockaddr_in serv_addr;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0); //create socket

    server = gethostbyname(argv[1]);

    memset(&serv_addr, '0', sizeof(serv_addr)); //initialise server address
    memset(recvBuff, '0', sizeof(recvBuff)); //initialise RECEIVE buffer
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_family = AF_INET; //Type of address – internet IP
    serv_addr.sin_port = htons(atoi(argv[2])); // port no

    if (connect(sockfd,  (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Error connecting to server\n");
    } else {
        printf("%s server\n", (char*)&serv_addr);
    }


    while ((n = read(connfd, recvBuff, sizeof(recvBuff)-1)) > 0) {
        printf("%s\n\n", recvBuff);
    }
}
