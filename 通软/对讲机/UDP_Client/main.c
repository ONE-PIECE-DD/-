
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
        printf("WSA��ʼ��ʧ��: %d\n", err);
    }

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        printf("Winsock.dll�汾����\n");
        WSACleanup();
    }

}

void sendinf()
{
    //���������׽���
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
    SOCKET sockSrv = socket(AF_INET,SOCK_DGRAM, 0); //����һ��socket���;
    if( sockSrv == INVALID_SOCKET )
    {
        printf("socket() fail:%d\n",WSAGetLastError());
    }

    SOCKADDR_IN  addrServ;
    memset(&addrServ,0,sizeof(addrServ));
    addrServ.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    addrServ.sin_family = AF_INET;
    addrServ.sin_port = htons(portOfclient); //��"�����ֽ�˳��" ת��Ϊ "�����ֽ�˳��"

    err = bind(sockSrv,(SOCKADDR*)&addrServ,sizeof(SOCKADDR)); //��socket �󶨵�ָ����ַ��
    if( err != 0 )
    {
        printf("%d:�˿ڰ�ʧ��\n",WSAGetLastError());
    }

    SOCKADDR_IN  addrClient;
    memset(&addrClient,0,sizeof(addrClient)); //������ÿ����ַ��Ա֮ǰ������addr�ṹӦ�ñ���ʼ��0;
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
