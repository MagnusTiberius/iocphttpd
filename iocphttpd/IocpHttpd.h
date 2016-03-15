#pragma once
#include "stdafx.h"
#include "SocketCompletionPortServer.h"
#include "DBServer.h"

using namespace IOCPHTTPL;

class IocpHttpd : public SocketCompletionPortServer
{
public:
	IocpHttpd();
	~IocpHttpd();

	void EvalGet(HttpRequest *httpRequest, HttpResponse *httpResponse);
	void EvalPost(HttpRequest *httpRequest, HttpResponse *httpResponse);

	static void HandleHome(HttpRequest *httpRequest, HttpResponse *httpResponse);
	static void HandleTest(HttpRequest *httpRequest, HttpResponse *httpResponse);
	static void HandleInit(HttpRequest *httpRequest, HttpResponse *httpResponse);
	static void HandleJsonTest(HttpRequest *httpRequest, HttpResponse *httpResponse);
	static void HandleJsonTestTwo(HttpRequest *httpRequest, HttpResponse *httpResponse);
	static void HandleJsonUrlParam1(HttpRequest *httpRequest, HttpResponse *httpResponse);
	static void HandleReset(HttpRequest *httpRequest, HttpResponse *httpResponse);
	static void HandleAutomobileProfile(HttpRequest *httpRequest, HttpResponse *httpResponse);
	static void HandleAutomobileProfileJson(HttpRequest *httpRequest, HttpResponse *httpResponse);
	static void HandleAutomobileMetaDataProfileJson(HttpRequest *httpRequest, HttpResponse *httpResponse);

	void AddDB(DBServer* db);

private:
	DBServer* dbServer;
};

