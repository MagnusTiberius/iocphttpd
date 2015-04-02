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
	httpResponse->Write("<html><h1>Test Home</h1><hr></html>");
}

void IocpHttpd::HandleTest(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	printf("IocpHttpd::HandleTest\n");
	httpResponse->Write("<html><h1>Test Test</h1><hr></html>");
}

void IocpHttpd::HandleInit(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	printf("IocpHttpd::HandleInit\n");
	httpResponse->Write("<html><h1>Test Init</h1><hr></html>");
}
