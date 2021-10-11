#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

/* returns a socket */
int kdvx_connect(int port, char* address);

/* protocol commands */
int kdvx_msg(int sock, char *channel, char *msg);
int kdvx_read(int sock, char *channel, char **buffer);
int kdvx_take(int sock, char *filename, char *savename);
int kdvx_send(int sock, char *filename);
int kdvx_exit(int sock);


#endif
