//msgRx.c
#include "msg.h"

int main(void)
{
        // 구조체 객체 선언
        struct my_msg_st some_data;
        int msgid;
        // running 전송할 데이터가 있는지 bool역할
        int running = 1;
        char buffer[BUFSIZ];

        // STEP 1-2. 메세지큐 생성
        msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
        // 메세지큐가 생성되지 못한경우 
        if(msgid == -1) {
                fprintf(stderr,"msgget failed with error : %d\n", errno);
                exit(EXIT_FAILURE);
        }

	while (running)
	{	
		// STEP 3.메세지큐 수신하기
		// IPC_NOWAY 대신 2번째0
		// msgrcv가 -1면 메세지지큐를 정상리딩x
		if (msgrcv(msgid, (void*)&some_data, BUFSIZ, 0, 0) == -1)
		{
			fprintf(stderr,"msgrcv failed");
                	exit(EXIT_FAILURE);
		}
		printf ("Recive : %s", some_data.some_text);

		if (strncmp(some_data.some_text, "end", 3)==0)
		{
			running = 0;
		}	
	}
	
	//STEP 4.메세지큐 삭제
	if(msgctl(msgid, IPC_RMID, 0) == -1)
	{
		fprintf(stderr,"msgctl(IPC_RMID) failed");
                exit(EXIT_FAILURE);
	}	
	exit(EXIT_SUCCESS);
}

