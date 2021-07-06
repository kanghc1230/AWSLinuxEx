#include "shm.h"

int main(void)
{
    //공유메모리 주소값을 가리키는 보이드 포인터
    void *shared_Mem = (void*)0; 
    //실제 공유메모리접근시 int포인터로 접근하기위한 포인터
    int *shmaddr;
    int shmid;
    // STEP 1. shmget
    shmid = shmget((key_t)KeyValue, sizeof(int)*SHMSIZE, 0666 | IPC_CREAT); //400byte
    //정상적으로 공유메모리 할당이 안됬을때
    if (shmid == -1)
    {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }

    // STEP 2. shmat
    shared_Mem = shmat(shmid, (void*)0 , 0); //특정주소값으로 할당하기 원하는경우. 인자 (void*)0위치에 주소
    //공유메모리가 할당되지 못하는경우
    if (shared_Mem == (void*)-1) //실패시 (void*형태로) -1이나옴
    {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Memory attached at 0x%p\n", shared_Mem);
    shmaddr = (int*)shared_Mem;
    
    // STEP 3. memory access
    // read
    for(int i =0; i<SHMSIZE; i++)
    {
        printf("shmaddr : %p,  data : %d\n", shmaddr+i,*(shmaddr+i));
    }
    // STEP 4. shmdt
    if(shmdt(shared_Mem) == -1)
    {
        fprintf(stderr, "shmgdt failed\n");
        exit(EXIT_FAILURE);
    }
    // STEP 5. shmctl(IPC_RMID) -> shmReader.c
    if(shmctl(shmid,IPC_RMID, 0)==-1)
    {
        fprintf(stderr, "shmctl failed\n");
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}