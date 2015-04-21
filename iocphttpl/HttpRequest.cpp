#include "HttpRequest.h"


HttpRequest::HttpRequest()
{
}


HttpRequest::~HttpRequest()
{
}


void HttpRequest::Parse(char *content)
{
	//httpHeader.Parse(content);
	//hParser.Parse(content);
	requestParser.Input(content);
	requestParser.Parse();
}

char* HttpRequest::GetUrl()
{
	char *url;
	//url  = httpHeader.GetUrl();
	//url = hParser.GetUrl();
	url = requestParser.GetUrl();
	return url;
}

MethodType HttpRequest::GetMethod()
{
	MethodType mt;
	//mt = httpHeader.GetMethod();
	mt = requestParser.GetMethod();
	return httpHeader.GetMethod();
}