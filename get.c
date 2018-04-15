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
    // printf("%s\n", req);
    const char* path_start = strchr(req, ' ') + 1;
    // printf("%c start\n", *path_start);
    const char* path_end = strrchr(req, ' ');
    // printf("%c final\n", *path_end);
    // int path_len = path_start - path_end;
    char path[path_end - path_start];
    strncpy(path, path_start, path_end - path_start); 
    path[sizeof(path)] = '\0'; // null terminator
    // printf("root %s\n", root);
    // printf("path %s\n", path);
    char full_path[sizeof(path) + sizeof(root)];
    strncpy(full_path, root, sizeof(root));
    strcat(full_path, path);
    // full_path[sizeof(full_path)-1] = 9;
    printf("waiittt %s end\n", full_path);
    find_file(full_path);
}

void find_file(char* path_to_file) {
    FILE *fp;

    fp = fopen(path_to_file, "r");
    if (fp != NULL) {
        printf("success...\n");fclose(fp);
    } else {
        printf("fail\n");
    }
    
}