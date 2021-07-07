//killTx.c
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ./killTx [SIGNAL Number] [pid]
// argc = 3
// argv[1] = SINGAL Number //argv[2] = pid
// "--help" : 사용방법 안내
int main (int argc, char *argv[])
{
    pid_t pid;
    int sigNum;
    int result;

    //인자가 3개가 아니거나 , $./killTx --help
    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        printf("%s sig-num pid\n" , argv[0]);
    
    //pid를 숫자로변환
    pid = atoi(argv[2]);
    sigNum = atoi (argv[1]);
    
    //내가 보내고자하는 프로세스(pid)에게 이 시그널발생
    //정상종료라면 result == 0
    result = kill(pid, sigNum);
    if (result != 0)
        printf("Error:system call kill().\n");
    else
        printf("Complete:system call kill().\n");

    return 0;
}