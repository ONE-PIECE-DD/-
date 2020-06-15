
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <windows.h>
#define aimip "127.0.0.1"
#define portOfclient 8888
#define portOfserver 4999
void initWSA();
void sendinf();
void receiveinf();
WORD wVersionRequested;
WSADATA wsaData;
int err;
int cansend;
int main()
{
    cansend=1;
    while(1)
    {
        if(cansend)
        {
         initWSA();
        sendinf();
        }
        else{
        initWSA();
        receiveinf();
        }

    }
    return 0;
}

void initWSA()
{
    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
    {
        printf("WSA初始化失败: %d\n", err);
    }

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        printf("Winsock.dll版本错误\n");
        WSACleanup();
    }

}

void sendinf()
{
    //建立连接套接字
    SOCKET sockClient = socket(AF_INET,SOCK_DGRAM, 0);
    char buffer[100];
    SOCKADDR_IN  addrServ;
    memset(&addrServ,0,sizeof(addrServ));
    addrServ.sin_addr.S_un.S_addr = inet_addr(aimip);
    addrServ.sin_family = AF_INET;
    addrServ.sin_port = htons(portOfserver);

    printf("B:\n");
    scanf("%s",buffer);
    sendto(sockClient,buffer,strlen(buffer),0,(SOCKADDR*)&addrServ,sizeof(SOCKADDR));
    cansend=0;
    closesocket(sockClient);

    WSACleanup();
}

void receiveinf()
{
    int n;
    SOCKET sockSrv = socket(AF_INET,SOCK_DGRAM, 0); //创建一个socket句柄;
    if( sockSrv == INVALID_SOCKET )
    {
        printf("socket() fail:%d\n",WSAGetLastError());
    }

    SOCKADDR_IN  addrServ;
    memset(&addrServ,0,sizeof(addrServ));
    addrServ.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    addrServ.sin_family = AF_INET;
    addrServ.sin_port = htons(portOfclient); //从"主机字节顺序" 转变为 "网络字节顺序"

    err = bind(sockSrv,(SOCKADDR*)&addrServ,sizeof(SOCKADDR)); //把socket 绑定到指定地址上
    if( err != 0 )
    {
        printf("%d:端口绑定失败\n",WSAGetLastError());
    }

    SOCKADDR_IN  addrClient;
    memset(&addrClient,0,sizeof(addrClient)); //在设置每个地址成员之前，整个addr结构应该被初始化0;
    int length = sizeof(SOCKADDR);
    char recvBuf[100];
    n=recvfrom(sockSrv,recvBuf,100,0,(SOCKADDR*)&addrClient,&length);
    recvBuf[n]='\0';
    printf("\n\t\t\t\t :A\n");
    printf("\t\t %s\n",recvBuf);
    cansend=1;
    closesocket(sockSrv);

    WSACleanup();

}
