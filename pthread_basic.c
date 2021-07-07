//pthread_basic.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int global_val = 3;

// 쓰레드 함수 생성할때 인자는 void*형태, 리턴값 void*형태
void* t_function(void *data) //인자로 a,b주소가옴 // 인자2개면 구조체 myData주소를 받으면됨
{
    int value;
    // 쓰레드의 ID값을 확인
    global_val++;
    pthread_t t_id;
    t_id = pthread_self();
    printf("pid = %d , t_id = %d , global_val = %d\n", getpid(), t_id, global_val);
    
    // 인자로 void포인터로 받아, int포인터로 전환후에 값(*)으로 value에 저장 
    value = *((int *)data);
    printf("value = %d\n",value);

    return (void*)value; //value 즉 값으로 전달해야함. 함수가 주소로 전달하면 날아가버림
}

int main(void)
{
    // 쓰레드를 id받을 쓰레드변수 2개 배열로 생성
    pthread_t pthread[2]; //프로세스에서 pid_t 역할
    
    int a = 1;
    int b = 2;
    int err;
    int status;

    printf("현재 PID = %d , 전역변수 global_val = %d\n" ,getpid(), global_val);

    // 쓰레드 2개 생성
    // pthread_create( 인자4개.
    // 1st : thread변수의 주소값, //call-by-reference
    // 2rd : thread의 attribute, 
    // 3nd : thread생성시 쓰레드함수 주소값,
    // 4th : thread생성시 쓰레드함수에 전달할 인자값 void*주소 //call-by-reference )
    err = pthread_create(&pthread[0], NULL, t_function, (void*)&a );
    if(err != 0) //쓰레드생성실패
    {
        perror("ERROR:pthread_create(0)\n");
        exit(-1);
    }
    err = pthread_create(&pthread[1], NULL, t_function, (void*)&b );
    if(err != 0)
    {
        perror("ERROR:pthread_create(1)\n");
        exit(-2);
    }

    // 쓰레드를 wait()종료대기
    pthread_join(pthread[0], (void**)&status); //조인함수가 status에 쓰레드함수의 리턴값이 들어옴
    printf("thread_join(0) : %d\n",status);

    pthread_join(pthread[1], (void**)&status);
    printf("thread_join(1) : %d\n",status);


}