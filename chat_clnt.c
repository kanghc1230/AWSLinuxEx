#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
	
#define BUF_SIZE 100
#define NAME_SIZE 20
	
void * send_msg(void * arg);
void * recv_msg(void * arg);
void error_handling(char * msg);
	
// 이름과 메세지를 전역변수로 쓰레드에서도 쓸수있게 
char name[NAME_SIZE]="[DEFAULT]";
char msg[BUF_SIZE];
	
// ./chat_serv [IP ADRESS] [PORT] [Your Name]  
int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t snd_thread, rcv_thread;
	void * thread_return;
	if(argc!=4) {
		printf("Usage : %s [IP ADRESS] [PORT] [Your Name] \n", argv[0]);
		exit(1);
	}
     
	//name에 [your name]포맷저장
	sprintf(name, "[%s]", argv[3]); 
    // STEP 1.socket()생성
	sock=socket(PF_INET, SOCK_STREAM, 0);
	
    // STEP 2. connect() 초기화, 프로토콜,IP addr,포트
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		error_handling("connect() error");
	
    // STEP 3. read() write() 받는 쓰레드 읽는 쓰레드
	pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
	pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);

	pthread_join(snd_thread, &thread_return); 
	pthread_join(rcv_thread, &thread_return);
	close(sock);  
	return 0;
}

// 보내는 쓰레드
void * send_msg(void * arg)   
{
	int sock=*((int*)arg);
	char name_msg[NAME_SIZE+BUF_SIZE];
	while(1) 
	{
		fgets(msg, BUF_SIZE, stdin);
		if(!strcmp(msg,"q\n")||!strcmp(msg,"Q\n")) 
		{
			close(sock);
			exit(0);
		}
        // name_msg 문자열로 복사
		sprintf(name_msg,"%s %s", name, msg); //이미 보낼때부터 [name] %s가 서버로전달
		write(sock, name_msg, strlen(name_msg));
	}
	return NULL;
}

// 받는 쓰레드
void * recv_msg(void * arg)  
{
	int sock=*((int*)arg);
	char name_msg[NAME_SIZE+BUF_SIZE];
	int str_len;
	while(1)
	{
		str_len=read(sock, name_msg, NAME_SIZE+BUF_SIZE-1);
		if(str_len==-1) 
			return (void*)-1;
		name_msg[str_len]=0;
		fputs(name_msg, stdout);
	}
	return NULL;
}
	
void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}













