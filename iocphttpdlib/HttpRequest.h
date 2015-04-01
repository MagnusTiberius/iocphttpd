#pragma once
#include "HttpHeader.h"


class HttpRequest
{
public:
	HttpRequest();
	~HttpRequest();

	void Parse(char *content);
	char* GetUrl();
	HttpHeader::MethodType GetMethod();

private:
	HttpHeader httpHeader;
};

