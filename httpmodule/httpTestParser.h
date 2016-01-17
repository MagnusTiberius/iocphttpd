#pragma once
#include "stdafx.h"

extern void hpParseIt(const char *str);

class HTTPMODULE_API httpTestParser
{
public:
	httpTestParser();
	~httpTestParser();

	void Test1()
	{
		char *str = "GET / HTTP/1.1\r\n\r\n";
		//hpParseIt("GET / HTTP/1.1\r\n\r\n");
	}

};

