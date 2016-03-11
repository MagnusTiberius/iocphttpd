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
	//AddRoute("/game/update.json", IocpHttpd::HandleJsonUrlParam1);
	AddRoute("/game/<id:[0-9a-zA-Z]+>/update.json", IocpHttpd::HandleJsonUrlParamUpdate);
	AddRoute("/game/<id:[0-9a-zA-Z]+>/replicateWorld.json", IocpHttpd::HandleJsonUrlParamReplicateWorld);

	gameEngine = new GameEngine();
	gameEngine->Start();
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

	int n = httpRequest->urlParams.size();
	printf("urlParams size=%d \n", n);


	string json_example = "{\"array\": \
		[\"elem1\", \
		\"elem2\"], \
		\"name1\": \
		\"value1\" \
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

void IocpHttpd::ReplicateWorld(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	printf("IocpHttpd::HandleJsonTestTwo\n");

	int n = httpRequest->urlParams.size();
	printf("urlParams size=%d \n", n);


	string json_example = "{\"array\": \
						  		[\"elem1\", \
										\"elem2\"], \
												\"name1\": \
														\"value1\" \
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


void IocpHttpd::HandleJsonUrlParamUpdate(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	printf("IocpHttpd::HandleJsonUrlParam1\n");

	int n = httpRequest->urlParams.size();
	printf("urlParams size=%d \n", n);

	const char* content = httpRequest->GetContent();

	string json_example1 = "{\"levelmapid\":";
	json_example1.append("\"");
	if (httpRequest->urlParams.size() > 0)
	{
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

}


void IocpHttpd::HandleJsonUrlParamReplicateWorld(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	printf("IocpHttpd::HandleJsonUrlParam1\n");

	int n = httpRequest->urlParams.size();
	printf("urlParams size=%d \n", n);

	string json_example1 = "{\"levelmapid\":";
	json_example1.append("\"");
	if (httpRequest->urlParams.size() > 0)
	{
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

}
