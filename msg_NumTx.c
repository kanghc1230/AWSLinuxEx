//msg_NumRx.c
#include "msg_Num.h"

int main(void)
{
        struct my_num_st some_data;
        int msgid;
        int running = 1;
        char buffer[BUFSIZ]; //데이터가 스트링으로
	long SUM,SUM2 = 0;
        //1.메세지큐생성
        msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
        if(msgid == -1){
                fprintf(stderr,"msgget failed : %d\n" , errno);
                exit(EXIT_FAILURE);
        }

	//자체 SUM2계산
        some_data.result = 0;
        for(int i=100001; i<=200000; i++)
                some_data.result += i;
        printf("NumRx result : %ld\n", some_data.result);
	SUM2 = some_data.result;

	//메세지큐 받아오기 
	if(msgrcv(msgid, (void*)&some_data, BUFSIZ, 0, 0) == -1)
	{
		fprintf(stderr,"msgrcv failed");
		exit(EXIT_FAILURE);
	}
	SUM = SUM2 + some_data.result;
	printf("SUM = %ld\n" ,SUM);
	
	exit(EXIT_SUCCESS);
}

