#include "stdafx.h"
#include "CppUnitTest.h"
#include "babudb/log/sequential_file.h"
#include "babudb/log/log_storage.h"

#include "babudb/test.h"
#include "yield/platform/disk_operations.h"

#define POSIX  // for O_ definitions from fcntl.h
#include <fcntl.h>

using namespace yield;
using namespace babudb;
#include "Util.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace database_test
{
	TEST_CLASS(SequentialFileTest)
	{
	public:
		
		TEST_METHOD(SequentialFileTest1)
		{
			LogStorage* file = PersistentLogStorage::Open(Util::testPath("testfile").getHostCharsetPath());
			SequentialFile sf(file);

			EXPECT_TRUE(sf.First().GetNext() == NULL);
			EXPECT_TRUE(sf.Last().GetPrevious() == NULL);

			sf.append(1);
			sf.append(2);
			sf.append(3);
			sf.commit();

			sf.close();

			file = PersistentLogStorage::OpenReadOnly(Util::testPath("testfile").getHostCharsetPath());
			SequentialFile sf2(file);

			SequentialFile::iterator i = sf2.First();
			EXPECT_FALSE(i.IsValid());
			EXPECT_TRUE(i.GetNext() != NULL);
			EXPECT_TRUE(i.GetRecord()->getPayloadSize() == 1);

			EXPECT_TRUE(i.GetNext() != NULL);
			EXPECT_TRUE(i.GetRecord()->getPayloadSize() == 2);

			EXPECT_TRUE(i.GetPrevious() != NULL);
			EXPECT_TRUE(i.GetRecord()->getPayloadSize() == 1);

			EXPECT_TRUE(i.GetNext() != NULL);
			EXPECT_TRUE(i.GetNext() != NULL);
			EXPECT_TRUE(i.GetRecord()->getPayloadSize() == 3);

			EXPECT_FALSE(i.GetNext() != NULL);

			i = sf2.Last();
			EXPECT_FALSE(i.IsValid());
			EXPECT_TRUE(i.GetPrevious() != NULL);
			EXPECT_TRUE(i.GetRecord()->getPayloadSize() == 3);
			EXPECT_TRUE(i.GetPrevious() != NULL);
			EXPECT_TRUE(i.GetRecord()->getPayloadSize() == 2);
			EXPECT_TRUE(i.GetPrevious() != NULL);
			EXPECT_TRUE(i.GetRecord()->getPayloadSize() == 1);

			EXPECT_FALSE(i.GetPrevious() != NULL);
		}

	};
}