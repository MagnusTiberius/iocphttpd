#include "WebSocketManager.h"


WebSocketManager::WebSocketManager()
{
}


WebSocketManager::~WebSocketManager()
{
}


HANDLE WebSocketManager::Start()
{
	if ((ThreadHandle = CreateThread(NULL, 0, ServerWorkerThread, this, 0, &ThreadID)) == NULL)
	{

	}
	return ThreadHandle;
}

DWORD WINAPI WebSocketManager::ServerWorkerThread(LPVOID lpObject)
{
	WebSocketManager* instance = (WebSocketManager*)lpObject;
	instance->websocket = new WebSocket::SocketCompletionPortServerWS();
	instance->websocket->Start();
	return 0;
}