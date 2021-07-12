#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <pthread.h>

// 서버에서 사용할 버퍼의 크기
#define BUF_SIZE 100 
// 서버가 최대로 접속 가능한 클라이언트 수
#define MAX_CLNT 256


// 전역변수를 선언해 모든스레드가 접근가능한 공유메모리변수를 선언
int clnt_cnt = 0;
int clnt_socks[MAX_CLNT]; //소켓의 개수를 관리할 clnt_ sock s

// 뮤텍스 변수 (데이터락, critical section을 관리)
pthread_mutex_t mutx;

// 함수 원형 미리 선언 해놓기
void * handle_clnt(void * arg);
void send_msg(char * msg, int len);
void error_handling(char * msg);

// 서버에서 클라이언트가 접속하면 클라이언트의 데이터의 송수신을 당담할 스레드
void* handle_clnt (void *arg)
{
    int clnt_sock=*((int*)arg);
    int str_len =0;
    int i;
    char msg[BUF_SIZE];

    // STEP 5.read() write()
    while((str_len=read(clnt_sock, msg, sizeof(msg)))!=0) // != 0 널문자가 올때까지==클라이언트close()
        send_msg(msg,str_len); // 모든 클라이언트에게 메세지 전송

    pthread_mutex_lock(&mutx);
    // critical section 클라이언트가 나가면 clnt_socks[i]자리를 한칸씩 위로 당김
    for(i=0; i<clnt_cnt; i++)
    {
        if(clnt_sock == clnt_socks[i])
        {
            while(i++<clnt_cnt-1)
                clnt_socks[i] = clnt_socks[i+1];
            break;
        }
    }
    clnt_cnt--;
    pthread_mutex_unlock(&mutx);

    close(clnt_sock);
    return NULL; // 쓰레드는 void*를 리턴해야함 os
}

// 각 클라이언트에게 메세지를 보내는 함수
void send_msg(char* msg, int len)
{
    int i;
	pthread_mutex_lock(&mutx);
    // 클라이언트 한명한명에게 메세지전송
	for(i=0; i<clnt_cnt; i++)
		write(clnt_socks[i], msg, len);
	pthread_mutex_unlock(&mutx);
}

// 에러처리 함수
void error_handling(char * msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

// ./chat_serv [PORT] 
int main(int argc, char* argv[])
{
    // 서버
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
    // 쓰레드 id값저장
    pthread_t t_id;
    // 뮤텍스 변수 초기화
    pthread_mutex_init(&mutx, NULL);

    //입력값관리
    if(argc != 2)
    {
        printf("Usage : %s [PORT]\n", argv[0]);
        exit(1);
    }

    // STEP 1. socket()생성
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    // STEP 2. bind() 초기화, 프로토콜,IP addr,포트
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET; 
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
    //sockaddr*구조체와 다른 구조체 형식은 위에 구조체 sockaddr_in
    if ( bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1 )
        error_handling("bind() error");
    
    // STEP 3. listen 대기열공간
    if (listen(serv_sock, 5) ==-1 )
        error_handling("listen() error");
    
    while(1)
    {
    // STEP 4. accept
    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
    // 클라이언트가 들어올때마다 clnt_sock를 증가시킨다.
	pthread_mutex_lock(&mutx);
	clnt_socks[clnt_cnt++]=clnt_sock;
	pthread_mutex_unlock(&mutx);

    // 방금 접속한 클라이언트 데이터 처리를 위한 스레드 생성 
    pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
    // 스레드가 생성후 생성한 메인스레드가 관리하지않고 생성된 스레드가 종료하게되면 알아서 스레드를 종료한다
    // 관리가 필요하지않음
    pthread_detach(t_id);
    printf("Conneted client IP: %s\n", inet_ntoa(clnt_adr.sin_addr));

    // STEP 5. read(), write  // 쓰레드함수 내부에서

    // STEP 6-1 clnt close() // 쓰레드함수 내부에서
    }

    // STEP 6-2 serv close()
    close(serv_sock);

    return 0;
}














