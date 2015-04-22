#include "HttpResponse.h"


HttpResponse::HttpResponse()
{
	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);
}


HttpResponse::~HttpResponse()
{

	::WaitForSingleObject(ghMutex, INFINITE);
	ibuflist_t itr;
	for (itr = m_bufferList.begin(); itr != m_bufferList.end(); itr++)
	{
		char *p = *itr;
		if (p != NULL)
		{
			free(p);
			p = NULL;
		}
	}
	::ReleaseMutex(ghMutex);
	::CloseHandle(ghMutex);
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
		contenType.assign(L"application/javascript"); 
	}
}

void HttpResponse::SetContentType(const char* str)
{
	std::string s;
	s.assign(str);
	contenType.assign(s.begin(), s.end());
}

const char* HttpResponse::GetContent()
{
	return "";
}

std::vector<byte> HttpResponse::GetStaticContent(const char *path)
{
	DWORD dwThreadId = GetCurrentThreadId();
	printf("%d::Reading filename: %s \n", dwThreadId, path);
	::WaitForSingleObject(ghMutex, INFINITE);

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

	::ReleaseMutex(ghMutex);

	return vec;
}

char* HttpResponse::GetStaticContent2(const char *file_name)
{
	::WaitForSingleObject(ghMutex, INFINITE);
	FILE *fp;
	char *buf = NULL;

	fopen_s(&fp, file_name, "r");
	if (fp == NULL)
	{
		return NULL;
	}
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	rewind(fp);

	buf = (char*)malloc(size + 1);
	memset(buf, 0, size);

	int i = 0;
	int ch;
	while ((ch = fgetc(fp)) != EOF)
	{
		buf[i++] = ch;
	}

	fclose(fp);

	m_bufferList.push_back(buf);

	::ReleaseMutex(ghMutex);

	return buf;

}

char* HttpResponse::GetStaticContent(std::wstring wfile_name)
{

	std::string file_name;
	file_name.assign(wfile_name.begin(), wfile_name.end());
	char *buf = GetStaticContent2(file_name.c_str());

	return buf;
}

void HttpResponse::WriteStatic(const char *path)
{
	m_sbResponse = GetStaticContent(path);
}


void HttpResponse::GetResponse(char* pszResponse, vector<byte> *pvb, DWORD dwSize)
{
	DWORD dwThreadId = GetCurrentThreadId();
	ZeroMemory(pszResponse, dwSize);
	sprintf_s(pszResponse, dwSize, "%s%s", resp_ok, "\n");
	sprintf_s(pszResponse, dwSize, "%s%s%s%s", pszResponse, "Date: " , "HTTP/1.x 200 OK" , "\n");
	std::string ctstr;
	ctstr.assign(contenType.begin(), contenType.end());
	sprintf_s(pszResponse, dwSize, "%s%s%s%s", pszResponse, "Content-Type: ", ctstr.c_str(), "\n");

	size_t siz = m_sbResponse.size();
	sprintf_s(pszResponse, dwSize, "%s%s%d%s", pszResponse, "Content-Length: ", siz, "\n\n");
	std::string str;
	str.assign(pszResponse);

	std::string str2(m_sbResponse.begin(), m_sbResponse.end());
	str.insert(str.end(), str2.begin(), str2.end());
	sprintf_s(pszResponse, dwSize, "%s", str.c_str());
	m_sbResponse.assign(str.begin(), str.end());
	pvb->assign(m_sbResponse.begin(), m_sbResponse.end());
}