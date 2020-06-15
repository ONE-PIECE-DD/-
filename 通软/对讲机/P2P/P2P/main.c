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
    //�ж������жԲ���
    if(argc != 3)
    {
        printf("�÷���%sĿ��IP ����ģʽ\n",argv[0]);
        exit(0);
    }
    mode=atoi(argv[2]);

    Initsocket();
    //�趨�жϴ�������������ʱ������
    //signal(SIGALRM,int_proc);
    //alarm(timer);
    //�������ݱ��׽���
    sockfd = socket(AF_INET,SOCK_DGRAM,0);//����һ��socket���
     if( sockfd == INVALID_SOCKET )
     {
         printf("socket() fail:%d\n",WSAGetLastError());
         return -2;
     }
    //Ϊ�ṹ����daddr�ĸ��ֶθ�ֵ������Դip Դ�˿�
    addrlen = sizeof(struct sockaddr_in);
    memset(&daddr,0,sizeof(daddr));
    daddr.sin_addr.S_un.S_addr= htonl(INADDR_ANY);//����IP����������IP
    daddr.sin_family = AF_INET;//Э���帳ֵ
    daddr.sin_port = htons(8888);//�������ֽ�˳��ת��Ϊ�����ֽ�˳��
    if(inet_addr(&daddr.sin_addr)<=0)//�жϱ���IP�Ƿ�Ū�ɹ�
    {
        fprintf(stderr,"Invaeld dest IP\n");
        exit(0);
    }
    //Ϊ�ṹ����saddr�ĸ��ֶθ�ֵ������Ŀ��ip�˿�
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(4399);
    saddr.sin_addr.S_un.S_addr = inet_addr(argv[1]);

    if(inet_addr(&saddr.sin_addr)<=0)
    {
        fprintf(stderr,"Invaild source IP!\n");
        exit(0);
    }
    //�󶨵�ַ
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
            printf("������ʽ������");

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
    //�ӱ�׼���뻺��������ַ����������͸�Ŀ�ĵ�ַ
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
    //�Ƚ����ݰ���Դ��ַ�뱣���ַ�Ƿ�һ��
    if(a>0)
    {
        printf("\t\t\t:B\n");
        printf("\t\t\tReceived:%s",buffer1);
        //if(memcmp(&cmpaddr,&saddr,addrlen)!=0)
          //  printf("����δ֪�û�");

    }
    else
        printf("δ�յ�");

}
