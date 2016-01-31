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

bool HttpRequest::IsValid(char* input)
{
	return false;
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
	return mt;
}

const char* HttpRequest::GetContent()
{
	const char* rv = requestParser.GetParameterValue("Content:");
	return rv;
}