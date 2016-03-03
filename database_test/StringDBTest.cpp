#include "stdafx.h"
#include "CppUnitTest.h"

#include "babudb/profiles/string_db.h"
#include <vector>
#include <utility>

#include "babudb/profiles/string_key.h"
#include "babudb/test.h"
#include "index/merger.h"

#include "yield/platform/memory_mapped_file.h"
using yield::MemoryMappedFile;
using namespace babudb;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "Util.h"

namespace database_test
{
	TEST_CLASS(StringDBTest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			std::vector<std::string> indices; indices.push_back("index");

			StringDB* db = StringDB::Open(Util::testPath("test"), indices);
			db->Add("index", "key1", "value1");
			db->Commit();
			ASSERT_TRUE(db->Lookup("index", "key1") == "value1");
			ASSERT_TRUE(db->Lookup("index", "key2") == "");
			delete db;

			db = StringDB::Open(Util::testPath("test"), indices);
			ASSERT_TRUE(db->Lookup("index", "key1") == "value1");
			ASSERT_TRUE(db->Lookup("index", "key2") == "");
			db->Compact(Util::testPath("alt_"));
			db->Add("index", "key2", "value2");
			db->Commit();
			ASSERT_TRUE(db->Lookup("index", "key1") == "value1");
			ASSERT_TRUE(db->Lookup("index", "key2") == "value2");
			delete db;

			db = StringDB::Open(Util::testPath("test"), indices);
			ASSERT_TRUE(db->Lookup("index", "key1") == "value1");
			ASSERT_TRUE(db->Lookup("index", "key2") == "value2");
			db->Compact(Util::testPath("alt_"));
			delete db;
		}

	};
}