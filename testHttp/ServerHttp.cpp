#include "stdafx.h"
#include "ServerHttp.h"


ServerHttp::ServerHttp()
{
}


ServerHttp::~ServerHttp()
{
}


void ServerHttp::AddProtocol(IProtocolHandler* p)
{
	protocol = p;
}

void ServerHttp::StartThread()
{
	if ((ThreadHandle = CreateThread(NULL, 0, WorkerThread, this, 0, &ThreadID)) == NULL)
	{

	}
}

DWORD WINAPI ServerHttp::WorkerThread(LPVOID lpObject)
{
	SocketCompletionPortServer server;
	ServerHttp* ptr = (ServerHttp*)lpObject;
	server.AddHandler(ptr->protocol);
	server.Start(8085);
	return 1;
}