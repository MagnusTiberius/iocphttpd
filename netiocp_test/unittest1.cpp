#include "stdafx.h"
#include "CppUnitTest.h"
#include "ServerIOCP.h"
#include "ClientIOCP.h"
#include "ServerHttp.h"
#include "ClientHttp.h"
#include "ScannerA.h"

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

			::Sleep(2000);

			ClientHttp client;
			client.StartThread();

			client.Join();
			server.Abort();
			//server.Join();
		}


		TEST_METHOD(TestMethod3)
		{
			HttpRequest httpRequest;
			httpRequest.IsValid("GET /pub/WWW/ HTTP/1.1\n\n");
			httpRequest.Parse("GET /pub/WWW/ HTTP/1.1\n\n");
			MethodType m = httpRequest.GetMethod();
		}

		TEST_METHOD(TestMethod4)
		{
			const char* inp = "GET /pub/WWW/ HTTP/1.1\n\n";
			ScannerA s;
			s.Input(inp);
			int token = RequestParser::token_t::BEGIN;
			bool isDone = false;
			while (isDone == false)
			{
				char* c1 = s.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
				if (c1 != NULL)
				{
					if (strcmp(c1, "GET") == 0 || strcmp(c1, "HTTP") == 0)
					{
						int x = 1;
						token = RequestParser::token_t::GET;
						continue;
					}
				}
				c1 = s.AcceptRun("\/");
				if (c1 != NULL)
				{
					s.Backup();
					c1 = s.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz\/-0123456789._");
					if (c1 != NULL)
					{
						int x = 1;
						token = RequestParser::token_t::URL;
						continue;
					}
				}
				c1 = s.AcceptRun("\n");
				if (c1 != NULL)
				{
					c1 = s.AcceptRun("\n");
					if (c1 != NULL)
					{
						isDone = true;
					}
				}
				s.Next();
				if (s.IsEOS())
				{
					isDone = true;
				}
			}

		}

		TEST_METHOD(TestMethod5)
		{
			const char* inp = "GET /pub/WWW/ HTTP/1.1\n\n";
			ScannerA s;
			s.Input(inp);
			int token = RequestParser::token_t::BEGIN;
			bool isDone = false;
			while (isDone == false)
			{
				char* c1 = s.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
				if (c1 != NULL)
				{
					if (strcmp(c1, "HTTP") == 0)
					{
						int x = 1;
						token = RequestParser::token_t::HTTPVERSION;
						isDone = true;
						continue;
					}
				}
				c1 = s.AcceptRun("\/");
				if (c1 != NULL)
				{
					s.Backup();
					c1 = s.AcceptRun("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz\/-0123456789._");
					if (c1 != NULL)
					{
						int x = 1;
						token = RequestParser::token_t::URL;
						continue;
					}
				}
				c1 = s.AcceptRun("\n");
				if (c1 != NULL)
				{
					c1 = s.AcceptRun("\n");
					if (c1 != NULL)
					{
						isDone = true;
					}
				}
				s.Next();
				if (s.IsEOS())
				{
					isDone = true;
				}
			}

			if (token == RequestParser::token_t::HTTPVERSION)
			{
				int k = 1;
			}

		}
	};


}