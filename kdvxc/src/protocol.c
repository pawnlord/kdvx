#include "protocol.h"

int kdvx_connect(int port, char *address){
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char *hello = "Hello from client";
	char buffer[1024] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}
	return sock;
}



int kdvx_read(int sock, char *channel, char **buffer){
	*buffer = malloc(2048);
	char *msg = malloc(100);
	sprintf(msg, "read %s:", channel);
	send(sock, msg, strlen(msg), 0);
	int valread = read(sock, *buffer, 2048);
	(*buffer)[valread] = 0;
}

int kdvx_msg(int sock, char *channel, char *msg){
	int size = 8 + strlen(channel) + strlen(msg); // constant_characters + channel length + message length
	char *final_msg = malloc(size);
	char* buffer = malloc(2048);
	sprintf(final_msg, "msg %s:%s", channel, msg);
	send(sock, final_msg, strlen(final_msg), 0);
	int valread = read(sock, buffer, 2048);
	int succ = strcmp(buffer, "succ 1:")==0;
	free(buffer);
	return succ;
}
