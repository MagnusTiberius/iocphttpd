#include "HttpResponse.h"


HttpResponse::HttpResponse()
{
}


HttpResponse::~HttpResponse()
{
}


void HttpResponse::Write(const char* str)
{
	std::vector<byte> rv;
	rv.assign(str, str + strlen(str));
	m_sbResponse = rv;
	//m_szResponse = std::string(str);
}

void HttpResponse::WriteTemplate(char* code)
{
	char buf[DATA_BUFSIZE];
	memset(buf, '\0', DATA_BUFSIZE);
	std::string str = GetTemplateContent(code);
	std::string filename = GetTemplateFilepath(code);
	GetPathExtension(filename.c_str());
	SetContentTypeFromExtension();
	std::vector<byte> rv;
	const char *ret = str.c_str();
	rv.assign(ret, ret + strlen(ret));
	m_sbResponse = rv;
}

void HttpResponse::SetStaticFileName(string path)
{
	m_path = path;
	GetPathExtension(path.c_str());
	SetContentTypeFromExtension();
}

const CHAR* HttpResponse::GetStaticFileName()
{
	return m_path.c_str();
}

PTSTR  HttpResponse::GetPathExtension(const char* pszPath)
{
	std::string str = std::string(pszPath);
	std::wstring wstr = std::wstring(str.begin(), str.end());
	PTSTR pstr = ::PathFindExtension(wstr.c_str());
	pathExtension.assign(pstr);
	szPath = pszPath;
	return pstr;
}

PTSTR  HttpResponse::GetPathExtension(PTSTR pszPath)
{
	PTSTR str = ::PathFindExtension(pszPath);
	pathExtension.assign(str);
	return str;
}

void HttpResponse::SetContentTypeFromExtension()
{
	contenType.assign(L"text/html");
	std::wstring wstr;
	wstr.assign(pathExtension);
	if (wstr.compare(L".jpg") == 0)
	{
		contenType.assign(L"image/jpg");
	}
	if (wstr.compare(L".png") == 0)
	{
		contenType.assign(L"image/png");
	}
	if (wstr.compare(L".css") == 0)
	{
		contenType.assign(L"text/css");
	}
	if (wstr.compare(L".js") == 0)
	{
		contenType.assign(L"text/javascript");
	}
}

std::vector<byte> HttpResponse::GetStaticContent(const char *path)
{

	if (strlen(path)==0)
	{
		std::vector<BYTE> vec;
		return vec;
	}

	// open the file:
	std::ifstream file(path, std::ios::binary);
	
	// Stop eating new lines in binary mode!!!
	file.unsetf(std::ios::skipws);

	// get its size:
	std::streampos fileSize;

	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	// reserve capacity
	std::vector<BYTE> vec;
	vec.reserve(fileSize);

	// read the data:
	vec.insert(vec.begin(),
		std::istream_iterator<BYTE>(file),
		std::istream_iterator<BYTE>());

	printf("GetStaticContent: %s is now read out of the file, copied into a buffer\n", path);

	return vec;
}

void HttpResponse::WriteStatic(const char *path)
{
	if (strlen(path) > 0)
	{
		m_sbResponse = GetStaticContent(path);
		printf("PATH: %s is now in the buffer\n", path);
	}
	else
	{
		printf("Static filepath is empty\n");
	}
}




void HttpResponse::GetResponse(char* pszResponse, vector<byte> *pvb, DWORD dwSize)
{
	DWORD dwThreadId = GetCurrentThreadId();
	ZeroMemory(pszResponse, dwSize);
	sprintf_s(pszResponse, dwSize, "%s%s", resp_ok, "\n");
	sprintf_s(pszResponse, dwSize, "%s%s%s%s", pszResponse, "Date: " , "HTTP/1.0 200 OK" , "\n");
	std::string ctstr;
	ctstr.assign(contenType.begin(), contenType.end());
	sprintf_s(pszResponse, dwSize, "%s%s%s%s", pszResponse, "Content-Type: ", ctstr.c_str(), "\n");

	size_t siz = m_sbResponse.size();
	sprintf_s(pszResponse, dwSize, "%s%s%d%s", pszResponse, "Content-Length: ", siz, "\n\n");
	std::string str;
	str.assign(pszResponse);

	//sprintf_s(pszResponse, dwSize, "%s%s%s", pszResponse, m_sbResponse, "\n");
	std::string str2(m_sbResponse.begin(), m_sbResponse.end());
	str.insert(str.end(), str2.begin(), str2.end());
	//pszResponse = str.c_str();
	//sprintf_s(pszResponse, dwSize, "%s", str.c_str());
	memcpy(pszResponse, str.c_str(), str.size());
	m_sbResponse.assign(str.begin(), str.end());
	//printf("%d::%s \n", dwThreadId, pszResponse);
	//vector<byte> tpvb = *pvb;
	//tpvb.assign(m_sbResponse.begin(), m_sbResponse.end());
	pvb->assign(m_sbResponse.begin(), m_sbResponse.end());
}

CHAR* HttpResponse::GetResponse2()
{
	DWORD dwThreadId = GetCurrentThreadId();
	CHAR pszResponse[DATA_BUFSIZE];
	std::string rv = "";
	ZeroMemory(&pszResponse, DATA_BUFSIZE);

	sprintf_s(pszResponse, DATA_BUFSIZE, "%s%s", resp_ok, "\n");
	rv.append(pszResponse);

	ZeroMemory(&pszResponse, DATA_BUFSIZE);
	sprintf_s(pszResponse, DATA_BUFSIZE, "%s%s%s", "Date: ", tmp_date, "\n");
	rv.append(pszResponse);


	std::string ctstr;
	ctstr.assign(contenType.begin(), contenType.end());
	ZeroMemory(&pszResponse, DATA_BUFSIZE);
	sprintf_s(pszResponse, DATA_BUFSIZE, "%s%s%s", "Content-Type: ", ctstr.c_str(), "\n");
	rv.append(pszResponse);

	size_t siz = m_sbResponse.size();
	ZeroMemory(&pszResponse, DATA_BUFSIZE);
	sprintf_s(pszResponse, DATA_BUFSIZE, "%s%d%s", "Content-Length: ", siz, "\n\n");
	rv.append(pszResponse);

	std::string str2(m_sbResponse.begin(), m_sbResponse.end());
	rv.append(str2);

	CHAR *cv = (CHAR*)malloc(rv.size() + 1);
	ZeroMemory(cv, rv.size() + 1);
	memcpy(cv, rv.c_str(), rv.size());

	return cv;
}


int HttpResponse::GetBufferSize()
{
	return m_sbResponse.size();
}