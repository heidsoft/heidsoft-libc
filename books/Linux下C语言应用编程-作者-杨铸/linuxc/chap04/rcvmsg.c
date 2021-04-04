#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

int main(void)
{
	int msgid, length;
	struct mymesg {
		long mtype;
		char mdata[512];
	} message;
	if ((msgid = msgget(3099, 0666 | IPC_CREAT)) < 0)
		{ perror("msgget"); exit(1); };
	//strcpy(message.mdata, "hello\n");
	if ((length = msgrcv(msgid, (void *)&message, 512, 0, 0)) < 0)
		{ perror("msgrcv"); exit(2); }
	printf("received message type is %ld\n", message.mtype);
	printf("received message length is %d\n", length);
	printf("received message : %s\n", message.mdata);
	return 0;
}
