//fork3Ex.c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//start부터 end까지 더해서 result로 리턴함
int adder (int start, int end)
{	
	int result = 0;
	for( ; start<=end; start++)
		result += start;
	return result;
}

int main (void)
{
	pid_t pid;
	int status;
	int result = 0;

	pid = fork();

	//can't create child proc
	if(pid < 0)
	{
		printf("fork() 생성 실패\n");
		return -1;
	}
	//child proc
	else if (pid == 0) 
	{
		printf("자식 proc : %d \n", pid);
		result = adder(100001,200000);
		printf("부모 result : %d\n", result);
	}
	//parent proc
	else //(pid>0)
	{
		printf("부모 proc에서 자식 proc :  %d\n", pid);	
		result = adder(1,100000);
		printf("자식 result : %d\n", result);
		
		//자식프로세스 기다리기
		wait(&status); 
	}

	return 0;
}
