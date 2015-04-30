#include "HttpUrlRoute.h"


HttpUrlRoute::HttpUrlRoute()
{
}


HttpUrlRoute::~HttpUrlRoute()
{
}

void HttpUrlRoute::SetHomeDir(char *path)
{
	std::string str(path);
	std::wstring wstr(str.begin(), str.end());
	bool bres = SetCurrentDirectory(wstr.c_str());
	if (!bres)
	{
		printf("Cannot set Home Dir, dying now.\n");
		exit(1);
	}
	else
	{
		printf("Home Dir is now at: %s \n", path);
	}
	homeDir.assign(path);

}

void HttpUrlRoute::SetStatic(char *code, char *path)
{
	STATICACCESS sa = std::make_pair(code, path);
	staticAccessList.push_back(sa);
}

PTSTR  HttpUrlRoute::GetPathExtension(const char* pszPath)
{
	std::string str = std::string(pszPath);
	std::wstring wstr = std::wstring(str.begin(), str.end());
	PTSTR pstr = ::PathFindExtension(wstr.c_str());
	pathExtension.assign(pstr);
	return pstr;
}

PTSTR  HttpUrlRoute::GetPathExtension(PTSTR pszPath)
{
	PTSTR str = ::PathFindExtension(pszPath);
	pathExtension.assign(str);
	return str;
}

void HttpUrlRoute::SetContentTypeFromExtension()
{
	std::wstring wstr;
	wstr.assign(pathExtension);
	if (wstr.compare(L".jpg") == 0)
	{
		contenType.assign(L"image/jpg");
	}
}

string HttpUrlRoute::GetFullPath(char *path)
{
	for (STATICACCESSMAPITERATOR i = staticAccessList.begin(); i != staticAccessList.end(); ++i)
	{
		char* t_path = i->first;
		char* t_path2 = i->second;
		std::string subject(path);
		std::string result;
		std::regex re(t_path);
		std::smatch match;
		auto res = std::regex_search(subject, match, re);
		auto siz = match.size();
		if (res &&  siz >= 1)
		{
			printf("siz=%d; position=%d\n", siz, match.position());
			std::string str = std::regex_replace(path, re, t_path2);
			std::string str2 = std::regex_replace(str, regex("\/"), "\\");
			GetPathExtension(str2.c_str());
			return str2;
		}
	}
	return "";

}

//
//
/*
      c:\www\static\
	        \static\img\im1.jpg
*/
char* HttpUrlRoute::MergePath(char *path1, char *path2)
{
	int p = GetMergePoint(path1, path2);
	printf("Merge Point %d \n", p);
	return "";
}

int HttpUrlRoute::GetMergePoint(char *path1, char *path2)
{
	int len1 = GetLen(path1);
	int len2 = GetLen(path2);
	bool bOk = false;
	int n = 0;
	while (path1[n] != '\0')
	{
		bOk = IsOverlapping(path1, path2, n);
		if (bOk)
		{
			printf("Overlap found\n");
		}
		n++;
	}
	if (bOk)
	{
		return n;
	}
	else
	{
		return -1;
	}
}


bool HttpUrlRoute::IsOverlapping(char *path1, char *path2, int pos)
{
	bool isMatching = false;
	int i = pos;
	while (path1[i] != '\0')
	{
		char c = path1[i];
		int j = 0;
		while (path2[j] != '\0')
		{
			char d = path2[j];
			if (c == d)
			{
				isMatching = true;
			}
			else
			{
				isMatching = false;
			}
			j++;
		}
		i++;
	}
	return isMatching;
}

int HttpUrlRoute::GetLen(char *s)
{
	int i = 0;
	int len = 0;
	while (s[i] != '\0')
	{
		len++;
		i++;
	}
	return len;
}

bool HttpUrlRoute::IsStatic(char *path)
{
	if (staticAccessList.size() == 0)
		return false;

	for (STATICACCESSMAPITERATOR i = staticAccessList.begin(); i != staticAccessList.end(); ++i)
	{
		char* t_path = i->first;
		std::string subject(path);
		std::string result;
		std::regex re(t_path);
		std::smatch match;
		//std::size_t found = subject.find()
		auto res = std::regex_search(subject, match, re);
		auto siz = match.size();
		if (res &&  siz >= 1)
		{
			return true;
		}
	}
	return false;
}

void* HttpUrlRoute::GetUrlParamHandler(char *path)
{
	if (HasUrlParams(path))
	{
		return urlParamHandler;
	}
}

bool HttpUrlRoute::HasUrlParams(char *path)
{
	if (urlRoutes.size() == 0)
		return false;

	urlParams.clear();

	std::vector<std::string> tparams;

	for (ROUTEMAPITERATOR i = urlRoutes.begin(); i != urlRoutes.end(); ++i)
	{
		char* url = i->first;
		urlParamHandler = i->second;

		if (urlParam.IsMatching(path, url))
		{
			urlParam.Parse(path, url, &tparams);
			urlParams.assign(tparams.begin(), tparams.end());
			return true;
		}


	}
	urlParamHandler = NULL;
	return false;
}


/*

bool HttpUrlRoute::HasUrlParams(char *path)
{
	if (urlRoutes.size() == 0)
		return false;

	urlParams.clear();

	for (ROUTEMAPITERATOR i = urlRoutes.begin(); i != urlRoutes.end(); ++i)
	{
		char* url = i->first;
		urlParamHandler = i->second;
		std::string subject(url);
		std::string repl_subject;
		std::regex re("<(.*?)>");
		//std::regex re("<");
		//<(.*?)>
		//<.+>
		std::smatch match;
		std::smatch::iterator itr1;
		auto bOk = std::regex_search(subject, match, re);
		auto bOks = std::regex_match(subject, re);
		if (bOk)
		{
			for (itr1 = match.begin(); itr1 != match.end(); itr1++)
			{
				auto iv = *itr1;
				const char *siv11 = &iv.first[0];
				const auto *siv21 = &iv.second[0];
				auto siv3 = iv.str();

				std::regex re2("[^<]*");
				std::smatch match2;
				auto bOk2 = std::regex_search(subject, match2, re2);
				if (bOk2)
				{
					std::string sm = match2[0];
					sm.append("*");
					std::string mpath(path);
					std::smatch match3;
					std::regex re3(sm);
					auto bOk3 = std::regex_search(mpath, match3, re3);
					if (bOk3)
					{
						std::string sm30 = match3[0];
						std::string sm31 = match3[1];
						std::string sm3p = match3.prefix();
						std::string sm3s = match3.suffix();
						printf("Url Param Pre Match\n");
						std::regex re4("[^/]*");
						std::smatch match4;
						auto bOk4 = std::regex_search(sm3s, match4, re4);
						if (bOk4)
						{
							std::string sm40 = match4[0];
							std::string sm41 = match4[1];
							std::string sm4p = match4.prefix();
							std::string sm4s = match4.suffix();
							urlParams.push_back(sm40);
							if (sm4s.compare("/") == 0 || sm4s.size() == 0)
							{
								return true;
							}
							repl_subject.append(sm30);
							repl_subject.append(sm40);
							//repl_subject.append(siv3);
							repl_subject.append(siv21);
							
							subject.assign(repl_subject);
							//subject.assign(sm4s);
							printf("Url Param Pre Match 2\n");
						}
					}
				}
			}
			return false;
		}
	}
	return false;
}

*/

void HttpUrlRoute::AddRoute(char *url, void* lpFunc)
{
	ROUTEURL w = std::make_pair(url, lpFunc);
	urlRoutes.push_back(w);
}

void* HttpUrlRoute::GetRoute(char* url)
{
	if (urlRoutes.size() == 0)
		return NULL;

	for (ROUTEMAPITERATOR i = urlRoutes.begin(); i != urlRoutes.end(); ++i)
	{
		char* t_url = i->first;
		if (strcmp(url, t_url) == 0)
		{
			void* rv = i->second;
			return rv;
		}
	}
	return NULL;
}