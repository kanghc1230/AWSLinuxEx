#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h> //소켓구조체 선언
#include <sys/socket.h> 

void error_handling(char *message)
{
    fputs(message, stderr);
	fputc('\n', stderr);
    exit(1);
}

int main (int argc, char* argv[])
{
    int str_len;
    int sock;
    //(어디에)접속할 프로토콜, IP주소, 포트번호
    struct sockaddr_in serv_addr;
    //메세지를 받아와 저장할 문자열 버퍼
    char message[30];

    // ./hello_client [서버IP주소값] [PORT]
    if(argc!=3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

    // STEP 1. sock() : 소켓을 생성
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) error_handling("socket() error");

    // STEP 2. connect() : 프로토콜, 서버 주소값, 포트 설정
    memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1) 
		error_handling("connect() error!");

    // STEP 3. read/ write : 서버에서 "Hello world" 문자열을 전송한것을 받는다
    str_len = read(sock, message, sizeof(message)-1);
    if(str_len==-1)
		error_handling("read() error!");
	
	printf("Message from server: %s \n", message);  
	close(sock);
	return 0;
}