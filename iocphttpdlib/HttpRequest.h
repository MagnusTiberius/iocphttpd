#pragma once
#include "HttpHeader.h"
#include "HParser.h"

class HttpRequest
{
public:
	HttpRequest();
	~HttpRequest();

	void Parse(char *content);
	char* GetUrl();
	HttpHeader::MethodType GetMethod();

private:
	headerparser::HParser hParser;
	HttpHeader httpHeader;
};

