#include "stdafx.h"
#include "CppUnitTest.h"
#include "Crypt.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define WEBSOCKETKEY0 "s3pPLMBiTxaQ9kYGzzhZRbK+xOo="
#define WEBSOCKETKEY1 "dGhlIHNhbXBsZSBub25jZQ=="
#define WEBSOCKETKEY2 "+1bfVHYyBdlCWc4j0J27og=="
#define WEBSOCKETKEY3 "Ip9TmzXinx3Rf2ws/zzt2A=="


namespace securechanneltest
{
	TEST_CLASS(CryptTest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// TODO: Your test code here
			//Crypt c;
			//c.Hash("s3pPLMBiTxaQ9kYGzzhZRbK+xOo=");
			string v1 = Crypt::HashIt(WEBSOCKETKEY3);
			string v2 = Crypt::urlDecode(v1);
		}

	};
}