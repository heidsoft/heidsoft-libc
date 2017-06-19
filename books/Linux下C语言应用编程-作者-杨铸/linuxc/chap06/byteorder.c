#include <stdio.h>

int main(void)
{
	unsigned int i, num=0xab127980;
	unsigned char *pc;

	printf("num's address is %p, and it's value is 0x%x\n\n", &num, num);	
	pc = (unsigned char *)&num;
	for(i=0; i<4; i++) {
		printf("%p: 0x%x\n", pc, (unsigned int)*pc);
		pc++;
	}

	unsigned short port;
	port = 0x6789;
	printf("port number in host    byteorder is 0x%x\n", port);
	printf("port number in network byteorder is 0x%x\n", htons(port));

}
