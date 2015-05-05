#pragma once
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#include "stdafx.h"


class SocketIocpController
{
public:
	SocketIocpController();
	~SocketIocpController();


	typedef struct
	{
		OVERLAPPED Overlapped;
		WSABUF DataBuf;
		CHAR *Buffer;
		byte *LPBuffer;
		vector<byte> byteBuffer;
		DWORD BytesSEND;
		DWORD BytesRECV;
	} PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;

	typedef struct
	{
		SOCKET Socket;
	} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

	typedef struct
	{
		PER_IO_OPERATION_DATA operationData;
		PER_HANDLE_DATA handleData;
	} SOCKET_IO_DATA, *LPSOCKET_IO_DATA;


private:


};

