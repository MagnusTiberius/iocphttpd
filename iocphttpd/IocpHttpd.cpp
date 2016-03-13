#include "IocpHttpd.h"


IocpHttpd::IocpHttpd()
{
	SetStatic("\/static\/*", "c:\\www\\static\\");

	AddRoute("/", IocpHttpd::HandleHome);
	AddRoute("/test", IocpHttpd::HandleTest);
	AddRoute("/init", IocpHttpd::HandleInit);
	AddRoute("/json/test", IocpHttpd::HandleJsonTest);
	AddRoute("/json/testtwo", IocpHttpd::HandleJsonTestTwo);
	AddRoute("/reset", IocpHttpd::HandleReset);

	//AddRoute("/user/profile/<id:[0-9]+>/", IocpHttpd::HandleJsonUrlParam1);
	AddRoute("/product/country/<id1:[0-9]+>/city/<id2:[0-9]+>/g.json", IocpHttpd::HandleJsonUrlParam1);
	AddRoute("/profile/<id:[A-Za-z0-9]+>/profile.html", IocpHttpd::HandleAutomobileProfile);
	AddRoute("/ws/profile/<id:[A-Za-z0-9]+>/profile.html", IocpHttpd::HandleAutomobileProfileJson);
	AddRoute("/ws/metadata/profile/<id:[A-Za-z0-9]+>/profile.html", IocpHttpd::HandleAutomobileMetaDataProfileJson);
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

void IocpHttpd::HandleReset(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	exit(1);
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

void IocpHttpd::HandleAutomobileProfile(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	printf("IocpHttpd::HandleAutomobileProfile\n");

	int n = httpRequest->urlParams.size();
	printf("urlParams size=%d \n", n);

	string v = httpRequest->urlParams[0];

	httpResponse->AddHeaderItem("AutomobileProfileName", v);

	if (httpResponse->FindTemplate("init") == false)
	{
		httpResponse->AddTemplate("init", "C:\\www\\automobile.html");
	}
	httpResponse->WriteTemplate("init");
}


void IocpHttpd::HandleAutomobileMetaDataProfileJson(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	printf("IocpHttpd::HandleAutomobileProfile\n");

	int n = httpRequest->urlParams.size();
	printf("urlParams size=%d \n", n);

	string profID = httpRequest->urlParams[0];

	string json_example = "{                                            \
		\"Versions\":[                                                  \
	{                                                                   \
		\"Version\":{                                                   \
			\"MSRP\":\"101,770\",                                        \
				\"Code\" : \"Premium\",                                       \
				\"HP\" : \"545\",                                       \
				\"mpgCity\" : \"16\",                                   \
				\"mpgHiway\" : \"23\",                                  \
				\"seats\" : \"4\",                                      \
				\"doors\" : \"2\",                                      \
				\"Features\" : [                                        \
					\"3.8-liter twin-turbo V6 engine\",                 \
						\"Dual clutch 6-speed transmission\",           \
						\"ATTESA E-TS® All-Wheel Drive\",               \
						\"20\\\" RAYS® wheels [*]\",                      \
						\"Nissan/Brembo® braking system [*]\"           \
				]                                                       \
		}                                                               \
	},                                                                  \
	{                                                                   \
		\"Version\":{                                                      \
			\"MSRP\":\"102,770\",                                        \
				\"Code\" : \"Gold\",                                       \
				\"HP\" : \"545\",                                       \
				\"mpgCity\" : \"16\",                                   \
				\"mpgHiway\" : \"23\",                                  \
				\"seats\" : \"4\",                                      \
				\"doors\" : \"2\"                                       \
		},                                                              \
		\"Features\":[                                                  \
			\"3.8-liter twin-turbo V6 engine\",                         \
				\"Dual clutch 6-speed transmission\",                   \
				\"ATTESA E-TS® All-Wheel Drive\",                       \
				\"20\\\" RAYS® wheels [*]\",                              \
				\"Nissan/Brembo® braking system [*]\"                   \
		]                                                               \
	},                                                                  \
	{                                                                   \
		\"Version\":{                                                     \
			\"MSRP\":\"110,510\",                                        \
				\"Code\" : \"Black\",                                       \
				\"HP\" : \"545\",                                       \
				\"mpgCity\" : \"16\",                                   \
				\"mpgHiway\" : \"23\",                                  \
				\"seats\" : \"4\",                                      \
				\"doors\" : \"2\"                                       \
		},                                                              \
		\"Features\":[                                                  \
			\"Includes Premium features plus:\",                        \
				\"Dry carbon-fiber rear spoiler [*]\",                  \
				\"20\\\" Special dark-finished RAYS® wheels [*]\",        \
				\"Black/Red Recaro® front seats [*]\"                   \
		]                                                               \
	}                                                                   \
		]                                                               \
}";

	httpResponse->AddHeaderItem("AutomobileProfileName", profID);

	Json::Value root;
	Json::Reader reader;
	bool parsedSuccess = reader.parse(json_example, root, false);
	if (parsedSuccess)
	{
		std::string s = root.toStyledString();
		httpResponse->Write(s.c_str());
		httpResponse->SetContentType("application/json");
	}
	else
	{
		httpResponse->Write(json_example.c_str());
	}
}


void IocpHttpd::HandleAutomobileProfileJson(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	printf("IocpHttpd::HandleAutomobileProfile\n");

	int n = httpRequest->urlParams.size();
	printf("urlParams size=%d \n", n);

	string profID = httpRequest->urlParams[0];

	string json_example = "{\"profileImages\": \
							[{\"img\" :\"\\\\static\\\\profile\\\\" + profID + "\\\\img\\\\0001.jpg\", \"itm\" :\"active\" }, \
							 {\"img\" :\"\\\\static\\\\profile\\\\" + profID + "\\\\img\\\\0002.jpg\", \"itm\" :\"\" }, \
							 {\"img\" :\"\\\\static\\\\profile\\\\" + profID + "\\\\img\\\\0003.jpg\", \"itm\" :\"\" }, \
							 {\"img\" :\"\\\\static\\\\profile\\\\" + profID + "\\\\img\\\\0004.jpg\", \"itm\" :\"\" }, \
							 {\"img\" :\"\\\\static\\\\profile\\\\" + profID + "\\\\img\\\\0005.jpg\", \"itm\" :\"\" }, \
							 {\"img\" :\"\\\\static\\\\profile\\\\" + profID + "\\\\img\\\\0006.jpg\", \"itm\" :\"\" }, \
							 {\"img\" :\"\\\\static\\\\profile\\\\" + profID + "\\\\img\\\\0007.jpg\", \"itm\" :\"\" }, \
							 {\"img\" :\"\\\\static\\\\profile\\\\" + profID + "\\\\img\\\\0008.jpg\", \"itm\" :\"\" }, \
							 {\"img\" :\"\\\\static\\\\profile\\\\" + profID + "\\\\img\\\\0009.jpg\", \"itm\" :\"\" }, \
							 {\"img\" :\"\\\\static\\\\profile\\\\" + profID + "\\\\img\\\\0010.jpg\", \"itm\" :\"\" }], \
							\"profileID\": \"" + profID + "\", \
							\"dir\": \"\\\\static\\\\profile\\\\" + profID + "\\\\img\\\\\" \
							}";

	httpResponse->AddHeaderItem("AutomobileProfileName", profID);

	Json::Value root;
	Json::Reader reader;
	bool parsedSuccess = reader.parse(json_example, root, false);
	if (parsedSuccess)
	{
		std::string s = root.toStyledString();
		httpResponse->Write(s.c_str());
		httpResponse->SetContentType("application/json");
	}
	else
	{
		httpResponse->Write(json_example.c_str());
	}
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

	//string json_example = "{\"array\": \
	//						[\"\static\profile\lancerevo\img0001.jpg\", \
	//						 \"\static\profile\lancerevo\img0002.jpg\", \
	//						 \"\static\profile\lancerevo\img0003.jpg\", \
	//						 \"\static\profile\lancerevo\img0004.jpg\", \
	//						 \"\static\profile\lancerevo\img0005.jpg\", \
	//						 \"\static\profile\lancerevo\img0006.jpg\", \
	//						 \"\static\profile\lancerevo\img0007.jpg\", \
	//						 \"\static\profile\lancerevo\img0008.jpg\", \
	//						 \"\static\profile\lancerevo\img0009.jpg\", \
	//						 \"\static\profile\lancerevo\img0010.jpg\"], \
	//						\"profileID\": \
	//						\"lancerevo\" \
	//						}";


	string json_example = "{\"profileImages\": \
							[{\"img\" :\"0001.jpg\", \"itm\" :\"active\" }, \
							 {\"img\" :\"0002.jpg\", \"itm\" :\"\" }, \
							 {\"img\" :\"0003.jpg\", \"itm\" :\"\" }, \
							 {\"img\" :\"0004.jpg\", \"itm\" :\"\" }, \
							 {\"img\" :\"0005.jpg\", \"itm\" :\"\" }, \
							 {\"img\" :\"0006.jpg\", \"itm\" :\"\" }, \
							 {\"img\" :\"0007.jpg\", \"itm\" :\"\" }, \
							 {\"img\" :\"0008.jpg\", \"itm\" :\"\" }, \
							 {\"img\" :\"0009.jpg\", \"itm\" :\"\" }, \
							 {\"img\" :\"0010.jpg\", \"itm\" :\"\" }], \
							\"profileID\": \"lancerevo\", \
							\"dir\": \"\\\\static\\\\profile\\\\lancerevo\\\\ \" \
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
	else
	{
		httpResponse->Write(json_example.c_str());
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
