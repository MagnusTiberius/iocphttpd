#include "stdafx.h"
#include "CppUnitTest.h"

#include "yield/platform/yunit.h"
#include "yield/platform/path.h"
using namespace yield;


#define TEST_FILE_NAME_ASCII "somefile.txt"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace database_test
{
	TEST_CLASS(PathTest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Path path(TEST_FILE_NAME_ASCII);
			ASSERT_TRUE(path.getHostCharsetPath() == TEST_FILE_NAME_ASCII);
#ifdef _WIN32
			path.getWidePath();
#endif		
		}

		TEST_METHOD(TestMethod2)
		{
			Path path(TEST_FILE_NAME_ASCII);
			std::string path_str = (std::string)path;
			ASSERT_TRUE(path.getHostCharsetPath() == path_str);
#ifdef _WIN32
			std::wstring path_wstr = (std::wstring)path;
			ASSERT_TRUE(path.getWidePath() == path_wstr);
#endif
		}


		TEST_METHOD(TestMethod3)
		{
			Path path1("hello"), path2(TEST_FILE_NAME_ASCII);
			Path path3 = path1 + path2;
#ifdef _WIN32
			ASSERT_TRUE(path3.getHostCharsetPath() == "hello\\somefile.txt");
#else
			ASSERT_TRUE(path3.getHostCharsetPath() == "hello/somefile.txt");
#endif
		}


		TEST_METHOD(TestMethod4)
		{
			{
				Path path("head");
				std::pair<Path, Path> split_path = path.split();
				ASSERT_TRUE((std::string)split_path.second == "head");
			}

			{
				Path path("head" + std::string(DISK_PATH_SEPARATOR_STRING));
				std::pair<Path, Path> split_path = path.split();
				ASSERT_TRUE((std::string)split_path.first == "head");
			}

			{
				Path path("head" + std::string(DISK_PATH_SEPARATOR_STRING) + "tail");
				std::pair<Path, Path> split_path = path.split();
				ASSERT_TRUE((std::string)split_path.first == "head");
				ASSERT_TRUE((std::string)split_path.second == "tail");
			}

			{
				Path path = Path("head1") + Path("head2") + Path("tail");
				std::pair<Path, Path> split_path = path.split();
				ASSERT_TRUE(split_path.first == (Path("head1") + Path("head2")));
				ASSERT_TRUE((std::string)split_path.second == "tail");
			}
		}

	};
}