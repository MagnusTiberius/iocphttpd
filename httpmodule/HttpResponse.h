#pragma once
#include "HttpHeader.h"
#include "HttpTemplate.h"
#include <fstream>
#include "Shlwapi.h"
#include "json.h"
#include <time.h>
#include <sstream>
#include "MimeTypes.h"
#include <assert.h> 
#include "CacheController.h"

class HTTPMODULE_API HttpResponse : public HttpTemplate
{
public:
	HttpResponse();
	~HttpResponse();

	typedef struct {
		char *name;
		char *content;
		vector<byte> *bytcontent;
		time_t rawtime;

	} static_content_t, *lpstatic_content_t;
	typedef std::vector<lpstatic_content_t> buflist_t;
	typedef std::vector<lpstatic_content_t>::iterator ibuflist_t;

	const char* resp_ok = "HTTP/1.x 200 OK";
	const char* resp_ct = "text/html";
	const char* tmp_date = "Fri, 31 Dec 1999 23:59:59 GMT";

	void Write(const char* str);
	void WriteTemplate(char* code);
	void GetResponse(char* szResponse, vector<byte> *pvb, DWORD dwSize);
	void SetStaticFileName(string path);
	std::vector<byte> GetStaticContent(const char *path);
	char* GetStaticContent(std::wstring wfile_name); 
	byte* GetStaticContent2(const char *file_name, long *len);
	std::vector<byte> GetStaticContent3(const char *file_name);
	void WriteStatic(const char *path);
	void SetContentTypeFromExtension();
	PTSTR  GetPathExtension(const char* pszPath);
	PTSTR  GetPathExtension(PTSTR pszPath);
	void SetContentType(const char* str);
	const char* GetContent();
	byte*  GetResponse2(ULONG *len);
	void GetResponse3(std::vector<byte> *pvb);
	void SetCacheController(CacheController* controller);

private:
	HttpHeader httpHeader;
	char* m_resp_body[DATA_BUFSIZE * 4];
	char* m_content_type[DATA_BUFSIZE];
	char* m_content_lenght[DATA_BUFSIZE];
	char* m_date[DATA_BUFSIZE];
	std::string m_szResponse;
	std::string m_path;
	std::vector<byte> m_sbResponsePackage;
	byte *m_byteContent;

	std::wstring pathExtension;
	std::wstring contenType;
	const char *szPath;
	long bufsize;
	HANDLE ghMutex;

	buflist_t m_bufferList;

	MimeTypes mimeTypes;
	CacheController* cacheController;
	
};

