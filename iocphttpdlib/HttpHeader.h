#pragma once
#include "stdafx.h"


class HttpHeader
{
public:
	HttpHeader();
	~HttpHeader();


public:


	typedef enum { PSTATE_GET, PSTATE_POST, PSTATE_URL, PSTATE_HTTP_VER
					, PSTATE_HOST_DETECTED, PSTATE_HOST_VALUE 
					, PSTATE_CONNECTION_DETECTED, PSTATE_CONNECTION_VALUE
	} ParseState;

	void Parse(char* content);
	MethodType GetMethod();
	bool Url(char *urlval);

	char* GetUrl();

private:
	char m_url[DATA_BUFSIZE];
	char m_connection[DATA_BUFSIZE];
	char m_host[DATA_BUFSIZE];
	char m_httpVer[DATA_BUFSIZE];
	char m_content[DATA_BUFSIZE];
	MethodType m_method;
	ParseState m_ps;

	int i;

};

