#pragma once
#include "stdafx.h"
#include "SocketClient.h"

class ClientIOCP
{
public:
	ClientIOCP();
	~ClientIOCP();

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

