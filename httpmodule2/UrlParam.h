#pragma once
#include "stdafx.h"
#include <regex>

class UrlParam
{
public:
	UrlParam();
	~UrlParam();

	void Parse(char *path, char* url, std::vector<std::string> *list);
	bool IsMatching(char *path, char* url);

	typedef struct {
		std::smatch match;
	} pairvalue_t, *lppairvalue_t;

	typedef std::pair<std::string, lppairvalue_t> pair_t;

private:
	char *m_path;
	char *m_url;

	void find_and_replace(string& source, string const& find, string const& replace);

	std::vector<pair_t> paramList;
	std::vector<pair_t>::iterator itrList;

	std::vector<std::string> urlParams;

};

