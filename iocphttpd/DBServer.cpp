#include "DBServer.h"



DBServer::~DBServer()
{
}


DBServer::DBServer()
{
	nThreads = THREAD_COUNT;
	connectID = 1;
	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);
	dwCtr = 0;
}

int DBServer::Start(HANDLE* hList)
{
	std::vector<std::string> indices;
	indices.push_back("connection");
	db = DatabaseDemo::DemoDB::Open("C:\\temp\\demodb", indices);

	//HANDLE hList[THREAD_COUNT];
	isLooping = true;
	dwThreadId = GetCurrentThreadId();
	for (int i = 0; i < THREAD_COUNT; i++)
	{
		if ((hList[i] = CreateThread(NULL, 0, ServerWorkerThread, this, 0, &ThreadID)) == NULL)
		{
			fprintf(stderr, "%d::CreateThread() failed with error %d\n", dwThreadId, GetLastError());
			return 1;
		}
		else
			fprintf(stderr, "%d::CreateThread() is OK!\n", dwThreadId);

		//CloseHandle(ThreadHandle);
	}
	return 1;
}

int DBServer::GetThreadNum()
{
	return THREAD_COUNT;
}

void DBServer::Stop()
{
	isLooping = false;
}

int DBServer::Connect()
{
	connectID++;
	return connectID;
}

void DBServer::UpdateDB(REQUEST* request)
{
	char buf[1024];
	ZeroMemory(buf, 1024);
	sprintf(buf, "Connect_%d", request->ConnectID);

	char bufv[1024];
	ZeroMemory(bufv, 1024);
	sprintf(bufv, "x=%d, y=%d", request->x, request->y);

	std::string v1 = db->Lookup("connection", buf);
	if (v1.size() == 0)
	{
		db->Add("connection", buf, bufv);
	}
	else
	{
		//db->Remove("connection", buf);
		db->Add("connection", buf, bufv);
	}
	db->Commit();
}

DWORD WINAPI DBServer::ServerWorkerThread(LPVOID lpObject)
{
	DBServer* instance = (DBServer*)lpObject;
	while (instance->isLooping)
	{
		Sleep(300);
		if (::WaitForSingleObject(instance->ghMutex, 10000) == WAIT_OBJECT_0)
		{
			instance->dwCtr++;
			if (instance->dwCtr >= LIMITCTR)
			{
				//instance->isLooping = false;
			}
			if (instance->requests.size() > 0)
			{
				REQUEST* req = instance->requests.top();
				if (req != NULL)
				{
					instance->requests.pop();
					instance->UpdateDB(req);
					::ReleaseMutex(instance->ghMutex);
				}
			}
		}
	}
	return 1;
}

void DBServer::Submit(REQUEST* request)
{
	requests.push(request);
}