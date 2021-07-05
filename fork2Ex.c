//fork2Ex.c
#include <stdio.h>
#include <unistd.h> //pid._t
#include <stdlib.h> //문자열처리
#include <sys/types.h>	//wait()
#include <sys/wait.h>	//wait()

int global = 10;
int main(void)
{
	int i = 20;
	pid_t pid;
	int status;

	pid = fork();
	
	//can't create child proc 
	if (pid < 0)
	{
		printf("fork()생성 실패\n");
		return 0;
	}
	//child proc
	else if (pid ==0)
	{
		global = global + 10;
		i = i + 10;
	}
	//parent proc
	else//(pid>0)
	{
		wait(&status);
		global = global + 100;
		i = i + 100;
	}
	printf("global = %d , i = %d \n", global , i);
	
}
