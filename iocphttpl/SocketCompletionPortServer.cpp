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

void SocketCompletionPortServer::Start(int PortNum)
{
	m_PortNum = PortNum;
	Start();
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
	DWORD dwThreadId = GetCurrentThreadId();

	if ((Ret = WSAStartup((2, 2), &wsaData)) != 0)
	{
		fprintf(stderr, "%d::WSAStartup() failed with error %d\n", dwThreadId, Ret);
		return 1;
	}
	else
		fprintf(stderr, "%d::WSAStartup() is OK!\n", dwThreadId);

	// Setup an I/O completion port
	if ((CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL)
	{
		fprintf(stderr, "%d::CreateIoCompletionPort() failed with error %d\n", dwThreadId, GetLastError());
		return 1;
	}
	else
		fprintf(stderr, "%d::CreateIoCompletionPort() is damn OK!\n", dwThreadId);


	// Determine how many processors are on the system
	GetSystemInfo(&SystemInfo);
	// Create worker threads based on the number of processors available on the
	// system. Create two worker threads for each processor
	for (i = 0; i < (int)SystemInfo.dwNumberOfProcessors * 2; i++)
	{
		// Create a server worker thread and pass the completion port to the thread
		if ((ThreadHandle = CreateThread(NULL, 0, ServerWorkerThread, this, 0, &ThreadID)) == NULL)
		{
			fprintf(stderr, "%d::CreateThread() failed with error %d\n", dwThreadId, GetLastError());
			return 1;
		}
		else
			fprintf(stderr, "%d::CreateThread() is OK!\n", dwThreadId);
		// Close the thread handle
		CloseHandle(ThreadHandle);
	}

	// Create a listening socket
	if ((Listen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		fprintf(stderr, "%d::WSASocket() failed with error %d\n", dwThreadId, WSAGetLastError());
		return 1;
	}
	else
		fprintf(stderr, "%d::WSASocket() is OK!\n", dwThreadId);

	InternetAddr.sin_family = AF_INET;
	InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	InternetAddr.sin_port = htons(m_PortNum);

	if (bind(Listen, (PSOCKADDR)&InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR)
	{
		fprintf(stderr, "%d::bind() failed with error %d\n", dwThreadId, WSAGetLastError());
		return 1;
	}
	else
		fprintf(stderr, "%d::bind() is fine!\n", dwThreadId);

	// Prepare socket for listening
	if (listen(Listen, 5) == SOCKET_ERROR)
	{
		fprintf(stderr, "%d::listen() failed with error %d\n", dwThreadId, WSAGetLastError());
		return 1;
	}
	else
		fprintf(stderr, "%d::listen() is working...\n", dwThreadId);

	// Accept connections and assign to the completion port
	BOOL isOK = true;
	while (isOK)
	{
		if ((Accept = WSAAccept(Listen, NULL, NULL, NULL, 0)) == SOCKET_ERROR)
		{
			fprintf(stderr, "%d::WSAAccept() failed with error %d\n", dwThreadId, WSAGetLastError());
			isOK = false;
			continue;
		}
		else
			fprintf(stderr, "%d::WSAAccept() looks fine!\n", dwThreadId);

		// Create a socket information structure to associate with the socket
		if ((PerHandleData = (LPPER_HANDLE_DATA)GlobalAlloc(GPTR, sizeof(PER_HANDLE_DATA))) == NULL)
			fprintf(stderr, "%d::GlobalAlloc() failed with error %d\n", GetLastError());
		else
			fprintf(stderr, "%d::GlobalAlloc() for LPPER_HANDLE_DATA is OK!\n", dwThreadId, dwThreadId);

		// Associate the accepted socket with the original completion port
		fprintf(stderr, "%d::Socket number %d got connected...\n", dwThreadId, Accept);
		PerHandleData->Socket = Accept;

		if (CreateIoCompletionPort((HANDLE)Accept, CompletionPort, (DWORD)PerHandleData, 0) == NULL)
		{
			fprintf(stderr, "%d::CreateIoCompletionPort() failed with error %d\n", dwThreadId, GetLastError());
			isOK = false;
			continue;
		}
		else
			fprintf(stderr, "%d::CreateIoCompletionPort() is OK!\n", dwThreadId);

		// Create per I/O socket information structure to associate with the WSARecv call below
		if ((PerIoData = (LPPER_IO_OPERATION_DATA)GlobalAlloc(GPTR, sizeof(PER_IO_OPERATION_DATA))) == NULL)
		{
			fprintf(stderr, "%d::GlobalAlloc() failed with error %d\n", dwThreadId, GetLastError());
			isOK = false;
			continue;
		}
		else
			fprintf(stderr, "%d::GlobalAlloc() for LPPER_IO_OPERATION_DATA is OK!\n", dwThreadId);

		ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
		PerIoData->BytesSEND = 0;
		PerIoData->BytesRECV = 0;
		PerIoData->DataBuf.len = DATA_BUFSIZE;
		PerIoData->DataBuf.buf = PerIoData->Buffer;
		PerIoData->LPBuffer = NULL;

		Flags = 0;
		DWORD dwRes = WSARecv(Accept, &(PerIoData->DataBuf), 1, &PerIoData->BytesRECV, &Flags, &(PerIoData->Overlapped), NULL);
		RecvBytes = PerIoData->BytesRECV;
		if (dwRes == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				fprintf(stderr, "%d::WSARecv() failed with error %d\n", dwThreadId, WSAGetLastError());
				isOK = true;
				continue;
			}
		}
		else
			fprintf(stderr, "%d::WSARecv() is OK!\n", dwThreadId);


		printf("%d::RECV bytes: %d\n", dwThreadId, PerIoData->BytesRECV);
		printf("\n%d\n\n---------------------------------------------------\nDATA: %s\n\n", dwThreadId, PerIoData->DataBuf.buf);

	}
	return 0;
}

HANDLE SocketCompletionPortServer::GetCompletionPort()
{
	return CompletionPort;
}

void SocketCompletionPortServer::EvalGet(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	fprintf(stderr, "SocketCompletionPortServer::EvalGet\n");
}

void SocketCompletionPortServer::EvalPost(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	fprintf(stderr, "SocketCompletionPortServer::EvalPost\n");
}

void SocketCompletionPortServer::Dispatch(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	httpResponse->isStatic = false;
	httpRequest->isStatic = false;

	printf("Dispatching %s \n", httpRequest->GetUrl());

	if (httpRequest->GetMethod() == MethodType::HTTP_GET)
	{
		EvalGet(httpRequest, httpResponse);
	}
	if (httpRequest->GetMethod() == MethodType::HTTP_POST)
	{
		EvalPost(httpRequest, httpResponse);
	}

	if (IsStatic(httpRequest->GetUrl()))
	{
		printf("Static Directory\n");
		httpResponse->isStatic = true;
		httpRequest->isStatic = true;
		EvalStatic(httpRequest, httpResponse);
		return;
	}

	LPSTATICFUNC lpFunc = (LPSTATICFUNC)GetRoute(httpRequest->GetUrl());
	if (lpFunc != NULL)
	{
		(*lpFunc)(httpRequest, httpResponse);
	}
	else
	{
		UrlNotFound(httpRequest, httpResponse);
	}
}

void SocketCompletionPortServer::EvalStatic(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	char *str = httpRequest->GetUrl();
	string s = GetFullPath(str);
	printf("Static %s\n", s.c_str());
	httpResponse->SetStaticFileName(s);
	httpResponse->WriteStatic(s.c_str());
}


void SocketCompletionPortServer::UrlNotFound(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	fprintf(stderr, "SocketCompletionPortServer::UrlNotFound: \n");
}

DWORD WINAPI SocketCompletionPortServer::ServerWorkerThread(LPVOID lpObject)
{
	SocketCompletionPortServer *obj = (SocketCompletionPortServer*)lpObject;
	HANDLE CompletionPort = (HANDLE)obj->GetCompletionPort();
	DWORD BytesTransferred;
	LPPER_HANDLE_DATA PerHandleData;
	LPPER_IO_OPERATION_DATA PerIoData;
	DWORD SendBytes, RecvBytes;
	DWORD Flags;

	HttpRequest httpRequest;
	HttpResponse httpResponse;

	DWORD dwThreadId = GetCurrentThreadId();

	while (TRUE)
	{
		fprintf(stderr, "%d::Calling GetQueuedCompletionStatus\n", dwThreadId);
		BOOL res1 = GetQueuedCompletionStatus(CompletionPort, &BytesTransferred, (PULONG_PTR)&PerHandleData, (LPOVERLAPPED *)&PerIoData, INFINITE);
		if (res1==0)
		{
			fprintf(stderr, "%d::++++++ServerWorkerThread--GetQueuedCompletionStatus() failed with error %d\n", dwThreadId, GetLastError());
			return 0;
		}
		else
			fprintf(stderr, "%d::++++++ServerWorkerThread--GetQueuedCompletionStatus() is OK!\n", dwThreadId);

		printf("%d::BytesTransferred=%d; DataBuf.len=%d\n", dwThreadId, BytesTransferred, PerIoData->DataBuf.len);
		printf("%d::BytesRECV=%d; BytesSEND=%d\n", dwThreadId, PerIoData->BytesRECV, PerIoData->BytesSEND);
		printf("DATA FROM QUEUE:\n%s\n", PerIoData->DataBuf.buf);

		// First check to see if an error has occurred on the socket and if so
		// then close the socket and cleanup the SOCKET_INFORMATION structure
		// associated with the socket
		if (BytesTransferred == 0 )
		{
			printf("%d::++++++ServerWorkerThread--Closing socket %d\n", dwThreadId, PerHandleData->Socket);
			if (closesocket(PerHandleData->Socket) == SOCKET_ERROR)
			{
				fprintf(stderr, "%d::++++++ServerWorkerThread--closesocket() failed with error %d\n", dwThreadId, WSAGetLastError());
				return 0;
			}
			else
				fprintf(stderr, "%d::++++++ServerWorkerThread--closesocket() is fine!\n", dwThreadId);

			GlobalFree(PerHandleData);
			GlobalFree(PerIoData);
			continue;
		}
		if (BytesTransferred > 0 || PerIoData->BytesRECV > 0)
		{
			httpRequest.Parse(PerIoData->DataBuf.buf);
			fprintf(stderr, "%d::++++++Calling dispatch\n", dwThreadId);
			obj->Dispatch(&httpRequest, &httpResponse);
			if (httpResponse.isStatic)
			{
				int nBufSize = httpResponse.GetBufferSize() + 1;
				fprintf(stderr, "%d::ServerWorkerThread: nBufSize=%d\n", dwThreadId, nBufSize);
				PerIoData->LPBuffer = (CHAR*)malloc(nBufSize);
				memset(PerIoData->LPBuffer, 0, nBufSize);
				PerIoData->LPBuffer = httpResponse.GetResponse2();
				PerIoData->DataBuf.buf = PerIoData->LPBuffer;
				PerIoData->DataBuf.len = nBufSize;
				PerIoData->BytesSEND = nBufSize;
				PerIoData->BytesRECV = 0;
				httpResponse.Write("");
				printf("Buffer size: %d\n", nBufSize);
				printf("Sending file out: %s\n", httpResponse.GetStaticFileName());
				printf("DATA: \n %s\n", PerIoData->DataBuf.buf);
			}
			else
			{
				ZeroMemory(PerIoData->Buffer, DATA_BUFSIZE);
				ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
				PerIoData->DataBuf.buf = PerIoData->Buffer;
				httpResponse.GetResponse(PerIoData->Buffer, &PerIoData->byteBuffer, DATA_BUFSIZE);
				PerIoData->BytesSEND = PerIoData->byteBuffer.size();
				PerIoData->BytesRECV = 0;
				httpResponse.Write("");
				auto n = strlen(PerIoData->Buffer);
				PerIoData->DataBuf.len = (ULONG)n;
			}
			SendBytes = PerIoData->BytesSEND;
			PerIoData->BytesRECV = 0;
			int res = WSASend(PerHandleData->Socket, &(PerIoData->DataBuf), 1, &SendBytes, 0, &(PerIoData->Overlapped), NULL);
			if (res == SOCKET_ERROR)
			{
				fprintf(stderr, "%d::++++++ServerWorkerThread--WSASend() failed with error %d\n", dwThreadId, WSAGetLastError());
			}
			printf("++++++Bytes sent: %d\n", SendBytes);
		}
		else
		{
			fprintf(stderr, "%d::++++++Nothing to do: BytesTransferred=%d; SendBytes=%d\n", dwThreadId, BytesTransferred, SendBytes);
		}

	}
}