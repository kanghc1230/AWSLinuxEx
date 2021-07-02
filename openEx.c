#include<stdio.h>
#include<fcntl.h>
int main(void)
{
    int fd;

    for(int i = 0; i<1028;i++) //리눅스는 1024에 뻗는다 fd=-1이나오면 오픈등록에러 //프로세스는 돌고있음 
    {
        fd = open("/home/kang/AWSLinuxEx/a.txt", O_RDONLY, O_TRUNC);
        printf("fd = %d\n", fd);
    }
    return 0;
}