//서버에서 클라이언트로 파일을 전송하는 프로그램
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h> 

#define BUF_SIZE 1024//파일 읽어올 크기

void error_handling(char *message)
{
    fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char *argv[])
{
    int sock;
    FILE *fp;
    
    char buf[BUFSIZ];
    int read_cnt;
    struct sockaddr_in serv_addr;
    
    if (argc != 3){
    	printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

    // STEP 1. socket() : 소켓생성
    sock = socket(PF_INET, SOCK_STREAM, 0);

    // STEP 2.1 프로토콜, 주소, 포트번호 설정
    memset(&serv_addr, 0 , sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
    // STEP 2.2 connect() : 서버에 접속요청
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    // STEP 3. read(), write()
    fp = fopen("receive.jpg", "wb");

    /* //read()함수를 bloacking I/O -> non blocking I/O로 바꾸는 설정
    int flag = fcntl( sock, F_GETFL, 0 );
    fcntl(sock, F_SETFL, flag | O_NONBLOCK );
    */
    while((read_cnt = read(sock, buf, BUF_SIZE)) != 0)
        fwrite((void*)buf, 1, read_cnt, fp);
    // 상태확인을 위해 클라이언트 터미널에 출력
    puts("Received file data");
    // 파일 데이터 수신이 끝나면 클라이언트는 땡큐 문자열 서버에 전송 
    write(sock,"Thank you", 10);   
    fclose(fp);

    // STEP 4. shutdonw(), close()
    close(sock);
    return 0;
}