#include "stdafx.h"
#include "CppUnitTest.h"

#include "babudb/database.h"
#include <vector>
#include <utility>

#include "babudb/profiles/string_key.h"
#include "babudb/lookup_iterator.h"
#include "babudb/test.h"
#include "merged_index.h"

#include "yield/platform/memory_mapped_file.h"
using yield::MemoryMappedFile;
using namespace babudb;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace database_test
{
	TEST_CLASS(MergedIndexTest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			StringOrder sorder;
			MergedIndex index("test", sorder);
			index.Add(Buffer::wrap("key1"), Buffer::wrap("val1"));
			index.Add(Buffer::wrap("key2"), Buffer::Empty());
			{
				Buffer value = index.Lookup(Buffer::wrap("key1"));
				EXPECT_EQUAL(value, Buffer::wrap("val1"));
				value = index.Lookup(Buffer::wrap("key2"));
				EXPECT_TRUE(value.isEmpty());
			}

			// Snapshot, then overwrite the value
			index.Snapshot(2);
			index.Add(Buffer::wrap("key1"), Buffer::wrap("val2"));
			{
				Buffer value = index.Lookup(Buffer::wrap("key1"));
				EXPECT_EQUAL(value, Buffer::wrap("val2"));
				value = index.Lookup(Buffer::wrap("key2"));
				EXPECT_TRUE(value.isEmpty());
			}

			// Check the snapshot
			LookupIterator snapshot = index.GetSnapshot(2);
			EXPECT_EQUAL(Buffer::wrap("val1"), (*snapshot).second);
		}

	};
}