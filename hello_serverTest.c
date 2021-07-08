#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message)
{
    fputs(message, stderr);
	fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[])
{
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char message[] = "hello world";
    if (argc!=2){
        printf("%s <port> \n", argv[0]);
        exit(1);
    }

    // STEP 1. sock() 생성
    serv_sock = sock(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) exit(-1);

    // STEP 2. memset과 bind() 떤지기
    // 메모리초기화
    memset( &serv_addr, 0 ,siszeof(serv_addr));
    // 구조체안에 변수에 값넣기
    serv_addr.sin_family = AF_INET; //프로토콜형식
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1 )
        error_handling("bind error");
    
    // STEP 3. listen() 사이즈정하기
    if(listen(serv_sock, 5) == -1)
        error_handling("liten error");
    
    // STEP 4. accept()
    clnt_addr_size = sizeof(clnt_addr);

    clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_size);
	if(clnt_sock==-1)
		error_handling("accept() error");  
	
    // STEP 5. WR
	write(clnt_sock, message, sizeof(message));
	close(clnt_sock);	
	close(serv_sock);
	return 0;
}