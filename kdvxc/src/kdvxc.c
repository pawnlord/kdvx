#include <stdio.h>
#include "protocol.h"


int main(){
	printf("KDVXC------------\n");
	printf("Standard, open-source client for kdvx\n");
	printf("version -- 0.0.1\n");
	int sock = kdvx_connect(8080, "127.0.0.1");
	char* buff;
	kdvx_msg(sock, "general", "hello!!");
	kdvx_read(sock, "general", &buff);
	printf("%s\n", buff);
}
