// socket_20170831.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <WinSock2.h>

int _tmain(int argc, _TCHAR* argv[])
{
	int err = 0;
	WSAData wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data))
	{
		exit(0);
	}
	SOCKET s = socket(PF_INET, SOCK_STREAM, 0);
	
	USHORT host_port = 0x1234;
	USHORT net_port;
	ULONG host_addr = 0x12345678;
	ULONG net_addr;

	net_port = htons(host_port);
	net_addr = htonl(host_addr);

	printf("host port is %#x \n",host_port);
	printf("net port is %#x \n", net_port);
	printf("host_addr is %#lx \n", host_addr);
	printf("net_addr is %#lx \n", net_addr);
	getchar();

	return 0;
}

