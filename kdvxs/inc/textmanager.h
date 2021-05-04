#ifndef TEXT_MANAGER_H
#define TEXT_MANAGER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


typedef struct {
	int uid;
	char* name;
	int pointer;
} uchinfo; /* a info on a users place in a channel */

typedef struct {
	char* name;
	FILE* fp;
	uchinfo* users;
	int unum;
} channel;

int init_channel(channel* c, char* name, int create_new);

void add_user(channel* c, int uid, char* name);

void check_user(channel* c, int uid);

int log_message(channel* c, int uid, char* msg);

int read_new(channel* c, int uid, char** outp);

#endif
/*ifndef TEXT_MANAGER_H*/

