#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

int main(void)
{
	int addrnum;
	char addrstring[16] = "192.168.6.100";
	printf("address in dotted-quad format is %s\n", addrstring);
	inet_pton(AF_INET, addrstring, &addrnum);
	printf("address in network byteorder integer is 0x%x\n", addrnum);

	char addrstring2[16] = "2.2.2.2";
	if (inet_ntop(AF_INET, &addrnum, addrstring2, 16) == NULL)
		perror("inet_ntop");
	printf("address in dotted-quad format is %s\n", addrstring2);
/*
	strcpy(addrstring, "www.ifl.com");
	if (inet_pton(AF_INET, addrstring, &addrnum) > 0) {
		printf("address in network byteorder integer is 0x%x\n", addrnum);
	} else {
		printf("convert failed\n");
	}
	struct hostent *host;
	char **p;
	host = gethostbyname(addrstring);
	printf("hostname is %s\n", host->h_name);
	printf("host alias is : \n");
	for (p = host->h_aliases; *p != NULL; p++)
		printf("%s\n", *p);
	printf("host address length is %d\n", host->h_length);
	printf("host address is : \n");
	for (p = host->h_addr_list; *p != NULL; p++)
		//printf("0x%x\n", ((struct in_addr *)(*p))->s_addr);
		printf("0x%x\n", *(int *)(*p));
*/
	return 0;
}

