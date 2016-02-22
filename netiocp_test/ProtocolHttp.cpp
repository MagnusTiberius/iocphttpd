#include "stdafx.h"
#include "ProtocolHttp.h"


ProtocolHttp::ProtocolHttp()
{
	SetStatic("\/static\/*", "c:\\www\\static\\");

	AddRoute("/", ProtocolHttp::HandleHome);

	//AddRoute("/user/profile/<id:[0-9]+>/", IocpHttpd::HandleJsonUrlParam1);
	//AddRoute("/product/country/<id1:[0-9]+>/city/<id2:[0-9]+>/g.json", IocpHttpd::HandleJsonUrlParam1);
}


ProtocolHttp::~ProtocolHttp()
{
}

void ProtocolHttp::HandleHome(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	printf("IocpHttpd::HandleHome\n");
	if (httpResponse->FindTemplate("Home") == false)
	{
		httpResponse->AddTemplate("Home", "C:\\www\\index.html");
	}
	httpResponse->WriteTemplate("Home");
}

void ProtocolHttp::HandleMessage(char* msg)
{
	char* s = msg;
}

void ProtocolHttp::HandleMessage(char* msg, char*& reply)
{
	char* s = msg;
	reply = strdup("I got the message.\n");
	httpRequest.Parse(msg);
	Dispatch(&httpRequest, &httpResponse);
	ULONG size;
	//reply = (char*)httpResponse.GetResponse2(&size);
}

bool ProtocolHttp::FileExist(const TCHAR *fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

void ProtocolHttp::EvalStatic(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	//EventLog eventLog;
	char msg[8192];
	try
	{
		char *str = httpRequest->GetUrl();
		string s = GetFullPath(str);

		ifstream file(s);
		if (file)
		{
			sprintf(msg, "Static %s\n", s.c_str());
			//eventLog.WriteEventLogEntry2(msg, EVENTLOG_ERROR_TYPE);
			httpResponse->SetStaticFileName(s);
			httpResponse->WriteStatic(s.c_str());
		}
		else
		{
			sprintf(msg, "Static %s not found\n", s.c_str());
			//eventLog.WriteEventLogEntry2(msg, EVENTLOG_ERROR_TYPE);
		}

	}
	catch (...)
	{
		printf("Exception in SocketCompletionPortServer::EvalStatic \n");
		exit(0);
	}
}

void ProtocolHttp::EvalHasUrlParams(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
}


void ProtocolHttp::UrlNotFound(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	fprintf(stderr, "SocketCompletionPortServer::UrlNotFound: \n");
}

void ProtocolHttp::EvalGet(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	fprintf(stderr, "SocketCompletionPortServer::EvalGet\n");
}

void ProtocolHttp::EvalPost(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	fprintf(stderr, "SocketCompletionPortServer::EvalPost\n");
}

void ProtocolHttp::Dispatch(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	//EventLog eventLog;
	char msg[8192];

	sprintf(msg, "URL: %s\n", httpRequest->GetUrl());
	//eventLog.WriteEventLogEntry2(msg, EVENTLOG_ERROR_TYPE);

	if (httpRequest->GetMethod() == MethodType::HTTP_GET)
	{
		EvalGet(httpRequest, httpResponse);
	}
	if (httpRequest->GetMethod() == MethodType::HTTP_POST)
	{
		EvalPost(httpRequest, httpResponse);
	}

	if (IsStatic(httpRequest->GetUrl()))
	{
		printf("Static Directory\n");
		EvalStatic(httpRequest, httpResponse);
		return;
	}

	if (HasUrlParams(httpRequest->GetUrl()))
	{
		printf("Has URL Params\n");
		LPSTATICFUNC lpFunc = (LPSTATICFUNC)GetUrlParamHandler(httpRequest->GetUrl());
		httpRequest->urlParams.assign(urlParams.begin(), urlParams.end());
		if (lpFunc != NULL)
		{
			(*lpFunc)(httpRequest, httpResponse);
		}
		else
		{
			UrlNotFound(httpRequest, httpResponse);
		}
		return;
	}


	if (httpRequest->GetUrl() != NULL)
	{
		LPSTATICFUNC lpFunc = (LPSTATICFUNC)GetRoute(httpRequest->GetUrl());
		if (lpFunc != NULL)
		{
			(*lpFunc)(httpRequest, httpResponse);
		}
		else
		{
			WCHAR  buffer[BUFSIZMIN];
			char *url = httpRequest->GetUrl();
			std::string surl;
			surl.append("./");
			surl.append(url);
			std::wstring wsurl(surl.begin(), surl.end());
			DWORD dwres = GetFullPathName(wsurl.c_str(), BUFSIZMIN, buffer, NULL);
			if (dwres > 0)
			{
				PTSTR str = GetPathExtension(buffer);
				std::wstring wsbuffer(buffer);
				std::string sbuffer(wsbuffer.begin(), wsbuffer.end());
				if (FileExist(wsbuffer.c_str()))
				{
					httpResponse->SetStaticFileName(sbuffer.c_str());
					httpResponse->WriteStatic(sbuffer.c_str());
					return;
				}
				else
				{
					printf("File not found: %s \n", sbuffer.c_str());
				}
			}
			UrlNotFound(httpRequest, httpResponse);
		}
	}
}
