#pragma once
#include "stdafx.h"
#include "SocketCompletionPortServer.h"
#include "IProtocolHandler.h"

class ServerHttp
{
public:
	ServerHttp();
	~ServerHttp();

public:
	void StartThread();
	void Abort()
	{
		::TerminateThread(ThreadHandle, dwExitCode);
	}

	void Join()
	{
		WaitForSingleObject(ThreadHandle, INFINITE);
	}

	void AddProtocol(IProtocolHandler* p);

protected:
	IProtocolHandler* protocol;

private:
	static DWORD WINAPI WorkerThread(LPVOID lpObject);

private:
	HANDLE ThreadHandle;
	DWORD ThreadID;
	DWORD dwExitCode;
};

