//msg_NumTx.c
#include "msg_Num.h"

int main(void)
{
	struct my_num_st some_data;
	int msgid;
	int running = 1;
	char buffer[BUFSIZ]; //데이터가 스트링으로
	
	//1.메세지큐생성
	msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
	if(msgid == -1){
		fprintf(stderr,"msgget failed : %d\n" , errno);
		exit(EXIT_FAILURE);
	}

	some_data.result = 0;
	for(int i=0; i<=100000; i++)
		some_data.result += i;
	printf("NumTx result : %ld\n", some_data.result);

	
	//2.메세지큐전달
	
		some_data.my_num_type = 1; //전송 type 기본동작에 1이 형식임 	

		if(msgsnd(msgid, (void*)&some_data, MAX_TEXT, 0) == -1 )
		{
			fprintf(stderr,"megsnd failed\n");
			exit(EXIT_FAILURE);
		}
	
	exit(EXIT_SUCCESS);
}
