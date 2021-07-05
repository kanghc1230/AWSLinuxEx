//popenEx.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	FILE *read_fp;
	int chars_read;
	char buffer[BUFSIZ +1]; //초기화 되지않은 buffer 
	//printf("BUFSIZ = %d" ,BUFSIZ); //BUF_SIZ 리눅스 헤더파일 8192
	
	//buffer를 buffer사이즈만큼 '\0'로 초기화
	memset(buffer, '\0', sizeof(buffer));
	//popen은 터미널프로세스 + "ls -al"가 출력문자열 read_fp로 리턴
	read_fp = popen("ls -al","r");
	
	//Popen이 정상적으로 실행
	if (read_fp!=NULL){
		//(buffer에저장,char씩단위로, BUFSIZ만큼, read_fp로부터읽어서) 
		//chars_read는 읽어온 바이트수
		chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
		if(chars_read>0)
			printf("---out---\n%s \n",buffer);
		pclose(read_fp);
        	return 0;
	}
	//poepn이 비정상적인 실행
	else 
		return -1;
}

//popen 으로 ls가 아니라 df를 읽어와서 용량을 스트링으로 읽어서 코딩가능
