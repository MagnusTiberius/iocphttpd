#include "stdafx.h"
#include "CppUnitTest.h"

#include "yield/platform/yunit.h"
#include "yield/platform/platform_exception.h"
#include "yield/platform/file.h"
using namespace yield;


#define TEST_FILE_NAME "File_test.txt"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace database_test
{		
	TEST_CLASS(FileTest)
	{
	public:
		
		TEST_METHOD(TestFile1)
		{
			try
			{
				{
					File f(TEST_FILE_NAME, O_CREAT | O_TRUNC | O_WRONLY | O_CLOSE_ON_DESTRUCT);
				}

				File* f = File::open(TEST_FILE_NAME);
				ASSERT_NE(f, NULL);
				delete f;

				{
					fd_t fd = DiskOperations::open(TEST_FILE_NAME);
					File f(fd);
				}
			}
			catch (PlatformException&)
			{
				try { DiskOperations::unlink(TEST_FILE_NAME); }
				catch (...) {}
				throw;
			}
		}



		TEST_METHOD(TestFile2)
		{
			try
			{
				{
					File f(TEST_FILE_NAME, O_CREAT | O_TRUNC | O_WRONLY | O_CLOSE_ON_DESTRUCT);
					f.write("hello");
				}

				char hello[6]; hello[5] = 0;
				{
					File f((const char*)TEST_FILE_NAME);
					ssize_t read_ret = f.read(hello, 5);
					ASSERT_EQUAL(read_ret, 5);
					ASSERT_TRUE(strcmp(hello, "hello") == 0);
				}
			}
			catch (PlatformException&)
			{
				try { DiskOperations::unlink(TEST_FILE_NAME); }
				catch (...) {}
				throw;
			}
		}

	};
}