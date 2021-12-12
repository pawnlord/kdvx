#include "textmanager.h"


/* generic file checker */
int checkfile(char* name);

/* get a uchinfo structure for logging purposes */
int getuser(uchinfo* dest, channel* from, int uid);

/* directly change users pointer variable */
void changeuserptr(channel* from, int uid, int pointer);

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
	fseek(c->fp, 0L, SEEK_END);

	c->unum = 0;

	c->users = (uchinfo*)malloc(1000);
	return 1;
}

void add_user(channel* c, int uid, char* name){
	c->users[c->unum].uid = uid;
	c->users[c->unum].name = malloc(strlen(name)+1);
	strcpy(c->users[c->unum].name, name);
	printf("%d\n", c->unum);
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
	if(!getuser(&user, c, uid)){
		printf("No User Found for %d\n", uid);
		return 0;
	}

	/* remove bad characters from message */
	int j = 0;
	for(int i = 0; msg[i] != 0; i++){
		if(msg[i] >= 32 && msg[i] <= 126){
			msg[j] = msg[i];
			j++;
		}
	}
	msg[j] = 0;
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
	printf("%s\n", finalmsg);

	fputs(finalmsg, c->fp);

	fflush(c->fp);
	return 1;
}

int read_new(channel* c, int uid, char** outp){
	/* setup user */
	uchinfo user;
	if(!getuser(&user, c, uid)){
		return 0;
	}

	/* setup output */
	fseek(c->fp, 0L, SEEK_END);
	int sz = ftell(c->fp);
	fseek(c->fp, user.pointer, SEEK_SET);

	(*outp) = malloc(sz - user.pointer + 1);
	char* out = *outp;

	/* read the file from user.pointer into out*/
	int end = fread(out, 1, sz-user.pointer+1, c->fp);

	changeuserptr(c, uid, sz);

	printf("%s\n", out);

	return 1;
}

int checkfile(char* name){
	return access(name, F_OK) == 0;
}

int getuser(uchinfo* dest, channel* from, int uid){
	for(int i = 0; i < from->unum; i++){
		if(from->users[i].uid == uid){
			*dest = from->users[i];
			return 1;
		}
	}
	return 0;
}

void changeuserptr(channel* from, int uid, int pointer){
	for(int i = 0; i < from->unum; i++){
		if(from->users[i].uid == uid){
			from->users[i].pointer = pointer;
			return;
		}
	}
}
