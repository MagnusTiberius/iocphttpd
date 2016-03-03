#include "stdafx.h"
#include "CppUnitTest.h"

#include "yield/platform/yunit.h"
#include "yield/platform/directory_walker.h"
#include "yield/platform/disk_operations.h"
using namespace yield;

using namespace std;

#define TEST_DIR_PATH "DirectoryWalker_test"
#ifdef _WIN32
#define TEST_FILE_PATH "DirectoryWalker_test\\file.txt"
#define TEST_SUBDIR_PATH "DirectoryWalker_test\\subdir\\"
#else
#define TEST_FILE_PATH "DirectoryWalker_test/file.txt"
#define TEST_SUBDIR_PATH "DirectoryWalker_test/subdir/"
#endif

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace database_test
{
	TEST_CLASS(DirectoryWalkerTest)
	{
	public:
		
		TEST_METHOD(DirectoryWalkerTest1)
		{
			//step 1
			try { DiskOperations::rmdir(TEST_SUBDIR_PATH); }
			catch (...) {}
			try { DiskOperations::unlink(TEST_FILE_PATH); }
			catch (...) {}
			try { DiskOperations::rmdir(TEST_DIR_PATH); }
			catch (...) {}


			//step 2
			DiskOperations::mkdir(TEST_DIR_PATH);
			DiskOperations::touch(TEST_FILE_PATH);
			DiskOperations::mkdir(TEST_SUBDIR_PATH);


			//step 3
			DirectoryWalker walker(TEST_DIR_PATH);

			bool seen_TEST_FILE_PATH = false, seen_TEST_SUBDIR_PATH = false;
			unsigned int seen_files_count = 0;
			while (walker.hasNext())
			{
				auto_ptr<DirectoryEntry> next_directory_entry = walker.getNext();
				const string& next_path = next_directory_entry.get()->getPath().getHostCharsetPath();
				if (next_path.compare(TEST_FILE_PATH) == 0) seen_TEST_FILE_PATH = true;
				else if (next_path.compare(TEST_SUBDIR_PATH) == 0) seen_TEST_SUBDIR_PATH = true;
				seen_files_count++;
			}

			if (seen_files_count != 2)
			{
				cerr << "expected 2 files, got " << seen_files_count;
				FAIL();
			}
			ASSERT_TRUE(seen_TEST_FILE_PATH);
			ASSERT_TRUE(seen_TEST_SUBDIR_PATH);
		}

	};
}