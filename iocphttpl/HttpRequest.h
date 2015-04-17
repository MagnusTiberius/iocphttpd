#pragma once
#include "HttpHeader.h"
#include "Enums.h"
#include "HttpTemplate.h"
#include "ParseHeader.h"

class IOCPHTTPL_API HttpRequest : public HttpTemplate
{
public:
	HttpRequest();
	~HttpRequest();

	void Parse(char *content);
	char* GetUrl();
	MethodType GetMethod();

	bool isStatic;

private:
	HttpHeader httpHeader;
	ParseHeader parseHeader;
};

