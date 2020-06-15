//注：需要在stdafx.h 里面添加 #pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <windows.h>
#define aimip "172.18.94.189"
#define portOfclient 4999
#define portOfserver 5555

void initWSA();
void sendinf();
void receiveinf();
WORD wVersionRequested;
WSADATA wsaData;
int cansend;
int err;
int main()
{
    cansend=0;
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
}

void initWSA()
{
    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
    {
        printf("WSA启动失败: %d\n", err);
    }

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        printf("找不到Winsock.dll\n");
        WSACleanup();
    }

}
void sendinf()
{
    SOCKET sockClient = socket(AF_INET,SOCK_DGRAM, 0);
    char buffer[100];
    SOCKADDR_IN  addrServ;
    memset(&addrServ,0,sizeof(addrServ));
    addrServ.sin_addr.S_un.S_addr = inet_addr(aimip);
    addrServ.sin_family = AF_INET;
    addrServ.sin_port = htons(portOfclient);

    printf("A:\n");
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
        printf("socket()初始化失败:%d\n",WSAGetLastError());
    }

    SOCKADDR_IN  addrServ;
    memset(&addrServ,0,sizeof(addrServ));
    addrServ.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    addrServ.sin_family = AF_INET;
    addrServ.sin_port = htons(portOfserver); //从"主机字节顺序" 转变为 "网络字节顺序"

    err = bind(sockSrv,(SOCKADDR*)&addrServ,sizeof(SOCKADDR)); //把socket 绑定到指定地址上
    if( err != 0 )
    {
        printf("绑定失败:%d\n",WSAGetLastError());
    }

    SOCKADDR_IN  addrClient;
    memset(&addrClient,0,sizeof(addrClient)); //在设置每个地址成员之前，整个addr结构应该被初始化0;
    int length = sizeof(SOCKADDR);
    char recvBuf[100];
    n=recvfrom(sockSrv,recvBuf,100,0,(SOCKADDR*)&addrClient,&length);
    recvBuf[n]='\0';
    printf("\n\t\t\t\t :B\n");
    printf("\t\t %s\n",recvBuf);
    cansend=1;
    closesocket(sockSrv);

    WSACleanup();

}
