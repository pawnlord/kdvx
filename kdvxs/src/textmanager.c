#include "textmanager.h"


/* generic file checker */
int checkfile(char* name);

/* get a uchinfo structure for logging purposes */
void getuser(uchinfo* dest, channel* from, int uid);

int init_channel(channel* c, char* name, int create_new) {
	/* build the name of the file we are opening from the channel name */
        c->name = malloc(strlen(name)+1+strlen(".log"));
        strcpy(c->name, name);
        strcat(c->name, ".log");

	/* check if the file exists.
	   if it does and we need to make a new one, open one and the  immediately close it. else, we found an error*/
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
	c->unum += 1;
}

void check_user(channel* c, int uid){
	for(int i = 0; i < c->unum; i++){
		if(c->users[i].uid == uid){
			printf("user %d: name %s, at %d characters into channel\n", c->users[i].uid, c->users[i].name, c->users[i].pointer);
			return;
		}
	}
}

int log_message(channel* c, int uid, char* msg){
	char* finalmsg;
	uchinfo user;
	getuser(&user, c, uid);

	int fmsize = strlen(msg)+1;
	fmsize+= strlen(user.name) + strlen(": \n");
	finalmsg = malloc(fmsize);
	for(int i = 0; i < fmsize; i++){
		finalmsg[i] = 0;
	}

	strcpy(finalmsg, user.name);
	strcat(finalmsg, ": ");
	strcat(finalmsg, msg);
	strcat(finalmsg, "\n");

	fputs(finalmsg, c->fp);
}

int checkfile(char* name){
	return access(name, F_OK) == 0;
}

void getuser(uchinfo* dest, channel* from, int uid){
	for(int i = 0; i < from->unum; i++){
		if(from->users[i].uid == uid){
			*dest = from->users[i];
			return;
		}
	}
}
