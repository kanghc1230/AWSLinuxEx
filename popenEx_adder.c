/*프로세스를 1개추가로만들어서 
 * 메인프로그램에서는 1~10만더하고
 * popen에서는 100001~200000 더한건 프로그램을 불러옴
 *
 * 결과값 메세지출력을 통해 전달받아 메인프로그램에서 1~200000을 더한값출력
 * */
//popen2Ex.c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int main(void)
{
	FILE *read_fp;
	int i;
	char buffer[BUFSIZ+1];//popen에서 결과값 받아올 스트링
	int c_read; //popen결과가 널이아닌지 체크할 int
	long result = 0;
	for (i=0; i<=100000; i++)
		result += i;
	printf("main result = %ld\n",result);
	
	read_fp = popen("./popenEx_adder2","r");
	//popen성공
	if(read_fp !=NULL){
		c_read = fread(buffer,sizeof(char), BUFSIZ, read_fp);
		if(c_read > 0){
			printf("popen result : %s \n", buffer);
			result += atoi(buffer);
			printf("main result + popen result = %ld\n", result);
			pclose(read_fp);
			return 0;
		}
	}
	//popen실패
	else
		return -1;

}
