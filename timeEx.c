#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

int main (void)
{
    time_t UTCtime;     // 틱 읽을 타임변수
    struct tm *tm;      // 변환시킬 tm구조체포인터 
    char buffer[BUFSIZ];// 사용자 문자열로 시간정보를 저장하기위한 문자열버퍼

    
    // STEP 1. 커널에서 시간정보를 읽어서 틱값을 time(타임변수형의 주소값) 변수에 넣어준다. 
    time(&UTCtime);
    printf("time tik: %u\n", (unsigned)UTCtime);

    // STEP 2. UTCtime 틱을 로컬타임으로 변환하는 함수.
    tm = localtime (&UTCtime);

    // STEP 3. 원하는 방식으로 변환 출력
    // asctime(tm구조체 포인터)
    printf("ascii time : %s\n", asctime(tm));

    // 원하는포맷으로 변환하는 함수 
    // strftime(문자열버퍼, 버퍼사이즈 , 형식 %Y년, %m월 %d일, %a요일, %H%M%S 시분초, 받아올 tm구조체포인터)
    strftime(buffer, sizeof(buffer), "[%Y-%m-%d] %a, %H:%M:%S" , tm);
    printf("strftime : %s\n",buffer);
    
    return 0;
}