// winsock20170822.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

int result=0;
void func(int* p, int icount)
{
	result = p[0];
	for (int i = 1; i < icount; i++)
	{
		result -= p[i];
	}
}
void func2(int* p, int icount)
{
	result = 1;
	for (int i = 0; i < icount; i++)
	{
		result *= p[i];
	}
}

//Server
int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		return -1;
	}
	if (LOBYTE(wsa_data.wVersion) != 2 || HIBYTE(wsa_data.wVersion) != 2)
	{
		return -1;
	}
	
	SOCKET serv_socket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8000);
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	if (bind(serv_socket, (sockaddr*)&addr, sizeof(sockaddr)) == SOCKET_ERROR)
	{
		return -1;
	}

	if (listen(serv_socket, 10) == SOCKET_ERROR)
	{
		return -1;
	}

	
		sockaddr_in clnt_addr;
		int clnt_addr_size=sizeof(sockaddr);
		SOCKET	clnt_socket = accept(serv_socket, (sockaddr*)&clnt_addr, &clnt_addr_size);
		/*printf("The client connecting ,the IP: %s, the Port:%s\n",inet_ntoa(clnt_addr.sin_addr.S_un.S_addr),ntohs(clnt_addr.sin_port) );*/
		char buf[1024];
		memset(buf, 0, 1024);
		recv(clnt_socket, buf, sizeof(buf), 0);
		int count = atoi(buf);//得到数字，建立循环
		int a[10];
		closesocket(clnt_socket);
		
		for (int i = 0; i < count+1; i++)
		{
			clnt_socket = accept(serv_socket, (sockaddr*)&clnt_addr, &clnt_addr_size);
			memset(buf, 0, sizeof(buf));
			recv(clnt_socket, buf, sizeof(buf), 0);
			
			if (i == count)
			{
				char *p = buf;
				switch (*p){
				case '+':for (int j = 0; j < count; j++){ result += a[j]; }
					break;
				case '-':func(&a[10],count);
					break;
				case '*':func2(&a[10], count);
					break;
				}
				memset(buf, 0, sizeof(buf));
				sprintf(buf, "%d", result);
				send(clnt_socket, buf, strlen(buf) + 1, 0);
				closesocket(clnt_socket);
			}
			else
			{
				a[i] = atoi(buf);
				closesocket(clnt_socket);
			}
			
		}
	closesocket(serv_socket);

	return 0;
}
