/* the include guards */
#ifndef GET_H_INCLUDED
#define GET_H_INCLUDED

#include <stdio.h>

/* Prototypes for the functions */
void get(char* req, char* root, FILE** content); // means everything incl header
int find_file(char* path_to_file);
void open_file(FILE** file, char* path_to_file);

#endif