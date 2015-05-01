#pragma once
#include "stdafx.h"




	typedef struct header {
		char szMethodAction[BUFSIZMAX];
		char szHeaderUrl[BUFSIZMAX];
		char szUrlParams[BUFSIZMAX];
		char szHttpVersion[BUFSIZMIN];
		char *pszHeaderUrl;
		char *pszMethodAction;
		char *pszHttpVersion;
	} Header, *lpHeader;


	typedef struct Content {
		char szContent[BUFSIZMAX];
	} Content, *lpContent;


	typedef struct httpdoc {
		Header header;
		Content content;
		DWORD threadId;
	} Httpdoc, *lpHttpdoc;

	Httpdoc g_Httpdoc;

	void SetUrl(char* v, lpHttpdoc doc)
	{
		DWORD dwThreadId = GetCurrentThreadId();
		doc->threadId = dwThreadId;
		printf("%d::SetUrl\n", dwThreadId);
		memset(doc->header.szHeaderUrl, '\0', BUFSIZMAX);
		//memset(g_Httpdoc.header.szHeaderUrl, '\0', BUFSIZMAX);
		size_t siz = strlen(v) + 1;
		sprintf_s(doc->header.szHeaderUrl, siz, "%s", v);
		//sprintf_s(g_Httpdoc.header.szHeaderUrl, siz, "%s", v);
		doc->header.pszHeaderUrl = _strdup(v);
		//fprintf_s(stderr, "%d::SetUrl", dwThreadId);
	}

	void SetUrlParams(char* v, lpHttpdoc doc)
	{
		DWORD dwThreadId = GetCurrentThreadId();
		doc->threadId = dwThreadId;
		memset(doc->header.szUrlParams, '\0', BUFSIZMAX);
		size_t siz = strlen(v) + 1;
		sprintf_s(doc->header.szUrlParams, siz, "%s", v);
		//fprintf_s(stderr, "%d::SetUrlParams", dwThreadId);
	}

	void SetMethodVersion(char* v, lpHttpdoc doc)
	{
		DWORD dwThreadId = GetCurrentThreadId();
		doc->threadId = dwThreadId;
		memset(doc->header.szHttpVersion, '\0', BUFSIZMIN);
		size_t siz = strlen(v) + 1;
		sprintf_s(doc->header.szHttpVersion, siz, "%s", v);
		doc->header.pszHttpVersion = _strdup(v);
		//fprintf_s(stderr, "%d::SetMethodVersion", dwThreadId);
	}

	void SetMethodAction(char* v, lpHttpdoc doc)
	{
		DWORD dwThreadId = GetCurrentThreadId();
		doc->threadId = dwThreadId;
		memset(doc->header.szMethodAction, '\0', BUFSIZMAX);
		sprintf_s(doc->header.szMethodAction, BUFSIZMAX, "%s", v);
		doc->header.pszMethodAction = _strdup(v);
		//fprintf_s(stderr, "%d::szMethodAction", dwThreadId);
	}

	int GetHttpUrl(char *action, size_t size)
	{
		DWORD dwThreadId = GetCurrentThreadId();
		//printf("%d::GetHttpUrl\n", dwThreadId);
		sprintf_s(action, size, "%s", g_Httpdoc.header.pszHeaderUrl);
		return 1;
	}

	int GetHttpMethod(char *v, size_t size)
	{
		sprintf_s(v, size, "%s", g_Httpdoc.header.pszMethodAction);
		return 1;
	}

