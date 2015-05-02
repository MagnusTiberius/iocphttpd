#include "IocpHttpd.h"


IocpHttpd::IocpHttpd()
{
	SetStatic("\/static\/*", "C:\\wwwmelonJS-2.0.x\\examples\\platformer\\static\\");
	SetHomeDir("C:\\wwwmelonJS-2.0.x\\examples\\platformer\\");

	AddRoute("/", IocpHttpd::HandleHome);
	//AddRoute("/test", IocpHttpd::HandleTest);
	//AddRoute("/init", IocpHttpd::HandleInit);
	//AddRoute("/json/test", IocpHttpd::HandleJsonTest);
	//AddRoute("/json/testtwo", IocpHttpd::HandleJsonTestTwo);

	//AddRoute("/user/profile/<id:[0-9]+>/", IocpHttpd::HandleJsonUrlParam1);
	AddRoute("/product/country/<id1:[0-9]+>/city/<id2:[0-9]+>/g.json", IocpHttpd::HandleJsonUrlParam1);
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
		httpResponse->AddTemplate("Home", "C:\\wwwmelonJS-2.0.x\\examples\\platformer\\index.html");
	}
	httpResponse->WriteTemplate("Home");
}

void IocpHttpd::HandleTest(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	printf("IocpHttpd::HandleTest\n");
	if (httpResponse->FindTemplate("Test") == false)
	{
		httpResponse->AddTemplate("Test", "C:\\www\\test.html");
	}
	httpResponse->WriteTemplate("Test");
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

	const char *content = httpRequest->GetContent();
	std::string scontent(content);

	Json::Value root;
	Json::Reader reader;
	bool parsedSuccess = reader.parse(scontent, root, false);
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

void IocpHttpd::HandleJsonUrlParam1(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	printf("IocpHttpd::HandleJsonUrlParam1\n");

	int n = httpRequest->urlParams.size();
	printf("urlParams size=%d \n", n);

	string json_example1 = "{\"name1\":";
	json_example1.append("\"");
	auto v = httpRequest->urlParams[0];
	json_example1.append(v);
	json_example1.append("\"");
	json_example1.append("}");

	Json::Value root;
	Json::Reader reader;
	bool parsedSuccess = reader.parse(json_example1, root, false);
	if (parsedSuccess)
	{
		std::string s = root.toStyledString();
		httpResponse->Write(s.c_str());
		httpResponse->SetContentType("application/json");
	}


}
