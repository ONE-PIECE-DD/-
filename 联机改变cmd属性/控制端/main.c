#include <winsock2.h>
#include <stdio.h>
#define IP "127.0.0.1"				//��һ̨������ϲ��ԣ�IPΪ���ػ��͵�ַ
#define PORT 15001					//ע�⣺�ͻ�������ͨ�ŵĶ˿� = ����˵Ķ˿�
#define size 1024			//���ݷ��ͻ�������С


int main()
{
    int i=0;
											//���������ַ���������
	while(1)
	{
        char buf[size];								//buf�����ſͻ��˷��͵���Ϣ
		WSADATA WSAData;
		if(WSAStartup(MAKEWORD(2,0),&WSAData)==SOCKET_ERROR)  //WSAStartup()������Winsock DLL���г�ʼ��
		{
			printf("Socket initialize fail!\n");
			continue;
		}
		SOCKET sock;											//�ͻ��˽��̴����׽���
		if((sock=socket(AF_INET,SOCK_STREAM,0))==ERROR)  //�������׽��֣������˱���һ�£�
		{
			printf("Socket create fail!\n");
			WSACleanup();
			continue;
		}
		struct sockaddr_in aimaddr;				//sockaddr_in�ṹ������ʶTCP/IPЭ���µĵ�ַ����ǿ��ת��Ϊsockaddr�ṹ
		aimaddr.sin_family=AF_INET;				//ָInternet��
		aimaddr.sin_port=htons(PORT);			//ָ���������Ԥ���Ķ˿�
		aimaddr.sin_addr.s_addr=inet_addr(IP);	//ָ����������󶨵�IP��ַ
		if(connect(sock,(LPSOCKADDR)&aimaddr,sizeof(aimaddr))==SOCKET_ERROR)  //����connect()����������������̷�����������
		{
			printf("Connect fail!\n");
			closesocket(sock);
			WSACleanup();
			continue;
		}
		i++;
		if(i<2)
        {
            printf("�˿����ӳɹ���\n%s\n��˿ڷ��Ϳ�ִ������:\n\n",inet_ntoa(aimaddr.sin_addr));
        }
        printf("��������>>>");

		gets(buf);
		if(buf[0]=='e' && buf[1]=='x' && buf[2]=='i' && buf[3]=='t')
		{
			printf("The End.\n");
			break;
		}
		send(sock,buf,size,0);				 //���������������
		closesocket(sock);							 //�ر��׽���
		WSACleanup();								//��ֹ��Winsock DLL��ʹ�ã����ͷ���Դ���Ա���һ��ʹ��
	}
	return 0;
}
