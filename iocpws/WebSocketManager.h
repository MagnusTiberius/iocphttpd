#pragma once
#include "stdafx.h"
#include "SocketCompletionPortServerWS.h"

class WebSocketManager 
{
public:
	WebSocketManager();
	~WebSocketManager();


	HANDLE Start();

private:
	WebSocket::SocketCompletionPortServerWS* websocket;
	static DWORD WINAPI WebSocketManager::ServerWorkerThread(LPVOID lpObject);
	HANDLE ThreadHandle;
	DWORD ThreadID;
};

