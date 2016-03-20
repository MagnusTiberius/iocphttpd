#include "GameEngine.h"


GameEngine::GameEngine()
{
}


GameEngine::~GameEngine()
{
}

HANDLE GameEngine::Start()
{
	ghHasMessageEvent = CreateEvent(NULL, TRUE, FALSE, TEXT("GameEngine1234"));
	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);

	inGame = true;
	if ((ThreadHandle = CreateThread(NULL, 0, ServerWorkerThread, this, 0, &ThreadID)) == NULL)
	{

	}
	return ThreadHandle;
}

DWORD WINAPI GameEngine::ServerWorkerThread(LPVOID lpObject)
{
	GameEngine* instance = (GameEngine*)lpObject;
	while (instance->inGame)
	{
		::WaitForSingleObject(instance->ghHasMessageEvent, INFINITE);
		::WaitForSingleObject(instance->ghMutex, INFINITE);

		string req = instance->requestList.top();
		instance->requestList.pop();

		if (instance->requestList.size() == 0)
		{
			::ResetEvent(instance->ghHasMessageEvent);
		}
		::ReleaseMutex(instance->ghMutex);
	}
	return 0;
}