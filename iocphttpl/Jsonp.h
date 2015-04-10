#pragma once
#include "stdafx.h"
#include "jsonp.tab.h"


class IOCPHTTPL_API Jsonp
{
public:
	Jsonp();
	~Jsonp();

	void Parse(char* content);

};

