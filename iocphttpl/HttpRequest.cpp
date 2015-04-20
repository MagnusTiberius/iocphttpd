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
	char *url = httpHeader.GetUrl();
	url = hParser.GetUrl();
	return url;
}

MethodType HttpRequest::GetMethod()
{
	return httpHeader.GetMethod();
}