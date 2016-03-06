#include "stdafx.h"
#include "CppUnitTest.h"

#include "DemoDB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace database_test
{
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

	};
}