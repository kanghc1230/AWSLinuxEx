//csv_msgRx.c
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
        if (msgid == -1)
       	{
                fprintf(stderr,"msgget failed : %d\n",errno);
                exit(EXIT_FAILURE);
        }
	
	//파일오픈
	FILE *fp = fopen("result.csv","w+a"); 
	//"a"로하면 파일추가 "w+a" w+가 파일지우고 a로쓰기
	
	int running = 1;
	while(running)
	{
		//구조체 초기화
		memset(&st, 0, sizeof(st));
		//메세지큐에서 받아오기
		st.msg_type = 1;
		if (msgrcv(msgid, (void*)&st, sizeof(struct Data), 0, 0) == -1)
		{ 
			fprintf(stderr,"msgrcv failed");
                       	exit(EXIT_FAILURE);
                }
		
		st.total = st.kor+st.eng+st.math+st.scien;
		st.avg = st.total/4;
		//출력문(파일로 내보낼부분확인)
		printf("name = %s, kor = %d, eng = %d, math = %d, scien = %d, avg = %d, total = %d\n", st.name, st.kor, st.eng, st.math, st.scien, st.avg, st.total);
		//파일로 내보내기
		//fseek(fp,0L,SEEK_END);
		fprintf(fp, "%s, %d, %d, %d, %d, avg=%d, total=%d\n", st.name, st.kor, st.eng, st.math, st.scien, st.avg, st.total);

		//반복문끝 본인이 마지막이라면
		if(st.file_flag == 1)
		{
			running = 0;
		}
	}
	//메세지큐 삭제
	if(msgctl(msgid, IPC_RMID, 0) == -1)
	{
		fprintf(stderr,"msgctl Failled");
		exit(EXIT_FAILURE);
	}
	//파일닫기
	fclose(fp);
	return 0;
}

