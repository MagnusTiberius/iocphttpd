#pragma once
#include "stdafx.h"
#include <stack>
#include <vector>
#include <algorithm>

class GameEngine
{
public:
	GameEngine();
	~GameEngine();

	HANDLE Start();

private:
	static DWORD WINAPI ServerWorkerThread(LPVOID lpObject);
	HANDLE ThreadHandle;
	DWORD ThreadID;
	bool inGame;

	HANDLE ghHasMessageEvent;
	HANDLE ghMutex;

	stack<string> requestList;
};

