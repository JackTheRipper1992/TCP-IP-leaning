// winsock20170823.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <WinSock2.h>
#pragma comment (lib,"ws2_32.lib")

int main()
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

	SOCKET clnt_socket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8000);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	if (connect(clnt_socket, (sockaddr*)&addr, sizeof(sockaddr))==SOCKET_ERROR)
	{
		return -1;
	}
	int count=0;
	printf("please input the count of the parameter!\n");
	scanf("%d", &count);
	char buf[1024];
	sprintf(buf, "%d", count);
	send(clnt_socket, buf, strlen(buf) + 1,0);
	for (int i = 0; i < count + 1; i++)
	{
		SOCKET clnt_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (connect(clnt_socket, (sockaddr*)&addr, sizeof(sockaddr)) == SOCKET_ERROR)
		{
			return -1;
		}
		memset(buf, 0, sizeof(buf));
		int a = 0;
		if (i!=count)
		{
			printf("please input one number:\n");
			fflush(stdin);
			scanf("%d", &a);
			sprintf(buf, "%d", a);
			send(clnt_socket, buf, strlen(buf) + 1, 0);
		}
		if (i == count)
		{
			char c=0;
			printf("please input one operator!\n");
			fflush(stdin);
			scanf("%c", &c);
			sprintf(buf, "%c", c);
			send(clnt_socket, buf, strlen(buf) + 1, 0);
			recv(clnt_socket, buf, 10, 0);
			printf("the result is %d", atoi(buf));
			fflush(stdout);
		}


	}

		

	
	fflush(stdin);
	getchar();


	return 0;
}




