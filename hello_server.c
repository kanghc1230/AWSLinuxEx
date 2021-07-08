#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h> //소켓구조체 선언
#include <sys/socket.h> 

//에러핸들링함수 인자로받아온 문자열 출력후 종료
void error_handling(char *message)
{
    fputs(message, stderr);
	fputc('\n', stderr);
    exit(1);
}

//현재 이 코드의 컨셉은 서버1대에 클라이언트 1대 접속만 가능한 컨셉
int main (int argc, char *argv[])
{
    int serv_sock; //서버소켓
    int clnt_sock; //클라이언트소켓 1개

    // 소켓의 주소값을 저장하는 구조체객체 선언 //교재p p68~p69
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    //서버 -> 클라이언트로 보낼 메세지
    char message[] = "Hello Server";
    
    if(argc!=2)
    {   // ./hello_server [port]
        printf("USAGE : %s <port>\n", argv[0]);
        exit(1);
    }

    // STEP 1. socket() : 서버소켓 생성
    // 0:stdin, 1:stdout 2:stderr  3
    // PF_INET : 프로토콜체계192.168,0,3    
    // TCP : SOCK_STREAM 지연신뢰성 데이터소멸X(은행,http)  UDP: SOCK_DGRAM 빠름실시간 데이터소멸가능(중계)
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);  //프로토콜IPv4, TCP
    if(serv_sock == -1)
        error_handling("socket() error");

    // STEP 2. bind() 서버 IP주소, 포트번호, 주소체계 설정.
    // bind() 호출전 구조체에 주소적기
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;     //주소체계
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //htonl(자기IP주소) host to network long(4byte)
    serv_addr.sin_port = htons (atoi(argv[1])); //포트번호 short(2byte)

    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1 )
		error_handling("bind() error"); 

    // STEP 3. listen() : 접속 대기열 버퍼의 크기설정
    if ( listen(serv_sock, 5) == -1 ) //대기열 버퍼 크기 5
        error_handling("listen() error");

    // STEP 4. accept() : 클라이언트의 접속 요청을 받아들인다.
    // 클라이언트 소켓을 생성
    clnt_addr_size = sizeof(clnt_addr);
    //클라이언트의 주소와 포트정보를 받아와서 번호할당
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if(clnt_sock==-1)
		error_handling("accept() error");

    // STEP 5. 데이터 read/write
    write(clnt_sock, message , sizeof(message));

    // STEP 6. 서버가 종료가 필요하면
    close(clnt_sock);
    close(serv_sock);
    return 0;
}