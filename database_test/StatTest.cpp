#include "stdafx.h"
#include "CppUnitTest.h"


#include "yield/platform/yunit.h"

#include "yield/platform/stat.h"
#include "yield/platform/disk_operations.h"

using namespace yield;

#define TEST_FILE_NAME "Stat_test.txt"
#define TEST_DIR_NAME "Stat_test"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace database_test
{
	TEST_CLASS(StatTest)
	{
	public:
		
		TEST_METHOD(StatTest_1)
		{
			try
			{
				DiskOperations::touch(TEST_FILE_NAME);
				Stat file_stat(TEST_FILE_NAME);
				DiskOperations::unlink(TEST_FILE_NAME);
				ASSERT_EQUAL(file_stat.getType(), Stat::File);
				ASSERT_EQUAL(file_stat.getSize(), 0);
				ASSERT_TRUE(file_stat.getCreationTime() != 0 && file_stat.getLastAccessTime() != 0 && file_stat.getLastWriteTime() != 0);
				ASSERT_FALSE(file_stat.isHidden());

				try { DiskOperations::mkdir(TEST_DIR_NAME); }
				catch (...) {}
				Stat dir_stat(TEST_DIR_NAME);
				ASSERT_EQUAL(dir_stat.getType(), Stat::Directory);
				DiskOperations::rmdir(TEST_DIR_NAME);
			}
			catch (std::exception&)
			{
				try { DiskOperations::unlink(TEST_FILE_NAME); }
				catch (...) {}
				try { DiskOperations::rmdir(TEST_DIR_NAME); }
				catch (...) {}
				throw;
			}
		}

	};
}