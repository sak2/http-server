/* 
 * Server program based on sample code
 */

#define FOUND_RESPONSE "HTTP/1.0 200 OK\r\n" /* add \n*/
#define NOT_FOUND_RESPONSE "HTTP/1.0 404\r\n\r\n"
#define BUF 100000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
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
	
    /* *************** using the get.c functionality here: */
	char* root = argv[2];
	// Extract just the file path from the request message into the char array 'path'.
    const char* path_start = strstr(buffer, "GET") + 4;
    const char* path_end = strstr(buffer, "HTTP")-1;
    char path[path_end - path_start];
    strncpy(path, path_start, path_end - path_start); 
    path[sizeof(path)] = '\0'; // null terminator
    char full_path[sizeof(path) + sizeof(root)];
    strcpy(full_path, root);
	strcat(full_path, path);
	strcpy(full_path, strchr(strchr(full_path, '/')+1, '/')+1);
    printf("%s\n (_(_(_)_)_) \n", full_path);
    if (find_file(full_path) == 0) { // 0 =found
		
		int fd = open(full_path, O_RDONLY);
		struct stat st;
		fstat(fd, &st);
		size_t size = st.st_size;
		
		char response[256];
		sprintf(response, "%sContent-Length: %zu\r\n\r\n", FOUND_RESPONSE, size);//
		printf("(%zu) Hellooo\n%s", size, response);
		write(newsockfd, response, strlen(response));
		sendfile(newsockfd, fd, NULL, size);
	} else{
		n = write(newsockfd, NOT_FOUND_RESPONSE, strlen(NOT_FOUND_RESPONSE));
	}


	printf("Here is the message: %s\n",buffer);
	printf("Here is the path calculated: %s + %s = %s\n", root, path, full_path);
	
	if (n < 0) {
		perror("ERROR writing to socket");
		exit(1);
	}
	
	/* close socket */
	close(sockfd);
	
	return 0; 
}
