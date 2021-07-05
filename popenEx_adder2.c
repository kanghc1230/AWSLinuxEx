#include <stdio.h>

int main(void)
{
	int i = 0;
	long result = 0;
	for(i=100001; i<=200000; i++)
		result += i;
	printf("%ld",result);
	return 0;
}

