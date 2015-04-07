#pragma once
#include "stdafx.h"



typedef struct header {
	char szMethodAction[BUFSIZMIN];
	char szUrl[BUFSIZMAX];
	char szUrlParams[BUFSIZMAX];
	char szHttpVersion[BUFSIZMIN];
} Header, *lpHeader;


typedef struct Content {
	char szContent[BUFSIZMAX];
} Content, *lpContent;


typedef struct httpdoc {
	Header header;
	Content content;
} Httpdoc, *lpHttpdoc;

Httpdoc g_Httpdoc;

void SetUrl(char* v, lpHttpdoc doc)
{
	DWORD dwThreadId = GetCurrentThreadId();
	memset(doc->header.szUrl, '\0', BUFSIZMAX);
	sprintf_s(doc->header.szUrl, BUFSIZMAX, "%s", v);
	fprintf_s(stderr, "%d::SetUrl", dwThreadId);
}

void SetUrlParams(char* v, lpHttpdoc doc)
{
	DWORD dwThreadId = GetCurrentThreadId();
	memset(doc->header.szUrlParams, '\0', BUFSIZMAX);
	sprintf_s(doc->header.szUrlParams, BUFSIZMAX, "%s", v);
	fprintf_s(stderr, "%d::SetUrlParams", dwThreadId);
}

void SetMethodVersion(char* v, lpHttpdoc doc)
{
	DWORD dwThreadId = GetCurrentThreadId();
	memset(doc->header.szMethodAction, '\0', BUFSIZMAX);
	sprintf_s(doc->header.szMethodAction, BUFSIZMAX, "%s", v);
	fprintf_s(stderr, "%d::SetMethodVersion", dwThreadId);
}

void SetMethodAction(char* v, lpHttpdoc doc)
{
	DWORD dwThreadId = GetCurrentThreadId();
	memset(doc->header.szMethodAction, '\0', BUFSIZMAX);
	sprintf_s(doc->header.szMethodAction, BUFSIZMAX, "%s", v);
	fprintf_s(stderr, "%d::szMethodAction", dwThreadId);
}

