/* the include guards */
#ifndef GET_H_INCLUDED
#define GET_H_INCLUDED

#include <stdio.h>

/* Prototypes for the functions */
void parse_request_and_send_response(char* root, char* buffer, int newsockfd);
int file_found(char* path_to_file);
void respond_OK_and_send_file(char* full_path, int newsockfd);
void respond_NOTFOUND(int newsockfd);
void get_mime_type(char* path, char* mime);
void make_response_header(char* response, char* full_path, size_t size);
size_t get_size(int fd);

#endif