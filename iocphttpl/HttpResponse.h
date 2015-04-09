#pragma once
#include "HttpHeader.h"



class IOCPHTTPL_API HttpResponse
{
public:
	HttpResponse();
	~HttpResponse();

	const char* resp_ok = "HTTP/1.0 200 OK";
	const char* resp_ct = "text/html";
	const char* tmp_date = "Fri, 31 Dec 1999 23:59:59 GMT";

	void Write(const char* str);

	void GetResponse(char* szResponse, DWORD dwSize);

private:
	HttpHeader httpHeader;
	char* m_resp_body[DATA_BUFSIZE * 4];
	char* m_content_type[DATA_BUFSIZE];
	char* m_content_lenght[DATA_BUFSIZE];
	char* m_date[DATA_BUFSIZE];
	std::string m_szResponse;
};

