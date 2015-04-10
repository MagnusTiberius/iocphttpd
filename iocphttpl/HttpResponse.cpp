#include "HttpResponse.h"


HttpResponse::HttpResponse()
{
}


HttpResponse::~HttpResponse()
{
}


void HttpResponse::Write(const char* str)
{
	m_szResponse = std::string(str);
}

void HttpResponse::WriteTemplate(char* code)
{
	char buf[DATA_BUFSIZE];
	memset(buf, '\0', DATA_BUFSIZE);
	std::string str = GetTemplateContent(code);
	m_szResponse = std::string(str);
}

void HttpResponse::GetResponse(char* pszResponse, DWORD dwSize)
{
	DWORD dwThreadId = GetCurrentThreadId();
	std::string hdr;
	ZeroMemory(pszResponse, dwSize);
	sprintf_s(pszResponse, dwSize, "%s%s", resp_ok, "\n");
	sprintf_s(pszResponse, dwSize, "%s%s%s%s", pszResponse, "Date: " , "HTTP/1.0 200 OK" , "\n");
	sprintf_s(pszResponse, dwSize, "%s%s%s%s", pszResponse, "Content-Type: " , resp_ct, "\n");

	size_t siz = strlen(m_szResponse.c_str());
	sprintf_s(pszResponse, dwSize, "%s%s%d%s", pszResponse, "Content-Length: ", siz, "\n\n");
	sprintf_s(pszResponse, dwSize, "%s%s%s", pszResponse, m_szResponse.c_str(), "\n");
	printf("%d::%s \n", dwThreadId, m_szResponse.c_str());

}