#include "SocketCompletionPortServer.h"
#include "EventLog.h"

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
			printf("HTTP CreateThread() failed with error %d\n", GetLastError());
			return 1;
		}
		else
			printf("HTTP CreateThread() is OK!\n");
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
	//websocket.Start();

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

int SocketCompletionPortServer::Start2()
{
	SOCKADDR_IN InternetAddr;
	SOCKET Listen;
	HANDLE ThreadHandle;
	SOCKET Accept;
	
	SYSTEM_INFO SystemInfo;
	SocketIocpController::LPPER_HANDLE_DATA PerHandleData;
	SocketIocpController::LPPER_IO_OPERATION_DATA PerIoData;
	int i;
	DWORD RecvBytes;
	DWORD Flags;
	DWORD ThreadID;
	WSADATA wsaData;
	DWORD Ret;
	DWORD dwThreadId = GetCurrentThreadId();


	char msg2[8192];
	EventLog* eventLog2;
	eventLog2 = new EventLog(L"IOCP Worker Thread");

	if ((Ret = WSAStartup((2, 2), &wsaData)) != 0)
	{
		fprintf(stderr, "%d::WSAStartup() failed with error %d\n", dwThreadId, Ret);
		return 1;
	}
	else
		fprintf(stderr, "%d::WSAStartup() is OK!\n", dwThreadId);


	if ((CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL)
	{
		fprintf(stderr, "%d::CreateIoCompletionPort() failed with error %d\n", dwThreadId, GetLastError());
		return 1;
	}
	else
		fprintf(stderr, "%d::CreateIoCompletionPort() is damn OK!\n", dwThreadId);



	GetSystemInfo(&SystemInfo);

	int nThreads = (int)SystemInfo.dwNumberOfProcessors * 2;

	if (nThreads <= 8)
	{
		nThreads = 8;
	}

	fprintf(stderr, "%d::Threads created %d\n", dwThreadId, nThreads);

	//nThreads = 6;

	//nThreads = (nThreads / 2);


	for (i = 0; i < nThreads; i++)
	{

		if ((ThreadHandle = CreateThread(NULL, 0, ServerWorkerThread, this, 0, &ThreadID)) == NULL)
		{
			fprintf(stderr, "%d::CreateThread() failed with error %d\n", dwThreadId, GetLastError());
			return 1;
		}
		else
			fprintf(stderr, "%d::CreateThread() is OK!\n", dwThreadId);

		CloseHandle(ThreadHandle);
	}


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

	int bRes = bind(Listen, (PSOCKADDR)&InternetAddr, sizeof(InternetAddr)) ;
	while (bRes == SOCKET_ERROR)
	{
		fprintf(stderr, "%d::bind() failed with error %d\nLooking for next port...\n", dwThreadId, WSAGetLastError());
		InternetAddr.sin_port = htons(++m_PortNum);
		bRes = bind(Listen, (PSOCKADDR)&InternetAddr, sizeof(InternetAddr));
	}
	fprintf(stderr, "%d::bind() is fine! Port number at  %d\n", dwThreadId, m_PortNum);

	// Prepare socket for listening
	if (listen(Listen, 5) == SOCKET_ERROR)
	{
		fprintf(stderr, "%d::listen() failed with error %d\n", dwThreadId, WSAGetLastError());
		return 1;
	}
	else
		fprintf(stderr, "%d::listen() is working...\n", dwThreadId);

	BOOL isOK = true;
	while (isOK)
	{
		Accept = WSAAccept(Listen, NULL, NULL, NULL, 0);
		if (Accept == SOCKET_ERROR)
		{
			fprintf(stderr, "%d::WSAAccept() failed with error %d\n", dwThreadId, WSAGetLastError());
			isOK = false;

			exit(1);
			continue;
		}
		else
			fprintf(stderr, "%d::WSAAccept() looks fine!\n", dwThreadId);


		SocketIocpController::LPSOCKET_IO_DATA lpiodata = socketIocpController.Allocate();

		assert(lpiodata != NULL);

		PerHandleData = &lpiodata->handleData;

		//if ((PerHandleData = (LPPER_HANDLE_DATA)GlobalAlloc(GPTR, sizeof(PER_HANDLE_DATA))) == NULL)
		//	fprintf(stderr, "%d::GlobalAlloc() failed with error %d\n", dwThreadId, GetLastError());
		//else
		//	fprintf(stderr, "%d::GlobalAlloc() for LPPER_HANDLE_DATA is OK!\n", dwThreadId);


		fprintf(stderr, "%d::Socket number %d got connected...\n", dwThreadId, Accept);
		PerHandleData->Socket = Accept;

		if (CreateIoCompletionPort((HANDLE)Accept, CompletionPort, (DWORD)PerHandleData, 0) == NULL)
		{
			fprintf(stderr, "%d::CreateIoCompletionPort() failed with error %d\n", dwThreadId, GetLastError());

			exit(1);
			isOK = false;
			continue;
		}
		else
			fprintf(stderr, "%d::CreateIoCompletionPort() is OK!\n", dwThreadId);


		PerIoData = &lpiodata->operationData;

		PerIoData->BytesSEND = 0;
		PerIoData->BytesRECV = 0;
		PerIoData->DataBuf.len = DATA_BUFSIZE;
		PerIoData->DataBuf.buf = PerIoData->Buffer;
		PerIoData->LPBuffer = NULL;

		Flags = 0;
		DWORD dwRes = WSARecv(Accept, &(PerIoData->DataBuf), 1, &PerIoData->BytesRECV, &Flags, &(PerIoData->Overlapped), NULL);
		RecvBytes = PerIoData->BytesRECV;

		sprintf(msg2, "%d::WSARECV1 Socket=%d, PerIoData->BytesRECV=%d; PerIoData->BytesSEND=%d\n", dwThreadId, PerHandleData->Socket, PerIoData->BytesRECV, PerIoData->BytesSEND);
		eventLog2->WriteEventLogEntry2(msg2, EVENTLOG_ERROR_TYPE);

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

	}
	return 0;
}

DWORD WINAPI SocketCompletionPortServer::ServerWorkerThread(LPVOID lpObject)
{
	SocketCompletionPortServer* instance = (SocketCompletionPortServer*)lpObject;
	HANDLE CompletionPort = instance->CompletionPort;
	DWORD BytesTransferred;
	LPPER_HANDLE_DATA PerHandleData;
	LPPER_IO_OPERATION_DATA PerIoData, PerIoDataSend;
	DWORD SendBytes, RecvBytes;
	DWORD Flags;

	HttpRequest httpRequest;
	HttpResponse httpResponse;

	EventLog* eventLog;
	DWORD dwThreadId = GetCurrentThreadId();
	char msg[8192];

	httpResponse.SetCacheController(&(instance->cacheController));

	eventLog = new EventLog(L"IOCP Worker Thread");

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

		//if (::WaitForSingleObject(PerIoData->Overlapped.hEvent, 5000) != WAIT_OBJECT_0)
		//{
		//	sprintf(msg, "%d::WaitForSingleObject 1 WAIT_OBJECT_0 Mismatch\n", dwThreadId);
		//	eventLog->WriteEventLogEntry2(msg, EVENTLOG_ERROR_TYPE);
		//	::ReleaseMutex(PerIoData->Overlapped.hEvent);
		//	continue;
		//}



		//if (::WaitForSingleObject(instance->ghMutex, 5000) != WAIT_OBJECT_0)
		//{
		//	sprintf(msg, "%d::WaitForSingleObject 2 WAIT_OBJECT_0 Mismatch\n", dwThreadId);
		//	eventLog->WriteEventLogEntry2(msg, EVENTLOG_ERROR_TYPE);
		//	//::ReleaseMutex(instance->ghMutex);
		//	//::ReleaseMutex(PerIoData->Overlapped.hEvent);
		//	continue;
		//}

		try
		{

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
		}
		catch (...)
		{
			continue;
		}


		httpRequest.Parse(PerIoData->DataBuf.buf);

		instance->Dispatch(&httpRequest, &httpResponse);
		ZeroMemory(PerIoData->Buffer, BUFSIZMIN);
		PerIoData->DataBuf.buf = (char*)httpResponse.GetResponse2(&PerIoData->DataBuf.len);
		PerIoData->BytesRECV = 0;

		// Create per I/O socket information structure to associate with the WSARecv call below
		//if ((PerIoDataSend = (LPPER_IO_OPERATION_DATA)GlobalAlloc(GPTR, sizeof(PER_IO_OPERATION_DATA))) == NULL)
		//{
		//	printf("GlobalAlloc() failed with error %d\n", GetLastError());
		//	return 1;
		//}
		//else
		//	printf("GlobalAlloc() for LPPER_IO_OPERATION_DATA is OK!\n");

		//ZeroMemory(&(PerIoDataSend->Overlapped), sizeof(OVERLAPPED));
		//PerIoDataSend->BytesSEND = 0;
		//PerIoDataSend->BytesRECV = 0;
		//PerIoDataSend->DataBuf.buf = (char*)httpResponse.GetResponse2(&PerIoDataSend->DataBuf.len);

		//int res = WSASend(PerHandleData->Socket, &(PerIoDataSend->DataBuf), 1, &PerIoDataSend->BytesSEND, 0, &(PerIoDataSend->Overlapped), NULL);

		int res = ::send(PerHandleData->Socket, PerIoData->DataBuf.buf, PerIoData->DataBuf.len, NULL);

		if (res != SOCKET_ERROR)
		{
			SendBytes = PerIoData->BytesSEND;
			sprintf(msg, "%d::WSASEND: Socket=%d; SendBytes=%d; PerIoDataSend->BytesRECV=%d; PerIoDataSend->BytesSEND=%d; PerIoDataSend->DataBuf.len=%d\n",
				dwThreadId, PerHandleData->Socket, SendBytes, PerIoData->BytesRECV, PerIoData->BytesSEND, PerIoData->DataBuf.len);
			eventLog->WriteEventLogEntry2(msg, EVENTLOG_SUCCESS);
		}
		else
		{
			sprintf(msg, "%d::ServerWorkerThread--WSASend() failed with error %d\n", dwThreadId, WSAGetLastError());
			eventLog->WriteEventLogEntry2(msg, EVENTLOG_ERROR_TYPE);
			//return 0;
		}
		free(PerIoData->DataBuf.buf);

		//::ReleaseMutex(instance->ghMutex);
		//::ReleaseMutex(PerIoData->Overlapped.hEvent);

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


		if (false)
		{
			// Check to see if the BytesRECV field equals zero. If this is so, then
			// this means a WSARecv call just completed so update the BytesRECV field
			// with the BytesTransferred value from the completed WSARecv() call
			if (PerIoData->BytesRECV == 0)
			{
				PerIoData->BytesRECV = BytesTransferred;
				PerIoData->BytesSEND = 0;
			}
			else
			{
				PerIoData->BytesSEND += BytesTransferred;
			}

			if (PerIoData->BytesRECV > PerIoData->BytesSEND)
			{
				// Post another WSASend() request.
				// Since WSASend() is not guaranteed to send all of the bytes requested,
				// continue posting WSASend() calls until all received bytes are sent.
				ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
				PerIoData->DataBuf.buf = PerIoData->Buffer + PerIoData->BytesSEND;
				PerIoData->DataBuf.len = PerIoData->BytesRECV - PerIoData->BytesSEND;

				if (WSASend(PerHandleData->Socket, &(PerIoData->DataBuf), 1, &SendBytes, 0,
					&(PerIoData->Overlapped), NULL) == SOCKET_ERROR)
				{
					if (WSAGetLastError() != ERROR_IO_PENDING)
					{
						printf("WSASend() failed with error %d\n", WSAGetLastError());
						return 0;
					}
				}
				else
					printf("WSASend() is OK!\n");
			}
			else
			{
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
	}
}


DWORD WINAPI SocketCompletionPortServer::ServerWorkerThread2(LPVOID lpObject)
{
	SocketCompletionPortServer *obj = (SocketCompletionPortServer*)lpObject;
	HANDLE CompletionPort = (HANDLE)obj->GetCompletionPort();
	DWORD BytesTransferred;
	SocketIocpController::LPPER_HANDLE_DATA PerHandleData;
	SocketIocpController::LPPER_IO_OPERATION_DATA PerIoData, PerIoDataSend;
	DWORD SendBytes, RecvBytes;
	DWORD Flags;
	EventLog* eventLog;
	HttpRequest httpRequest;
	HttpResponse httpResponse;
	char msg[8192];
	httpResponse.SetCacheController(&(obj->cacheController));

	eventLog = new EventLog(L"IOCP Worker Thread");

	DWORD dwThreadId = GetCurrentThreadId();
	
	try
	{

		while (TRUE)
		{
			BytesTransferred = 0;
			BOOL res1 = GetQueuedCompletionStatus(CompletionPort, &BytesTransferred, (PULONG_PTR)&PerHandleData, (LPOVERLAPPED *)&PerIoData, INFINITE);
			SOCKET ts = PerHandleData->Socket;
			if (res1 == NULL)
			{
				sprintf(msg, "%d::ServerWorkerThread--GetQueuedCompletionStatus() returned null error %d\n", dwThreadId, GetLastError());
				eventLog->WriteEventLogEntry2(msg, EVENTLOG_ERROR_TYPE);
				obj->socketIocpController.FreeBySocket(ts);
				continue;
			}
			if (res1 == 0)
			{
				sprintf(msg, "%d::ServerWorkerThread--GetQueuedCompletionStatus() failed with error %d\n", dwThreadId, GetLastError());
				eventLog->WriteEventLogEntry2(msg, EVENTLOG_ERROR_TYPE);
				obj->socketIocpController.FreeBySocket(ts);
				continue;
				return 0;
			}
			else
				fprintf(stderr, "%d::ServerWorkerThread--GetQueuedCompletionStatus() is OK!\n", dwThreadId);

			//if (::WaitForSingleObject(PerIoData->Overlapped.hEvent, INFINITE) != WAIT_OBJECT_0)
			if (::WaitForSingleObject(PerIoData->Overlapped.hEvent, 10000) != WAIT_OBJECT_0)
			{
				sprintf(msg, "%d::WaitForSingleObject 1 WAIT_OBJECT_0 Mismatch\n", dwThreadId);
				eventLog->WriteEventLogEntry2(msg, EVENTLOG_ERROR_TYPE);
				obj->socketIocpController.FreeBySocket(ts);
				continue;
			}

			sprintf(msg, "%d::WSARECV2 Socket=%d, BytesTransferred=%d; PerIoData->BytesRECV=%d; PerIoData->BytesSEND=%d; PerIoData->DataBuf.len=%d; PerIoData.state=%d\n", dwThreadId, PerHandleData->Socket, BytesTransferred, PerIoData->BytesRECV, PerIoData->BytesSEND, PerIoData->DataBuf.len, PerIoData->state);
			eventLog->WriteEventLogEntry2(msg, EVENTLOG_ERROR_TYPE);
			//printf("%d::BytesTransferred = %d\n", GetCurrentThreadId(), BytesTransferred);
			//printf("%d::        BytesRECV = %d\n", GetCurrentThreadId(), PerIoData->BytesRECV);
			//printf("%d::        BytesSEND = %d\n", GetCurrentThreadId(), PerIoData->BytesSEND);
			//printf("%d::      DataBuf.len = %d\n", GetCurrentThreadId(), PerIoData->DataBuf.len);
			//printf("%d::      DataBuf.buf = %s\n", GetCurrentThreadId(), PerIoData->DataBuf.buf);
			//printf("%d::  PerIoData.state=%d\n", GetCurrentThreadId(), PerIoData->state);


			bool bCond1 = (BytesTransferred > 0 && PerIoData->BytesRECV == 0 && PerIoData->BytesSEND == 0 && PerIoData->DataBuf.len > 0);
			bool bCond2 = (PerIoData->BytesRECV > 0);
			//if (bCond1 || bCond2)
			if (PerIoData->state == 0)
			{
				SocketIocpController::LPSOCKET_IO_DATA lpiodata = obj->socketIocpController.Allocate();
				lpiodata->operationData.state = 1;
				assert(lpiodata != NULL);
				if (::WaitForSingleObject(obj->ghMutex, 15000) != WAIT_OBJECT_0)
				{
					sprintf(msg, "%d::WaitForSingleObject 2 WAIT_OBJECT_0 Mismatch\n", dwThreadId);
					eventLog->WriteEventLogEntry2(msg, EVENTLOG_ERROR_TYPE);
					obj->socketIocpController.FreeBySocket(ts);
					::ReleaseMutex(obj->ghMutex);
					::ReleaseMutex(PerIoData->Overlapped.hEvent);
					continue;
				}
				PerIoDataSend = &lpiodata->operationData;
				lpiodata->handleData.Socket = PerHandleData->Socket;

				assert(PerIoDataSend != NULL);
				httpRequest.Parse(PerIoData->DataBuf.buf);

				obj->Dispatch(&httpRequest, &httpResponse);
				ZeroMemory(PerIoDataSend->Buffer, BUFSIZMIN);
				//ZeroMemory(&(PerIoDataSend->Overlapped), sizeof(OVERLAPPED));
				PerIoDataSend->DataBuf.buf = (char*)httpResponse.GetResponse2(&PerIoDataSend->DataBuf.len);
				PerIoDataSend->BytesRECV = 0;
				PerIoDataSend->mallocFlag = 1;
				int res = WSASend(PerHandleData->Socket, &(PerIoDataSend->DataBuf), 1, &PerIoDataSend->BytesSEND, 0, &(PerIoDataSend->Overlapped), NULL);

				if (res == 0)
				{
					SendBytes = PerIoDataSend->BytesSEND;
					sprintf(msg, "%d::WSASEND: Socket=%d; SendBytes=%d; PerIoDataSend->BytesRECV=%d; PerIoDataSend->BytesSEND=%d; PerIoDataSend->DataBuf.len=%d; PerIoData.state=%d\n",
						dwThreadId, PerHandleData->Socket, SendBytes, PerIoDataSend->BytesRECV, PerIoDataSend->BytesSEND, PerIoDataSend->DataBuf.len, PerIoDataSend->state);
					eventLog->WriteEventLogEntry2(msg, EVENTLOG_SUCCESS);
					//if (PerIoData->Overlapped.Internal > 0)
					//{
					//	printf("Testing this area of logic A1 \n");
					//	//obj->socketIocpController.FreeByIndex(PerIoData->sequence);
					//}
					//if (PerIoData->Overlapped.InternalHigh == 0)
					//{
					//	printf("Testing this area of logic A2 \n");
					//	//obj->socketIocpController.FreeByIndex(PerIoData->sequence);
					//}

					//DWORD dwWaitResult = WaitForSingleObject(PerIoDataSend->Overlapped.hEvent, INFINITE);
					//switch (dwWaitResult)
					//{
					//case WAIT_OBJECT_0:
					//	printf("Thread %d :: Done performing the IO\n", GetCurrentThreadId());
					//	obj->socketIocpController.FreeByIndex(PerIoData->sequence);
					//	break;
					//default:
					//	printf("Wait error (%d)\n", GetLastError());
					//}
				}
				else
				{
					sprintf(msg, "%d::ServerWorkerThread--WSASend() failed with error %d\n", dwThreadId, WSAGetLastError());
					eventLog->WriteEventLogEntry2(msg, EVENTLOG_ERROR_TYPE);
					//return 0;
				}

				DWORD transferred;
				DWORD Flags;
				bool bres = WSAGetOverlappedResult(PerHandleData->Socket, &(PerIoDataSend->Overlapped), &transferred, FALSE, &Flags);
				if (bres)
				{

				}

				::ReleaseMutex(obj->ghMutex);
				::ReleaseMutex(PerIoData->Overlapped.hEvent);

				continue;
			}

			//bool bCond3 = BytesTransferred > 0 && PerIoData->BytesRECV == 0 && PerIoData->DataBuf.len > 0;
			//bool bCond4 = BytesTransferred == 0 && PerIoData->BytesRECV == 0 && PerIoData->DataBuf.len > 0;

			//if (bCond3 || bCond4)
			//{
			if (PerIoData->state == 1)
			{
				sprintf(msg, "%d::ServerWorkerThread--Closing socket %d\n", dwThreadId, PerHandleData->Socket);
				eventLog->WriteEventLogEntry2(msg, EVENTLOG_ERROR_TYPE);
				obj->socketIocpController.FreeBySocket(PerHandleData->Socket);
				//if (closesocket(PerHandleData->Socket) == SOCKET_ERROR)
				//{
				//	fprintf(stderr, "%d::ServerWorkerThread--closesocket() failed with error %d\n", dwThreadId, WSAGetLastError());
				//	//return 0;
				//}
				//else
				//{
				//	fprintf(stderr, "%d::ServerWorkerThread--closesocket() is fine!\n", dwThreadId);
				//	obj->socketIocpController.FreeBySocket(ts);
				//}
				continue;
			}
			//}

			printf("\n\nZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ\n\nUnhandled condition. IP should not reach line. \n\n\n\n");
			//exit(1);
		}
	}
	catch (...)
	{
		eventLog->WriteEventLogEntry2("Exception in ServerWorkerThread", EVENTLOG_ERROR_TYPE);
		printf("\n\QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ\n\nException SocketCompletionPortServer::ServerWorkerThread. \n\n\n\n");
		return 1;
	}
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
	EventLog eventLog;
	char msg[8192];

	sprintf(msg, "URL: %s\n", httpRequest->GetUrl());
	eventLog.WriteEventLogEntry2(msg, EVENTLOG_ERROR_TYPE);

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

	if (HasUrlParams(httpRequest->GetUrl()))
	{
		printf("Has URL Params\n");
		LPSTATICFUNC lpFunc = (LPSTATICFUNC)GetUrlParamHandler(httpRequest->GetUrl());
		httpRequest->urlParams.assign(urlParams.begin(), urlParams.end());
		if (lpFunc != NULL)
		{
			(*lpFunc)(httpRequest, httpResponse);
		}
		else
		{
			UrlNotFound(httpRequest, httpResponse);
		}
		return;
	}


	if (httpRequest->GetUrl() != NULL)
	{
		LPSTATICFUNC lpFunc = (LPSTATICFUNC)GetRoute(httpRequest->GetUrl());
		if (lpFunc != NULL)
		{
			(*lpFunc)(httpRequest, httpResponse);
		}
		else
		{
			WCHAR  buffer[BUFSIZMIN];
			char *url = httpRequest->GetUrl();
			std::string surl;
			surl.append("./");
			surl.append(url);
			std::wstring wsurl(surl.begin(), surl.end());
			DWORD dwres = GetFullPathName(wsurl.c_str(), BUFSIZMIN, buffer, NULL);
			if (dwres > 0)
			{
				PTSTR str = GetPathExtension(buffer);
				std::wstring wsbuffer(buffer);
				std::string sbuffer(wsbuffer.begin(), wsbuffer.end());
				if (FileExist(wsbuffer.c_str()))
				{
					httpResponse->SetStaticFileName(sbuffer.c_str());
					httpResponse->WriteStatic(sbuffer.c_str());
					return;
				}
				else
				{
					printf("File not found: %s \n", sbuffer.c_str());
				}
			}
			UrlNotFound(httpRequest, httpResponse);
		}
	}
}

bool SocketCompletionPortServer::FileExist(const TCHAR *fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

void SocketCompletionPortServer::EvalStatic(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	EventLog eventLog;
	char msg[8192];
	try
	{
		char *str = httpRequest->GetUrl();
		string s = GetFullPath(str);

		ifstream file(s);
		if (file)
		{
			sprintf(msg, "Static %s\n", s.c_str());
			eventLog.WriteEventLogEntry2(msg, EVENTLOG_ERROR_TYPE);
			httpResponse->SetStaticFileName(s);
			httpResponse->WriteStatic(s.c_str());
		}
		else
		{
			sprintf(msg, "Static %s not found\n", s.c_str());
			eventLog.WriteEventLogEntry2(msg, EVENTLOG_ERROR_TYPE);
		}

	}
	catch (...)
	{
		printf("Exception in SocketCompletionPortServer::EvalStatic \n");
		exit(0);
	}
}

void SocketCompletionPortServer::EvalHasUrlParams(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
}


void SocketCompletionPortServer::UrlNotFound(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	fprintf(stderr, "SocketCompletionPortServer::UrlNotFound: \n");
}

