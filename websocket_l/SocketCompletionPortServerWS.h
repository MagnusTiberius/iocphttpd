#pragma once
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#include "stdafx.h"

 
namespace WebSocket
{

	class SocketCompletionPortServerWS
	{
	public:
		SocketCompletionPortServerWS();
		~SocketCompletionPortServerWS();

		SocketCompletionPortServerWS(int PortNum);

		void Start(int PortNum);

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


		int Start();
		int Start2();

		HANDLE GetCompletionPort();


		HANDLE ghMutex;


	private:
		static DWORD WINAPI ServerWorkerThread(LPVOID CompletionPortID);
		static DWORD WINAPI ServerWorkerThread2(LPVOID CompletionPortID);
		HANDLE CompletionPort;
		int m_PortNum = PORT;
		bool FileExist(const TCHAR *fileName);


	private:

	};


}