#include "HttpRequest.h"


HttpRequest::HttpRequest()
{
}


HttpRequest::~HttpRequest()
{
}


void HttpRequest::Parse(char *content)
{
	httpHeader.Parse(content);
	hParser.Parse(content);
}

char* HttpRequest::GetUrl()
{
	return httpHeader.GetUrl();
}

MethodType HttpRequest::GetMethod()
{
	return httpHeader.GetMethod();
}