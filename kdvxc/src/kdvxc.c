#include <stdio.h>
#include <stdlib.h>
#include "clio.h"
#include "protocol.h"
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

/* shift buff left by n */
void shift(char* buff, int n){
	for(int i = 0; i < (strlen(buff)-n); i++){
		buff[i] = buff[i+n];
	}
	buff[strlen(buff)-n] = 0;
}


int main(){
	startbuff();
	printf("KDVXC------------\n");
	printf("Standard, open-source client for kdvx\n");
	printf("version -- 0.0.1\n");
	int sock = kdvx_connect(8080, "127.0.0.1");
	printf("connected to %d\n", sock);	
	char* buff;
	char* inp = malloc(300);
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	int possible_max = w.ws_col*(w.ws_row-4);
	char* relevant_log = malloc(possible_max);
	for(int i = 0; i < possible_max; i++){
		relevant_log[i]=0;
	}
	while(strcmp(inp, "exit")!=0){
		char newc = input(inp);
		clear();
		if(newc == '\n'){
			kdvx_msg(sock, "general", inp);
			inp[0]=0; // reset input
		}
		kdvx_read(sock, "general", &buff);
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
		int width = w.ws_col, height = w.ws_row;
		gotoxy(1,1);
		printf("%s", relevant_log);
		gotoxy(1, height-2);
		printf("%s", inp);
		if(strlen(relevant_log) + strlen(buff) >= possible_max-2) { /* if we are out of space, delete some previous log. 2046 for a buffer */
			shift(relevant_log, strlen(buff));
		}
		if(strcmp(buff, "succ 1:") != 0) { // ignore general response
			strcat(relevant_log, buff);
		}
		int line_count = 0;
		int to_shift = 0;
		for(int i = 0; relevant_log[i] != 0; i++){
			if(relevant_log[i] == '\n'){
				line_count++;
			}
			if(line_count >= height-4){
				to_shift+=1;
			}
		}
		shift(relevant_log, to_shift);
	}
	endbuff();
}
