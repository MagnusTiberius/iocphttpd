#include "SocketClientA.h"


SocketClientA::SocketClientA()
{
}


SocketClientA::~SocketClientA()
{
}


HANDLE SocketClientA::GetCompletionPort()
{
	return CompletionPort;
}

int SocketClientA::Start()
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

	nThreads = 2;

	//nThreads = (nThreads / 2);

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

	return 0;
}

DWORD WINAPI SocketClientA::ServerWorkerThread(LPVOID lpObject)
{
	SocketClientA *obj = (SocketClientA*)lpObject;
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
		if (res1 == 0)
		{
			fprintf(stderr, "%d::ServerWorkerThread--GetQueuedCompletionStatus() failed with error %d\n", dwThreadId, GetLastError());
			return 0;
		}
		else
			fprintf(stderr, "%d::ServerWorkerThread--GetQueuedCompletionStatus() is OK!\n", dwThreadId);

		printf("\n\n%d::WSARECV2 Socket=%d, BytesTransferred=%d; PerIoData->BytesRECV=%d; PerIoData->BytesSEND=%d\n\n", dwThreadId, PerHandleData->Socket, BytesTransferred, PerIoData->BytesRECV, PerIoData->BytesSEND);

	}
}