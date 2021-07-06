#include<stdio.h>
#include<string.h>

struct Data
{	
	//msg_type=1; csv파일에서 읽어서, 프로세서간 메세지큐전송가능
	long int msg_type;
	char name[10];
 	int kor;
	int eng;
	int math;
	int scien;
	int avg;
	int total;
};
//read_scv(), write_scv()
int main(void)
{	
	char buffer[10];
	//구조체 객체 선언
	struct Data st;
	//구조체 객체 초기화
	memset(&st, 0, sizeof(st));
	
	//키입력을 받아서 csv파일에 저장하게 만든다
	printf("name : ");
	scanf("%s", st.name);
	//fgets(st.name,sizeof(st.name),stdin);
	
	printf("kor : ");
        scanf("%d", &st.kor);
	//fgets(st.kor,sizeof(buffer),stdin);
	
	printf("eng : ");
	scanf("%d", &st.eng);
        //fgets(st.eng,sizeof(buffer),stdin);

	printf("math : ");
	scanf("%d", &st.math);
        //fgets(st.math,sizeof(buffer),stdin);

	printf("scien : ");
        scanf("%d", &st.scien);
	//fgets(st.scien,sizeof(buffer),stdin);
	
	//test.csv 파일을 생성. write로 읽기
	FILE *fp = fopen("test.scv", "a");
	//fseek(fp,0L,SEEK_END);
	fprintf(fp, "%s,%d,%d,%d,%d\n"
			,st.name, st.kor, st.eng, st.math, st.scien);
	
	fclose(fp);
	return 0;
}

