#include <winsock2.h>
#include <stdio.h>
#define IP "127.0.0.1"				//在一台计算机上测试，IP为本地回送地址
#define PORT 15001					//注意：客户端设置通信的端口 = 服务端的端口
#define size 1024			//数据发送缓冲区大小


int main()
{
    int i=0;
											//用于输入字符自增变量
	while(1)
	{
        char buf[size];								//buf数组存放客户端发送的消息
		WSADATA WSAData;
		if(WSAStartup(MAKEWORD(2,0),&WSAData)==SOCKET_ERROR)  //WSAStartup()函数对Winsock DLL进行初始化
		{
			printf("Socket initialize fail!\n");
			continue;
		}
		SOCKET sock;											//客户端进程创建套接字
		if((sock=socket(AF_INET,SOCK_STREAM,0))==ERROR)  //创建流套接字（与服务端保持一致）
		{
			printf("Socket create fail!\n");
			WSACleanup();
			continue;
		}
		struct sockaddr_in aimaddr;				//sockaddr_in结构用来标识TCP/IP协议下的地址，可强制转换为sockaddr结构
		aimaddr.sin_family=AF_INET;				//指Internet域
		aimaddr.sin_port=htons(PORT);			//指定服务端所预留的端口
		aimaddr.sin_addr.s_addr=inet_addr(IP);	//指定服务端所绑定的IP地址
		if(connect(sock,(LPSOCKADDR)&aimaddr,sizeof(aimaddr))==SOCKET_ERROR)  //调用connect()函数，向服务器进程发出连接请求
		{
			printf("Connect fail!\n");
			closesocket(sock);
			WSACleanup();
			continue;
		}
		i++;
		if(i<2)
        {
            printf("端口连接成功：\n%s\n向端口发送可执行命令:\n\n",inet_ntoa(aimaddr.sin_addr));
        }
        printf("发送命令>>>");

		gets(buf);
		if(buf[0]=='e' && buf[1]=='x' && buf[2]=='i' && buf[3]=='t')
		{
			printf("The End.\n");
			break;
		}
		send(sock,buf,size,0);				 //向服务器发送数据
		closesocket(sock);							 //关闭套接字
		WSACleanup();								//终止对Winsock DLL的使用，并释放资源，以备下一次使用
	}
	return 0;
}
