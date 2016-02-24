#pragma once
#include "stdafx.h"
#include "IProtocolHandler.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpUrlRoute.h"

using namespace NETIOCP;

class ProtocolHttp : public HttpUrlRoute, public IProtocolHandler
{
public:
	ProtocolHttp();
	~ProtocolHttp();

	void HandleMessage(char* msg);
	void HandleMessage(char* msg, char*& reply);

	typedef /*static*/ void(*LPSTATICFUNC)(HttpRequest *httpRequest, HttpResponse *httpResponse);

private:
	HttpRequest httpRequest;
	HttpResponse httpResponse;

	void Dispatch(HttpRequest *httpRequest, HttpResponse *httpResponse);
	void EvalGet(HttpRequest *httpRequest, HttpResponse *httpResponse);
	void EvalPost(HttpRequest *httpRequest, HttpResponse *httpResponse);
	bool FileExist(const TCHAR *fileName);
	void EvalStatic(HttpRequest *httpRequest, HttpResponse *httpResponse);
	void EvalHasUrlParams(HttpRequest *httpRequest, HttpResponse *httpResponse);
	void UrlNotFound(HttpRequest *httpRequest, HttpResponse *httpResponse);

protected:
	static void HandleHome(HttpRequest *httpRequest, HttpResponse *httpResponse);
};

