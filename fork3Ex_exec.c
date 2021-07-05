//fork3Ex.c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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
		//시스템콜 ls
		//execl("/bin/ls", "ls", "-al", NULL);
		//c파일 실행
		//execl("/bin/test","test",NULL);
		//파이썬 실행
		execl("/usr/bin/python", "python","hello.py", NULL);
	}
	//parent proc
	else //(pid>0)
	{
		printf("부모 proc에서 자식 proc :  %d\n", pid);	
		
		//자식프로세스 기다리기
		wait(&status); 
	}

	return 0;
}
