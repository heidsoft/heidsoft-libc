/* cocurrent server
 * serverfork.c
 */

#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>

#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

/*
void sig_child(int signo)
{
	while (waitpid(-1, NULL, WNOHANG) > 0)
		;
	return;
}
*/

int port = 8000;
int main(void)
{
	struct sockaddr_in sin, pin;
	int sock_descriptor, temp_sock_descriptor;
	socklen_t address_size;
	int i, len, pid, on=1;
	char buf[16384];

	//signal(SIGCHLD, sig_child);

	sock_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_descriptor == -1) {
		perror("call to socket");
		exit(1);
	}
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(port);
	//sin.sin_port = port;
	setsockopt(sock_descriptor,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	if (bind(sock_descriptor, (struct sockaddr*)&sin, sizeof(sin)) == -1) {
		perror("call to bind");
		exit(1);
	}
	if (listen(sock_descriptor, 20) == -1) {
		perror("call to listen");
		exit(1);
	}
	printf("Accepting connections ...\n");

	while(1) {
		temp_sock_descriptor = accept(sock_descriptor, (struct sockaddr *)&pin, &address_size);
		if (temp_sock_descriptor == -1) {
			perror("call to accept");
			exit(1);
		}
		if ((pid=fork()) < 0){
			perror("fork");
			exit(1);
		} else if (pid == 0) { //child
			close(sock_descriptor);
			if (recv(temp_sock_descriptor, buf, 16384, 0) == -1) {
				perror("call to recv");
				exit(1);
			}
			printf("received from client: %s\n", buf);
      /* for this server example, we just convert the characters to upper case: */
			len = strlen(buf);
			for (i=0; i<len; i++)
				buf[i] = toupper(buf[i]);
			if (send(temp_sock_descriptor, buf, len + 1, 0) == -1) {
				perror("call to send");
				exit(1);
			}
			sleep(60);
			close(temp_sock_descriptor);
			exit(0);
		}
		close(temp_sock_descriptor);
	}
}
