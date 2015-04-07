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

	private:
		char m_Url[DATA_BUFSIZE];
	};

}