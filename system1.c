#include<stdio.h>
#include<stdlib.h>


int main(void)
{	
	printf("Running ps with system\n");
	
	/*시스템에서 실행할문장부분*/
	//But 각자프로세스를 생성해서 자원을 많이낭비한다.
	//system("ls -al");
	//system("./cmd_arg 1234 abcd");
	system("./cmd_arg 1234 abcd &"); //&를 넣으면 분가해서 따로실행
	
	printf("Process is Done\n");
	//return 0;
	exit(0);
}
