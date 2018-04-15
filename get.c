#include "get.h"
#include <stdio.h>
#include <string.h>


void valid_get(char* buffer) { // checks if the request is valid
    // const char* path_start = strchr(buffer, ' ');
    // const char* path_end = strrchr(req, ' ');

    // printf("gucci\n");
}

void parse(char* req, char* root) {
    // Extract just the file path from the request
    // message into the char array 'path'.
    const char* path_start = strchr(req, ' ') + 1;
    const char* path_end = strrchr(req, ' ');
    char path[path_end - path_start];
    strncpy(path, path_start, path_end - path_start); 
    path[sizeof(path)] = '\0'; // null terminator
    // printf("root %s\n", root);
    char full_path[sizeof(path) + sizeof(root)];
    strcpy(full_path, root);
    strcat(full_path, path);
    find_file(full_path);
}

void find_file(char* path_to_file) {
    FILE *fp;

    fp = fopen(path_to_file, "r");
    if (fp != NULL) {
        printf("success...\n");
        fclose(fp);
    } else {
        printf("fail\n");
    }
    
}