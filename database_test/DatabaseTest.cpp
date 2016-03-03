#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "babudb/database.h"
#include <vector>
#include <utility>

#include "babudb/profiles/string_key.h"
#include "babudb/test.h"
#include "index/merger.h"
#include "Util.h"
#include "yield/platform/memory_mapped_file.h"
using yield::MemoryMappedFile;
using namespace babudb;
using namespace std;

namespace database_test
{
	TEST_CLASS(DatabaseTest)
	{
	public:
		Database* db;

		TEST_METHOD(Database)
		{
			StringOrder myorder;
			std::vector<babudb::IndexDescriptor> indices;
			indices.push_back(std::make_pair("testidx", &myorder));
			db = Database::Open(Util::testPath("test").getHostCharsetPath(), indices);

			StringSetOperation(1, "testidx", "Key1", "data1").ApplyTo(*db, 1);
			StringSetOperation(2, "testidx", "Key2", "data2").ApplyTo(*db, 2);

			EXPECT_FALSE(db->Lookup("testidx", babudb::Buffer::wrap("Key1")).isEmpty());

			StringSetOperation(3, "testidx", "Key1").ApplyTo(*db, 3);

			EXPECT_TRUE(db->Lookup("testidx", babudb::Buffer::wrap("Key1")).isNotExists());

			delete db;
		}


		TEST_METHOD(DatabaseMigration)
		{
			StringOrder myorder;

			IndexMerger* merger = new IndexMerger(Util::testPath("test-testidx").getHostCharsetPath(), myorder);
			merger->Add(1, Buffer("Key1"), Buffer("data1"));
			merger->Add(2, Buffer("Key2"), Buffer("data2"));
			merger->Run();
			delete merger;

			std::vector<babudb::IndexDescriptor> indices;
			indices.push_back(std::make_pair("testidx", &myorder));
			db = Database::Open(Util::testPath("test").getHostCharsetPath(), indices);

			EXPECT_EQUAL(db->GetMinimalPersistentLSN(), 2);
			EXPECT_FALSE(db->Lookup("testidx", babudb::Buffer::wrap("Key1")).isEmpty());
			EXPECT_FALSE(db->Lookup("testidx", babudb::Buffer::wrap("Key2")).isEmpty());
			EXPECT_TRUE(db->Lookup("testidx", babudb::Buffer::wrap("Key3")).isNotExists());

			StringSetOperation(3, "testidx", "Key3").ApplyTo(*db, 3);
			EXPECT_FALSE(db->Lookup("testidx", babudb::Buffer::wrap("Key1")).isEmpty());
			EXPECT_FALSE(db->Lookup("testidx", babudb::Buffer::wrap("Key2")).isEmpty());
			EXPECT_TRUE(db->Lookup("testidx", babudb::Buffer::wrap("Key3")).isNotExists());
			EXPECT_TRUE(db->Lookup("testidx", babudb::Buffer::wrap("Key4")).isNotExists());
			delete db;

		}

		TEST_METHOD(Database_SnapshotAndImport)
		{
			StringOrder myorder;
			std::vector<babudb::IndexDescriptor> indices;
			indices.push_back(std::make_pair("testidx", &myorder));
			indices.push_back(std::make_pair("testidx2", &myorder));

			db = Database::Open(Util::testPath("test").getHostCharsetPath(), indices);

			EXPECT_EQUAL(0, db->GetMinimalPersistentLSN());
			EXPECT_EQUAL(0, db->GetCurrentLSN());

			StringSetOperation(1, "testidx", "Key1", "data1").ApplyTo(*db, 1);
			StringSetOperation(2, "testidx", "Key2", "data2").ApplyTo(*db, 2);
			StringSetOperation(3, "testidx2", "Key1", "data1").ApplyTo(*db, 3);
			EXPECT_EQUAL(0, db->GetMinimalPersistentLSN());
			EXPECT_EQUAL(3, db->GetCurrentLSN());

			vector<pair<string, babudb::lsn_t> > index_versions = db->GetIndexVersions();
			for (vector<pair<string, babudb::lsn_t> >::iterator i = index_versions.begin(); i != index_versions.end(); ++i) {
				db->Snapshot(i->first);
				db->CompactIndex(i->first, db->GetCurrentLSN());
			}

			// The database has not loaded the new index base yet,
			// we merely created index files with a newer version
			index_versions = db->GetIndexVersions();
			for (vector<pair<string, babudb::lsn_t> >::iterator i = index_versions.begin();
				i != index_versions.end(); ++i) {
				EXPECT_EQUAL(0, i->second);
			}

			// Now reopen the index files we have just compacted
			db->ReopenIndices();
			index_versions = db->GetIndexVersions();
			for (vector<pair<string, babudb::lsn_t> >::iterator i = index_versions.begin();
				i != index_versions.end(); ++i) {
				EXPECT_EQUAL(db->GetCurrentLSN(), i->second);
			}

			delete db;

			db = Database::Open(Util::testPath("test").getHostCharsetPath(), indices);
			EXPECT_EQUAL(3, db->GetMinimalPersistentLSN());
			EXPECT_EQUAL(3, db->GetCurrentLSN());
			const int current_lsn = db->GetCurrentLSN();

			index_versions = db->GetIndexVersions();
			for (vector<pair<string, babudb::lsn_t> >::iterator i = index_versions.begin(); i != index_versions.end(); ++i) {
				EXPECT_EQUAL(3, i->second);
			}
			delete db;

			// And now import the index snapshots into a new database
			EXPECT_TRUE(Database::ImportIndex(
				Util::testPath("copy").getHostCharsetPath(), "testidx", current_lsn,
				Util::testPath("test-testidx_3.idx"), false));
			EXPECT_TRUE(Database::ImportIndex(
				Util::testPath("copy").getHostCharsetPath(), "testidx2", current_lsn,
				Util::testPath("test-testidx2_3.idx"), false));

			db = Database::Open(Util::testPath("copy").getHostCharsetPath(), indices);
			EXPECT_EQUAL(3, db->GetMinimalPersistentLSN());
			EXPECT_EQUAL(3, db->GetCurrentLSN());
			delete db;

		}

	};
}