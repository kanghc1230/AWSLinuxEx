//pthread_basic.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

struct Data{
    int start;
    int end;
};
long long adder (int start, int end)
{
    long long result = 0;
    for( ; start<=end ; start++)
        result += start;
    return result;
}
void* t_function(void* data)
{
    long long result = 0;
    struct Data *value = (struct Data*)data;
    printf ("in t_func : (*value).start = %d , (*value).end = %d\n", (*value).start, (*value).end);
    result = adder ((*value).start ,(*value).end);
    return (void*)result;
}
int main(void)
{
    // 쓰레드를 id받을 쓰레드변수 2개 배열로 생성
    pthread_t pthread[2]; 
    struct Data t1; //스트럭쳐 stack으로 보냈을때
    struct Data t2; 

    t1.start = 1;
    t1.end = 100000;
    t2.start = 100001;
    t2.end = 200000;

    int err;
    long long status;
    long long result = 0;

    err = pthread_create(&pthread[0], NULL, t_function, (void*)&t1 );
    if(err != 0) //쓰레드생성실패
    {
        perror("ERROR:pthread_create(0)\n");
        exit(-1);
    }
    err = pthread_create(&pthread[1], NULL, t_function, (void*)&t2 );
    if(err != 0)
    {
        perror("ERROR:pthread_create(1)\n");
        exit(-2);
    }

    pthread_join(pthread[0], (void**)&status); 
    printf("thread_join(0) : %lld\n",status);
    result += status;

    pthread_join(pthread[1], (void**)&status);
    printf("thread_join(1) : %lld\n",status);
    result += status;

    printf("result : %lld\n",result);
    
    return 0;
}
