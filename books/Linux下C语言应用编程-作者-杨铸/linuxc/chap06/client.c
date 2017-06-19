/* P329
 * client.c
 */
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

char *host_name = "127.0.0.1";
//char *host_name = "192.168.6.9";  //local host
//char *host_name = "www.ifl.com";

int port = 8000;

int main(int argc, char *argv[])
{
	char buf[8192];  //, message[256];
	int socket_descriptor;
	struct sockaddr_in pin;
//	struct hostent *server_host_name;
	char *str = "A default test string";
	
	if(argc < 2) {
		printf("Usage: client \"Any test string\" \"ip address\"\n");
		printf("We will send a default test string.\n");
	} else {
		str = argv[1];
		if (argc ==3)
			host_name = argv[2];
	}
/*
	if((server_host_name = gethostbyname(host_name)) == NULL) { 
		perror("Error resolving local host\n");
		exit(1);
	}
*/
	bzero(&pin, sizeof(pin));
	pin.sin_family = AF_INET;
	//pin.sin_addr.s_addr = ((struct in_addr *)(server_host_name->h_addr))->s_addr;
	inet_pton(AF_INET, host_name, &pin.sin_addr);
	pin.sin_port = htons(port);
	if((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Error opening socket\n");
		exit(1);
	}
	if (connect(socket_descriptor, (struct sockaddr *)&pin, sizeof(pin)) == -1) {
		perror("Error connecting to socket\n");
		exit(1);
	}
	
	printf("Sending message %s to server...\n", str);
/*
	if (send(socket_descriptor, str, strlen(str)+1, 0) == -1) {
		perror("Error in send\n");
		exit(1);
	}
*/
	if (write(socket_descriptor, str, strlen(str)+1) == -1) {
		perror("Error in send\n");
		exit(1);
	}
	printf(".. sent message.. wait for response...\n");
	//close(socket_descriptor);
	//exit(1);
/*
	if(recv(socket_descriptor, buf, 8192, 0) == -1) {
		perror("Error in receiving response from server\n");
		exit(1);
	}
*/
	if(read(socket_descriptor, buf, 8192) == -1) {
		perror("Error in receiving response from server\n");
		exit(1);
	}
	printf("\nResponse from server:\n\n%s\n", buf);
	close(socket_descriptor);
	return 1;
}
