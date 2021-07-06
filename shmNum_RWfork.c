#include "shmNum.h"
long long adder(int start, int end)
{
    long long result = 0;
    for ( ; start<=end; start++)
        result += start;
    return result;
}
int main(void)
{
    //fork선언
    pid_t pid;
    int status;
    long long result = 0;
    
    // 공유메모리 선언
    void* shared_Mem = (void*)0;
    long long* shmaddr;

    // STEP 1. shmget 공유메모리 id생성
    int shmid;
    shmid = shmget((key_t)KeyValue, sizeof(long long)*2, 0666 | IPC_CREAT);
    if (shmid == -1 )
    {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }
    printf("shmid = %d\n", shmid);

    // STEP 2. shmat 공유메모리 접근 생성
    shared_Mem = shmat(shmid, (void*)0, 0);
    if ( shared_Mem == (void*)-1 )
    {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }
    shmaddr = (long long*)shared_Mem;

    //fork() 자식프로세스 생성
    pid = fork();
    //fork생성 실패시
    if(pid <0){
        printf("자식생성실패\n");
        return -1;
    }
    // STEP 3. memory access 데이터접근 작업진행
    //자식 프로세스
    else if (pid == 0)
    {
        *(shmaddr+1) = adder(10000001,20000000);
        printf("shmaddr : %p , data : %lld\n", shmaddr+1, *(shmaddr+1));

        return 1;
    }
    //부모 프로세스
    else // (pid > 0)
    {
        *shmaddr = adder(1,10000000);
        printf("shmaddr : %p , data : %lld\n", shmaddr, *shmaddr);
        
        //대기. 자식프로세스 종료시
        wait(&status); 
        //결과값출력
        result = *shmaddr + *(shmaddr+1);
        printf("result = %lld \n", result);

        // STEP 4. shmdt 부모가 공유메모리떼기
        if (shmdt(shared_Mem) == -1)
        {
            fprintf(stderr, "shmdt failed\n");
            exit(EXIT_FAILURE);
        }
        // STEP 5. shmctl(IPC_RMID) 부모가 공유메모리 삭제
        if (shmctl(shmid, IPC_RMID,0) == -1)
        {
            fprintf(stderr, "shmctl failed\n");
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}