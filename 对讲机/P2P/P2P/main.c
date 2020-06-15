#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <winsock2.h>
#include <signal.h>
#define send_first 1
#define receive_first 0

int Initsocket();
void int_proc(int signo){};
void sendinf();
void receiveinf();

SOCKADDR_IN daddr;
SOCKADDR_IN saddr;
SOCKADDR_IN cmpaddr;
SOCKET sockfd;
int timer = 3;
char buffer[100];
int addrlen,n;
int daddrlen;
int mode;
int main(int argc,char**argv)
{
    //判断命令行对不对
    if(argc != 3)
    {
        printf("用法：%s目的IP 工作模式\n",argv[0]);
        exit(0);
    }
    mode=atoi(argv[2]);

    Initsocket();
    //设定中断处理函数，并设置时间限制
    //signal(SIGALRM,int_proc);
    //alarm(timer);
    //建立数据报套接字
    sockfd = socket(AF_INET,SOCK_DGRAM,0);//创建一个socket句柄
     if( sockfd == INVALID_SOCKET )
     {
         printf("socket() fail:%d\n",WSAGetLastError());
         return -2;
     }
    //为结构变量daddr的各字段赋值，设置源ip 源端口
    addrlen = sizeof(struct sockaddr_in);
    memset(&daddr,0,sizeof(daddr));
    daddr.sin_addr.S_un.S_addr= htonl(INADDR_ANY);//本机IP，本机所有IP
    daddr.sin_family = AF_INET;//协议族赋值
    daddr.sin_port = htons(8888);//将主机字节顺序转变为网络字节顺序
    if(inet_addr(&daddr.sin_addr)<=0)//判断本地IP是否弄成功
    {
        fprintf(stderr,"Invaeld dest IP\n");
        exit(0);
    }
    //为结构变量saddr的各字段赋值，设置目的ip端口
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(4399);
    saddr.sin_addr.S_un.S_addr = inet_addr(argv[1]);

    if(inet_addr(&saddr.sin_addr)<=0)
    {
        fprintf(stderr,"Invaild source IP!\n");
        exit(0);
    }
    //绑定地址
    if(bind(sockfd,(SOCKADDR*)&daddr,sizeof(saddr))!=0)
    {
        fprintf(stderr,"bind local addr error!\n");
        exit(1);
    }

    while(1)
    {
        if(mode==send_first)
        {
            printf("\nA:\n");
            sendinf();
            receiveinf();
        }
        else if(mode==receive_first)
        {
            printf("\nB:\n");
            receiveinf();
             sendinf();

        }
        else
            printf("工作方式有问题");

    }
    closesocket(sockfd);

}

int Initsocket()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested =MAKEWORD( 2, 2 );
    err = WSAStartup( wVersionRequested, &wsaData );
    if ( err != 0 )
    {
        printf("WSAStartup failed with error: %d\n", err);
        exit(1);
    }
    if ( LOBYTE( wsaData.wVersion ) != 2 ||
            HIBYTE( wsaData.wVersion ) != 2 )
    {
        printf("Could not find a usable version of Winsock.dll\n");
        WSACleanup( );
        exit(1);

    }
    return 0;
}

void sendinf()
{
    //从标准输入缓冲区获得字符串，并发送给目的地址
    if(fgets(buffer,100,stdin) == NULL)exit(0);
    if(sendto(sockfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&saddr,addrlen)==-1)
    {
        fprintf(stderr,"sendto error!\n");
        exit(2);
    }

}

void receiveinf()
{
    char buffer1[100];
    int a;
    do{
        a=recvfrom(sockfd,buffer1,100,0,(SOCKADDR*)&cmpaddr,&addrlen);

    }while(a<=0);
    //比较数据包来源地址与保存地址是否一致
    if(a>0)
    {
        printf("\t\t\t:B\n");
        printf("\t\t\tReceived:%s",buffer1);
        //if(memcmp(&cmpaddr,&saddr,addrlen)!=0)
          //  printf("来自未知用户");

    }
    else
        printf("未收到");

}
