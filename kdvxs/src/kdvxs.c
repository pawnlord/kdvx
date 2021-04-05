#include <stdio.h>
#include "server.h"

int main(int argc, char **argv){
	printf("------------WELCOME------------\n");
	printf("             kdvxs             \n");
	printf("a server for the kdvx protocol.\n");
	printf("  this is a decentralized chat \n");
	printf("  server for people who want a \n");
	printf("   more 'modern' feel to IRC.  \n");
	start_server(8080);
}
