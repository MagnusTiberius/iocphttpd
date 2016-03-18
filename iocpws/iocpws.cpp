// iocpws.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WebSocketManager.h"

int _tmain(int argc, _TCHAR* argv[])
{
	printf("In main...\n");
	WebSocketManager wsmanager;
	HANDLE hThread = wsmanager.Start();
	::WaitForSingleObject(hThread, INFINITE);

	printf("In main...exiting\n");
	return 0;
}

