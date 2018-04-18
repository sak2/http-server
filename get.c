#define FOUND_RESPONSE "HTTP/1.0 200 OK\r\n" /* add \n*/
#define NOT_FOUND_RESPONSE "HTTP/1.0 404\r\n\r\n"

#include "get.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FOUND 1
#define NOT_FOUND 0

void parse_request_and_send_response(char* root, char* buffer, int newsockfd) {
    const char* path_start = strstr(buffer, "GET") + 4;
    const char* path_end = strstr(buffer, "HTTP")-1;
    char path[path_end - path_start];
    strncpy(path, path_start, path_end - path_start); 
    path[sizeof(path)] = '\0'; // null terminator
    char full_path[sizeof(path) + sizeof(root)];
    strcpy(full_path, root);
	strcat(full_path, path);

    /* If the file is found, repond with the 200 header and the file */
    if (file_found(full_path)) respond_OK_and_send_file(full_path, newsockfd);
    /* otherwise, respond with a 404 NOT FOUND response */
    else respond_NOTFOUND(newsockfd);
}

int file_found(char* path_to_file) {
    /* Try to open the file at given path, check
       if a non-NULL FILE pointer is returned */
    FILE* fp = fopen(path_to_file, "r");
    if (fp != NULL) {
        fclose(fp);
        return FOUND;
    } else {
        return NOT_FOUND;
    }  
}

void respond_OK_and_send_file(char* full_path, int newsockfd) {
    /* First open the file, get the information for
       the header (size, mime type) and send the file */
    int fd = open(full_path, O_RDONLY);
    size_t size = get_size(fd);
    char response[128];
    make_response_header(response, full_path, size);
    write(newsockfd, response, strlen(response));
    sendfile(newsockfd, fd, NULL, size);
    close(fd);
}

void respond_NOTFOUND(int newsockfd) {
    /* Send a 404 NOT FOUND response to the client */
    write(newsockfd, NOT_FOUND_RESPONSE, strlen(NOT_FOUND_RESPONSE));
}

size_t get_size(int fd) {
    struct stat st;
    fstat(fd, &st);
    return st.st_size;
}

void get_mime_type(char* path, char* mime) {
    if (strstr(path, ".jpg") != NULL) {
		strcpy(mime, "image/jpeg");
    } else if (strstr(path, ".html") != NULL) {
        strcpy(mime, "text/html");
    } else if (strstr(path, ".css") != NULL) {
        strcpy(mime, "text/css");
    } else if (strstr(path, ".js") != NULL) {
        strcpy(mime, "text/javascript");
    } else {
        strcpy(mime, "unknown");
    }
}

void make_response_header(char* response, char* path, size_t size) {
    char mime[64];
    get_mime_type(path, mime);
    sprintf(response, "%sContent-Length: %zu\r\nContent-Type: %s\r\n\r\n", FOUND_RESPONSE, size, mime);
}