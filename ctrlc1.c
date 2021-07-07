#include <signal.h>
#include <stdio.h>
#include <unistd.h>

//SIGINT 핸들러 (인자로 시그번호 들어옴)
void sigHandler(int sig)
{
    printf("OUCH! - I got signal %d\n", sig);
    //SIGINT를 다시 디폴트로 바꾸는방법
    //(void)signal(SIGINT, SIG_DFL);    
}

int main(void)
{
    // signal을 활용하기 위해선 signal(원하는시그널, 정의핸들러함수) 정의
    // SIGINT(ctrl+c)라는 시그널이 들어오면 sigHandler()로 점프해서 이동해라(커널에게)
    signal(SIGINT, sigHandler); //sig

    //실험용 초당1번루프
    while(1)
    {
        printf("hello world\n");
        sleep(10); //시그널과 sleep(10);이 만나면 시간을 보장안함.
    }

    return 0;
}
