#pragma once
#include "stdafx.h"
#include "ScannerA.h"

class IOCPHTTPL_API ParseHeader : public ScannerA
{
public:
	ParseHeader();
	~ParseHeader();

	typedef enum TOKEN {
		GET = 20000,
		POST = 20001,
		URL = 20002,
		HTTPVERSION = 20003,
		PROPERTYNAME = 20004,
		PROPERTYVALUE = 20005,
		ENDTOKEN = 20006
	}token_t;

	typedef struct HeaderMap {
		CHAR *name;
		CHAR *value;
	} headermap_t, *lpheadermap_t;

	typedef vector<lpheadermap_t> HEADERMAPLIST;

	int Token();
	void Parse();

private:
	CHAR *m_token;
	HEADERMAPLIST m_headermap;
};
