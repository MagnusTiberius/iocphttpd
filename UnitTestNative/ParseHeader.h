#pragma once
#include "stdafx.h"

class ParseHeader : public ScannerA
{
public:
	ParseHeader();
	~ParseHeader();

	typedef enum TOKEN {
		GET,
		POST,
		URL,
		HTTPVERSION,
		PROPERTYNAME,
		PROPERTYVALUE,
		NEWLINE,
		CONTENTFOLLOWSNEXT,
		ENDTOKEN,
		UNDEFINED
	}token_t;

	token_t Token();
	void Parse();
	bool IsEmpty();
private:
	CHAR *token;
};

