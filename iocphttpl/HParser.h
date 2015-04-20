#pragma once
#include "stdafx.h"
#include "httpparser.tab.h"

namespace headerparser
{

	class IOCPHTTPL_API HParser
	{
	public:
		HParser();
		~HParser();

		void Parse();
		void Parse(char* content);
		char* GetUrl();
		const char* GetMethod();

	private:
		char m_Url[DATA_BUFSIZE];
		char m_Method[DATA_BUFSIZE];

		HANDLE ghMutex;
	};

}