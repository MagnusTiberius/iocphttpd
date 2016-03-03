#include "stdafx.h"
#include "CppUnitTest.h"
#include "babudb/profiles/string_key.h"
#include "babudb/test.h"
#include "babudb/test_helper.h"

#include "babudb/log/log_iterator.h"
#include "babudb/log/log.h"
#include "Util.h"
#include "yield/platform/memory_mapped_file.h"
using yield::MemoryMappedFile;
using namespace babudb;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace database_test
{
	TEST_CLASS(LogIteratorTest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			{
				Log log(Util::testPath("testlog"));

				std::auto_ptr<Log::iterator> i(log.First());  // try an empty log first
				EXPECT_FALSE(i->IsValid());
				EXPECT_FALSE((bool)i->GetNext());
				EXPECT_FALSE((bool)i->GetNext());
				EXPECT_FALSE(i->IsValid());
				EXPECT_FALSE((bool)i->GetPrevious());
				EXPECT_FALSE((bool)i->GetPrevious());
				EXPECT_FALSE(i->IsValid());
				i.reset(log.Last());
				EXPECT_FALSE(i->IsValid());
				EXPECT_FALSE((bool)i->GetNext());
				EXPECT_FALSE((bool)i->GetNext());
				EXPECT_FALSE(i->IsValid());
				EXPECT_FALSE((bool)i->GetPrevious());
				EXPECT_FALSE((bool)i->GetPrevious());
				EXPECT_FALSE(i->IsValid());
			}
		}

	};
}