#ifndef TEXT_SERVER_H
#define TEXT_SERVER_H

#include <stdio.h>

int add_text(char* text);

int get_text(int from, char* out);

int add_file(char* filename, char* text);

int add_to_file(char* filename, char* text);

int get_file(char* filename, char* out, int osize);

#endif
