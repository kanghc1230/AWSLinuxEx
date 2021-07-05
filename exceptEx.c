#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>

void pr_exit(int status)
{
	//1. 정상적인 종료
	if(WIFEXITED(status))
	//WIFEXITED(Status)는 정상적종료면 true리턴
		printf("자식정상적인 종료, 종료조건 %d\n", WEXITSTATUS(status));
		//WEXITSTATUS(status)는 자식의 값을 리턴하는 함수
	//2. 시그널에 의해 종료
	else if(WIFSIGNALED(status))
		printf("시그널에 의한 비정상 종료, 종료조건 %d %s\n", 
		WTERMSIG(status), WCOREDUMP(status)? "core file gernerated":"");
	//3. 시그널에 의해 정지
	else if(WIFSTOPPED(status))
		printf("자식 프로세스가 STOPPED %d\n", WSTOPSIG(status));
}

int main (void)
{
	pid_t pid, end_pid;
	int status;
	int fd;
	/*
	//perror()함수의 사용법
	fd = open("aaa.txt",O_RDONLY);
	if (fd<0)
		perror("Can't open file"); 
		//Can't open file: No such file or directory
		//내가 출력메세지: 시스템 에러 메세지
	*/
	
	
	//fork 1번 : 정상적인 종료
	pid = fork();

	//자식proc 생성  실패
	if(pid<0)
	{
		// 2:stderr 로 메세지 출력하는 perror
		perror("정상적으로 fork()가 실행되지 않았습니다.");
	}
	//부모proc 실행되는 부분
	else if(pid>0)
	{
		//
	}
	//자식proc 실행되는 부분 
	else 
	{
		//exit(7);
		return 7;
	}

	//wait()함수가 호출시, status변수의 주소값을 전달하면
	//call-by-reference. status에 wait가 종료조건을 적어준다.
	if(wait(&status)==pid)	//반환된 값이 호출한 자식proc pid와 일치하면
	{
		pr_exit(status); //1.정상적으로 종료
	}
	
	/*2번째 fork()*/
	//SIGABART 시그널을 발생시켜서 종료
	if((pid = fork()) <0)
		perror("fork eeror");
	//자식프로세스
	else if (pid==0)
	{	
		//SIGABRT 시그널 발생시키기
		abort(); //2.시그널6으로 종료
	}

	if (wait(&status)==pid)
	{
		pr_exit(status);
	}

	/*3번째 fork()*/
	//SIGFPE:부동 소수점 나눗셈 ERROR
        if((pid = fork()) <0)
                perror("fork eeror");
        //자식프로세스
        else if (pid==0)
        {
                //불가능한 0으로 나눈값이 status가 뭐가나올지
                status /=0; //시그널8로 종료
        }

        if (wait(&status)==pid)
        {
                pr_exit(status);
        }


	return 0; //프로세스가 어떻게 종료되는지 (return 0~255 정상적인 종료)
}
