#include "shm.h"

int main(void)
{   //공유메모리 주소값을 가리키는 보이드 포인터 (공유멤이 보이드 포인터로줌) 
    void *shared_Mem = (void*)0; //(void*)0 널포인터로 초기화
    //실제 공유메모리접근시 int포인터로 접근하기위한 포인터
    int *shmaddr;
    // STEP 1. shmget
    int shmid;
    shmid = shmget((key_t)KeyValue, sizeof(int)*SHMSIZE, 0666 | IPC_CREAT); //400byte
    //정상적으로 공유메모리ID가 못 만들어지면
    if (shmid == -1)
    {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }
    printf("shmid = %d\n",shmid);
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
    // write (0x0000001~0x00000100)
    for(int i =0; i<SHMSIZE; i++)
    {
        *(shmaddr+i) = i+1;
        printf("shmaddr : %p,  data : %d\n", shmaddr+i,*(shmaddr+i));
    }
    // STEP 4. shmdt
    if(shmdt(shared_Mem) == -1)
    {
        fprintf(stderr, "shmgdt failed\n");
        exit(EXIT_FAILURE);
    }
    // STEP 5. shmctl(IPC_RMID) -> shmReader.c
    // 

    exit(EXIT_SUCCESS);
}