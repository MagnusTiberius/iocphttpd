#include "stdafx.h"
#include "CppUnitTest.h"

#include "yield/platform/yunit.h"

#include "yield/platform/memory_mapped_file.h"

#include <memory>
using std::memcpy;
#include <cstring>
using std::strcmp;

using namespace yield;

#define TEST_FILE_NAME "MemoryMappedFile_test.dat"
#define TESTSTRING "Test string"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace database_test
{
	TEST_CLASS(MemoryMappedFileTest)
	{
	public:
		
		TEST_METHOD(TestMemoryMappedFile)
		{
			{
				MemoryMappedFile mmf(TEST_FILE_NAME, strlen(TESTSTRING) + 1, O_CREAT | O_RDWR | O_SYNC);
				memcpy(mmf.getRegionStart(), TESTSTRING, strlen(TESTSTRING) + 1);
				mmf.writeBack();
			}

			MemoryMappedFile mmf(TEST_FILE_NAME, 0);
			ASSERT_EQUAL(mmf.getRegionSize(), strlen(TESTSTRING) + 1);
			ASSERT_EQUAL(strcmp(mmf.getRegionStart(), TESTSTRING), 0);


			try 
			{ 
				DiskOperations::unlink(TEST_FILE_NAME); 
			}
			catch (...) 
			{
			
			}
		}

	};
}