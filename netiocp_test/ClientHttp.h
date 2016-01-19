#pragma once
#include "stdafx.h"
#include "SocketClient.h"

class ClientHttp
{
public:
	ClientHttp();
	~ClientHttp();

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

private:
	static DWORD WINAPI WorkerThread(LPVOID lpObject);


private:
	HANDLE ThreadHandle;
	DWORD ThreadID;
	DWORD dwExitCode;
};

