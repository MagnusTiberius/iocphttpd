#include "HttpResponse.h"


namespace IOCPHTTPL
{

	HttpResponse::HttpResponse()
	{
		ghMutex = CreateMutex(
			NULL,              // default security attributes
			FALSE,             // initially not owned
			NULL);



	}


	void HttpResponse::SetCacheController(CacheController* controller)
	{
		cacheController = controller;
	}

	HttpResponse::~HttpResponse()
	{

		if (::WaitForSingleObject(ghMutex, 10000) == WAIT_OBJECT_0)
		{
			ibuflist_t itr;
			for (itr = m_bufferList.begin(); itr != m_bufferList.end(); itr++)
			{
				lpstatic_content_t p = *itr;
				if (p != NULL)
				{
					if (p->name != NULL)
					{
						free(p->name);
						p->name = NULL;
					}
					if (p->content != NULL)
					{
						free(p->content);
						p->content = NULL;
					}
					free(p);
					p = NULL;
				}
			}
		}
		::ReleaseMutex(ghMutex);
		::CloseHandle(ghMutex);
	}


	void HttpResponse::Write(const char* str)
	{
		std::vector<byte> rv;
		rv.assign(str, str + strlen(str));
		m_sbResponsePackage = rv;
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
		m_sbResponsePackage = rv;
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

		mimeTypes.AssignContentType(wstr, &contenType);

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
		std::vector<BYTE> vec;
		DWORD dwThreadId = GetCurrentThreadId();
		if (::WaitForSingleObject(ghMutex, 10000) == WAIT_OBJECT_0)
		{
			printf("%d::Reading filename: %s \n", dwThreadId, path);
			/*
			ibuflist_t itr;
			for (itr = m_bufferList.begin(); itr != m_bufferList.end(); itr++)
			{
			lpstatic_content_t pitem = *itr;
			int diff = strcmp(pitem->name, path);
			if (diff == 0)
			{
			pitem->rawtime = time(0);
			::ReleaseMutex(ghMutex);
			return *pitem->bytcontent;
			}
			}
			*/

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

			vec.reserve(fileSize);

			// read the data:
			vec.insert(vec.begin(),
				std::istream_iterator<BYTE>(file),
				std::istream_iterator<BYTE>());

			/*
			lpstatic_content_t p1 = (lpstatic_content_t)malloc(sizeof(static_content_t));
			p1->name = _strdup(path);
			p1->bytcontent = new std::vector<byte>();
			p1->bytcontent->assign(vec.begin(), vec.end());
			p1->rawtime = time(0);
			m_bufferList.push_back(p1);
			*/
		}
		::ReleaseMutex(ghMutex);

		return vec;
	}

	std::vector<byte> HttpResponse::GetStaticContent3(const char *file_name)
	{
		// There's a bug here, str will stop at '\0' for binary chars.
		// Won't work for images and other bin files.
		byte *buf = GetStaticContent2(file_name, &bufsize);
		std::vector<byte> bytbuf;
		std::string str;
		str.assign((char*)buf);
		bytbuf.assign(str.begin(), str.end());
		return bytbuf;
	}


	void HttpResponse::AddHeaderItem(string name, string value)
	{
		headerList[name] = value;
	}

	byte* HttpResponse::GetStaticContent2(const char *file_name, long *len)
	{
		FILE *fp;
		char *buf = NULL;

		DWORD res = ::WaitForSingleObject(ghMutex, 10000);
		if (res == WAIT_OBJECT_0)
		{
			char* fname = _strdup(file_name);
			if (cacheController->FindTemplate(fname))
			{
				string mcontent = cacheController->GetTemplateContent(fname);
				byte* result = (byte*)mcontent.c_str();
				return result;
			}
			else
			{
				cacheController->AddTemplate(fname, fname);
				string mcontent = cacheController->GetTemplateContent(fname);
				byte* result = (byte*)mcontent.c_str();
				return result;
			}

			/*
			time_t now = time(0);
			ibuflist_t itr;
			for (itr = m_bufferList.begin(); itr != m_bufferList.end(); itr++)
			{
			lpstatic_content_t pitem = *itr;
			int diff = strcmp(pitem->name, file_name);
			if (diff == 0)
			{
			pitem->rawtime = time(0);
			::ReleaseMutex(ghMutex);
			return pitem->content;
			}
			double dt = difftime(now, pitem->rawtime);

			}
			*/




			fopen_s(&fp, file_name, "rb");

			assert(fp != NULL);

			//if (fp == NULL)
			//{
			//	return NULL;
			//}
			fseek(fp, 0, SEEK_END);
			long size = ftell(fp);
			*len = size;
			rewind(fp);

			buf = (char*)malloc(size + 1);
			assert(buf != NULL);
			memset(buf, 0, size);

			int i = 0;
			int ch;
			while ((ch = fgetc(fp)) != EOF)
			{
				buf[i++] = ch;
			}

			fclose(fp);

			/*
			lpstatic_content_t p1 = (lpstatic_content_t)malloc(sizeof(static_content_t));
			p1->name = _strdup(file_name);
			p1->content = buf;
			p1->rawtime = time(0);
			m_bufferList.push_back(p1);
			*/

		}

		::ReleaseMutex(ghMutex);

		return (byte*)buf;

	}

	char* HttpResponse::GetStaticContent(std::wstring wfile_name)
	{

		std::string file_name;
		file_name.assign(wfile_name.begin(), wfile_name.end());
		byte *buf = GetStaticContent2(file_name.c_str(), &bufsize);

		return (char*)buf;
	}

	void HttpResponse::WriteStatic(const char *path)
	{
		m_sbResponsePackage = GetStaticContent(path);
		m_byteContent = GetStaticContent2(path, &bufsize);
	}


	void HttpResponse::GetResponse(char* pszResponse, vector<byte> *pvb, DWORD dwSize)
	{
		DWORD dwThreadId = GetCurrentThreadId();
		ZeroMemory(pszResponse, dwSize);
		sprintf_s(pszResponse, dwSize, "%s%s", resp_ok, "\n");
		sprintf_s(pszResponse, dwSize, "%s%s%s%s", pszResponse, "Date: ", "HTTP/1.x 200 OK", "\n");
		std::string ctstr;
		ctstr.assign(contenType.begin(), contenType.end());
		sprintf_s(pszResponse, dwSize, "%s%s%s%s", pszResponse, "Content-Type: ", ctstr.c_str(), "\n");

		map<string, string>::iterator it;
		if (headerList.size() > 0)
		{
			for (it = headerList.begin(); it != headerList.end(); it++)
			{
				string name = it->first;
				string value = it->second;
				sprintf_s(pszResponse, dwSize, "%s%s%s%s", pszResponse, name.c_str(), value.c_str(), "\n");
			}
		}

		size_t siz = m_sbResponsePackage.size();
		sprintf_s(pszResponse, dwSize, "%s%s%d%s", pszResponse, "Content-Length: ", siz, "\n\n");
		std::string str;
		str.assign(pszResponse);

		std::string str2(m_sbResponsePackage.begin(), m_sbResponsePackage.end());
		str.insert(str.end(), str2.begin(), str2.end());
		sprintf_s(pszResponse, dwSize, "%s", str.c_str());
		m_sbResponsePackage.assign(str.begin(), str.end());
		pvb->assign(m_sbResponsePackage.begin(), m_sbResponsePackage.end());
	}

	/*
	byte* HttpResponse::GetResponse2(ULONG *len)
	{
	*len = 0;

	std::string ctstr;
	ctstr.assign(contenType.begin(), contenType.end());

	std::ostringstream oss;
	oss << resp_ok << "\n";
	oss << "Date: " << "May 10, 2015" << "\n";
	oss << "Content-Type: " << ctstr.c_str() << "\n";
	oss << "Content-Length: " << m_sbResponse.size() << "\n";
	oss << "\n";

	std::string s = oss.str();

	int bufsiz = s.length() + m_sbResponse.size();
	byte* buffer2 = (byte*)malloc(bufsiz);
	if (buffer2)
	{
	std::copy(s.begin(), s.end(), buffer2);
	std::copy(m_sbResponse.begin(), m_sbResponse.end(), &buffer2[s.length()]);
	*len = bufsiz;
	}

	return buffer2;
	}
	*/

	byte*  HttpResponse::GetResponse2(ULONG *len)
	{
		byte *content = NULL;
		DWORD dwThreadId = GetCurrentThreadId();
		char txtbuf[DATA_BUFSIZE];
		memset(txtbuf, 0, DATA_BUFSIZE);

		std::vector<byte> binbuffer = m_sbResponsePackage;

		std::string ctstr;
		ctstr.assign(contenType.begin(), contenType.end());
		size_t contentsiz = binbuffer.size();
		std::string ssiz = std::to_string(contentsiz);

		time_t t = time(0);   // get time now
		struct tm  now = *localtime(&t);
		char* dt = ctime(&t);

		if (PageNotFound)
		{
			strcpy_s(txtbuf, DATA_BUFSIZE, resp_404);
			strcat_s(txtbuf, DATA_BUFSIZE, "\n");
			strcat_s(txtbuf, DATA_BUFSIZE, "Date: ");
			strcat_s(txtbuf, DATA_BUFSIZE, dt);
			strcat_s(txtbuf, DATA_BUFSIZE, "\n");
			strcat_s(txtbuf, DATA_BUFSIZE, "Content-Type: ");
			strcat_s(txtbuf, DATA_BUFSIZE, ctstr.c_str());
			strcat_s(txtbuf, DATA_BUFSIZE, "\n");
			strcat_s(txtbuf, DATA_BUFSIZE, "\n");

			int nbuffersize = strlen(txtbuf);
			char* nbuf = _strdup(txtbuf);
			return (byte*)nbuf;
		}

		if (contentsiz == 0)
		{
			strcpy_s(txtbuf, DATA_BUFSIZE, resp_ok);
			strcat_s(txtbuf, DATA_BUFSIZE, "\n");
			strcat_s(txtbuf, DATA_BUFSIZE, "Date: ");
			strcat_s(txtbuf, DATA_BUFSIZE, dt);
			//strcat_s(txtbuf, DATA_BUFSIZE, "\n");
			strcat_s(txtbuf, DATA_BUFSIZE, "\n");
			int nbuffersize = strlen(txtbuf);
			content = (byte*)malloc(nbuffersize);
			assert(content != NULL);
			memset(content, 0, nbuffersize);
			memcpy(content, txtbuf, nbuffersize);
			*len = nbuffersize;

			return content;
		}


		strcpy_s(txtbuf, DATA_BUFSIZE, resp_ok);
		strcat_s(txtbuf, DATA_BUFSIZE, "\n");
		strcat_s(txtbuf, DATA_BUFSIZE, "Date: ");
		strcat_s(txtbuf, DATA_BUFSIZE, dt);
		//strcat_s(txtbuf, DATA_BUFSIZE, "\n");
		strcat_s(txtbuf, DATA_BUFSIZE, "Content-Type: ");
		strcat_s(txtbuf, DATA_BUFSIZE, ctstr.c_str());
		strcat_s(txtbuf, DATA_BUFSIZE, "\n");

		map<string, string>::iterator it;
		if (headerList.size() > 0)
		{
			for (it = headerList.begin(); it != headerList.end(); it++)
			{
				string name = it->first;
				name.append(": ");
				string value = it->second;
				strcat_s(txtbuf, DATA_BUFSIZE, name.c_str());
				strcat_s(txtbuf, DATA_BUFSIZE, value.c_str());
				strcat_s(txtbuf, DATA_BUFSIZE, "\n");
			}
		}

		strcat_s(txtbuf, DATA_BUFSIZE, "Content-Length: ");
		strcat_s(txtbuf, DATA_BUFSIZE, ssiz.c_str());
		strcat_s(txtbuf, DATA_BUFSIZE, "\n");
		strcat_s(txtbuf, DATA_BUFSIZE, "\n");

		int nbuffersize = strlen(txtbuf);
		int bufsiz = strlen(txtbuf) + binbuffer.size() + 1;
		content = (byte*)malloc(bufsiz);
		assert(content != NULL);
		memset(content, 0, bufsiz);
		memcpy(content, txtbuf, nbuffersize);
		byte *p = &content[nbuffersize];
		memcpy(content + nbuffersize, &binbuffer[0], binbuffer.size());
		*len = bufsiz;
		return content;
	}

	void HttpResponse::GetResponse3(std::vector<byte> *pvbHeaderContent)
	{
		DWORD dwThreadId = GetCurrentThreadId();
		std::ostringstream oss;

		std::string ctstr;
		ctstr.assign(contenType.begin(), contenType.end());

		if (m_sbResponsePackage.size() == 0)
		{
			oss << resp_ok << "\n";
			oss << "Date: " << "May 10, 2015" << "\n";
			oss << "\n";
			std::string headerBuffer = oss.str();
			std::vector<byte> tmp;
			tmp.insert(tmp.end(), headerBuffer.begin(), headerBuffer.end());
			pvbHeaderContent->clear();
			pvbHeaderContent->assign(tmp.begin(), tmp.end());
			return;
		}

		oss << resp_ok << "\n";
		oss << "Date: " << "May 10, 2015" << "\n";
		oss << "Content-Type: " << ctstr.c_str() << "\n";
		oss << "Content-Length: " << m_sbResponsePackage.size() << "\n";
		oss << "\n";
		std::string headerBuffer = oss.str();

		std::vector<byte> tmp;
		tmp.insert(tmp.end(), headerBuffer.begin(), headerBuffer.end());
		tmp.insert(tmp.end(), m_sbResponsePackage.begin(), m_sbResponsePackage.end());

		pvbHeaderContent->clear();
		pvbHeaderContent->assign(tmp.begin(), tmp.end());

		return;
	}


}