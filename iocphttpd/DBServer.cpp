#include "DBServer.h"


DBServer* DBServer::_instance = 0;

DBServer* DBServer::Instance() {
	if (_instance == 0) {
		_instance = new DBServer();
	}
	return _instance;
}

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
	ghHasMessageEvent = CreateEvent(NULL, TRUE, FALSE, TEXT("DBServer"));
}

int DBServer::Start(HANDLE* hList)
{
	std::vector<std::string> indices;
	indices.push_back(DATADBNAME);
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

void DBServer::InitData()
{
	string profID[5] = {"nissangtr","lancerevo","fordmustang","audia4titanium","subarubrz"};

	for (int i = 0; i < 5; i++)
	{
		string key = "/ws/metadata/profile/" + profID[i] + "/profile.html";
		key = "/ws/profile/" + profID[i] + "/profile.html";
		string value = "{\"profileImages\":                                                                                   \
						[{\"img\" :\"\\\\static\\\\profile\\\\" + profID[i] + "\\\\img\\\\0001.jpg\", \"itm\" :\"active\" },     \
							{\"img\" :\"\\\\static\\\\profile\\\\" + profID[i] + "\\\\img\\\\0002.jpg\", \"itm\" :\"\" },        \
							{\"img\" :\"\\\\static\\\\profile\\\\" + profID[i] + "\\\\img\\\\0003.jpg\", \"itm\" :\"\" },        \
							{\"img\" :\"\\\\static\\\\profile\\\\" + profID[i] + "\\\\img\\\\0004.jpg\", \"itm\" :\"\" },        \
							{\"img\" :\"\\\\static\\\\profile\\\\" + profID[i] + "\\\\img\\\\0005.jpg\", \"itm\" :\"\" },        \
							{\"img\" :\"\\\\static\\\\profile\\\\" + profID[i] + "\\\\img\\\\0006.jpg\", \"itm\" :\"\" },        \
							{\"img\" :\"\\\\static\\\\profile\\\\" + profID[i] + "\\\\img\\\\0007.jpg\", \"itm\" :\"\" },        \
							{\"img\" :\"\\\\static\\\\profile\\\\" + profID[i] + "\\\\img\\\\0008.jpg\", \"itm\" :\"\" },        \
							{\"img\" :\"\\\\static\\\\profile\\\\" + profID[i] + "\\\\img\\\\0009.jpg\", \"itm\" :\"\" },        \
							{\"img\" :\"\\\\static\\\\profile\\\\" + profID[i] + "\\\\img\\\\0010.jpg\", \"itm\" :\"\" }],       \
						\"profileID\": \"" + profID[i] + "\",                                                                    \
						\"dir\": \"\\\\static\\\\profile\\\\" + profID[i] + "\\\\img\\\\\"                                       \
						}";
		db->Add(DATADBNAME, key, value);
		string v = GetValue(key);
		// ---------------------------------------

		key = "/ws/metadata/profile/" + profID[i] + "/profile.html";
		value = "{                                            \
			\"Versions\":[                                                  \
		{                                                                   \
			\"Version\":{                                                   \
				\"MSRP\":\"101,770\",                                       \
					\"Code\" : \"Premium\",                                 \
					\"HP\" : \"545\",                                       \
					\"mpgCity\" : \"16\",                                   \
					\"mpgHiway\" : \"23\",                                  \
					\"seats\" : \"4\",                                      \
					\"doors\" : \"2\",                                      \
					\"Features\" : [                                        \
						\"3.8-liter twin-turbo V6 engine\",                 \
							\"Dual clutch 6-speed transmission\",           \
							\"ATTESA E-TS® All-Wheel Drive\",               \
							\"20\\\" RAYS® wheels [*]\",                    \
							\"Nissan/Brembo® braking system [*]\"           \
					]                                                       \
			}                                                               \
		},                                                                  \
		{                                                                   \
			\"Version\":{                                                   \
				\"MSRP\":\"102,770\",                                       \
					\"Code\" : \"Gold\",                                    \
					\"HP\" : \"545\",                                       \
					\"mpgCity\" : \"16\",                                   \
					\"mpgHiway\" : \"23\",                                  \
					\"seats\" : \"4\",                                      \
					\"doors\" : \"2\"                                       \
			},                                                              \
			\"Features\":[                                                  \
				\"3.8-liter twin-turbo V6 engine\",                         \
					\"Dual clutch 6-speed transmission\",                   \
					\"ATTESA E-TS® All-Wheel Drive\",                       \
					\"20\\\" RAYS® wheels [*]\",                            \
					\"Nissan/Brembo® braking system [*]\"                   \
			]                                                               \
		},                                                                  \
		{                                                                   \
			\"Version\":{                                                   \
				\"MSRP\":\"110,510\",                                       \
					\"Code\" : \"Black\",                                   \
					\"HP\" : \"545\",                                       \
					\"mpgCity\" : \"16\",                                   \
					\"mpgHiway\" : \"23\",                                  \
					\"seats\" : \"4\",                                      \
					\"doors\" : \"2\"                                       \
			},                                                              \
			\"Features\":[                                                  \
				\"Includes Premium features plus:\",                        \
					\"Dry carbon-fiber rear spoiler [*]\",                  \
					\"20\\\" Special dark-finished RAYS® wheels [*]\",      \
					\"Black/Red Recaro® front seats [*]\"                   \
			]                                                               \
		}                                                                   \
			]                                                               \
		}";

		db->Add(DATADBNAME, key, value);
	}

}

void DBServer::UpdateDB(REQUEST* request)
{
	std::string v1 = db->Lookup(DATADBNAME, request->key);
	if (v1.size() == 0)
	{
		db->Add(DATADBNAME, request->key, request->value);
	}
	else
	{
		//db->Remove(DATADBNAME, buf);
		db->Add(DATADBNAME, request->key, request->value);
	}
	db->Commit();
}

DWORD WINAPI DBServer::ServerWorkerThread(LPVOID lpObject)
{
	DBServer* instance = (DBServer*)lpObject;
	while (instance->isLooping)
	{
		Sleep(300);
		::WaitForSingleObject(instance->ghHasMessageEvent, INFINITE);
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
				}
			}

			if (instance->requests.size() == 0)
			{
				::ResetEvent(instance->ghHasMessageEvent);
			}
			::ReleaseMutex(instance->ghMutex);
		}

	}
	return 1;
}

void DBServer::Submit(REQUEST* request)
{
	requests.push(request);
	SetEvent(ghHasMessageEvent);
}

string DBServer::GetValue(string key)
{
	string value = db->Lookup(DATADBNAME, key);
	return value;
}
