#include "IocpHttpd.h"


IocpHttpd::IocpHttpd()
{
	AddRoute("/", IocpHttpd::HandleHome);
	AddRoute("/test", IocpHttpd::HandleTest);
	AddRoute("/init", IocpHttpd::HandleInit);
	AddRoute("/json/test", IocpHttpd::HandleJsonTest);
	AddRoute("/json/testtwo", IocpHttpd::HandleJsonTestTwo);
	SetStatic("\/static\/*", "c:\\www\\static\\");
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

void IocpHttpd::HandleJsonTest(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	printf("IocpHttpd::HandleJsonTest\n");

	string json_example = "{\"array\": \
							[\"item1\", \
							\"item2\"], \
							\"not an array\": \
							\"asdf\" \
							}";

	Json::Value root;
	Json::Reader reader;
	bool parsedSuccess = reader.parse(json_example, root, false);
	if (parsedSuccess)
	{
		std::string s = root.toStyledString();
		httpResponse->Write(s.c_str());
		httpResponse->SetContentType("application/json");
	}
}

void IocpHttpd::HandleJsonTestTwo(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	printf("IocpHttpd::HandleJsonTestTwo\n");

	string json_example = "{\"array\": \
							[\"item1\", \
							\"item2\"], \
							\"not an array\": \
							\"asdf\" \
							}";

	Json::Value root;
	Json::Reader reader;
	bool parsedSuccess = reader.parse(json_example, root, false);
	if (parsedSuccess)
	{
		std::string s = root.toStyledString();
		httpResponse->Write(s.c_str());
		httpResponse->SetContentType("application/json");
	}
}
