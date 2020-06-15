/*����������*/
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <windows.h>
#include <string.h>
#define NumOfClient 10//�������ͻ�����
#define LOCALPORT 4999
void initWSA();
void sendinf();
void receiveinf();
int theaddrisexist(unsigned long,unsigned short port);
void pre_addr_exit(unsigned long,unsigned short port);
void add_pre_addr(unsigned long addr,unsigned short port);

WORD wVersionRequested;
WSADATA wsaData;
HANDLE hThread1;
HANDLE hThread2;
HANDLE hMutex = NULL;
char recvBuf[255];
char sendBuf[255];
unsigned long alladdr[NumOfClient];
unsigned long allport[NumOfClient];
unsigned long pre_addr;
unsigned short pre_port;
SOCKET sockfd;
int numofclient;
int needsendinf;
int err;
int main()

{
    initWSA();
    //�����׽���
    numofclient=0;
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
        printf("�׽ӿڰ�ʧ��");
    hMutex = CreateMutex(NULL,0,NULL);
    //���������߳�
    hThread1 = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)receiveinf,NULL,0,0);
    if(hThread1!=NULL)
        CloseHandle(hThread1);
    //���������߳�
    hThread2 = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)sendinf,NULL,0,0);
    if(hThread2!=NULL)
        CloseHandle(hThread2);

    while(1)
    {        //�����������е������Ӷ��еĶ�ͷ��أ�������Ϊ����ý������ߣ����в���ʱ����
        Sleep(1000);//�ó����̵߳�ʱ��Ƭ��Ϊ���̵߳�ִ����·
    }
    if(hThread1!=NULL||hThread2!=NULL)
    {
        CloseHandle(hThread1);
        CloseHandle(hThread2);
        printf("�������ر�");
    }
    CloseHandle(hMutex);
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

    int i,byte = 0;
    char* name;
    name = (char*)malloc(16*sizeof(char));
    IN_ADDR IPNAME;
    SOCKADDR_IN  addrServ;
    memset(&addrServ,0,sizeof(addrServ));
        //addrServ.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    addrServ.sin_family = AF_INET;
    while(1)
    {
        if(needsendinf)
        {
            WaitForSingleObject(hMutex,INFINITE);
            if(theaddrisexist(pre_addr,pre_port)==0)
                add_pre_addr(pre_addr,pre_port);
            if(strcmp(sendBuf,"exit")==0)
            {
                pre_addr_exit(pre_addr,pre_port);
                strcpy(sendBuf,"��ϵͳ��ʾ��>>>�˳�");
            }
            IPNAME.S_un.S_addr = pre_addr;
            name = inet_ntoa(IPNAME);//�������ַת���ɵ��ʮ�����ַ���
            strcat(sendBuf,name);
            for(i=0;i<numofclient;i++)
            {
                if(alladdr[i]==pre_addr)
                    continue;
                else
                {
                    addrServ.sin_addr.S_un.S_addr = alladdr[i];
                    addrServ.sin_port = allport[i];
                   // byte = send(sockClient,sendBuf,sizeof(sendBuf)+1,0);
                    byte = sendto(sockfd,sendBuf,strlen(sendBuf),0,(SOCKADDR*)&addrServ,sizeof(SOCKADDR));
                    if(byte<=0){
                        printf("����ʧ��");
                        break;}
                }
            }
            needsendinf=0;
            ReleaseMutex(hMutex);
            Sleep(1000);
        }
        else
        {
            Sleep(1000);
        }

    }
    exit(-1);
}

void receiveinf()
{
    int byte = 0;
    SOCKADDR_IN addrClient;
    memset(&addrClient,0,sizeof(addrClient)); //������ÿ����ַ��Ա֮ǰ������addr�ṹӦ�ñ���ʼ��0;
    int length = sizeof(SOCKADDR);
    while(1)
    {
        //WaitForSingleObject(hMutex,INFINITE);
        byte=recvfrom(sockfd,recvBuf,255,0,(SOCKADDR*)&addrClient,&length);
        if(byte<=0)
            {byte=0;printf("����ʧ��");}
        else{
        recvBuf[byte]='\0';
        WaitForSingleObject(hMutex,INFINITE);
        needsendinf=1;
        pre_addr = addrClient.sin_addr.S_un.S_addr;//��IP�������ֽ�ת��Ϊ�����ֽ�
        pre_port = addrClient.sin_port;
        printf("%s:\n",inet_ntoa(addrClient.sin_addr));
        printf("%s\n",recvBuf);
        strcpy(sendBuf,recvBuf);
        if(strcmp(sendBuf,"exit")==0&&theaddrisexist(pre_addr,pre_port)==0)//����ǳ�Ա�����˳�ָ��������
            needsendinf=0;
        if(needsendinf)
        {
            ReleaseMutex(hMutex);
            Sleep(1000);
        }
        }
    }
    exit(-1);
}

int theaddrisexist(unsigned long addr,unsigned short port)
{
    int i,exist;
    exist=0;
    for(i=0;i<NumOfClient;i++)
    {
        if(alladdr[i]!=0&&allport[i]!=0){
            if(addr==alladdr[i]&&port==allport[i])
                {exist=1;break;}
        }
        else
                break;
    }
    return exist;
}
void add_pre_addr(unsigned long addr,unsigned short port)
{
    int i;
    for(i=0;i<NumOfClient;i++)
    {
        if(alladdr[i]==0&&allport[i]==0)
        {
            alladdr[i]=addr;
            allport[i]=port;
            numofclient++;
            break;
        }
    }
    if(i==NumOfClient)
        printf("�ͻ�����������");

}

void pre_addr_exit(unsigned long addr,unsigned short port)
{
    int i;
    for(i=0;i<NumOfClient;i++)
    {
        if(alladdr[i]!=0&&allport[i]!=0){
            if(addr==alladdr[i]&&port==allport[i])
            {
                alladdr[i]=0;
                allport[i]=0;
                numofclient--;
                break;
            }
        }
        else
            printf("δ֪����");
    }
}
