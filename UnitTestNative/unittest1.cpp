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
			printf("%s/n", c1);
			scanner.SkipEmpty();
			CHAR *c2 = scanner.AcceptRun("\/ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_");
			printf("%s/n", c2);
			scanner.SkipEmpty();
			CHAR *c3 = scanner.AcceptRun("\/ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_.0123456789");
			printf("%s/n", c3);
		}


	};
}