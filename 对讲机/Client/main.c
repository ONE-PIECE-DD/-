/*����������*/
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <windows.h>
#include <string.h>
#define NumOfClient 10
#define IpOfServer "192.168.43.128"
#define PortOfServer 4999
#define LOCALPORT 7777
void initWSA();
void sendinf();
void receiveinf();

WORD wVersionRequested;
WSADATA wsaData;
HANDLE hThread1;
HANDLE hThread2;
HANDLE hMutex = NULL;
int needsendinf;
char recvBuf[255];
char sendBuf[255];
SOCKET sockfd;
int err;
int main()

{
    initWSA();
    //�����׽���
    needsendinf=1;
    sockfd = socket(AF_INET,SOCK_DGRAM, 0); //����һ��socket���;
    if( sockfd == INVALID_SOCKET )
    {
        printf("socket() fail:%d\n",WSAGetLastError());
        return -2;
    }
    SOCKADDR_IN  local_addr;
    memset(&local_addr,0,sizeof(local_addr));
    local_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(LOCALPORT); //��"�����ֽ�˳��" ת��Ϊ "�����ֽ�˳��"
    //���׽ӿڰ󶨵����ص�ָ���������ַ������δ������,
    if(bind(sockfd,(SOCKADDR*)&local_addr,sizeof(SOCKADDR))!=0)
    {
        printf("��ʧ��");
        if(WSAGetLastError()==WSAEINPROGRESS)
            printf("һ���������׽ӿڵ�������������");
        exit(-1);
    }
   hThread2 = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)sendinf,NULL,0,0);
    if(hThread2!=NULL)
        CloseHandle(hThread2);
        //���������߳�
    hThread1 = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)receiveinf,NULL,0,0);
    if(hThread1!=NULL)
        CloseHandle(hThread1);
   while(strcmp(sendBuf,"exit")!=0)
    {
        Sleep(1000);//�ó����̵߳�ʱ��Ƭ��Ϊ���̵߳�ִ����·
    }
    Sleep(1000);
    if(hThread1!=NULL||hThread2!=NULL)
    {
        CloseHandle(hThread1);
        CloseHandle(hThread2);
        printf("�������ر�");
    }
    closesocket(sockfd);
    WSACleanup();


}

void initWSA()
{
    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
        printf("WSAStartup failed with error: %d\n", err);
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        printf("Could not find a usable version of Winsock.dll\n");
        WSACleanup();
    }

}

void sendinf()
{
    int byte = 0;
    SOCKADDR_IN  addrServ;
    memset(&addrServ,0,sizeof(addrServ));
    addrServ.sin_addr.S_un.S_addr = inet_addr(IpOfServer);
    addrServ.sin_family = AF_INET;
    addrServ.sin_port = htons(PortOfServer);
    strcpy(sendBuf,"\n(ϵͳ��ʾ)>>>һ������������������\n");
    while(1)
    {
        if(!needsendinf)
        {
            gets(sendBuf);
            if(strlen(sendBuf)>0)
            needsendinf=1;
        }
        if(needsendinf)
        {
            byte = sendto(sockfd,sendBuf,strlen(sendBuf),0,(SOCKADDR*)&addrServ,sizeof(SOCKADDR));
            if(byte>0)
            {
                if(strcmp(sendBuf,"exit")==0)
                    break;
                needsendinf=0;
                sendBuf[0]='\0';
            }
            else
            {
                needsendinf=1;
            }
            Sleep(1000);

        }
    }
}



void receiveinf()
{
    int byte = 0;
    SOCKADDR_IN addrClient;
    memset(&addrClient,0,sizeof(addrClient)); //������ÿ����ַ��Ա֮ǰ������addr�ṹӦ�ñ���ʼ��0;
    int length = sizeof(SOCKADDR);
    while(strcmp(sendBuf,"exit")!=0)
    {
        byte=recvfrom(sockfd,recvBuf,255,0,(SOCKADDR*)&addrClient,&length);
        if(byte<=0)
            {byte=0;printf("����ʧ��");}
        else
        {
        recvBuf[byte]='\0';
        //WaitForSingleObject(hMutex,INFINITE);
        printf("\t\t\t\t%s \n",&recvBuf[byte-9]);
        recvBuf[byte-9]='\0';
        printf("\t\t\t\t %s \n",recvBuf);
        byte=0;
        }
    }
}
