#include "stdafx.h"
#include "CppUnitTest.h"

#include <utility>
using std::pair;

#include "babudb/profiles/string_key.h"
#include "yield/platform/memory_mapped_file.h"
#include "index/index_writer.h"
#include "index/index.h"
#include "index/merger.h"
#include "log_index.h"
using namespace yield;
using namespace babudb;

#include "babudb/test.h"
#include "Util.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace database_test
{


	TEST_CLASS(IndexWriterTest)
	{
	public:
		
		TEST_METHOD(IndexWriterTest1)
		{
			// create ImmutableIndex from LogIndex
			StringOrder sorder;
			const string strPath = Util::testPath("testdb-testidx");
			IndexMerger* merger = new IndexMerger(strPath, sorder);

			merger->Add(1, Buffer("key1"), Buffer("data1"));
			merger->Add(2, Buffer("key2"), Buffer("data2"));
			merger->Add(3, Buffer("key3"), Buffer("data3"));
			merger->Add(4, Buffer("key4"), Buffer("data4"));
			merger->Add(4, Buffer("key5"), Buffer::Empty());

			merger->Run();
			delete merger;

			// load it and check
			ImmutableIndex::DiskIndices indices = ImmutableIndex::FindIndices(
				Util::testPath("testdb-testidx"));
			ImmutableIndex* loadedindex = ImmutableIndex::LoadLatestIntactIndex(indices, sorder);

			ASSERT_EQUAL(loadedindex->GetLastLSN(), 4);
			ASSERT_TRUE(!loadedindex->Lookup(Buffer("key1")).isEmpty());
			ASSERT_TRUE(!loadedindex->Lookup(Buffer("key2")).isEmpty());
			ASSERT_TRUE(!loadedindex->Lookup(Buffer("key3")).isEmpty());
			ASSERT_TRUE(!loadedindex->Lookup(Buffer("key4")).isEmpty());
			ASSERT_TRUE(!loadedindex->Lookup(Buffer("key5")).isNotExists());
			ASSERT_TRUE(loadedindex->Lookup(Buffer("key5")).isEmpty());

			// create another LogIndex
			merger = new IndexMerger(Util::testPath("testdb-testidx"), sorder, loadedindex);

			merger->Add(5, Buffer("key12"), Buffer("data12"));
			merger->Add(6, Buffer("key22"), Buffer("data22"));
			merger->Add(7, Buffer("key32"), Buffer("data32"));
			merger->Add(8, Buffer("key42"), Buffer("data42"));

			merger->Run();
			delete merger;
			delete loadedindex;

			// load it and check
			indices = ImmutableIndex::FindIndices(Util::testPath("testdb-testidx"));
			loadedindex = ImmutableIndex::LoadLatestIntactIndex(indices, sorder);
			ASSERT_EQUAL(loadedindex->GetLastLSN(), 8);
			ASSERT_TRUE(!loadedindex->Lookup(Buffer("key1")).isEmpty());
			ASSERT_TRUE(!loadedindex->Lookup(Buffer("key2")).isEmpty());
			ASSERT_TRUE(!loadedindex->Lookup(Buffer("key3")).isEmpty());
			ASSERT_TRUE(loadedindex->Lookup(Buffer("key5")).isEmpty());

			ASSERT_TRUE(loadedindex->Lookup(Buffer("key123")).isNotExists());

			ASSERT_TRUE(!loadedindex->Lookup(Buffer("key12")).isEmpty());
			ASSERT_TRUE(!loadedindex->Lookup(Buffer("key22")).isEmpty());
			ASSERT_TRUE(!loadedindex->Lookup(Buffer("key32")).isEmpty());
		}

	};
}