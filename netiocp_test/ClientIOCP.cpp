#include "stdafx.h"
#include "ClientIOCP.h"


ClientIOCP::ClientIOCP()
{
}


ClientIOCP::~ClientIOCP()
{
}


void ClientIOCP::StartThread()
{
	if ((ThreadHandle = CreateThread(NULL, 0, WorkerThread, this, 0, &ThreadID)) == NULL)
	{

	}
}

DWORD WINAPI ClientIOCP::WorkerThread(LPVOID lpObject)
{
	SocketClient client;
	char* reply;

	int res = client.Connect(8085);
	client.Send("This is a test\n", reply);
	return 1;
}