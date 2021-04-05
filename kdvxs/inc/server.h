#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include "text_server.h"
#include "voice_server.h"
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>

int start_server(int port);

#endif
