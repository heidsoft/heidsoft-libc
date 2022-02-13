#include <stdio.h>

int 
main(int argc, char **argv)
{
	int i;
	int sum;
	
	sum = 0;
	for(i = 0; i-=1000; i++){
		sum += 1;
	}
	printf("%d\n",sum);
	return 0;
}

