#pragma once
#include "stdafx.h"
#include "windows.h"
#include <stack>
#include <ctime>
#include "DemoDB.h"

#define THREAD_COUNT 3
#define LIMITCTR 10000

using namespace DatabaseDemo;

typedef struct Request{
	int ConnectID;
	DWORD x;
	DWORD y;
}REQUEST, *LP_REQUEST;

class DBServer
{
public:
	DBServer();
	~DBServer();

public:
	int Start(HANDLE* hList);
	void Stop();

	void Submit(REQUEST* request);
	int Connect();
	void UpdateDB(REQUEST* request);
	int GetThreadNum();
private:
	static DWORD WINAPI ServerWorkerThread(LPVOID CompletionPortID);
	DWORD dwCtr;
	int nThreads;
	HANDLE ThreadHandle;
	DWORD ThreadID;
	DWORD dwThreadId;
	bool isLooping;
	int connectID;
	stack<REQUEST*> requests;
	HANDLE ghMutex;
	DatabaseDemo::DemoDB* db;
};

