/* 服务端程序 server.c */ 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <stdio.h> 
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
 
#define SERVER_PORT 8888 
#define MAX_MSG_SIZE 1024

void udps_respon(int sockfd) 
{ 
	struct sockaddr_in addr; 
	int n, i;
	unsigned int addrlen = sizeof(struct sockaddr_in); 
	char msg[MAX_MSG_SIZE]; 
	while(1) { /* 从网络上度，写到网络上面去 */ 
		n = recvfrom(sockfd, msg, MAX_MSG_SIZE, 0, (struct sockaddr*)&addr, &addrlen); 
		msg[n] = 0;
		/* 显示服务端已经收到了信息 */ 
		fprintf(stdout, "I have received %s", msg);

		n = strlen(msg);
                for(i = 0; i < n; i++)
                        msg[i] = toupper(msg[i]);
 
		sendto(sockfd, msg, n, 0, (struct sockaddr *)&addr, addrlen); 
	}
}

int main(void)
{ 
	int sockfd;
	struct sockaddr_in addr;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
	if (sockfd < 0) {
		perror("socket"); 
		exit(1);
	}
	bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(SERVER_PORT); 
	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0) { 
		perror("bind");
		exit(1); 
	}
	udps_respon(sockfd); 
	close(sockfd);
	return 0;
} 
