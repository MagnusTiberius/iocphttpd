#include "stdafx.h"
#include "CppUnitTest.h"
#include "ServerIOCP.h"
#include "ClientIOCP.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace netiocp_test
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// TODO: Your test code here
			ServerIOCP server;
			server.StartThread();

			::Sleep(3000);

			ClientIOCP client;
			client.StartThread();

			client.Join();
			server.Join();
		}

	};


}