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


void HttpResponse::GetResponse(char* pszResponse, DWORD dwSize)
{
	std::string hdr;
	ZeroMemory(pszResponse, dwSize);
	sprintf_s(pszResponse, dwSize, "%s%s", resp_ok, "\n");
	sprintf_s(pszResponse, dwSize, "%s%s%s%s", pszResponse, "Date: " , "HTTP/1.0 200 OK" , "\n");
	sprintf_s(pszResponse, dwSize, "%s%s%s%s", pszResponse, "Content-Type: " , resp_ct, "\n");

	size_t siz = strlen(m_szResponse.c_str());
	sprintf_s(pszResponse, dwSize, "%s%s%d%s", pszResponse, "Content-Length: ", siz, "\n\n");
	sprintf_s(pszResponse, dwSize, "%s%s%s", pszResponse, m_szResponse.c_str(), "\n");
	printf("%s \n", m_szResponse.c_str());

}