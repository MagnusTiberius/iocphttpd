#include "stdafx.h"
#include "GameEngine.h"


GameEngine::GameEngine()
{
	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);
	ghMutex2 = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);
	ghMutex3 = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);
	ghMutex4 = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);

	ghHasMessageEvent = CreateEvent(NULL, TRUE, FALSE, TEXT("GameEngine"));
	ghHasMessageEvent2 = CreateEvent(NULL, TRUE, FALSE, TEXT("GameEngine2"));

	nThreads = THREAD_COUNT;
	ctr = 0;
	ctr2 = 0;
}


GameEngine::~GameEngine()
{
}



void GameEngine::Start()
{
	for (int i = 0; i < nThreads; i++)
	{
		if ((ThreadHandle = CreateThread(NULL, 0, WorkerThread, this, 0, &ThreadID)) == NULL)
		{
			printf("CreateThread() failed with error %d\n", GetLastError());
			return;
		}
		else
		{
			printf("CreateThread() is OK!\n");
			ghEvents[i] = ThreadHandle;
		}
		//if ((ThreadHandle = CreateThread(NULL, 0, WorkerThread2, this, 0, &ThreadID)) == NULL)
		//{
		//	printf("CreateThread() failed with error %d\n", GetLastError());
		//	return;
		//}
		//else
		//{
		//	printf("CreateThread() is OK!\n");
		//	ghEvents2[i] = ThreadHandle;
		//}
	}
}

void GameEngine::Join()
{
	::WaitForMultipleObjects(THREAD_COUNT, ghEvents, TRUE, INFINITE);
}

void GameEngine::Stop()
{
	DWORD dwCode = 0;

	for (int i = 0; i < THREAD_COUNT; i++)
	{
		::TerminateThread(ghEvents[i], dwCode);
	}
	int total = ctr;
}

void GameEngine::SendJobMessage(Structs::LP_JOBREQUEST job)
{
	URLOBJECT* urlObject;
	LP_PLAYER player = NULL;
	stack<Structs::LP_JOBREQUEST>* levelMap;
	char* url = job->header.url;
	if (levelMapList.find(url) != levelMapList.end())
	{
		urlObject = levelMapList[url];
		levelMap = urlObject->levelMap;
	}
	else
	{
		urlObject = new URLOBJECT();
		urlObject->isBusy = false;
		urlObject->levelMap = new stack<Structs::LP_JOBREQUEST>();
		urlObject->memberList = new vector<SOCKET>();
		urlObject->playerList = new map<string, LP_PLAYER>();
		levelMap = urlObject->levelMap;
		levelMapList[url] = urlObject;
	}

	::WaitForSingleObject(ghMutex3, INFINITE);
	levelMap->push(job);
	urlObject->levelMap = levelMap;
	::ReleaseMutex(ghMutex3);

	string name(job->header.name);
	map<string, LP_PLAYER> &playerList = *urlObject->playerList;
	if (playerList.find(name) != playerList.end())
	{
		player = playerList[name];
		if (player->properties == NULL)
		{
			player->properties = new map<string, char*>();
		}
		map<string, char*>::iterator it;
		map<string, char*>* prp = job->header.properties;
		if (job->header.properties != NULL)
		{
			for (it = prp->begin(); it != prp->end(); it++)
			{
				string name = it->first;
				char* value = it->second;
				map<string, char*> &dest = *player->properties;
				dest[name] = _strdup(value);
			}
		}
	}
	else
	{
		player = new PLAYER();
		player->isValid = true;
		player->_socket = job->socket;
		player->name = _strdup(job->header.name);
		player->properties = new map<string, char*>();
		map<string, char*>::iterator it;
		map<string, char*>* prp = job->header.properties;
		if (job->header.properties != NULL)
		{
			for (it = prp->begin(); it != prp->end(); it++)
			{
				string name = it->first;
				char* value = it->second;
				map<string, char*> &dest = *player->properties;
				dest[name] = _strdup(value);
			}
		}
		playerList[name] = player;
		//urlObject->playerList = &playerList;
	}

	bool exists = false;
	for (auto &item : *urlObject->memberList)
	{
		if (item == job->socket)
		{
			exists = true;
			break;
		}
	}
	if (!exists)
	{
		::WaitForSingleObject(ghMutex3, INFINITE);
		urlObject->memberList->push_back(job->socket);
		levelMapList[url] = urlObject;
		::ReleaseMutex(ghMutex3);
	}
	::WaitForSingleObject(ghMutex2, INFINITE);
	ctr2++;
	::ReleaseMutex(ghMutex2);
	::SetEvent(ghHasMessageEvent2);
}

void GameEngine::AddMessage(Structs::LP_JOBREQUEST job)
{
	::WaitForSingleObject(ghMutex4, INFINITE);
	jobList.push(job);
	::ReleaseMutex(ghMutex4);
	::SetEvent(ghHasMessageEvent);
}

DWORD WINAPI GameEngine::WorkerThread2(LPVOID obj)
{
	GameEngine* instance = (GameEngine*)obj;
	URLOBJECT* urlObject = NULL;

	while (true)
	{
		::WaitForSingleObject(instance->ghHasMessageEvent2, INFINITE);
		::WaitForSingleObject(instance->ghMutex2, INFINITE);
		map<string, URLOBJECT*>::iterator itr;
		for (itr = instance->levelMapList.begin(); itr != instance->levelMapList.end(); itr++)
		{
			urlObject = itr->second;
			if (!urlObject->isBusy)
			{
				urlObject->isBusy = true;
				break;
			}
		}
		stack<Structs::LP_JOBREQUEST>* stk = urlObject->levelMap;
		map<string, LP_PLAYER> &playerList = *urlObject->playerList;

		if (stk->size() > 0)
		{
			::WaitForSingleObject(instance->ghMutex3, INFINITE);
			Structs::LP_JOBREQUEST item = stk->top();
			stk->pop();
			::ReleaseMutex(instance->ghMutex3);
			instance->ctr2--;
			char* msg = _strdup(item->data);
			string sout;
			map<string, LP_PLAYER>::iterator it;
			for (it = playerList.begin(); it != playerList.end(); it++)
			{
				PLAYER &player = *it->second;

				if (player._socket == NULL)
				{
					continue;
				}

				vector<SOCKET>::iterator itSocket = find(instance->disconnectedSockets.begin(), instance->disconnectedSockets.end(), player._socket);
				if (itSocket != instance->disconnectedSockets.end())
				{
					player.isValid = false;
					continue;
				}

				map<string, char*> &prop = *player.properties;
				map<string, char*>::iterator it2;
				
				sout.append("name:");
				sout.append(player.name);
				sout.append(";");
				for (it2 = prop.begin(); it2 != prop.end(); it2++)
				{
					string name = it2->first;
					string value = it2->second;
					sout.append(name);
					sout.append("=");
					sout.append(value);
					sout.append("; ");
				}
				sout.append("\n");
			}

			char hdr[1024];
			ZeroMemory(hdr, 1024);
			sprintf(hdr, "GAME/1.0 200 OK\ncontent-length:%d\ncontent-type:text\nname:%s\n\n%s\n\n", sout.length(), item->header.name, sout.c_str());

			for (it = playerList.begin(); it != playerList.end(); it++)
			{
				PLAYER &player = *it->second;

				if (player._socket == NULL)
				{
					continue;
				}

				if (player.isValid == false)
				{
					closesocket(player._socket);
					player._socket = NULL;
					continue;
				}
#ifdef PRODUCTION
				int bRes = send(player._socket, hdr, strlen(hdr), 0);
				//printf("Loop counter:===> %d; data=%s; len=%d; sent=%d\n", loopCtr1, nextJob->data, nextJob->len, bRes);
				if (bRes == SOCKET_ERROR)
				{
					printf("SOCKET_ERROR in GameEngine::WorkerThread2 \n");
					instance->disconnectedSockets.push_back(player._socket);
				}
#endif
			}
		}
		urlObject->isBusy = false;
		if (instance->ctr2 <= 0)
		{
			::ResetEvent(instance->ghHasMessageEvent2);
		}
		::ReleaseMutex(instance->ghMutex2);
	}

}

DWORD WINAPI GameEngine::WorkerThread(LPVOID obj)
{
	GameEngine* instance = (GameEngine*)obj;

	while (true)
	{
		::WaitForSingleObject(instance->ghHasMessageEvent, INFINITE);
		::WaitForSingleObject(instance->ghMutex, INFINITE);
		if (instance->jobList.size() > 0)
		{
			::WaitForSingleObject(instance->ghMutex4, INFINITE);
			Structs::LP_JOBREQUEST job = instance->jobList.top();
			instance->jobList.pop();
			::ReleaseMutex(instance->ghMutex4);
			if (job != NULL)
			{
				instance->SendJobMessage(job);
			}
			instance->ctr++;
			::ReleaseMutex(instance->ghMutex);
		}

		if (instance->jobList.size() == 0)
		{
			::ResetEvent(instance->ghHasMessageEvent);
		}
		::ReleaseMutex(instance->ghMutex);
	}

}