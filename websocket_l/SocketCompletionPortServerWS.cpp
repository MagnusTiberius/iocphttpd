#include "SocketCompletionPortServerWS.h"

namespace WebSocket
{

	SocketCompletionPortServerWS::SocketCompletionPortServerWS()
	{
		ghMutex = CreateMutex(
			NULL,              // default security attributes
			FALSE,             // initially not owned
			NULL);

	}


	SocketCompletionPortServerWS::~SocketCompletionPortServerWS()
	{
		::CloseHandle(ghMutex);
	}

	SocketCompletionPortServerWS::SocketCompletionPortServerWS(int PortNum)
	{
		m_PortNum = PortNum;
	}

	void SocketCompletionPortServerWS::Start(int PortNum)
	{
		m_PortNum = PortNum;
		Start();
	}

	int SocketCompletionPortServerWS::Start()
	{
		SOCKADDR_IN InternetAddr;
		SOCKET Listen;
		HANDLE ThreadHandle;
		SOCKET Accept;

		SYSTEM_INFO SystemInfo;
		LPPER_HANDLE_DATA PerHandleData;
		LPPER_IO_OPERATION_DATA PerIoData;
		int i;
		DWORD RecvBytes;
		DWORD Flags;
		DWORD ThreadID;
		WSADATA wsaData;
		DWORD Ret;
		DWORD dwThreadId = GetCurrentThreadId();

		if ((Ret = WSAStartup((2, 2), &wsaData)) != 0)
		{
			printf("WSAStartup() failed with error %d\n", Ret);
			return 1;
		}
		else
			printf("WSAStartup() is OK!\n");

		// Setup an I/O completion port
		if ((CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL)
		{
			printf("CreateIoCompletionPort() failed with error %d\n", GetLastError());
			return 1;
		}
		else
			printf("CreateIoCompletionPort() is damn OK!\n");

		// Determine how many processors are on the system
		GetSystemInfo(&SystemInfo);
		// Create worker threads based on the number of processors available on the
		// system. Create two worker threads for each processor

		int nThreads = (int)SystemInfo.dwNumberOfProcessors * 2;

		if (nThreads <= 8)
		{
			nThreads = 8;
		}

		for (i = 0; i < nThreads; i++)
		{
			// Create a server worker thread and pass the completion port to the thread
			if ((ThreadHandle = CreateThread(NULL, 0, ServerWorkerThread, this, 0, &ThreadID)) == NULL)
			{
				printf("WebSocket CreateThread() failed with error %d\n", GetLastError());
				return 1;
			}
			else
				printf("WebSocket CreateThread() is OK!\n");
			// Close the thread handle
			CloseHandle(ThreadHandle);
		}

		// Create a listening socket
		if ((Listen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
		{
			printf("WSASocket() failed with error %d\n", WSAGetLastError());
			return 1;
		}
		else
			printf("WSASocket() is OK!\n");

		InternetAddr.sin_family = AF_INET;
		InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		InternetAddr.sin_port = htons(m_PortNum);

		if (bind(Listen, (PSOCKADDR)&InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR)
		{
			printf("bind() failed with error %d\n", WSAGetLastError());
			return 1;
		}
		else
			printf("bind() is fine!\n");

		// Prepare socket for listening
		if (listen(Listen, 5) == SOCKET_ERROR)
		{
			printf("listen() failed with error %d\n", WSAGetLastError());
			return 1;
		}
		else
			printf("listen() is working...\n");

		//completionPortStackListener.Start();

		// Accept connections and assign to the completion port
		while (TRUE)
		{
			if ((Accept = WSAAccept(Listen, NULL, NULL, NULL, 0)) == SOCKET_ERROR)
			{
				printf("WSAAccept() failed with error %d\n", WSAGetLastError());
				return 1;
			}
			else
				printf("WSAAccept() looks fine!\n");

			// Create a socket information structure to associate with the socket
			if ((PerHandleData = (LPPER_HANDLE_DATA)GlobalAlloc(GPTR, sizeof(PER_HANDLE_DATA))) == NULL)
				printf("GlobalAlloc() failed with error %d\n", GetLastError());
			else
				printf("GlobalAlloc() for LPPER_HANDLE_DATA is OK!\n");

			// Associate the accepted socket with the original completion port
			printf("Socket number %d got connected...\n", Accept);
			PerHandleData->Socket = Accept;

			if (CreateIoCompletionPort((HANDLE)Accept, CompletionPort, (DWORD)PerHandleData, 0) == NULL)
			{
				printf("CreateIoCompletionPort() failed with error %d\n", GetLastError());
				return 1;
			}
			else
				printf("CreateIoCompletionPort() is OK!\n");

			// Create per I/O socket information structure to associate with the WSARecv call below
			if ((PerIoData = (LPPER_IO_OPERATION_DATA)GlobalAlloc(GPTR, sizeof(PER_IO_OPERATION_DATA))) == NULL)
			{
				printf("GlobalAlloc() failed with error %d\n", GetLastError());
				return 1;
			}
			else
				printf("GlobalAlloc() for LPPER_IO_OPERATION_DATA is OK!\n");

			ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
			PerIoData->BytesSEND = 0;
			PerIoData->BytesRECV = 0;
			PerIoData->DataBuf.len = DATA_BUFSIZE;
			PerIoData->DataBuf.buf = PerIoData->Buffer;

			Flags = 0;
			if (WSARecv(Accept, &(PerIoData->DataBuf), 1, &RecvBytes, &Flags, &(PerIoData->Overlapped), NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != ERROR_IO_PENDING)
				{
					printf("WSARecv() failed with error %d\n", WSAGetLastError());
					return 1;
				}
			}
			else
				printf("WSARecv() is OK!\n");

		}

	}


	DWORD WINAPI SocketCompletionPortServerWS::ServerWorkerThread(LPVOID lpObject)
	{
		SocketCompletionPortServerWS* instance = (SocketCompletionPortServerWS*)lpObject;
		HANDLE CompletionPort = instance->CompletionPort;
		DWORD BytesTransferred;
		LPPER_HANDLE_DATA PerHandleData;
		LPPER_IO_OPERATION_DATA PerIoData, PerIoDataSend;
		DWORD SendBytes, RecvBytes;
		DWORD Flags;

		DWORD dwThreadId = GetCurrentThreadId();
		char msg[8192];



		while (TRUE)
		{
			if (GetQueuedCompletionStatus(CompletionPort, &BytesTransferred,
				(LPDWORD)&PerHandleData, (LPOVERLAPPED *)&PerIoData, INFINITE) == 0)
			{
				printf("GetQueuedCompletionStatus() failed with error %d\n", GetLastError());
				return 0;
			}
			else
				printf("GetQueuedCompletionStatus() is OK!\n");

			// First check to see if an error has occurred on the socket and if so
			// then close the socket and cleanup the SOCKET_INFORMATION structure
			// associated with the socket
			if (BytesTransferred == 0)
			{
				printf("Closing socket %d\n", PerHandleData->Socket);
				if (closesocket(PerHandleData->Socket) == SOCKET_ERROR)
				{
					printf("closesocket() failed with error %d\n", WSAGetLastError());
					return 0;
				}
				else
					printf("closesocket() is fine!\n");

				GlobalFree(PerHandleData);
				GlobalFree(PerIoData);
				continue;
			}


			if (PerIoData == NULL)
			{
				continue;
			}

			if (PerIoData->DataBuf.buf == NULL)
			{
				continue;
			}

			if (strlen(PerIoData->DataBuf.buf) == 0)
			{
				continue;
			}



			//httpRequest.Parse(PerIoData->DataBuf.buf);

			//instance->Dispatch(&httpRequest, &httpResponse);
			//ZeroMemory(PerIoData->Buffer, BUFSIZMIN);
			//PerIoData->DataBuf.buf = (char*)httpResponse.GetResponse2(&PerIoData->DataBuf.len);
			//PerIoData->BytesRECV = 0;


			int len = strlen(PerIoData->DataBuf.buf);
			int res = ::send(PerHandleData->Socket, PerIoData->DataBuf.buf, len, NULL);

			if (res != SOCKET_ERROR)
			{
				SendBytes = PerIoData->BytesSEND;
				sprintf(msg, "%d::WSASEND: Socket=%d; SendBytes=%d; PerIoDataSend->BytesRECV=%d; PerIoDataSend->BytesSEND=%d; PerIoDataSend->DataBuf.len=%d\n",
					dwThreadId, PerHandleData->Socket, SendBytes, PerIoData->BytesRECV, PerIoData->BytesSEND, PerIoData->DataBuf.len);
			}
			else
			{
				sprintf(msg, "%d::ServerWorkerThread--WSASend() failed with error %d\n", dwThreadId, WSAGetLastError());
				//return 0;
			}
			//free(PerIoData->DataBuf.buf);


			PerIoData->BytesRECV = 0;
			// Now that there are no more bytes to send post another WSARecv() request
			Flags = 0;
			ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
			PerIoData->DataBuf.len = DATA_BUFSIZE;
			PerIoData->DataBuf.buf = PerIoData->Buffer;

			if (WSARecv(PerHandleData->Socket, &(PerIoData->DataBuf), 1, &RecvBytes, &Flags,
				&(PerIoData->Overlapped), NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != ERROR_IO_PENDING)
				{
					printf("WSARecv() failed with error %d\n", WSAGetLastError());
					return 0;
				}
			}
			else
				printf("WSARecv() is OK!\n");

		}
	}



	HANDLE SocketCompletionPortServerWS::GetCompletionPort()
	{
		return CompletionPort;
	}


}