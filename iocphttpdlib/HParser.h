#pragma once
#include "stdafx.h"

//#ifdef __cplusplus
//extern "C" {
//#endif
//
//#include "calc.tab.h"
//
//#ifdef __cplusplus
//}
//#endif

namespace headerparser
{

	class HParser
	{
	public:
		HParser();
		~HParser();

		void Parse();
		void Parse(char* content);
		const char* GetUrl();
		const char* GetMethod();

	private:
		char m_Url[DATA_BUFSIZE];
		char m_Method[DATA_BUFSIZE];
	};

}