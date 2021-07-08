// 멀티 스레드 환경 구성
// 뮤텍스를 사용해 공유변수를(메모리공간) 동시접근못하게

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>//pthread
#include <sched.h>  //mutex

// mutex 에서 테스트할 변수
static int count = 10;

// mutex이름 countlcok = 전역변수로 선언후 초기화
static pthread_mutex_t countlock = PTHREAD_MUTEX_INITIALIZER;

// count 값을 증가시키는 스레드함수
void* t_countAdd(void *data)
{
    int err;
    while(1)
    {
        // lock이 걸려있으면 다른 쓰레드가 unlock할때가지 작동안함.
        err = pthread_mutex_lock(&countlock);
        if (err)
            printf("ERROR : Add lock \n");
        
        // Critical Section
        count++;
        // unlock
        err = pthread_mutex_unlock(&countlock);
        if (err)
            printf("ERROR : Add lock \n");
        
        // mutex 동작을 원하는 다른 스레드에게 우선순위 양보
        err = sched_yield();
        if (err != 0)
            printf("ERROR : Add yeild\n");
        usleep(1000);
    }   
}
// count 값을 감소시키는 스레드함수
void* t_countMin(void *data)
{
     int err;
    while(1)
    {
        // lock이 걸려있으면 다른 쓰레드가 unlock할때가지 작동안함.
        err = pthread_mutex_lock(&countlock);
        if (err)
            printf("ERROR : Min lock \n");
        
        // Critical Section
        count--;
        // unlock
        err = pthread_mutex_unlock(&countlock);
        if (err)
            printf("ERROR : MIn lock \n");
        
        // mutex 동작을 원하는 다른 스레드에게 우선순위 양보
        err = sched_yield();
        if (err != 0)
            printf("ERROR : Min yeild\n");
        usleep(1000);
    }   
}

// count 값을 출력하는 스레드함수
void* t_countGet(void *data)
{
 int err;
 int value;
    while(1)
    {
        // lock이 걸려있으면 다른 쓰레드가 unlock할때가지 작동안함.
        err = pthread_mutex_lock(&countlock);
        if (err)
            printf("ERROR : Get lock \n");
        
        // Critical Section
        value = count;
        // unlock
        err = pthread_mutex_unlock(&countlock);
        if (err)
            printf("ERROR : Get lock \n");
        
        // mutex 동작을 원하는 다른 스레드에게 우선순위 양보
        err = sched_yield();
        if (err != 0)
            printf("ERROR : Get yeild\n");

        sleep (1);
        printf("count = %d\n", value);
    }   
}

int main (void)
{
    // thread id값을 저장할 변수 3개
    pthread_t pthread[2];
    int err;

    //뮤텍스 초기화
    pthread_mutex_init (&countlock, NULL);

    //쓰레드생성, 동작해라 함수를
    err = pthread_create(&pthread[0], NULL, t_countAdd , NULL );
    if(err!=0) 
    {
        perror("Add error\n"); exit(-1);
    }

    err = pthread_create(&pthread[1], NULL, t_countMin , NULL );
    if(err!=0) 
    {
        perror("Min error\n"); exit(-1);
    }

    err = pthread_create(&pthread[2], NULL, t_countGet , NULL );
    if(err!=0) 
    {
        perror("Get error\n"); exit(-1);
    }

    pthread_detach(pthread[0]);
    pthread_detach(pthread[1]);
    pthread_detach(pthread[2]);

    while(1);

    return 0;
}