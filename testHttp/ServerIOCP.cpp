#include "stdafx.h"
#include "ServerIOCP.h"


ServerIOCP::ServerIOCP()
{
	protocol = new ChatProtocol();
}


ServerIOCP::~ServerIOCP()
{
}

void ServerIOCP::AddProtocol(IProtocolHandler* p)
{
	protocol = p;
}

void ServerIOCP::StartThread()
{
	if ((ThreadHandle = CreateThread(NULL, 0, WorkerThread, this, 0, &ThreadID)) == NULL)
	{

	}
}

DWORD WINAPI ServerIOCP::WorkerThread(LPVOID lpObject)
{
	SocketCompletionPortServer server;
	ServerIOCP* ptr = (ServerIOCP*)lpObject;
	server.AddHandler(ptr->protocol);
	server.Start(8085);
	return 1;
}