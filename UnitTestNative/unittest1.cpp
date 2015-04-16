#include "stdafx.h"
#include "CppUnitTest.h"
#include "ScannerA.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestNative
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			ScannerA scanner;
			scanner.Input("GET /test HTTP/1.1\r\n\r\n");
			bool b1 = scanner.Accept("G");
			scanner.Next();
			bool b2 = scanner.Accept("E");
			scanner.Next();
			bool b3 = scanner.Accept("T");
		}

		TEST_METHOD(TestMethod2)
		{
			ScannerA scanner;
			scanner.Input("GET /test HTTP/1.1\r\n\r\n");
			CHAR *c1 = scanner.AcceptRun("GET");
			CHAR *d1 = _strdup(c1);
			printf("%s/n", d1);
			scanner.SkipEmpty();
			CHAR *c2 = scanner.AcceptRun("\/ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_");
			CHAR *d2 = _strdup(c2);
			printf("%s/n", d2);
			scanner.SkipEmpty();
			CHAR *c3 = scanner.AcceptRun("\/ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_.0123456789");
			CHAR *d3 = _strdup(c3);
			printf("%s/n", d3);
			free(d1);
			free(d2);
			free(d3);
		}


	};
}