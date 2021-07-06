//csv_msgTx.c
#include "csv_msg.h"
//read_scv(), write_scv()
int main(void)
{
	char buffer[30];
	//구조체 객체 선언
	struct Data st;
	//구조체 객체 초기화
	memset(&st, 0, sizeof(st));
	//메세지큐 생성
	int msgid = msgget((key_t)1234, 0666 | IPC_CREAT);  
	if (msgid == -1) {
		fprintf(stderr,"msgget failed : %d\n",errno);
		exit(EXIT_FAILURE);
	}
	
	//"test.csv" 파일을 read로 읽기
	char *fpState;
	FILE *fp = fopen("test.scv", "r");
	//while문을위해 첫번쨰 읽어들이기
	fpState = fgets(buffer, sizeof(buffer), fp);
	while(fpState != NULL){
		//strtok : 문자열을 token으로 나누어준다.
		char *ptr = strtok(buffer,",");
		strcpy(st.name, ptr);
		ptr = strtok(NULL,",");
		st.kor = atoi(ptr);
        	ptr = strtok(NULL,",");
        	st.eng = atoi(ptr);
       		ptr = strtok(NULL,",");
        	st.math = atoi(ptr);
        	ptr = strtok(NULL,",");
        	st.scien = atoi(ptr);

		//파일플래그기본0 본인이 마지막이면1
		//다음타자 한문장씩 buffer에 미리 읽어들인다
                fpState = fgets(buffer, sizeof(buffer), fp);
                //본인이 마지막이면 1
		if (fpState ==NULL) 
                        st.file_flag = 1;

		//출력문(메세지큐로 보낼부분확인)
		printf("name = %s, kor = %d, eng = %d, math = %d, scien = %d\n", 
				st.name, st.kor, st.eng, st.math, st.scien);
		//메세지큐 보내기
		st.msg_type = 1;
		if(msgsnd(msgid, (void*)&st, sizeof(struct Data), 0) == -1)
		{
			fprintf(stderr, "msgsnd failed\n");
			exit(EXIT_FAILURE);
		}
			
	}

	fclose(fp);
	return 0;
}

