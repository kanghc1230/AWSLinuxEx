#include<stdio.h>
#include<unistd.h> //pid_t, getpid(), sleep()

int main(void)
{ 
    pid_t pid,ppid;//pid를 받는 자료형 pid_t 
    pid = getpid();//현재 자신의 프로세스 id를 읽어온다
    ppid = getppid();

    printf("my pid = %d\n", pid);
    printf("my ppid = %d\n", ppid);
    sleep(10);


}

