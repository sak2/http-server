/* 
 * Server program based on sample code
 */

#define FOUND_RESPONSE "HTTP/1.0 200 OK\n noicee\r\n"
#define NOT_FOUND_RESPONSE "HTTP/1.0 404 File Not Found noob feeder enjoy 4x report\r\n"
#define BUF 100000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/sendfile.h>

#include "get.h"

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, portno;
	char buffer[BUF];
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen;
	int n;

	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}

	 /* Create TCP socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("ERROR opening socket");
		exit(1);
	}

	
	bzero((char *) &serv_addr, sizeof(serv_addr));

	portno = atoi(argv[1]);
	
	/* Create address we're going to listen on (given port number)
	 - converted to network byte order & any IP address for 
	 this machine */
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);  // store in machine-neutral format

	 /* Bind address to the socket */
	if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR on binding");
		exit(1);
	}
	
	/* Listen on socket - means we're ready to accept connections - 
	 incoming connection requests will be queued */
	listen(sockfd,5);
	clilen = sizeof(cli_addr);

	/* Accept a connection - block until a connection is ready to
	 be accepted. Get back a new file descriptor to communicate on. */
	newsockfd = accept(	sockfd, (struct sockaddr *) &cli_addr, 
						&clilen);

	if (newsockfd < 0) {
		perror("ERROR on accept");
		exit(1);
	}
	
	bzero(buffer,BUF);
	/* Read characters from the connection,
		then process */
	n = read(newsockfd,buffer,BUF-1);

	if (n < 0) {
		perror("ERROR reading from socket");
		exit(1);
	}
	
    /* using the get.c functionality here: */
	char* root = argv[2];
	FILE* content = NULL;
	ssize_t read;
    get(buffer, root, &content);
	if (content) {
		// n = write(newsockfd, FOUND_RESPONSE, strlen(FOUND_RESPONSE));
		// if (n < 0) {
		// perror("ERROR writing to socket");
		// exit(1);
		// } 
		write(newsockfd, content, sizeof(content));
		// sendfile(newsockfd, fileno(content), NULL, 5000);
		// close(fileno(content));
		// char* line = NULL;
    	// size_t len = 0;
		// while ((read = getline(&line, &len, content)) != -1) {
		// 	printf("%s", line);
		// 	n = write(newsockfd,"I got your message",18);
		// }
		// n = write(newsockfd,"I got your mess3434age",18);
	} else {
		n = write(newsockfd, NOT_FOUND_RESPONSE, strlen(NOT_FOUND_RESPONSE));
		if (n < 0) {
		perror("ERROR writing to socket");
		exit(1);
		}
	}

	printf("Here is the message: %s\n",buffer);

	// n = write(newsockfd,"I got your message",18);
	
	if (n < 0) {
		perror("ERROR writing to socket");
		exit(1);
	}
	
	/* close socket */
	close(sockfd);
	
	return 0; 
}
