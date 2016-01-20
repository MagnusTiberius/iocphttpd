#include "stdafx.h"
#include "ClientHttp.h"


ClientHttp::ClientHttp()
{
}


ClientHttp::~ClientHttp()
{
}

void ClientHttp::StartThread()
{
	if ((ThreadHandle = CreateThread(NULL, 0, WorkerThread, this, 0, &ThreadID)) == NULL)
	{

	}
}

DWORD WINAPI ClientHttp::WorkerThread(LPVOID lpObject)
{
	SocketClient client;
	char* reply;

	int res = client.Connect(8085);
	if (res == 0)
	{
		//client.Send("GET / HTTP/1.1\n", reply);
		client.Send("GET \/ HTTP\/1.1\n\n", reply);
	}
	return 1;
}