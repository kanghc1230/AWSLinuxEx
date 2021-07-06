#include<stdio.h>
#include<string.h>
#include<stdlib.h>
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
	char buffer[30];
	//구조체 객체 선언
	struct Data st;
	//구조체 객체 초기화
	memset(&st, 0, sizeof(st));
	
	
	//test.csv 파일을 read로 읽기
	FILE *fp = fopen("test.scv", "r");
	//fgets 한줄씩 읽어들이기
	fgets(buffer, sizeof(buffer), fp);
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

	printf("name = %s, kor = %d, eng = %d, math = %d, scien = %d\n", st.name, st.kor, st.eng, st.math, st.scien);
	

	fclose(fp);
	return 0;
}

