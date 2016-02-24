#pragma once
#include "stdafx.h"
#define DATA_BUFSIZE 8192

namespace IOCPHTTPL
{

	class IOCPHTTPL_API ParserHeader
	{
	public:
		ParserHeader();
		~ParserHeader();

		typedef enum { HTTP_NONE, HTTP_GET, HTTP_POST } MethodType;

		typedef enum {
			PSTATE_GET, PSTATE_POST, PSTATE_URL, PSTATE_HTTP_VER
			, PSTATE_HOST_DETECTED, PSTATE_HOST_VALUE
			, PSTATE_CONNECTION_DETECTED, PSTATE_CONNECTION_VALUE
		} ParseState;

		void Parse(const char* s);

	private:
		void ParseLine(char *buf1);

		MethodType m_methodType;
		ParseState m_ps;
	};

}