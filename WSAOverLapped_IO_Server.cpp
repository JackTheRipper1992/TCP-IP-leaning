// WSAOverLapped_IO_Server.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <WinSock2.h>
#include <Windows.h>

#include <stdlib.h>
#pragma comment (lib,"ws2_32.lib")
int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;
	SOCKET hServerSocket,hClientSocket;
	SOCKADDR_IN servAddrin,clntAddrIn;
	int iClntAddrInSize;
	const int iBufferSize = 1024;

	WSABUF wsaBuffer;
	char msg[iBufferSize];
	memset(msg, 0, iBufferSize);
	ULONG iRecvBytes = 0;

	WSAEVENT wsaEvent;
	WSAOVERLAPPED wsaOverLapped;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return -1;

	hServerSocket = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&servAddrin, 0, sizeof(servAddrin));
	servAddrin.sin_family = AF_INET;
	servAddrin.sin_port = htons(8888);
	servAddrin.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	if (bind(hServerSocket, (sockaddr*)&servAddrin, sizeof(servAddrin)) == SOCKET_ERROR)
		return -1;
	if (listen(hServerSocket, 10) == SOCKET_ERROR)
		return -1;
	
	memset(&clntAddrIn, 0, sizeof(clntAddrIn));
	iClntAddrInSize = sizeof(clntAddrIn);
	hClientSocket = accept(hServerSocket, (sockaddr*)&clntAddrIn, &iClntAddrInSize);

	wsaEvent = WSACreateEvent();
	memset(&wsaOverLapped, 0, sizeof(wsaOverLapped));
	wsaOverLapped.hEvent = wsaEvent;
	wsaBuffer.len = iBufferSize;
	wsaBuffer.buf = msg;

	if (WSARecv(hClientSocket, &wsaBuffer, 1, &iRecvBytes, 0, &wsaOverLapped, NULL) == SOCKET_ERROR)
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			printf("background data recving!!");
			WSAWaitForMultipleEvents(1, &wsaEvent,TRUE,WSA_INFINITE,FALSE );
			WSAGetOverlappedResult(hClientSocket, &wsaOverLapped, &iRecvBytes, FALSE, NULL);
		}
		else
		{
			printf("WSARecv error!!!!");
		}
	}
	printf("Received %d byte message :%s", iRecvBytes, msg);
	WSACloseEvent(wsaEvent);
	closesocket(hClientSocket);
	closesocket(hServerSocket);
	system("pause");
	WSACleanup();









	return 0;
}

