#pragma once
#include "stdafx.h"

namespace IOCPHTTPL
{

	class IOCPHTTPL_API HttpHeader
	{
	public:
		HttpHeader();
		~HttpHeader();


	public:

		void Parse(char* content);
		MethodType GetMethod();
		bool Url(char *urlval);

		char* GetUrl();

	private:
		char m_url[DATA_BUFSIZE];
		char m_connection[DATA_BUFSIZE];
		char m_host[DATA_BUFSIZE];
		char m_httpVer[DATA_BUFSIZE];
		char m_content[DATA_BUFSIZE];
		MethodType m_method;
		ParseState m_ps;

		int i;

	};

}