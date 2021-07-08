// 멀티 프로세스를 이용해서 여러명의 클라이언트를 동시에 처리할수 있는 서버구성
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <arpa/inet.h>
#include <sys/socket.h> 

#include <signal.h> //멀티프로세서
#include <sys/wait.h> 

#define BUF_SIZE 1024//파일 읽어올 크기

void error_handling(char *message)
{
    fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
void read_childproc(int sig){
	pid_t pid;
	int status;
	pid=waitpid(-1, &status, WNOHANG); //wait(&status); 에 WNOHANG: 종료안해도리턴
	printf("removed proc id: %d \n", pid);
}
int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr,clnt_adr;

    pid_t pid;
    socklen_t adr_sz;
    char buf[BUF_SIZE];
    int str_len;
    if(argc!=2) {
	    printf("Usage : %s <port>\n", argv[0]);
	    exit(1);
	}
    // 자식종료 return 0; 시 발생하는 SIGCHLD
    // SIGCHLD를 받으면 read_childproc()함수로 이동
    signal (SIGCHLD, read_childproc);

    // STEP 1. socket()
    serv_sock=socket(PF_INET, SOCK_STREAM, 0);

    // STEP 2. bind()
    memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
    if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");

    // STEP 3. listen()
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");

    // STEP 4.1 accept() 반복
    while (1)
    {
        // STEP 4.2 accept()
        adr_sz=sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
		if(clnt_sock==-1) //재접속시도
			continue;
		else
			puts("new client connected...");
        // 새로운 클라이언트가 접속했다면
        // 새로운 클라이언트 하나당 프로세스 하나생성
        pid = fork();
        // 정상적으로 프로세스가 생성되지 않은경우
        if(pid==-1)
        {
            close(clnt_sock);
            continue;
        }
        // 자식프로세스 (클라이언트 처리 프로세스)
        else if (pid==0)
        {
			close(serv_sock); // (&) 자식에선 서버소켓닫기

            // STEP 5. read(), write() 
			while((str_len=read(clnt_sock, buf, BUF_SIZE))!=0)
				write(clnt_sock, buf, str_len);
            // STEP 6. client close()
			close(clnt_sock);
			puts("client disconnected...");
			return 0;
        }
        // 부모프로세스
        else //pid>0
            close(clnt_sock); // (&) 부모에선 클라소켓닫기
    } //부모는 다시 while(1)따라 accept처리하러감

    // STEP 6. shutdown(), close()
    close(serv_sock);
    return 0;
}