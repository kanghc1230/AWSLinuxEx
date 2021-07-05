//forkEx.c
#include <stdio.h>
#include <unistd.h>//pid_t 프로세스ID값 저장 데이터타입
#include <stdlib.h>//문자열처리
#include <sys/types.h>	//wait() sleep()
#include <sys/wait.h>	//wait() sleep()
#include <fcntl.h> //open

#define BUFF_SIZE 1024

int main (void)
{	int status;
	int i = 3;
	pid_t pid;
	
	int fd;
	char buff[BUFF_SIZE];
	int rdByte;	//read예외처리변수

	//file descriptor table
	// 0 : stdin, 1:stdout, 2:stderr 프로세스생성과함께 시스템이 자동생성
	fd = open("a.txt", O_RDONLY); //fd -> 3:예상
	//파일읽기실패시 예외처리
	if(fd<0)
	{
		printf("Can't open \n");
		return 0;
	}
	
	//fork() 자식프로세스 생성 분리시작.
	pid = fork();
	
	//fork()가 정상적으로 실행되지 않은경우
	if (pid<0)
	{
		printf("Can't create child process \n");
		return 0;
	}

	//부모 프로세스
	if (pid>0)
	{
		printf("부모 프로세스 %d : %d\n", getpid(), pid);
		printf("부모i = %d\n", i);

		rdByte = read(fd, buff, BUFF_SIZE);
		if (rdByte<0)
			printf("can't read file\n");
		else
			printf("read : %s\n", buff);
		//wait(&status);

	}
	//자식 프로세스
	else if (pid==0){
		sleep(1);//wait뺀부모가 자식보다 먼저 종료할경우 끝나고출력
		printf("자식 프로세스 %d \n", getpid());
		printf("자식i = %d \n", i);
		
		//read출력
		rdByte = read(fd, buff, BUFF_SIZE);
                if (rdByte<0)
                        printf("can't read file\n");
                else
                        printf("read : %s\n", buff);

	}
	
	return 0;
}
