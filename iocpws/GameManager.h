#pragma once
#include "stdafx.h"
#include "GameEngine.h"

class GameManager
{
public:
	GameManager();
	~GameManager();

	HANDLE Start();

private:
	static DWORD WINAPI ServerWorkerThread(LPVOID lpObject);
	HANDLE ThreadHandle;
	DWORD ThreadID;
	GameEngine gameEngine;
};

