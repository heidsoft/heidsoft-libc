#include <stdio.h>

int main(int argc ,char* argv[])
{
	struct node{
		int i;
		struct node *p;
	};
	
	printf("sizeof(struct node)=%d\n",sizeof(struct node));
	return 0;
}
