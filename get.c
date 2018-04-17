#include "get.h"
#include <stdio.h>
#include <string.h>

#define FOUND 0
#define NOT_FOUND -1

void get(char* req, char* root, FILE** content) {
    // Extract just the file path from the request message into the char array 'path'.
    const char* path_start = strstr(req, "GET") + 4;
    const char* path_end = strstr(req, "HTTP")-1;
    char path[path_end - path_start];
    // int l = (path_end-path_start);
    // printf("%c", l);
    strncpy(path, path_start, path_end - path_start); 
    path[sizeof(path)] = '\0'; // null terminator
    char full_path[sizeof(path) + sizeof(root)];
    strcpy(full_path, root);
    strcat(full_path, path);
    printf("%s\n _____ \n", full_path);
    if (find_file(full_path) == FOUND) open_file(content, full_path);
    else *content = NULL;

}

int find_file(char* path_to_file) {
    FILE *fp;
    fp = fopen(path_to_file, "r");
    if (fp != NULL) {
        fclose(fp);
        printf("FOUND\n");
        return FOUND;
    } else {
        printf("NOT FOUND\n");
        return NOT_FOUND;
    }  
}

void open_file(FILE** file, char* path_to_file) {
    *file = fopen(path_to_file, "r");
}