#pragma once
#include "stdafx.h"
#include <map>
#include <vector>
#include <regex>
#include "UrlParam.h"

class IOCPHTTPL_API HttpUrlRoute 
{
public:
	HttpUrlRoute();
	~HttpUrlRoute();

	typedef std::pair<char*, void*> ROUTEURL;
	typedef std::pair<char*, char*> STATICACCESS;
	typedef std::vector<ROUTEURL> ROUTEMAP;
	typedef std::vector<STATICACCESS> STATICACCESSMAP;
	typedef ROUTEMAP::iterator ROUTEMAPITERATOR;
	typedef STATICACCESSMAP::iterator STATICACCESSMAPITERATOR;

	void AddRoute(char *url, void* lpFunc);
	void* GetRoute(char* url);
	void SetStatic(char *code, char *path);
	bool IsStatic(char *path);
	string GetFullPath(char *path);
	char* MergePath(char *path1, char *path2);
	PTSTR  GetPathExtension(PTSTR pszPath);
	PTSTR  GetPathExtension(const char* pszPath);
	void SetContentTypeFromExtension();
	bool HasUrlParams(char *path);
	void* GetUrlParamHandler(char *path);
	void SetHomeDir(char *path);

protected:
	ROUTEMAP urlRoutes;
	STATICACCESSMAP staticAccessList;
	int GetLen(char *s);
	bool IsOverlapping(char *path1, char *path2, int pos);
	int GetMergePoint(char *path1, char *path2);
	std::wstring pathExtension;
	std::wstring contenType;

	std::vector<std::string> urlParams;

	UrlParam urlParam;
	void* urlParamHandler;
	std::string homeDir;
};

