// 멀티 프로세스를 이용해서 여러명의 클라이언트를 동시에 처리할수 있는 서버구성


// fail.//////////////////////////////////////////////////////////////
//나중에 테스트 
//////////////////////////////////////////////////////////////////////

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

// 에러핸들러
void error_handling(char *message)
{
    fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

// signal (SIGCHLD, read_childproc);
// 자식 프로세스가 종료됬는지 체크
void read_childproc(int sig){
	pid_t pid;
	int status;
	pid=waitpid(-1, &status, WNOHANG); //wait(&status); //+WNOHANG:종료안해도리턴 
	printf("removed proc id: %d \n", pid);
}

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr,clnt_adr;

    pid_t pid;
    socklen_t adr_sz;
    char input_message[BUF_SIZE]; //받는메세지
    char output_message[BUF_SIZE]; //보내는메세지
    char buf[BUF_SIZE]; 
    int str_len;
    if(argc!=2) {
	    printf("Usage : %s <port>\n", argv[0]);
	    exit(1);
	}
    // 자식종료 return 0; 시 발생하는 SIGCHLD
    // SIGCHLD를 받으면 read_childproc()함수로 이동
    signal (SIGCHLD, read_childproc);

    // STEP 1. socket() 생성 _iob[3]즉 sock에 번호값 3
    // _iob[0]:stdin _iob[1]:stdout _iob[3]:stderr
    serv_sock=socket(PF_INET, SOCK_STREAM, 0);
    printf("serv_sock : %d\n",serv_sock); // 3

    // STEP 2.1 bind()전 adr에 값넣기
    memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
    // STEP 2.2 bind() 소캣과 주소 묶기
    if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");

    // STEP 3. listen() 대기열공간할당
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");

    // STEP 4.1 accept() 반복
    while (1)
    {
        // STEP 4.2 accept()
        adr_sz=sizeof(clnt_adr);
        // clnt sock에 4
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
		printf("clnt_sock : %d\n", clnt_sock); // 4
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
			while(1) // !=0 NULL 파일의끝이나 클라이언트가close(sock)이오면
            {
                //친거읽어서 클라에게 쓰기 q면나가고
                fputs("Input message(Q to quit): ", stdout);
                fgets(output_message, BUF_SIZE, stdin);   
                if(!strcmp(output_message,"q\n") || !strcmp(output_message,"Q\n")) //본인이 종료하면 끝
                    break;
                write(clnt_sock, output_message, strlen(output_message));

                //리드읽어들여서 출력. 읽어온메세지가 널이아니면(값이있으면)
                if(str_len=read(clnt_sock, input_message, BUF_SIZE-1)!=0)
                {   
                    input_message[str_len]=0;
                    printf("Message from client: %s", input_message);
                    //널나올떄까지 문장끝까지 출력
                    while(str_len=read(clnt_sock, input_message, BUF_SIZE-1)!=0)//출력
                    {
                        input_message[str_len]=0;
                        printf("%s\n", input_message);
                    }               
                }
                else //클라가 널보내면 (종료면)
                    break;
            }
            // STEP 6. client close()
			close(clnt_sock);
			puts("client disconnected...");
            
			return 0;
        }
        // 부모프로세스
        else //pid>0
            close(clnt_sock); // (&) 부모에선 클라소켓닫기
            // 멀티프로세스에선 프로세스를 종료clnt_sock이 4번을 계속출력, 멀티쓰레드면 4,5,6,7 소켓추가시킴

    } //부모는 다시 while(1)따라 accept처리하러감

    // STEP 6. shutdown(), close()
    close(serv_sock);
    return 0;
}