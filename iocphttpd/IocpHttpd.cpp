#include "IocpHttpd.h"


IocpHttpd::IocpHttpd()
{
	AddRoute("/", IocpHttpd::HandleHome);
	AddRoute("/test", IocpHttpd::HandleTest);
	AddRoute("/init", IocpHttpd::HandleInit);

}


IocpHttpd::~IocpHttpd()
{
}

void IocpHttpd::EvalGet(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	printf("IocpHttpd::EvalGet\n");
}

void IocpHttpd::EvalPost(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	printf("IocpHttpd::EvalPost\n");
}

void IocpHttpd::HandleHome(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	printf("IocpHttpd::HandleHome\n");
	if (httpResponse->FindTemplate("Home") == false)
	{
		httpResponse->AddTemplate("Home", "C:\\www\\index.html");
	}
	httpResponse->WriteTemplate("Home");
}

void IocpHttpd::HandleTest(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	printf("IocpHttpd::HandleTest\n");
	if (httpResponse->FindTemplate("Home") == false)
	{
		httpResponse->AddTemplate("Home", "C:\\www\\index.html");
	}
	httpResponse->WriteTemplate("Home");
}

void IocpHttpd::HandleInit(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	printf("IocpHttpd::HandleInit\n");
	if (httpResponse->FindTemplate("init") == false)
	{
		httpResponse->AddTemplate("init", "C:\\www\\init.html");
	}
	httpResponse->WriteTemplate("init");
}
