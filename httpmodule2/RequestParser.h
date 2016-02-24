#pragma once
#include "stdafx.h"
#include "ScannerA.h"

namespace HTTPMODULE2
{


	class RequestParser : public ScannerA
	{
	public:
		RequestParser();
		~RequestParser();

		typedef enum TOKEN {
			GET = 20000,
			POST = 20001,
			URL = 20002,
			HTTPVERSION = 20003,
			PROPERTYNAME = 20004,
			PROPERTYVALUE = 20005,
			ENDTOKEN = 20006,
			QUERYSTRING = 20007,
			BEGIN = 20008
		}token_t;

		typedef struct HeaderMap {
			CHAR *name;
			CHAR *value;
		} headermap_t, *lpheadermap_t;

		typedef vector<lpheadermap_t> HEADERMAPLIST;

		typedef vector<char*> tokenlist_t;
		typedef vector<char*>::iterator itokenlist_t;

		int Token();
		void Parse();
		CHAR* GetUrl();
		MethodType GetMethod();
		void Reset();
		const char* GetParameterValue(const char *p);

		bool IsValid();

	protected:
		CHAR *m_token;
		HEADERMAPLIST m_headermap;
		void ParseContent();
		const CHAR* StripOut(CHAR* item, const CHAR *strip);
		tokenlist_t tokenlist;
	};

}