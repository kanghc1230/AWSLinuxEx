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
    int serv_sock, clnt_sock;
    // 이예제 컨셉은 파일을 전송하고 다루기위해 파일포인터 선언
    FILE* fp;
    char buf[BUF_SIZE];
    int read_cnt; //읽을떄 카운트값

    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;

	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

    // STEP 1. socket() : 서버의 소켓을 생성
    serv_sock = socket(PF_INET, SOCK_STREAM, 0); 

    // STEP 2. bind() 서버에 프로토콜,IP주소,포트번호 설정
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    // STEP 3. listen() 대기열버퍼공간
    listen(serv_sock, 5);

    // STEP 4. accept() 클라이언트 접속요청 허용
    clnt_addr_size = sizeof(clnt_addr); //구조체크기만큼
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

    // STEP 5. read() ,write() 데이터 송수신
    fp = fopen("cat.jpg", "rb");
    while(1){
        read_cnt=fread((void*)buf, 1, BUF_SIZE, fp); //파일읽기
        // 풀버퍼가 아니라 남는값이있으면 짜투리처리
        if(read_cnt<BUF_SIZE)
        {
                write(clnt_sock, buf, read_cnt);
                break;
        }
        // 버퍼가 모두 찼을경우 저장
        write(clnt_sock, buf, BUF_SIZE);
    }
    fclose(fp);
    // STEP 6. close() : 접속종료
    // close(clnt_sock);
    // 반만닫기 half close 우아한종료
    shutdown(clnt_sock, SHUT_WR); //더이상 보낼게없다.(,보내기만종료)

    read(clnt_sock, buf, BUF_SIZE);
    printf("Message from client : %s \n", buf);

    //나머지 반 닫기. SHUT_RD와 동일
    close(clnt_sock); close(serv_sock);
    return 0;
}