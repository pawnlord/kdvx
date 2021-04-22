#include "textmanager.h"


/* generic file checker */
int checkfile(char* name);

int init_channel(channel* c, char* name, int create_new) {
	/* build the name of the file we are opening from the channel name */
        c->name = malloc(strlen(name)+1+strlen(".log"));
        strcpy(c->name, name);
        strcat(c->name, ".log");

	/* check if the file exists.
	   if it does and we need to make a new one, open one and then
	   immediately close it. else, we found an error*/
	if(!checkfile(c->name)){
		if(create_new){
			c->fp = fopen(c->name, "w");
			fclose(c->fp);
		} else {
			return 0;
		}
	}
	c->fp = fopen(c->name, "r+");

	c->unum = 0;

	c->users = (uchinfo*)malloc(1000);
	return 1;
}

void add_user(channel* c, int uid, char* name){
	c->users[c->unum].uid = uid;
	c->users[c->unum].name = malloc(strlen(name)+1);
	strcpy(c->users[c->unum].name, name);
	c->users[c->unum].uid = 0;
}

void check_user(channel* c, int uid){
	for(int i = 0; i < c->unum; i++){
		if(c->users[i].uid == uid){
			printf("user %d: name %s, at %d characters into channel\n", c->users[i].uid, c->users[i].name, c->users[i].pointer);
			return;
		}
	}
}

int checkfile(char* name){
	return access(name, F_OK) == 0;
}
