#include "stdafx.h"
#include "CppUnitTest.h"
#include "ServerIOCP.h"
#include "ClientIOCP.h"
#include "ServerHttp.h"
#include "ClientHttp.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace netiocp_test
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			ServerIOCP server;
			server.StartThread();

			::Sleep(3000);

			ClientIOCP client;
			client.StartThread();

			client.Join();
			server.Abort();
			//server.Join();
		}


		TEST_METHOD(TestMethod2)
		{
			ServerHttp server;
			server.StartThread();

			::Sleep(5000);

			ClientHttp client;
			client.StartThread();

			client.Join();
			server.Abort();
			//server.Join();
		}

	};


}