#include<stdio.h>
#include<stdlib.h>
extern char **environ;
int main(void){
	//환경변수에 MYDIR, USER_FOLDER 라는것을 2개 추가
	setenv("MYDIR" , "/home/kang", 0); 
	setenv("USER_FOLDER" , "/usr" , 0);
	//환경변수 설정 확인 getenv()	
	printf("MYDIR = %s\n" , getenv("MYDIR"));
	printf("USER_FOLDER = %s\n" , getenv("USER_FOLDER"));
		
	return 0;
}
