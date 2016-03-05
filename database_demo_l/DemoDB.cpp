#include "DemoDB.h"



namespace DatabaseDemo {

	DemoDB::DemoDB(const string& name) : name(name) { }

	DemoDB::~DemoDB() {
		delete log;
		delete db;
	}

	DemoDB* DemoDB::Open(const string& name, const std::vector<string>& indices) {
		DemoDB* db = new DemoDB(name);

		vector<IndexDescriptor> descs;
		for (vector<string>::const_iterator i = indices.begin(); i != indices.end(); ++i) {
			descs.push_back(make_pair(*i, &db->key_order));
		}

		db->db = babudb::Database::Open(name, descs);
		db->log = new babudb::Log(name);
		db->index_names = indices;

		// Now replay the log against the database to recreate the current state
		lsn_t min_persistent_lsn = db->db->GetMinimalPersistentLSN();
		db->log->Open(min_persistent_lsn);
		std::auto_ptr<babudb::Log::iterator> i(db->log->First());
		while (i->GetNext()) {
			StringSetOperation op;
			op.Deserialize(i->AsData());
			if (op.GetLSN() > min_persistent_lsn) {
				op.ApplyTo(*db->db, op.GetLSN());
			}
		}

		return db;
	}

	void DemoDB::Add(const std::string& index_name, const std::string& key,
		const std::string& value) {
		lsn_t lsn = db->GetCurrentLSN() + 1;
		log->GetTail(lsn)->Append(StringSetOperation(lsn, index_name, key, value));
		db->Add(index_name, lsn, Buffer::wrap(key), Buffer::wrap(value));
	}

	void DemoDB::Remove(const std::string& index_name,
		const std::string& key) {
		lsn_t lsn = db->GetCurrentLSN() + 1;
		log->GetTail(lsn)->Append(StringSetOperation(lsn, index_name, key));
		db->Add(index_name, lsn, Buffer::wrap(key), Buffer::Deleted());
	}

	void DemoDB::Commit() {
		lsn_t lsn = db->GetCurrentLSN();
		log->GetTail(lsn)->Commit();
	}

	string DemoDB::Lookup(const std::string& index,
		const std::string& key) {
		Buffer result = db->Lookup(index, Buffer::wrap(key));

		if (result.data) {
			return std::string(static_cast<char*>(result.data), result.size);
		}
		else {
			return std::string();
		}
	}

	LookupIterator DemoDB::Lookup(const std::string& index,
		const std::string& lower,
		const std::string& upper) {
		return db->Lookup(index, Buffer::wrap(lower), Buffer::wrap(upper));
	}

	void DemoDB::Compact(const string& to) {
		for (vector<string>::iterator name = index_names.begin();
			name != index_names.end(); ++name) {
			db->Snapshot(*name);
			db->CompactIndex(*name, db->GetCurrentLSN());
		}

		log->Cleanup(db->GetCurrentLSN(), to);

		// TODO: This won't clean up the last immutable index because the database 
		// does not know yet that there is a newer one. We would need to trigger a
		// reload of the last immutable index and then call cleanup.
		db->Cleanup(to);
	}

}
