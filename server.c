#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


void main(int argc, char *argv[]) {
    int listenfd = 0, connfd = 0;
    char sendBuff[1025];
    struct sockaddr_in serv_addr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0); //create socket
    if (listenfd < 0) {
        printf("Error creating socket\n");
    }
    memset(&serv_addr, '0', sizeof(serv_addr)); //initialise server address
    memset(sendBuff, '0', sizeof(sendBuff)); //initialise send buffer
    serv_addr.sin_family = AF_INET; //Type of address – internet IP
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //Listen on ANY IP Addr
    // serv_addr.sin_port = htons(5000); //Listen on port 5000
    serv_addr.sin_port = htons(atoi(argv[1]));
    if (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Error binding\n");
    }
    listen(listenfd, 10); // maximum number of client connections to queue

    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
    if (connfd < 0) {
        printf("Error accepting\n");
    }
    snprintf(sendBuff, sizeof(sendBuff), "Hello World!");
    write(connfd, sendBuff, strlen(sendBuff));
    close(connfd);
}