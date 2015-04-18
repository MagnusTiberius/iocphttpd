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
	parseHeader.Input(content);
	parseHeader.Parse();
}

char* HttpRequest::GetUrl()
{
	//char *url = httpHeader.GetUrl();
	char *url = NULL;
	url = parseHeader.GetUrl();
	return url;
}

MethodType HttpRequest::GetMethod()
{
	return parseHeader.GetMethod();
}