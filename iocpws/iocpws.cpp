// iocpws.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WebSocketManager.h"
#include "GameManager.h"

int _tmain(int argc, _TCHAR* argv[])
{
	printf("In main...\n");
	WebSocketManager wsmanager;
	HANDLE hThread1 = wsmanager.Start();

	GameManager gameManager;
	HANDLE hThread2 = gameManager.Start();

	::WaitForSingleObject(hThread1, INFINITE);
	::WaitForSingleObject(hThread2, INFINITE);

	printf("In main...exiting\n");
	return 0;
}

