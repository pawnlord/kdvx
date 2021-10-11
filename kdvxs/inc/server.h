#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include "textmanager.h"
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

int start_server(int port);

#endif
