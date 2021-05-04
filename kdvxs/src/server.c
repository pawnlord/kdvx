#include "server.h"
#define LOG_SIZE 10000
#define IGNORE 0
#define ERR_RECIEVED -1
#define SUCCESSFUL 1

channel c;

void handle(int failed, char* process){
	if(failed){
		perror(process);
		exit(EXIT_FAILURE);
	}
}

/* main loop function */
int main_loop(int socket, char* buffer, int id);

/* print out a command for debugging purposes*/
void print_command(char** out, int argc);
/* main loop starter that conforms with pthread_create */
void* main_loop_thread(void *vargp);

/* parse a recieved command */
int get_command(char* in, char*** out);

/* get only the command name from in */
int get_simple_command(char* in, char** out);

/* use command once found */
int use_command(char** comm, int size, int sock, char** retmsgp);

int check_success(int sock);

int start_server(int port){
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	text = malloc(LOG_SIZE);
	for(int i = 0; i < LOG_SIZE; i++){
		text[i] = 0;
	}
	// Creating socket file descriptor
	handle((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0, "socket");

	// Forcefully attaching socket to the port 8080
	handle(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
	                                          &opt, sizeof(opt)), "setsocketopt");

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( port );

	// Forcefully attaching socket to the port 8080
	handle(bind(server_fd, (struct sockaddr *)&address, 
	                         sizeof(address))<0, "bind");

	/* initialize channel */
	init_channel(&c, "general", 1);

	handle(listen(server_fd, 30) < 0, "listen");
	while(1){
		handle((new_socket = accept(server_fd, (struct sockaddr *)&address, 
	        	       (socklen_t*)&addrlen))<0, "accept");
		pthread_t tid;
		pthread_create(&tid, NULL, main_loop_thread, (void*)&new_socket);
	}
	return 0;

}

void* main_loop_thread(void *vargp){
	static int id = 0;
	int this_id = id;
	id += 1;
	int loop = 1;
	int socket = *(int*)vargp;
	char* buffer = malloc(1024);

	for(int i = 0; i < 1024; i++){
		buffer[i] = 0;
	}

	while(loop){
		loop = main_loop(socket, buffer, this_id);
	}
	free(buffer);
}

int main_loop(int socket, char* buffer, int id) {
	int valread = read(socket, buffer, 1024);
	buffer[valread] = 0;
	printf("%d . \"%s\"\n", id, buffer);
	sprintf(text, "%s%d: %s\n", text, id, buffer);

	char** out;
	int size = get_command(buffer, &out);
	print_command(out, size);
	char* errmsg;
	if(use_command(out, size, socket, &errmsg) != ERR_RECIEVED){
		int i;
		for(i = 0; i < 1024; i++){
			if(i > strlen(text)){
				buffer[i] = 0;
				break;
			}
			buffer[i] = text[(int)fmax(0, (int)strlen(text)-1024)+i];
		}

		buffer[i] = 0;
	} else {
		strcpy(buffer, errmsg);
	}

	/* cleanup */
	for(int i = 0; i < size; i++) {
		free(out[i]);
	}
	free(out);
	free(errmsg);

	/* final send, end loop */
	send(socket, buffer, strlen(buffer), 0);
	return valread;
}

int use_command(char** comm, int size, int sock, char** retmsgp) {
	(*retmsgp) = malloc(1024);
	char* retmsg = *retmsgp;
	if(size < 1){
		printf("No command, skipping..\n");
		return IGNORE;
	}
	if(strcmp(comm[0], "take") == 0){
		if(size < 2){
			strcpy(retmsg, "err 6: Not Enough Arguments Provided");
			return ERR_RECIEVED;
		}
		FILE* fp = fopen(comm[1], "r");
		char* buff = malloc(1024);

		/* Get size of file */
		fseek(fp, 0, SEEK_END);
		int fsize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		int currp = 0, currerrs = 0, success = SUCCESSFUL;

		while(currp < fsize+1024){
			if(success == SUCCESSFUL) {
				int end = fread(buff, 1, 1024, fp);
				buff[end] = 0;
			}
			send(sock, buff, strlen(buff), 0);
			success = check_success(sock);
			if(success == ERR_RECIEVED){
				currerrs += 1;
				if(currerrs > 20) break;
			} else {
				currp += 1024;
				currerrs = 0;
			}
		}
		if(currerrs > 20){
			printf("AAAAA\n");
			strcpy(retmsg, "20 Failed attempts, aborting");
		}
		return (currerrs > 20)? ERR_RECIEVED:SUCCESSFUL;
	} else if(strcmp(comm[0], "send") == 0) {
		if(size < 2){
			strcpy(retmsg, "err 6: Not Enough Arguments Provided");
			return ERR_RECIEVED;
		}
		char* filename = comm[1];
		/*if( access(filename, F_OK) == 0){
			strcpy(retmsg, "err 8: File already Exists");
			return ERR_RECIEVED;
		}*/
		printf("%s", filename);
		FILE* fp = fopen(filename, "w");
		char *newcomm = malloc(255), *buffer = malloc(1024);
		strcpy(buffer, "succ 2:");
		send(sock, buffer, strlen(buffer), 0);
		while(1){
			int valread = read(sock, buffer, 1024);
			buffer[valread] = 0;

			get_simple_command(buffer, &newcomm);
			if(strcmp(newcomm, "dnes") == 0) break;
			fputs(buffer, fp);

			printf("%d, %s\n", valread, buffer);

			strcpy(buffer, "succ 2:");
			send(sock, buffer, strlen(buffer), 0);
		}
		free(newcomm);
		free(buffer);
		fclose(fp);
		return SUCCESSFUL;
	}
	strcpy(retmsg, "err 7: Not a command");
	return ERR_RECIEVED;

}

int check_success(int sock){
	char* buff = malloc(1024);
	char** out;
	int valread = read(sock, buff, 1024);
	int size = get_command(buff, &out);
	free(buff);
	if(size < 1 || strcmp(out[0], "succ")==0){
		for(int i = 0; i < size; i++){
			free(out[i]);
		}
		free(out);
		return SUCCESSFUL;
	}
	for(int i = 0; i < size; i++){
		free(out[i]);
	}
	free(out);
	return ERR_RECIEVED;
}

int get_command(char* in, char*** out){
	int out_word = 0;
	int out_char = 0;
	(*out) = malloc(32);
	(*out)[0] = malloc(255);
	for(int i = 0; i < 255; i++){
		(*out)[0][i] = 0;
	}
	for(int i = 0; in[i] != 0 && in[i] != 0; i++){
		if(in[i] == ':'){
			break;
		} else if(in[i] == ' ') {
			out_word++;
			out_char = 0;
			(*out)[out_word] = malloc(255);
 			for(int j = 0; j < 255; j++){
				(*out)[out_word][j] = 0;
			}
		} else{
			(*out)[out_word][out_char] = in[i];
			out_char++;
		}
	}
	return out_word+1;
}

int get_simple_command(char* in, char** out){
	if(strchr(in, ':') == NULL){
		strcpy(*out, "");
		return 0;
	}
	for(int i = 0; in[i] != 0; i++){
		if(in[i] == ' ' || in[i] == ':'){
			int temp = in[i];
			in[i] = 0;
			strcpy(*out, in);
			printf("found: %s\n", *out);
			in[i] = temp;
			return 1;
		}
	}
	return 0;
}

void print_command(char** out, int argc){
	for(int i = 0; i < argc; i++){
		printf("%d -- %s\n", i, out[i]);
	}
}


