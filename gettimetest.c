//pthread_Num
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

//전달할 구조체 선언
struct Data{
    int start;
    int end;
    long long result; //쓰레드에서 받아올
};
//쓰레드 함수내에서 계산할 start부터 end까지의 합
long long adder (int start, int end)
{
    long long result = 0;
    for( ; start<=end ; start++)
        result += start;

    return result;
}
//쓰레드 함수
void* t_function(void *data)
{
    long long result = 0;
    struct Data *value = (struct Data*)data;
    (*value).result = adder((*value).start, (*value).end);

    return (void*)0;
}

int main (void)
{
    // 쓰레드에 포인터로 전달
    // pthread_STtest.c 일반 스트럭쳐 전달후 리턴
    struct Data *t1 = malloc (sizeof(struct Data));
    struct Data *t2 = malloc (sizeof(struct Data));
    long long result = 0;
    long long status;
    int err;
    pthread_t pthread[2];

    (*t1).start = 1;
    (*t1).end = 1000000000;
    (*t2).start = 1000000001;
    (*t2).end = 2000000000;
    // printf ("struct *t1 : %p, struct t1 : %d\n", t1, t1->start);
    
    struct timeval timetest_s, timetest_e;
    gettimeofday(&timetest_s, NULL);


    //쓰레드 생성
    err = pthread_create(&pthread[0], NULL, t_function, (void*)t1);
    if (err != 0)
    {
        printf("ERROR create t1\n");
        return -1;
    }
    err = pthread_create(&pthread[1], NULL , t_function, (void*)t2);
    if (err != 0)
    {
        printf("ERROR create t2\n");
        return -2;
    }

    //쓰레드 지우면서 반환받기
    err = pthread_join(pthread[0], (void**)&status);
    if(err!=0) return -1;
    err = pthread_join(pthread[1], (void**)&status);
    if(err!=0) return -2;

    result = t1->result + t2->result;
    printf ("result = %lld\n", result);
    
    gettimeofday(&timetest_e, NULL);
    printf("런타임:%ld.%ld \n", timetest_e.tv_sec - timetest_s.tv_sec , timetest_e.tv_usec - timetest_s.tv_usec );
    //ms를나타내는 usec에서 음수처리 발생 음수시 sec에서 -1초, usec은 양수로*(-1)

    return 0;
}