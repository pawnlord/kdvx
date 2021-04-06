#include "server.h"
#define LOG_SIZE 10000

char* text;

void handle(int failed, char* process){
	if(failed){
		perror(process);
		exit(EXIT_FAILURE);
	}
}

int main_loop(int socket, char* buffer, int id);
void* main_loop_thread(void *vargp);

int start_server(int port){
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	char *hello = "Hello from server";
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
}

int main_loop(int socket, char* buffer, int id){
	int valread = read(socket, buffer, 1024);
	buffer[valread] = 0;
	printf("%s: %d\n", buffer, valread);
	sprintf(text, "%s%d: %s\n", text, id, buffer);
	printf("succsessful cat\n");
	int i;
	for(i = 0; i < 1024; i++){
		if(i > strlen(text)){
			buffer[i] = 0;
			break;
		}
		printf("%d\n", strlen(text)-1024);
		buffer[i] = text[(int)fmax(0, (int)strlen(text)-1024)+i];
	}
	printf("buffer: %s\n", buffer);
	buffer[i] = 0;
	send(socket, buffer, strlen(buffer), 0);
	return valread;
}

int get_command(char* in, char** out){
	// STUB
}

