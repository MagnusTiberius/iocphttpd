#pragma once
#include "babudb/profiles/string_key.h"
#include "babudb/profiles/string_db.h"

#include "babudb/log/log_iterator.h"
#include "babudb/log/log.h"
#include "babudb/log/log_section.h"
#include "babudb/lookup_iterator.h"
#include "babudb/test.h"
#include "babudb/key.h"
#include "babudb/database.h"
#include "index/merger.h"
#include "util.h"

#include <algorithm>
#include <sstream>
using namespace std;
using namespace babudb;

#include <string>
using std::string;

namespace DatabaseDemo {

	class Database;
	class Log;

	class DemoDB {
	public:
		static DemoDB* Open(const string& name, const std::vector<string>& indices);
		~DemoDB();

		void Add(const string& index_name, const string& key, const string& value);
		void Remove(const string& index_name, const string& key);
		void Commit();

		string Lookup(const string& index, const string& key);
		// TODO: hide the LookupIterator
		babudb::LookupIterator Lookup(const string& index, const string& lower, const string& upper);

		// Merge the log and indices; truncate the log; prefix the obsolete data with "to"
		void Compact(const string& to);

	private:
		DemoDB(const string& name);
		babudb::StringOrder key_order;

		babudb::Database* db;
		babudb::Log* log;
		std::vector<string> index_names;
		string name;
	};

}