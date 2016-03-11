#pragma once
#include "stdafx.h"
#include "SocketCompletionPortServer.h"
#include "GameEngine.h"

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
	static void HandleJsonUrlParamUpdate(HttpRequest *httpRequest, HttpResponse *httpResponse);
	static void HandleJsonUrlParamReplicateWorld(HttpRequest *httpRequest, HttpResponse *httpResponse);
	static void ReplicateWorld(HttpRequest *httpRequest, HttpResponse *httpResponse);
private:

	GameEngine* gameEngine;

};

