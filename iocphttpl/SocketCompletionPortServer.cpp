#include "SocketCompletionPortServer.h"


SocketCompletionPortServer::SocketCompletionPortServer()
{
	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);

}


SocketCompletionPortServer::~SocketCompletionPortServer()
{
	::CloseHandle(ghMutex);
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

	int nThreads = (int)SystemInfo.dwNumberOfProcessors * 2;

	nThreads = (nThreads / 2);

	// Create worker threads based on the number of processors available on the
	// system. Create two worker threads for each processor
	for (i = 0; i < nThreads; i++)
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
			printf("####################################################################################\n");
			printf("####################################################################################\n");
			printf("###################### ERROR                                        ################\n");
			printf("####################################################################################\n");
			printf("####################################################################################\n");
			exit(1);
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
			printf("####################################################################################\n");
			printf("####################################################################################\n");
			printf("###################### ERROR                                        ################\n");
			printf("####################################################################################\n");
			printf("####################################################################################\n");
			exit(1);
			isOK = false;
			continue;
		}
		else
			fprintf(stderr, "%d::CreateIoCompletionPort() is OK!\n", dwThreadId);

		// Create per I/O socket information structure to associate with the WSARecv call below
		if ((PerIoData = (LPPER_IO_OPERATION_DATA)GlobalAlloc(GPTR, sizeof(PER_IO_OPERATION_DATA))) == NULL)
		{
			fprintf(stderr, "%d::GlobalAlloc() failed with error %d\n", dwThreadId, GetLastError());
			printf("####################################################################################\n");
			printf("####################################################################################\n");
			printf("###################### ERROR                                        ################\n");
			printf("####################################################################################\n");
			printf("####################################################################################\n");
			exit(1);
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

		printf("\n\n%d::WSARECV1 Socket=%d, PerIoData->BytesRECV=%d; PerIoData->BytesSEND=%d\n\n", dwThreadId, PerHandleData->Socket, PerIoData->BytesRECV, PerIoData->BytesSEND);

		if (dwRes == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				fprintf(stderr, "%d::WSARecv() failed with error %d\n", dwThreadId, WSAGetLastError());
				printf("####################################################################################\n");
				printf("####################################################################################\n");
				printf("###################### ERROR                                        ################\n");
				printf("####################################################################################\n");
				printf("####################################################################################\n");
				exit(1);
				isOK = true;
				continue;
			}
		}
		else
			fprintf(stderr, "%d::WSARecv() is OK!\n", dwThreadId);

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
	LPPER_IO_OPERATION_DATA PerIoData, PerIoDataSend;
	DWORD SendBytes, RecvBytes;
	DWORD Flags;

	HttpRequest httpRequest;
	HttpResponse httpResponse;

	DWORD dwThreadId = GetCurrentThreadId();

	while (TRUE)
	{
		BytesTransferred = 0;
		BOOL res1 = GetQueuedCompletionStatus(CompletionPort, &BytesTransferred, (PULONG_PTR)&PerHandleData, (LPOVERLAPPED *)&PerIoData, INFINITE);
		if (res1==0)
		{
			fprintf(stderr, "%d::ServerWorkerThread--GetQueuedCompletionStatus() failed with error %d\n", dwThreadId, GetLastError());
			return 0;
		}
		else
			fprintf(stderr, "%d::ServerWorkerThread--GetQueuedCompletionStatus() is OK!\n", dwThreadId);

		printf("\n\n%d::WSARECV2 Socket=%d, BytesTransferred=%d; PerIoData->BytesRECV=%d; PerIoData->BytesSEND=%d\n\n", dwThreadId, PerHandleData->Socket, BytesTransferred, PerIoData->BytesRECV, PerIoData->BytesSEND);


		if (BytesTransferred > 0 && PerIoData->BytesRECV == 0 && PerIoData->BytesSEND == 0 && PerIoData->DataBuf.len > 0)
		{
			::WaitForSingleObject(obj->ghMutex, INFINITE);
			if ((PerIoDataSend = (LPPER_IO_OPERATION_DATA)GlobalAlloc(GPTR, sizeof(PER_IO_OPERATION_DATA))) == NULL)
			{

			}

			httpRequest.Parse(PerIoData->DataBuf.buf);
			printf("%d::Content: %d\n", dwThreadId, httpRequest.GetContent());
			obj->Dispatch(&httpRequest, &httpResponse);
			ZeroMemory(PerIoDataSend->Buffer, DATA_BUFSIZE);
			ZeroMemory(&(PerIoDataSend->Overlapped), sizeof(OVERLAPPED));
			PerIoDataSend->DataBuf.buf = PerIoDataSend->Buffer;
			httpResponse.GetResponse(PerIoDataSend->Buffer, &PerIoDataSend->byteBuffer, DATA_BUFSIZE);
			httpResponse.Write("");
			auto n = strlen(PerIoDataSend->Buffer);
			PerIoDataSend->DataBuf.len = (ULONG)n;
			if (PerIoDataSend->byteBuffer.size() > 0)
			{
				size_t bufsiz = PerIoDataSend->byteBuffer.size() * sizeof(PerIoDataSend->byteBuffer[0]);
				if (PerIoDataSend->LPBuffer != NULL)
				{
					free(PerIoDataSend->LPBuffer);
					PerIoDataSend->LPBuffer = NULL;
				}
				PerIoDataSend->LPBuffer = (CHAR*)malloc(bufsiz);
				memset(PerIoDataSend->LPBuffer, '\0', bufsiz);
				memcpy(PerIoDataSend->LPBuffer, &PerIoDataSend->byteBuffer[0], bufsiz);
				PerIoDataSend->DataBuf.buf = PerIoDataSend->LPBuffer;
				PerIoDataSend->DataBuf.len = bufsiz;
			}
			PerIoDataSend->BytesRECV = 0;
			int res = WSASend(PerHandleData->Socket, &(PerIoDataSend->DataBuf), 1, &PerIoDataSend->BytesSEND, 0, &(PerIoDataSend->Overlapped), NULL);
			if (res == SOCKET_ERROR)
			{
				fprintf(stderr, "%d::ServerWorkerThread--WSASend() failed with error %d\n", dwThreadId, WSAGetLastError());
			}
			SendBytes = PerIoDataSend->BytesSEND;
			printf("\n\n%d::WSASEND: Socket=%d; SendBytes=%d; PerIoDataSend->BytesRECV=%d; PerIoDataSend->BytesSEND=%d\n\n", dwThreadId, PerHandleData->Socket, SendBytes, PerIoDataSend->BytesRECV, PerIoDataSend->BytesSEND);
			::ReleaseMutex(obj->ghMutex);

			continue;
		}

		// First check to see if an error has occurred on the socket and if so
		// then close the socket and cleanup the SOCKET_INFORMATION structure
		// associated with the socket
		if (BytesTransferred == 0 || PerIoData->BytesRECV == 0 || PerIoData->DataBuf.len == 0)
		{
			printf("%d::ServerWorkerThread--Closing socket %d\n", dwThreadId, PerHandleData->Socket);
			if (closesocket(PerHandleData->Socket) == SOCKET_ERROR)
			{
				fprintf(stderr, "%d::ServerWorkerThread--closesocket() failed with error %d\n", dwThreadId, WSAGetLastError());
				return 0;
			}
			else
				fprintf(stderr, "%d::ServerWorkerThread--closesocket() is fine!\n", dwThreadId);

			GlobalFree(PerHandleData);
			GlobalFree(PerIoData);
			continue;
		}


		if (PerIoData->BytesRECV > 0)
		{
			::WaitForSingleObject(obj->ghMutex, INFINITE);
			if ((PerIoDataSend = (LPPER_IO_OPERATION_DATA)GlobalAlloc(GPTR, sizeof(PER_IO_OPERATION_DATA))) == NULL)
			{

			}

			httpRequest.Parse(PerIoData->DataBuf.buf);
			printf("%d::Content: %d\n", dwThreadId, httpRequest.GetContent());
			obj->Dispatch(&httpRequest, &httpResponse);
			ZeroMemory(PerIoDataSend->Buffer, DATA_BUFSIZE);
			ZeroMemory(&(PerIoDataSend->Overlapped), sizeof(OVERLAPPED));
			PerIoDataSend->DataBuf.buf = PerIoDataSend->Buffer;
			httpResponse.GetResponse(PerIoDataSend->Buffer, &PerIoDataSend->byteBuffer, DATA_BUFSIZE);
			httpResponse.Write("");
			auto n = strlen(PerIoDataSend->Buffer);
			PerIoDataSend->DataBuf.len = (ULONG)n;
			if (PerIoDataSend->byteBuffer.size() > 0)
			{
				size_t bufsiz = PerIoDataSend->byteBuffer.size() * sizeof(PerIoDataSend->byteBuffer[0]);
				if (PerIoDataSend->LPBuffer != NULL)
				{
					free(PerIoDataSend->LPBuffer);
					PerIoDataSend->LPBuffer = NULL;
				}
				PerIoDataSend->LPBuffer = (CHAR*)malloc(bufsiz);
				memset(PerIoDataSend->LPBuffer, '\0', bufsiz);
				memcpy(PerIoDataSend->LPBuffer, &PerIoDataSend->byteBuffer[0], bufsiz);
				PerIoDataSend->DataBuf.buf = PerIoDataSend->LPBuffer;
				PerIoDataSend->DataBuf.len = bufsiz;
			}
			PerIoDataSend->BytesRECV = 0;
			int res = WSASend(PerHandleData->Socket, &(PerIoDataSend->DataBuf), 1, &PerIoDataSend->BytesSEND, 0, &(PerIoDataSend->Overlapped), NULL);
			if (res == SOCKET_ERROR)
			{
				fprintf(stderr, "%d::ServerWorkerThread--WSASend() failed with error %d\n", dwThreadId, WSAGetLastError());
			}
			SendBytes = PerIoDataSend->BytesSEND;
			printf("\n\n%d::WSASEND: Socket=%d; SendBytes=%d; PerIoDataSend->BytesRECV=%d; PerIoDataSend->BytesSEND=%d\n\n", dwThreadId, PerHandleData->Socket, SendBytes, PerIoDataSend->BytesRECV, PerIoDataSend->BytesSEND);
			::ReleaseMutex(obj->ghMutex);
		}

	}
}