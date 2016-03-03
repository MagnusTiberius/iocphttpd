#include "stdafx.h"
#include "CppUnitTest.h"

#include "yield/platform/yunit.h"
#include "yield/platform/platform_exception.h"
#include "yield/platform/disk_operations.h"
using namespace yield;


#define TEST_FILE_NAME "DiskOperations_test.txt"
#define TEST_DIR_NAME "DiskOperations_test"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace database_test
{
	TEST_CLASS(DiskOperationTest)
	{
	public:
		
		TEST_METHOD(DiskOperationTest_1)
		{
			try
			{
				DiskOperations::touch(TEST_FILE_NAME);
				ASSERT_TRUE(DiskOperations::exists(TEST_FILE_NAME));
				ASSERT_TRUE(DiskOperations::exists(TEST_FILE_NAME));
				fd_t fd = DiskOperations::open(TEST_FILE_NAME, O_RDONLY | O_THROW_EXCEPTIONS);
				ASSERT_TRUE(DiskOperations::exists(TEST_FILE_NAME));
				DiskOperations::close(fd);
				DiskOperations::unlink(TEST_FILE_NAME);
				ASSERT_FALSE(DiskOperations::exists(TEST_FILE_NAME));

				DiskOperations::mkdir(TEST_DIR_NAME);
				ASSERT_TRUE(DiskOperations::exists(TEST_DIR_NAME));
				DiskOperations::rmdir(TEST_DIR_NAME);
				ASSERT_FALSE(DiskOperations::exists(TEST_DIR_NAME));

				Path tree_path(Path(TEST_DIR_NAME) + DISK_PATH_SEPARATOR_STRING + Path(TEST_DIR_NAME) + DISK_PATH_SEPARATOR_STRING + Path(TEST_DIR_NAME));
				DiskOperations::mktree(tree_path);
				ASSERT_TRUE(DiskOperations::exists(tree_path));
				DiskOperations::touch(tree_path + TEST_FILE_NAME);
				DiskOperations::rmtree(TEST_DIR_NAME);
				ASSERT_FALSE(DiskOperations::exists(TEST_DIR_NAME));
			}
			catch (PlatformException&)
			{
				try { DiskOperations::unlink(TEST_FILE_NAME); }
				catch (...) {}
				try { DiskOperations::rmtree(TEST_DIR_NAME); }
				catch (...) {}
				throw;
			}
		}

	};
}