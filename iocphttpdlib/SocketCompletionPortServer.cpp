#include "SocketCompletionPortServer.h"


SocketCompletionPortServer::SocketCompletionPortServer()
{
}


SocketCompletionPortServer::~SocketCompletionPortServer()
{
}

SocketCompletionPortServer::SocketCompletionPortServer(int PortNum)
{
	m_PortNum = PortNum;
}

int SocketCompletionPortServer::Start()
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
	for (i = 0; i < (int)SystemInfo.dwNumberOfProcessors * 2; i++)
	{
		// Create a server worker thread and pass the completion port to the thread
		if ((ThreadHandle = CreateThread(NULL, 0, ServerWorkerThread, this, 0, &ThreadID)) == NULL)
		{
			printf("CreateThread() failed with error %d\n", GetLastError());
			return 1;
		}
		else
			printf("CreateThread() is OK!\n");
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

	// Accept connections and assign to the completion port
	BOOL isOK = true;
	while (isOK)
	{
		if ((Accept = WSAAccept(Listen, NULL, NULL, NULL, 0)) == SOCKET_ERROR)
		{
			printf("WSAAccept() failed with error %d\n", WSAGetLastError());
			isOK = false;
			continue;
		}
		else
			printf("WSAAccept() looks fine!\n");

		// Create a socket information structure to associate with the socket
		if ((PerHandleData = (LPPER_HANDLE_DATA)GlobalAlloc(GPTR, sizeof(PER_HANDLE_DATA))) == NULL)
			printf("GlobalAlloc() failed with error %d\n", GetLastError());
		else
			printf("GlobalAlloc() for LPPER_HANDLE_DATA is OK!\n");
		//return 1;

		// Associate the accepted socket with the original completion port
		printf("Socket number %d got connected...\n", Accept);
		PerHandleData->Socket = Accept;

		if (CreateIoCompletionPort((HANDLE)Accept, CompletionPort, (DWORD)PerHandleData, 0) == NULL)
		{
			printf("CreateIoCompletionPort() failed with error %d\n", GetLastError());
			isOK = false;
			continue;
		}
		else
			printf("CreateIoCompletionPort() is OK!\n");

		// Create per I/O socket information structure to associate with the WSARecv call below
		if ((PerIoData = (LPPER_IO_OPERATION_DATA)GlobalAlloc(GPTR, sizeof(PER_IO_OPERATION_DATA))) == NULL)
		{
			printf("GlobalAlloc() failed with error %d\n", GetLastError());
			isOK = false;
			continue;
		}
		else
			printf("GlobalAlloc() for LPPER_IO_OPERATION_DATA is OK!\n");

		ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
		PerIoData->BytesSEND = 0;
		PerIoData->BytesRECV = 0;
		PerIoData->DataBuf.len = DATA_BUFSIZE;
		PerIoData->DataBuf.buf = PerIoData->Buffer;

		Flags = 0;
		DWORD dwRes = WSARecv(Accept, &(PerIoData->DataBuf), 1, &PerIoData->BytesRECV, &Flags, &(PerIoData->Overlapped), NULL);
		RecvBytes = PerIoData->BytesRECV;
		if (dwRes == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				printf("WSARecv() failed with error %d\n", WSAGetLastError());
				isOK = false;
				continue;
			}
		}
		else
			printf("WSARecv() is OK!\n");

	}
	return 0;
}

HANDLE SocketCompletionPortServer::GetCompletionPort()
{
	return CompletionPort;
}

void SocketCompletionPortServer::EvalGet(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	printf("SocketCompletionPortServer::EvalGet\n");
}

void SocketCompletionPortServer::EvalPost(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	printf("SocketCompletionPortServer::EvalPost\n");
}

void SocketCompletionPortServer::Dispatch(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	if (httpRequest->GetMethod() == HttpHeader::MethodType::HTTP_GET)
	{
		EvalGet(httpRequest, httpResponse);
	}
	if (httpRequest->GetMethod() == HttpHeader::MethodType::HTTP_POST)
	{
		EvalPost(httpRequest, httpResponse);
	}

	LPSTATICFUNC lpFunc = (LPSTATICFUNC)GetRoute(httpRequest->GetUrl());
	if (lpFunc != NULL)
	{
		(*lpFunc)(httpRequest, httpResponse);
	}
}

DWORD WINAPI SocketCompletionPortServer::ServerWorkerThread(LPVOID lpObject)
{
	SocketCompletionPortServer *obj = (SocketCompletionPortServer*)lpObject;
	HANDLE CompletionPort = (HANDLE)obj->GetCompletionPort();
	DWORD BytesTransferred;
	LPPER_HANDLE_DATA PerHandleData;
	LPPER_IO_OPERATION_DATA PerIoData;
	LPPER_IO_OPERATION_DATA PerIoDataSend;
	LPPER_IO_OPERATION_DATA PerIoDataRecv;
	DWORD SendBytes, RecvBytes;
	DWORD Flags;

	HttpRequest httpRequest;
	HttpResponse httpResponse;

	while (TRUE)
	{
		BOOL res1 = GetQueuedCompletionStatus(CompletionPort, &BytesTransferred, (PULONG_PTR)&PerHandleData, (LPOVERLAPPED *)&PerIoData, INFINITE);
		if (res1==0)
		{
			printf("ServerWorkerThread--GetQueuedCompletionStatus() failed with error %d\n", GetLastError());
			return 0;
		}
		else
			printf("ServerWorkerThread--GetQueuedCompletionStatus() is OK!\n");

		// First check to see if an error has occurred on the socket and if so
		// then close the socket and cleanup the SOCKET_INFORMATION structure
		// associated with the socket
		if (BytesTransferred == 0 || PerIoData->BytesRECV == 0)
		{
			printf("ServerWorkerThread--Closing socket %d\n", PerHandleData->Socket);
			if (closesocket(PerHandleData->Socket) == SOCKET_ERROR)
			{
				printf("ServerWorkerThread--closesocket() failed with error %d\n", WSAGetLastError());
				return 0;
			}
			else
				printf("ServerWorkerThread--closesocket() is fine!\n");

			GlobalFree(PerHandleData);
			GlobalFree(PerIoData);
			continue;
		}
		if (PerIoData->BytesRECV > 0)
		{
			httpRequest.Parse(PerIoData->DataBuf.buf);
			obj->Dispatch(&httpRequest, &httpResponse);
			ZeroMemory(PerIoData->Buffer, DATA_BUFSIZE);
			ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
			PerIoData->DataBuf.buf = PerIoData->Buffer;
			httpResponse.GetResponse(PerIoData->Buffer, DATA_BUFSIZE);
			httpResponse.Write("");
			auto n = strlen(PerIoData->Buffer);
			PerIoData->DataBuf.len = (ULONG)n;
			int res = WSASend(PerHandleData->Socket, &(PerIoData->DataBuf), 1, &SendBytes, 0, &(PerIoData->Overlapped), NULL);
			if (res == SOCKET_ERROR)
			{
				printf("ServerWorkerThread--WSASend() failed with error %d\n", WSAGetLastError());
			}
			/*
			Flags = 0;
			ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
			ZeroMemory(PerIoData->Buffer, DATA_BUFSIZE);
			PerIoData->DataBuf.len = DATA_BUFSIZE;
			PerIoData->DataBuf.buf = PerIoData->Buffer;

			DWORD res2 = WSARecv(PerHandleData->Socket, &(PerIoData->DataBuf), 1, &RecvBytes, &Flags,
				&(PerIoData->Overlapped), NULL);

			if (res2 == SOCKET_ERROR)
			{
				printf("ServerWorkerThread--WSARecv() failed with error %d\n", WSAGetLastError());
			}
			*/
		}

	}
}