//msgTx.c
#include "msg.h" //헤더파일 불러오기

int main(void)
{	
	// 구조체 객체 선언
	struct my_msg_st some_data;
	int msgid;
	// running 전송할 데이터가 있는지 bool역할
	int running = 1;
	char buffer[BUFSIZ];
	
	// STEP 1. 메세지큐 생성 //(key_t)암호화키
	msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
	// 메세지큐가 생성되지 못한경우 
	if(msgid == -1)	{
		fprintf(stderr,"msgget failed with error : %d\n", errno);
		exit(EXIT_FAILURE);
	}

	while(running)
	{	
		printf("Enter some text:");
		// (buffer에 저장,BUFSIZ만큼, 키보드로 입력을 받아라)
		fgets(buffer, BUFSIZ, stdin);
		some_data.my_msg_type = 1;
		// (some_text에복사 buffer에있는걸)
		strcpy(some_data.some_text, buffer);
		
		// STEP 2.메세지큐에 보내기
		// (void*)오직 주소값만
		// msgsnd가 -1면 메세지큐에 정상전송x
		if(msgsnd(msgid, (void*)&some_data, MAX_TEXT, 0) == -1)
		{	
			fprintf(stderr, "msgsnd failed\n");
			exit(EXIT_FAILURE);	
		}
		if (strncmp(buffer,"end" ,3)==0)
		{
			running = 0;
		}

	}

	exit(EXIT_SUCCESS);
}
