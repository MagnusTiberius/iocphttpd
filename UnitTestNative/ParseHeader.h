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
		ENDTOKEN
	}token_t;

	token_t Token();
	void Parse();

private:
	CHAR *token;
};

