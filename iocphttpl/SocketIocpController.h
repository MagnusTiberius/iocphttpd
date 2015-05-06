#pragma once
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#include "stdafx.h"


class IOCPHTTPL_API SocketIocpController
{
public:
	SocketIocpController();
	~SocketIocpController();


	typedef struct
	{
		OVERLAPPED Overlapped;
		WSABUF DataBuf;
		CHAR Buffer[BUFSIZMIN];
		byte *LPBuffer;
		vector<byte> byteBuffer;
		DWORD BytesSEND;
		DWORD BytesRECV;
		int sequence;
		int mallocFlag;
	} PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;

	typedef struct
	{
		SOCKET Socket;
	} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

	typedef struct
	{
		int sequence;
		PER_IO_OPERATION_DATA operationData;
		PER_HANDLE_DATA handleData;
	} SOCKET_IO_DATA, *LPSOCKET_IO_DATA;


	LPSOCKET_IO_DATA Allocate();
	void Free(LPSOCKET_IO_DATA data);
	void Free(int index);
	void FreeBySocket(SOCKET index);

private:
	SOCKET_IO_DATA m_DataBuffer[DATASIZMIN];
	int m_ActiveFlag[DATASIZMIN];
	HANDLE ghMutex;
	DWORD m_DataBufferSize;
};

