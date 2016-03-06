#include "stdafx.h"
#include "CppUnitTest.h"

#include "DemoDB.h"
#include <stack>
#include <ctime>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define THREAD_COUNT 4
#define LIMITCTR 10000
namespace database_test
{
	// ==============================================================================================
	//
	//
	// ==============================================================================================
	typedef struct Request{
		int ConnectID;
		DWORD x;
		DWORD y;
	}REQUEST, *LP_REQUEST;

	// ==============================================================================================
	//
	//
	// ==============================================================================================
	class Server
	{
	public:
		Server();
		int Start(HANDLE* hList);
		void Stop();

		void Submit(REQUEST* request);
		int Connect();
		void UpdateDB(REQUEST* request);
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

	Server::Server()
	{
		nThreads = THREAD_COUNT;
		connectID = 1;
		ghMutex = CreateMutex(
			NULL,              // default security attributes
			FALSE,             // initially not owned
			NULL);
		dwCtr = 0;
	}

	int Server::Start(HANDLE* hList)
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

	void Server::Stop()
	{
		isLooping = false;
	}

	int Server::Connect()
	{
		connectID++;
		return connectID;
	}

	void Server::UpdateDB(REQUEST* request)
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

	DWORD WINAPI Server::ServerWorkerThread(LPVOID lpObject)
	{
		Server* instance = (Server*)lpObject;
		while (instance->isLooping)
		{
			Sleep(300);
			if (::WaitForSingleObject(instance->ghMutex, 10000) == WAIT_OBJECT_0)
			{
				instance->dwCtr++;
				if (instance->dwCtr >= LIMITCTR)
				{
					instance->isLooping = false;
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

	void Server::Submit(REQUEST* request)
	{
		requests.push(request);
	}

	// ==============================================================================================
	//
	//
	// ==============================================================================================
	class Client
	{
	public:
		Client();
		Client(Server* s);

		int Start(HANDLE*);

	private:
		Server* server;
		static DWORD WINAPI ClientWorkerThread(LPVOID CompletionPortID);
		int nThreads;
		HANDLE ThreadHandle;
		DWORD ThreadID;
		DWORD dwThreadId;
		bool isLooping;
		HANDLE ghMutexClient;
		DWORD dwCtr;
	};

	Client::Client()
	{
		nThreads = THREAD_COUNT;
		ghMutexClient = CreateMutex(
			NULL,              // default security attributes
			FALSE,             // initially not owned
			NULL);
		dwCtr = 0;
	}

	Client::Client(Server* s)
	{
		server = s;
		nThreads = THREAD_COUNT;
		ghMutexClient = CreateMutex(
			NULL,              // default security attributes
			FALSE,             // initially not owned
			NULL);
		dwCtr = 0;
	}

	int Client::Start(HANDLE* hList)
	{
		//HANDLE hList[THREAD_COUNT];
		isLooping = true;
		dwThreadId = GetCurrentThreadId();
		for (int i = 0; i < THREAD_COUNT; i++)
		{
			if ((hList[i] = CreateThread(NULL, 0, ClientWorkerThread, this, 0, &ThreadID)) == NULL)
			{
				fprintf(stderr, "%d::CreateThread() failed with error %d\n", dwThreadId, GetLastError());
				return 1;
			}
			else
				fprintf(stderr, "%d::CreateThread() is OK!\n", dwThreadId);

		}
		return 1;
	}

	DWORD WINAPI Client::ClientWorkerThread(LPVOID lpObject)
	{
		Client* instance = (Client*)lpObject;
		REQUEST* req = new REQUEST();
		srand(time(NULL));

		int connectID = instance->server->Connect();
		req->ConnectID = connectID;
		req->x = 10;
		req->y = 15;

		while (instance->isLooping)
		{
			Sleep(1000);
			if (::WaitForSingleObject(instance->ghMutexClient, 10000) == WAIT_OBJECT_0)
			{
				int v = (rand() % 2);
				if (v)
				{
					req->x++;
				}
				else
				{
					req->x--;
				}
				v = (rand() % 2);
				if (v)
				{
					req->y++;
				}
				else
				{
					req->y--;
				}

				instance->server->Submit(req);

				instance->dwCtr++;
				if (instance->dwCtr >= LIMITCTR)
				{
					instance->isLooping = false;
				}
				::ReleaseMutex(instance->ghMutexClient);
			}
		}
		return 1;
	}

	// ==============================================================================================
	//
	//
	// ==============================================================================================
	class Simulator
	{
	public:
		Simulator();

		void Start();

	private:
		Server server;
		Client* client;
	};

	Simulator::Simulator()
	{

	}

	void Simulator::Start()
	{
		HANDLE hList1[THREAD_COUNT];
		HANDLE hList2[THREAD_COUNT];
		DWORD dwEvent;
		server.Start(hList1);
		client = new Client(&server);
		client->Start(hList2);
		bool isLooping = true;

		while (isLooping)
		{
			dwEvent = WaitForMultipleObjects(THREAD_COUNT, hList1, FALSE, 5000);
			switch (dwEvent)
			{
				// ghEvents[0] was signaled
			case WAIT_OBJECT_0 + 0:
				// TODO: Perform tasks required by this event
				printf("First event was signaled.\n");
				isLooping = false;
				break;

				// ghEvents[1] was signaled
			case WAIT_OBJECT_0 + 1:
				// TODO: Perform tasks required by this event
				printf("Second event was signaled.\n");
				isLooping = false;
				break;

			case WAIT_TIMEOUT:
				printf("Wait timed out.\n");
				break;

				// Return value is invalid.
			default:
				printf("Wait error: %d\n", GetLastError());
				//ExitProcess(0);
			}
		}
	}

	// ==============================================================================================
	//
	//
	// ==============================================================================================
	TEST_CLASS(DemoDBTest)
	{
	public:
		
		TEST_METHOD(DemoDBTest1)
		{
			DatabaseDemo::DemoDB* db;
			std::vector<std::string> indices; 
			indices.push_back("connection");
			db = DatabaseDemo::DemoDB::Open("C:\\temp\\demodb", indices);
			char buf[1024];
			for (int i = 0; i < 100000; i++)
			{
				ZeroMemory(buf, 1024);
				sprintf(buf, "Key%d", i);
				db->Add("connection", buf, "{\"employees\":[{\"firstName\":\"John\", \"lastName\" : \"Doe\"},{ \"firstName\":\"Anna\", \"lastName\" : \"Smith\" },	{ \"firstName\":\"Peter\", \"lastName\" : \"Jones\" }]}");
			}
			db->Commit();
			std::string v1 = db->Lookup("connection", "key1");
			delete db;

			db = DatabaseDemo::DemoDB::Open("C:\\temp\\demodb", indices);
			std::string v2 = db->Lookup("connection", "key1");
			db->Compact("C:\\temp\\demodb_compact");
		}

		TEST_METHOD(DemoDBTest2)
		{
			database_test::Simulator s;
			s.Start();
		}

	};







}