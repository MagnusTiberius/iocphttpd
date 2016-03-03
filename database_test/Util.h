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

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

class Util
{
public:
	static yield::Path testPath(const std::string& filename = "") {
		return yield::Path(TEST_OUTPUT_DIR) + filename;
	}
};