// WSAOverLapped_IO_Client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <WinSock2.h>
#include <Windows.h>

#include <stdlib.h>
#pragma comment (lib,"ws2_32.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;
	SOCKET hClientSocket;
	SOCKADDR_IN ClientAddrIn;

	WSABUF wsaBuffer;
	char msg[] = "This";
	ULONG iSendBytes=0;

	WSAEVENT wsaEvent;
	WSAOVERLAPPED wsaOverLapped;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return -1;

	hClientSocket = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&ClientAddrIn, 0, sizeof(sockaddr_in));
	ClientAddrIn.sin_family = AF_INET;
	ClientAddrIn.sin_port = htons(8888);
	ClientAddrIn.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	if (connect(hClientSocket, (sockaddr*)&ClientAddrIn, sizeof(sockaddr)) == SOCKET_ERROR)
		return -1;

	//WSAEvent初始化
	wsaEvent = WSACreateEvent();
	memset(&wsaOverLapped, 0, sizeof(WSAOVERLAPPED));
	wsaOverLapped.hEvent = wsaEvent;
	wsaBuffer.len = strlen(msg) + 1;
	wsaBuffer.buf = msg;
	
	if (WSASend(hClientSocket, &wsaBuffer, 1, &iSendBytes, 0, &wsaOverLapped, NULL) == SOCKET_ERROR)
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			printf("Background sending!!\n");
			WSAWaitForMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, FALSE);
			WSAGetOverlappedResult(hClientSocket, &wsaOverLapped, &iSendBytes, false, NULL);
		}
		else
		{
			printf("WSASend error!!\n");
		}
	}
	printf("send data size %d\n", iSendBytes);
	WSACloseEvent(wsaEvent);
	closesocket(hClientSocket);
	system("pause");
	WSACleanup();
	return 0;
}

