#pragma once
#include "stdafx.h"
#include "windows.h"
#include <stack>
#include <ctime>
#include "DemoDB.h"

#define THREAD_COUNT 3
#define LIMITCTR 10000
#define DATADBNAME "datadb"

using namespace DatabaseDemo;

typedef struct Request{
	string key;
	string value;
}REQUEST, *LP_REQUEST;

class DBServer
{
public:
	static DBServer* Instance();

protected:
	DBServer();
	~DBServer();

public:
	int Start(HANDLE* hList);
	void Stop();

	void Submit(REQUEST* request);
	int Connect();
	void UpdateDB(REQUEST* request);
	int GetThreadNum();
	void InitData();
	string GetValue(string key);

private:
	static DBServer* _instance;
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
	HANDLE ghHasMessageEvent;
};

