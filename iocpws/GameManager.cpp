#include "GameManager.h"


GameManager::GameManager()
{
}


GameManager::~GameManager()
{
}

HANDLE GameManager::Start()
{
	if ((ThreadHandle = CreateThread(NULL, 0, ServerWorkerThread, this, 0, &ThreadID)) == NULL)
	{

	}
	return ThreadHandle;
}

DWORD WINAPI GameManager::ServerWorkerThread(LPVOID lpObject)
{
	GameManager* instance = (GameManager*)lpObject;
	instance->gameEngine.Start();
	return 0;
}