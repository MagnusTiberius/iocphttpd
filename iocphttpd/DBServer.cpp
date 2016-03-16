#include "DBServer.h"


DBServer* DBServer::_instance = 0;

DBServer* DBServer::Instance() {
	if (_instance == 0) {
		_instance = new DBServer();
	}
	return _instance;
}

DBServer::~DBServer()
{
}


DBServer::DBServer()
{
	nThreads = THREAD_COUNT;
	connectID = 1;
	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);
	dwCtr = 0;
	ghHasMessageEvent = CreateEvent(NULL, TRUE, FALSE, TEXT("DBServer"));
}

int DBServer::Start(HANDLE* hList)
{
	std::vector<std::string> indices;
	indices.push_back(DATADBNAME);
	db = DatabaseDemo::DemoDB::Open("C:\\temp\\demodb", indices);

	//HANDLE hList[THREAD_COUNT];
	isLooping = true;
	dwThreadId = GetCurrentThreadId();
	for (int i = 0; i < THREAD_COUNT; i++)
	{
		if ((hList[i] = CreateThread(NULL, 0, ServerWorkerThread, this, 0, &ThreadID)) == NULL)
		{
			fprintf(stderr, "%d::CreateThread() failed with error %d\n", dwThreadId, GetLastError());
			return 1;
		}
		else
			fprintf(stderr, "%d::CreateThread() is OK!\n", dwThreadId);

		//CloseHandle(ThreadHandle);
	}
	return 1;
}

int DBServer::GetThreadNum()
{
	return THREAD_COUNT;
}

void DBServer::Stop()
{
	isLooping = false;
}

int DBServer::Connect()
{
	connectID++;
	return connectID;
}

void DBServer::InitData()
{
	string profID[5] = {"nissangtr","lancerevo","fordmustang","audia4titanium","subarubrz"};
	string value = "";
	int i = 0;
	int n = 0;
	for (i = 0; i < 5; i++)
	{
		string key = "/ws/metadata/profile/" + profID[i] + "/profile.html";
		key = "/ws/profile/" + profID[i] + "/profile.html";
		value = "{\"profileImages\":                                                                                   \
					[{\"img\" :\"\\\\static\\\\profile\\\\" + profID[i] + "\\\\img\\\\0001.jpg\", \"itm\" :\"active\" },     \
						{\"img\" :\"\\\\static\\\\profile\\\\" + profID[i] + "\\\\img\\\\0002.jpg\", \"itm\" :\"\" },        \
						{\"img\" :\"\\\\static\\\\profile\\\\" + profID[i] + "\\\\img\\\\0003.jpg\", \"itm\" :\"\" },        \
						{\"img\" :\"\\\\static\\\\profile\\\\" + profID[i] + "\\\\img\\\\0004.jpg\", \"itm\" :\"\" },        \
						{\"img\" :\"\\\\static\\\\profile\\\\" + profID[i] + "\\\\img\\\\0005.jpg\", \"itm\" :\"\" },        \
						{\"img\" :\"\\\\static\\\\profile\\\\" + profID[i] + "\\\\img\\\\0006.jpg\", \"itm\" :\"\" },        \
						{\"img\" :\"\\\\static\\\\profile\\\\" + profID[i] + "\\\\img\\\\0007.jpg\", \"itm\" :\"\" },        \
						{\"img\" :\"\\\\static\\\\profile\\\\" + profID[i] + "\\\\img\\\\0008.jpg\", \"itm\" :\"\" },        \
						{\"img\" :\"\\\\static\\\\profile\\\\" + profID[i] + "\\\\img\\\\0009.jpg\", \"itm\" :\"\" },        \
						{\"img\" :\"\\\\static\\\\profile\\\\" + profID[i] + "\\\\img\\\\0010.jpg\", \"itm\" :\"\" }],       \
					\"profileID\": \"" + profID[i] + "\",                                                                    \
					\"dir\": \"\\\\static\\\\profile\\\\" + profID[i] + "\\\\img\\\\\"                                       \
					}";
		db->Add(DATADBNAME, key, value);
		string v = GetValue(key);
		// ---------------------------------------

		n = i;
		key = "/ws/metadata/profile/" + profID[i] + "/profile.html";
		if (n == 0)
		{
			value = "{                                            \
				\"Versions\":[                                                  \
			{                                                                   \
				\"Version\":{                                                   \
					\"MSRP\":\"101,770\",                                       \
						\"Code\" : \"Premium\",                                 \
						\"HP\" : \"545\",                                       \
						\"mpgCity\" : \"16\",                                   \
						\"mpgHiway\" : \"23\",                                  \
						\"seats\" : \"4\",                                      \
						\"doors\" : \"2\",                                      \
						\"Features\" : [                                        \
							\"3.8-liter twin-turbo V6 engine\",                 \
								\"Dual clutch 6-speed transmission\",           \
								\"ATTESA E-TS® All-Wheel Drive\",               \
								\"20\\\" RAYS® wheels [*]\",                    \
								\"Nissan/Brembo® braking system [*]\"           \
						]                                                       \
				}                                                               \
			},                                                                  \
			{                                                                   \
				\"Version\":{                                                   \
					\"MSRP\":\"102,770\",                                       \
						\"Code\" : \"Gold\",                                    \
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
						\"20\\\" RAYS® wheels [*]\",                            \
						\"Nissan/Brembo® braking system [*]\"                   \
				]                                                               \
			},                                                                  \
			{                                                                   \
				\"Version\":{                                                   \
					\"MSRP\":\"110,510\",                                       \
						\"Code\" : \"Black\",                                   \
						\"HP\" : \"545\",                                       \
						\"mpgCity\" : \"16\",                                   \
						\"mpgHiway\" : \"23\",                                  \
						\"seats\" : \"4\",                                      \
						\"doors\" : \"2\"                                       \
				},                                                              \
				\"Features\":[                                                  \
					\"Includes Premium features plus:\",                        \
						\"Dry carbon-fiber rear spoiler [*]\",                  \
						\"20\\\" Special dark-finished RAYS® wheels [*]\",      \
						\"Black/Red Recaro® front seats [*]\"                   \
				]                                                               \
			}                                                                   \
				]                                                               \
			}";
		}
		else if (n == 1)
		{
			value = "{                                            \
				\"Versions\":[                                                  \
			{                                                                   \
				\"Version\":{                                                   \
					\"MSRP\":\"34,495\",                                       \
						\"Code\" : \"GSR\",                                 \
						\"HP\" : \"291\",                                       \
						\"mpgCity\" : \"17\",                                   \
						\"mpgHiway\" : \"23\",                                  \
						\"seats\" : \"4\",                                      \
						\"doors\" : \"4\",                                      \
						\"Features\" : [                                        \
							\"291-horsepower turbocharged 2.0-liter DOHC MIVEC engine\",                 \
								\"Super-All Wheel Control\",           \
								\"18x8.5\\\" Enkei® alloy wheel with 245/40 R18 93W Yokohama® ADVAN® high-performance tires\",               \
								\"BREMBO® high performance braking system\",                    \
								\"Active Yaw Control (AYC)\"           \
						]                                                       \
				}                                                               \
			},                                                                  \
			{                                                                   \
				\"Version\":{                                                   \
					\"MSRP\":\"38,995\",                                       \
						\"Code\" : \"MR\",                                 \
						\"HP\" : \"291\",                                       \
						\"mpgCity\" : \"17\",                                   \
						\"mpgHiway\" : \"23\",                                  \
						\"seats\" : \"4\",                                      \
						\"doors\" : \"4\",                                      \
						\"Features\" : [                                        \
							\"291-horsepower turbocharged 2.0-liter DOHC MIVEC engine\",                 \
								\"Super-All Wheel Control\",           \
								\"18x8.5\\\" Enkei® alloy wheel with 245/40 R18 93W Yokohama® ADVAN® high-performance tires\",               \
								\"BREMBO® high performance braking system\",                    \
								\"Active Yaw Control (AYC)\"           \
						]                                                       \
				}                                                               \
			},                                                                  \
			{                                                                   \
				\"Version\":{                                                   \
					\"MSRP\":\"37,995\",                                       \
						\"Code\" : \"FE\",                                 \
						\"HP\" : \"291\",                                       \
						\"mpgCity\" : \"17\",                                   \
						\"mpgHiway\" : \"23\",                                  \
						\"seats\" : \"4\",                                      \
						\"doors\" : \"4\",                                      \
						\"Features\" : [                                        \
							\"291-horsepower turbocharged 2.0-liter DOHC MIVEC engine\",                 \
								\"Super-All Wheel Control\",           \
								\"18x8.5\\\" Enkei® alloy wheel with 245/40 R18 93W Yokohama® ADVAN® high-performance tires\",               \
								\"Black headliner, pillars, sun visors, and assist handles\",           \
								\"Red accent stitching for front seats, steering wheel, shift knob, console lid, floor mats, and park brake handle.\",                    \
								\"Black painted aluminum roof panel\"           \
						]                                                       \
				}                                                               \
			}                                                                  \
                                                            \
				]                                                               \
			}";
		}
		else if (n == 2)
		{
			value = "{                                            \
				\"Versions\":[                                                  \
			{                                                                   \
				\"Version\":{                                                   \
					\"MSRP\":\"24,145\",                                       \
						\"Code\" : \"V6 Fastback\",                                 \
						\"HP\" : \"300\",                                       \
						\"mpgCity\" : \"17\",                                   \
						\"mpgHiway\" : \"28\",                                  \
						\"seats\" : \"4\",                                      \
						\"doors\" : \"2\",                                      \
						\"Features\" : [                                        \
							\"3.7L Ti-VCT V6 engine\",                 \
								\"6-speed manual transmission\",           \
								\"Rear view camera\",               \
								\"SYNC\",                    \
								\"Automatic headlamps with wiper activation\"           \
						]                                                       \
				}                                                               \
			},                                                                  \
			{                                                                   \
				\"Version\":{                                                   \
					\"MSRP\":\"25,645\",                                       \
						\"Code\" : \"EcoBoost Fastback\",                                 \
						\"HP\" : \"300\",                                       \
						\"mpgCity\" : \"22\",                                   \
						\"mpgHiway\" : \"31\",                                  \
						\"seats\" : \"4\",                                      \
						\"doors\" : \"4\",                                      \
						\"Features\" : [                                        \
							\"2.3L EcoBoost\",                 \
								\"RECARO® cloth front sport seats with manual adjustment (requires Ebony interior)\",           \
								\"6-speed manual transmission\",               \
								\"18\\\" Magnetic Gloss painted/machined aluminum wheels\",                    \
								\"Dual brigth exhaust with slashed cut tips\"           \
						]                                                       \
				}                                                               \
			},                                                                  \
			{                                                                   \
				\"Version\":{                                                   \
					\"MSRP\":\"29,645\",                                       \
						\"Code\" : \"V6 Convertible\",                                 \
						\"HP\" : \"300\",                                       \
						\"mpgCity\" : \"17\",                                   \
						\"mpgHiway\" : \"28\",                                  \
						\"seats\" : \"4\",                                      \
						\"doors\" : \"2\",                                      \
						\"Features\" : [                                        \
							\"3.7L Ti-VCT V6 engine\",                 \
								\"6-speed manual transmission\",           \
								\"Rear view camera\",               \
								\"SYNC\",                    \
								\"Automatic headlamps with wiper activation\"           \
						]                                                       \
				}                                                               \
			}                                                                  \
                                                            \
				]                                                               \
			}";
		}
		else if (n == 3)
		{
			value = "{                                            \
				\"Versions\":[                                                  \
			{                                                                   \
				\"Version\":{                                                   \
					\"MSRP\":\"37,300\",                                       \
						\"Code\" : \"Premium\",                                 \
						\"HP\" : \"252\",                                       \
						\"mpgCity\" : \"tbd\",                                   \
						\"mpgHiway\" : \"tbd\",                                  \
						\"seats\" : \"4\",                                      \
						\"doors\" : \"4\",                                      \
						\"Features\" : [                                        \
							\"2.0 TFSI\",                 \
								\"Xenon plus headlights with LED daytime running lights\",           \
								\"Three-zone automatic climate control\",               \
								\"Sunroof with sunshade, power tilt and slide features\",                    \
								\"Leather seating surfaces\"           \
						]                                                       \
				}                                                               \
			},                                                                  \
			{                                                                   \
				\"Version\":{                                                   \
					\"MSRP\":\"41,100\",                                       \
						\"Code\" : \"Premium Plus\",                                 \
						\"HP\" : \"252\",                                       \
						\"mpgCity\" : \"tbd\",                                   \
						\"mpgHiway\" : \"tbd\",                                  \
						\"seats\" : \"4\",                                      \
						\"doors\" : \"4\",                                      \
						\"Features\" : [                                        \
							\"2.0 TFSI\",                 \
								\"Full LED headlights\",           \
								\"Heated front seats\",               \
								\"S line® exterior appearance-front and rear bumpers, body-colored side sills, S line® fender badges, Matte Twilight Gray rear diffuser with honeycomb depositors\",                    \
								\"Bang & Olufsen® 3D Sound System with 19 speakers, 16-channel amplifier and 755 watts\"           \
						]                                                       \
				}                                                               \
			},                                                                  \
			{                                                                   \
				\"Version\":{                                                   \
					\"MSRP\":\"45,900\",                                       \
						\"Code\" : \"Prestige\",                                 \
						\"HP\" : \"252\",                                       \
						\"mpgCity\" : \"tbd\",                                   \
						\"mpgHiway\" : \"tbd\",                                  \
						\"seats\" : \"4\",                                      \
						\"doors\" : \"4\",                                      \
						\"Features\" : [                                        \
							\"2.0 TFSI\",                 \
								\"MMI® navigation plus with voice control system with 8.3” color display2\",           \
								\"Audi virtual cockpit (12.3\\\" LCD fully digital instrument cluster)\",               \
								\"MMI® touch with handwriting-recognition technology\",                    \
								\"Head-up display with navigation and assistance systems information\"           \
						]                                                       \
				}                                                               \
			}                                                                  \
                                                            \
				]                                                               \
			}";
		}
		else if (n == 4)
		{
			value = "{                                            \
				\"Versions\":[                                                  \
			{                                                                   \
				\"Version\":{                                                   \
					\"MSRP\":\"26,190\",                                       \
						\"Code\" : \"Premium\",                                 \
						\"HP\" : \"200\",                                       \
						\"mpgCity\" : \"22\",                                   \
						\"mpgHiway\" : \"30\",                                  \
						\"seats\" : \"4\",                                      \
						\"doors\" : \"2\",                                      \
						\"Features\" : [                                        \
							\"2.0-liter SUBARU BOXER® 4-cylinder\",                 \
								\"P215/45WR17 BSW Performance Tires\",           \
								\"6-speed manual transmission\",               \
								\"TORSEN® limited-slip rear differential\",                    \
								\"Vehicle Stability Control\"           \
						]                                                       \
				}                                                               \
			},                                                                  \
			{                                                                   \
				\"Version\":{                                                   \
					\"MSRP\":\"28,190\",                                       \
						\"Code\" : \"Limited\",                                 \
						\"HP\" : \"200\",                                       \
						\"mpgCity\" : \"22\",                                   \
						\"mpgHiway\" : \"30\",                                  \
						\"seats\" : \"4\",                                      \
						\"doors\" : \"2\",                                      \
						\"Features\" : [                                        \
							\"2.0-liter SUBARU BOXER® 4-cylinder\",                 \
								\"P215/45WR17 BSW Performance Tires\",           \
								\"Alcantara®/leather-trimmed upholstery\",               \
								\"Automatic transmission (option)\",                    \
								\"Keyless Access with Push-Button Start\"           \
						]                                                       \
				}                                                               \
			}                                                                  \
                                                            \
				]                                                               \
			}";
		}
		db->Add(DATADBNAME, key, value);
	}
	db->Commit();
}

void DBServer::UpdateDB(REQUEST* request)
{
	std::string v1 = db->Lookup(DATADBNAME, request->key);
	if (v1.size() == 0)
	{
		db->Add(DATADBNAME, request->key, request->value);
	}
	else
	{
		//db->Remove(DATADBNAME, buf);
		db->Add(DATADBNAME, request->key, request->value);
	}
	db->Commit();
}

DWORD WINAPI DBServer::ServerWorkerThread(LPVOID lpObject)
{
	DBServer* instance = (DBServer*)lpObject;
	while (instance->isLooping)
	{
		Sleep(300);
		::WaitForSingleObject(instance->ghHasMessageEvent, INFINITE);
		if (::WaitForSingleObject(instance->ghMutex, 10000) == WAIT_OBJECT_0)
		{
			instance->dwCtr++;
			if (instance->dwCtr >= LIMITCTR)
			{
				//instance->isLooping = false;
			}
			if (instance->requests.size() > 0)
			{
				REQUEST* req = instance->requests.top();
				if (req != NULL)
				{
					instance->requests.pop();
					instance->UpdateDB(req);
				}
			}

			if (instance->requests.size() == 0)
			{
				::ResetEvent(instance->ghHasMessageEvent);
			}
			::ReleaseMutex(instance->ghMutex);
		}

	}
	return 1;
}

void DBServer::Submit(REQUEST* request)
{
	requests.push(request);
	SetEvent(ghHasMessageEvent);
}

string DBServer::GetValue(string key)
{
	string value = db->Lookup(DATADBNAME, key);
	return value;
}
