#pragma once
// Link to ws2_32.lib
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#include "stdafx.h"

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpUrlRoute.h"
#include "HttpTemplate.h"

#define PORT 5150

class SocketClientA
{
public:
	SocketClientA();
	~SocketClientA();

	typedef struct
	{
		OVERLAPPED Overlapped;
		WSABUF DataBuf;
		CHAR Buffer[DATA_BUFSIZE];
		byte *LPBuffer;
		vector<byte> byteBuffer;
		DWORD BytesSEND;
		DWORD BytesRECV;
	} PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;

	typedef struct
	{
		SOCKET Socket;
	} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

	HANDLE GetCompletionPort();
	int Start();

private:
	static DWORD WINAPI ServerWorkerThread(LPVOID CompletionPortID);
	HANDLE CompletionPort;
	int m_PortNum = PORT;
};

