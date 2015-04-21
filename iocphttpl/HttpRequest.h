#pragma once
#include "HttpHeader.h"
#include "HParser.h"
#include "Enums.h"
#include "HttpTemplate.h"
#include "RequestParser.h"

class IOCPHTTPL_API HttpRequest : public HttpTemplate
{
public:
	HttpRequest();
	~HttpRequest();

	void Parse(char *content);
	char* GetUrl();
	MethodType GetMethod();

private:
	headerparser::HParser hParser;
	HttpHeader httpHeader;
	RequestParser requestParser;
};

