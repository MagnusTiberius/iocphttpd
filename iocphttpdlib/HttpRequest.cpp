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
}

char* HttpRequest::GetUrl()
{
	return httpHeader.GetUrl();
}

HttpHeader::MethodType HttpRequest::GetMethod()
{
	return httpHeader.GetMethod();
}