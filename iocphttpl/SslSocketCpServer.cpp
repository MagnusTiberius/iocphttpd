#include "SslSocketCpServer.h"


SslSocketCpServer::SslSocketCpServer()
{
	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);

	IsExternalCert = false;

	hCS = 0;
	hCred.dwLower = 0;
	hCred.dwUpper = 0;
	hCtx.dwLower = 0;
	hCtx.dwUpper = 0;

}


SslSocketCpServer::~SslSocketCpServer()
{
	::CloseHandle(ghMutex);
}

SslSocketCpServer::SslSocketCpServer(int PortNum)
{
	m_PortNum = PortNum;
}

void SslSocketCpServer::Start(int PortNum)
{
	m_PortNum = PortNum;
	Start();
}

int SslSocketCpServer::Start()
{
	SOCKADDR_IN InternetAddr;
	SOCKET Listen;
	HANDLE ThreadHandle;
	SOCKET Accept;

	SYSTEM_INFO SystemInfo;
	LPPER_HANDLE_DATA PerHandleData;
	LPPER_IO_OPERATION_DATA PerIoData;
	int i;
	DWORD RecvBytes;
	DWORD Flags;
	DWORD ThreadID;
	WSADATA wsaData;
	DWORD Ret;
	DWORD dwThreadId = GetCurrentThreadId();

	if ((Ret = WSAStartup((2, 2), &wsaData)) != 0)
	{
		fprintf(stderr, "%d::WSAStartup() failed with error %d\n", dwThreadId, Ret);
		return 1;
	}
	else
		fprintf(stderr, "%d::WSAStartup() is OK!\n", dwThreadId);


	if ((CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL)
	{
		fprintf(stderr, "%d::CreateIoCompletionPort() failed with error %d\n", dwThreadId, GetLastError());
		return 1;
	}
	else
		fprintf(stderr, "%d::CreateIoCompletionPort() is damn OK!\n", dwThreadId);


	GetSystemInfo(&SystemInfo);

	int nThreads = (int)SystemInfo.dwNumberOfProcessors * 2;

	nThreads = 2;

	//nThreads = (nThreads / 2);


	for (i = 0; i < nThreads; i++)
	{

		if ((ThreadHandle = CreateThread(NULL, 0, ServerWorkerThread, this, 0, &ThreadID)) == NULL)
		{
			fprintf(stderr, "%d::CreateThread() failed with error %d\n", dwThreadId, GetLastError());
			return 1;
		}
		else
			fprintf(stderr, "%d::CreateThread() is OK!\n", dwThreadId);

		CloseHandle(ThreadHandle);
	}


	if ((Listen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		fprintf(stderr, "%d::WSASocket() failed with error %d\n", dwThreadId, WSAGetLastError());
		return 1;
	}
	else
		fprintf(stderr, "%d::WSASocket() is OK!\n", dwThreadId);

	InternetAddr.sin_family = AF_INET;
	InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	InternetAddr.sin_port = htons(m_PortNum);

	int bRes = bind(Listen, (PSOCKADDR)&InternetAddr, sizeof(InternetAddr));
	while (bRes == SOCKET_ERROR)
	{
		fprintf(stderr, "%d::bind() failed with error %d\nLooking for next port...\n", dwThreadId, WSAGetLastError());
		InternetAddr.sin_port = htons(++m_PortNum);
		bRes = bind(Listen, (PSOCKADDR)&InternetAddr, sizeof(InternetAddr));
	}
	fprintf(stderr, "%d::bind() is fine! Port number at  %d\n", dwThreadId, m_PortNum);

	// Prepare socket for listening
	if (listen(Listen, 5) == SOCKET_ERROR)
	{
		fprintf(stderr, "%d::listen() failed with error %d\n", dwThreadId, WSAGetLastError());
		return 1;
	}
	else
		fprintf(stderr, "%d::listen() is working...\n", dwThreadId);

	if (ServerInit(true) == -1)
	{
		printf("Server Init fail \n");
		exit(1);
	}


	BOOL isOK = true;
	while (isOK)
	{
		if ((Accept = WSAAccept(Listen, NULL, NULL, NULL, 0)) == SOCKET_ERROR)
		{
			fprintf(stderr, "%d::WSAAccept() failed with error %d\n", dwThreadId, WSAGetLastError());
			isOK = false;

			exit(1);
			continue;
		}
		else
			fprintf(stderr, "%d::WSAAccept() looks fine!\n", dwThreadId);


		if ((PerHandleData = (LPPER_HANDLE_DATA)GlobalAlloc(GPTR, sizeof(PER_HANDLE_DATA))) == NULL)
			fprintf(stderr, "%d::GlobalAlloc() failed with error %d\n", dwThreadId, GetLastError());
		else
			fprintf(stderr, "%d::GlobalAlloc() for LPPER_HANDLE_DATA is OK!\n", dwThreadId);


		fprintf(stderr, "%d::Socket number %d got connected...\n", dwThreadId, Accept);
		PerHandleData->Socket = Accept;

		if (CreateIoCompletionPort((HANDLE)Accept, CompletionPort, (DWORD)PerHandleData, 0) == NULL)
		{
			fprintf(stderr, "%d::CreateIoCompletionPort() failed with error %d\n", dwThreadId, GetLastError());

			exit(1);
			isOK = false;
			continue;
		}
		else
			fprintf(stderr, "%d::CreateIoCompletionPort() is OK!\n", dwThreadId);


		if ((PerIoData = (LPPER_IO_OPERATION_DATA)GlobalAlloc(GPTR, sizeof(PER_IO_OPERATION_DATA))) == NULL)
		{
			fprintf(stderr, "%d::GlobalAlloc() failed with error %d\n", dwThreadId, GetLastError());

			exit(1);
			isOK = false;
			continue;
		}
		else
			fprintf(stderr, "%d::GlobalAlloc() for LPPER_IO_OPERATION_DATA is OK!\n", dwThreadId);

		ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
		PerIoData->BytesSEND = 0;
		PerIoData->BytesRECV = 0;
		PerIoData->DataBuf.len = DATA_BUFSIZE;
		PerIoData->DataBuf.buf = PerIoData->Buffer;
		PerIoData->LPBuffer = NULL;

		Flags = 0;
		DWORD dwRes = WSARecv(Accept, &(PerIoData->DataBuf), 1, &PerIoData->BytesRECV, &Flags, &(PerIoData->Overlapped), NULL);
		RecvBytes = PerIoData->BytesRECV;

		printf("\n\n%d::WSARECV1 Socket=%d, PerIoData->BytesRECV=%d; PerIoData->BytesSEND=%d\n\n", dwThreadId, PerHandleData->Socket, PerIoData->BytesRECV, PerIoData->BytesSEND);

		if (dwRes == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				fprintf(stderr, "%d::WSARecv() failed with error %d\n", dwThreadId, WSAGetLastError());

				isOK = true;
				continue;
			}
		}
		else
			fprintf(stderr, "%d::WSARecv() is OK!\n", dwThreadId);

	}
	return 0;
}

HANDLE SslSocketCpServer::GetCompletionPort()
{
	return CompletionPort;
}

void SslSocketCpServer::EvalGet(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	fprintf(stderr, "SslSocketCpServer::EvalGet\n");
}

void SslSocketCpServer::EvalPost(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	fprintf(stderr, "SslSocketCpServer::EvalPost\n");
}

void SslSocketCpServer::Dispatch(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	if (httpRequest->GetMethod() == MethodType::HTTP_GET)
	{
		EvalGet(httpRequest, httpResponse);
	}
	if (httpRequest->GetMethod() == MethodType::HTTP_POST)
	{
		EvalPost(httpRequest, httpResponse);
	}

	if (IsStatic(httpRequest->GetUrl()))
	{
		printf("Static Directory\n");
		EvalStatic(httpRequest, httpResponse);
		return;
	}

	if (HasUrlParams(httpRequest->GetUrl()))
	{
		printf("Has URL Params\n");
		LPSTATICFUNC lpFunc = (LPSTATICFUNC)GetUrlParamHandler(httpRequest->GetUrl());
		httpRequest->urlParams.assign(urlParams.begin(), urlParams.end());
		if (lpFunc != NULL)
		{
			(*lpFunc)(httpRequest, httpResponse);
		}
		else
		{
			UrlNotFound(httpRequest, httpResponse);
		}
		return;
	}

	LPSTATICFUNC lpFunc = (LPSTATICFUNC)GetRoute(httpRequest->GetUrl());
	if (lpFunc != NULL)
	{
		(*lpFunc)(httpRequest, httpResponse);
	}
	else
	{
		WCHAR  buffer[BUFSIZMIN];
		char *url = httpRequest->GetUrl();
		std::string surl;
		surl.append("./");
		surl.append(url);
		std::wstring wsurl(surl.begin(), surl.end());
		DWORD dwres = GetFullPathName(wsurl.c_str(), BUFSIZMIN, buffer, NULL);
		if (dwres > 0)
		{
			PTSTR str = GetPathExtension(buffer);
			std::wstring wsbuffer(buffer);
			std::string sbuffer(wsbuffer.begin(), wsbuffer.end());
			if (FileExist(wsbuffer.c_str()))
			{
				httpResponse->SetStaticFileName(sbuffer.c_str());
				httpResponse->WriteStatic(sbuffer.c_str());
				return;
			}
			else
			{
				printf("File not found: %s \n", sbuffer.c_str());
			}
		}
		UrlNotFound(httpRequest, httpResponse);
	}
}

bool SslSocketCpServer::FileExist(const TCHAR *fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

void SslSocketCpServer::EvalStatic(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	char *str = httpRequest->GetUrl();
	string s = GetFullPath(str);
	printf("Static %s\n", s.c_str());
	httpResponse->SetStaticFileName(s);
	httpResponse->WriteStatic(s.c_str());
}

void SslSocketCpServer::EvalHasUrlParams(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
}


void SslSocketCpServer::UrlNotFound(HttpRequest *httpRequest, HttpResponse *httpResponse)
{
	fprintf(stderr, "SslSocketCpServer::UrlNotFound: \n");
}

DWORD WINAPI SslSocketCpServer::ServerWorkerThread(LPVOID lpObject)
{
	SslSocketCpServer *obj = (SslSocketCpServer*)lpObject;
	HANDLE CompletionPort = (HANDLE)obj->GetCompletionPort();
	DWORD BytesTransferred;
	LPPER_HANDLE_DATA PerHandleData;
	LPPER_IO_OPERATION_DATA PerIoData, PerIoDataSend;
	DWORD SendBytes, RecvBytes;
	DWORD Flags;

	HttpRequest httpRequest;
	HttpResponse httpResponse;

	DWORD dwThreadId = GetCurrentThreadId();



	while (TRUE)
	{
		BytesTransferred = 0;
		BOOL res1 = GetQueuedCompletionStatus(CompletionPort, &BytesTransferred, (PULONG_PTR)&PerHandleData, (LPOVERLAPPED *)&PerIoData, INFINITE);
		if (res1 == 0)
		{
			fprintf(stderr, "%d::ServerWorkerThread--GetQueuedCompletionStatus() failed with error %d\n", dwThreadId, GetLastError());
			return 0;
		}
		else
			fprintf(stderr, "%d::ServerWorkerThread--GetQueuedCompletionStatus() is OK!\n", dwThreadId);

		printf("\n\n%d::WSARECV2 Socket=%d, BytesTransferred=%d; PerIoData->BytesRECV=%d; PerIoData->BytesSEND=%d\n\n", dwThreadId, PerHandleData->Socket, BytesTransferred, PerIoData->BytesRECV, PerIoData->BytesSEND);


		bool cond1 = (BytesTransferred > 0 && PerIoData->BytesRECV == 0 && PerIoData->BytesSEND == 0 && PerIoData->DataBuf.len > 0);
		bool cond2 = (PerIoData->BytesRECV > 0);
		if (cond1 || cond2)
		{
			::WaitForSingleObject(obj->ghMutex, INFINITE);
			if ((PerIoDataSend = (LPPER_IO_OPERATION_DATA)GlobalAlloc(GPTR, sizeof(PER_IO_OPERATION_DATA))) == NULL)
			{
				printf("%d::ERROR: allocate of PerIoDataSend is null \n", dwThreadId);
				continue;
			}
			assert(PerIoDataSend != NULL);
			httpRequest.Parse(PerIoData->DataBuf.buf);

			obj->Dispatch(&httpRequest, &httpResponse);
			ZeroMemory(PerIoDataSend->Buffer, DATA_BUFSIZE);
			ZeroMemory(&(PerIoDataSend->Overlapped), sizeof(OVERLAPPED));
			PerIoDataSend->DataBuf.buf = (char*)httpResponse.GetResponse2(&PerIoDataSend->DataBuf.len);
			PerIoDataSend->BytesRECV = 0;
			int res = WSASend(PerHandleData->Socket, &(PerIoDataSend->DataBuf), 1, &PerIoDataSend->BytesSEND, 0, &(PerIoDataSend->Overlapped), NULL);
			if (res == SOCKET_ERROR)
			{
				fprintf(stderr, "%d::ServerWorkerThread--WSASend() failed with error %d\n", dwThreadId, WSAGetLastError());
			}
			SendBytes = PerIoDataSend->BytesSEND;
			printf("\n\n%d::WSASEND: Socket=%d; SendBytes=%d; PerIoDataSend->BytesRECV=%d; PerIoDataSend->BytesSEND=%d\n\n", dwThreadId, PerHandleData->Socket, SendBytes, PerIoDataSend->BytesRECV, PerIoDataSend->BytesSEND);
			::ReleaseMutex(obj->ghMutex);

			continue;
		}


		bool b1 = BytesTransferred > 0 && PerIoData->BytesRECV == 0 && PerIoData->DataBuf.len > 0;
		bool b2 = BytesTransferred == 0 && PerIoData->BytesRECV == 0 && PerIoData->DataBuf.len > 0;

		if (b1 || b2)
		{
			printf("%d::ServerWorkerThread--Closing socket %d\n", dwThreadId, PerHandleData->Socket);
			if (closesocket(PerHandleData->Socket) == SOCKET_ERROR)
			{
				fprintf(stderr, "%d::ServerWorkerThread--closesocket() failed with error %d\n", dwThreadId, WSAGetLastError());
				return 0;
			}
			else
				fprintf(stderr, "%d::ServerWorkerThread--closesocket() is fine!\n", dwThreadId);

			GlobalFree(PerHandleData);
			GlobalFree(PerIoData);
			continue;
		}

		printf("\n\n\n\nUnhandled condition. IP should not reach line. \n\n\n\n");
		exit(1);
	}
}

PCCERT_CONTEXT SslSocketCpServer::CreateOurCertificate()
{
	// CertCreateSelfSignCertificate(0,&SubjectName,0,0,0,0,0,0);
	HRESULT hr = 0;
	HCRYPTPROV hProv = NULL;
	PCCERT_CONTEXT p = 0;
	HCRYPTKEY hKey = 0;
	CERT_NAME_BLOB sib = { 0 };
	BOOL AX = 0;

	// Step by step to create our own certificate
	try
	{
		// Create the subject
		char cb[1000] = { 0 };
		sib.pbData = (BYTE*)cb;
		sib.cbData = 1000;
		wchar_t*	szSubject = L"CN=Certificate";
		if (!CertStrToName(CRYPT_ASN_ENCODING, szSubject, 0, 0, sib.pbData, &sib.cbData, NULL))
			throw;


		// Acquire Context
		wchar_t* pszKeyContainerName = L"Container";

		if (!CryptAcquireContext(&hProv, pszKeyContainerName, MS_DEF_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET | CRYPT_MACHINE_KEYSET))
		{
			hr = GetLastError();
			if (GetLastError() == NTE_EXISTS)
			{
				if (!CryptAcquireContext(&hProv, pszKeyContainerName, MS_DEF_PROV, PROV_RSA_FULL, CRYPT_MACHINE_KEYSET))
				{
					throw;
				}
			}
			else
				throw;
		}

		// Generate KeyPair
		if (!CryptGenKey(hProv, AT_KEYEXCHANGE, CRYPT_EXPORTABLE, &hKey))
			throw;

		// Generate the certificate
		CRYPT_KEY_PROV_INFO kpi = { 0 };
		kpi.pwszContainerName = pszKeyContainerName;
		kpi.pwszProvName = MS_DEF_PROV;
		kpi.dwProvType = PROV_RSA_FULL;
		kpi.dwFlags = CERT_SET_KEY_CONTEXT_PROP_ID;
		kpi.dwKeySpec = AT_KEYEXCHANGE;

		SYSTEMTIME et;
		GetSystemTime(&et);
		et.wYear += 1;

		CERT_EXTENSIONS exts = { 0 };
		p = CertCreateSelfSignCertificate(hProv, &sib, 0, &kpi, NULL, NULL, &et, &exts);

		AX = CryptFindCertificateKeyProvInfo(p, CRYPT_FIND_MACHINE_KEYSET_FLAG, NULL);
		hCS = CertOpenStore(CERT_STORE_PROV_MEMORY, 0, 0, CERT_STORE_CREATE_NEW_FLAG, 0);
		/*AX = CertAddCertificateContextToStore(hCS,p,CERT_STORE_ADD_NEW,0);
		AX = CryptFindCertificateKeyProvInfo(p,CRYPT_FIND_MACHINE_KEYSET_FLAG,NULL);*/
	}

	catch (...)
	{
	}

	if (hKey)
		CryptDestroyKey(hKey);
	hKey = 0;

	if (hProv)
		CryptReleaseContext(hProv, 0);
	hProv = 0;
	return p;
}




int SslSocketCpServer::ServerInit(bool NoLoop)
{
	SECURITY_STATUS ss = 0;

	/*	if (wcslen(un))
	{
	// Find certificate in the store
	// Open Certificate Store
	hCS = CertOpenSystemStore(0,_T("MY"));
	if (!hCS)
	return -1;

	CERT_RDN cert_rdn;
	CERT_RDN_ATTR cert_rdn_attr;

	cert_rdn.cRDNAttr = 1;
	cert_rdn.rgRDNAttr = &cert_rdn_attr;

	cert_rdn_attr.pszObjId = szOID_COMMON_NAME;
	cert_rdn_attr.dwValueType = CERT_RDN_ANY_TYPE;
	cert_rdn_attr.Value.cbData = (DWORD)wcslen(un);

	cert_rdn_attr.Value.pbData = (BYTE *)un;
	OurCertificate = CertFindCertificateInStore(hCS, X509_ASN_ENCODING | PKCS_7_ASN_ENCODING ,0,cft,&cert_rdn,NULL);
	}*/
	if (IsExternalCert)
	{
		;
	}
	else
	{
		//BOOL AX;
		OurCertificate = CreateOurCertificate();
	}

	// Configure our SSL SChannel
	memset(&m_SchannelCred, 0, sizeof(m_SchannelCred));
	m_SchannelCred.dwVersion = SCHANNEL_CRED_VERSION;
	m_SchannelCred.dwFlags |= SCH_CRED_NO_DEFAULT_CREDS;
	m_SchannelCred.dwFlags = SCH_CRED_NO_DEFAULT_CREDS | SCH_CRED_NO_SYSTEM_MAPPER | SCH_CRED_REVOCATION_CHECK_CHAIN;
	m_SchannelCred.hRootStore = hCS;
	m_SchannelCred.dwMinimumCipherStrength = 128;




	if (OurCertificate)
	{
		m_SchannelCred.cCreds = 1;
		m_SchannelCred.paCred = &OurCertificate;
	}

	// AcquireCredentialsHandle

	ss = AcquireCredentialsHandle(NULL, SCHANNEL_NAME, SECPKG_CRED_INBOUND, NULL, &m_SchannelCred, NULL, NULL, &hCred, &tsExpiry);
	//	ss = AcquireCredentialsHandle(0,UNISP_NAME,SECPKG_CRED_INBOUND,0,&m_SchannelCred,0,0,&hCred,0);
	if (FAILED(ss))
		return -1;

	if (NoLoop)
		return 0;
	return 0;
}



int SslSocketCpServer::ServerOff()
{
	// Server wants to disconnect
	SECURITY_STATUS ss;
	Z<SecBuffer> OutBuffers(100);
	DWORD dwType = SCHANNEL_SHUTDOWN;
	OutBuffers[0].pvBuffer = &dwType;
	OutBuffers[0].BufferType = SECBUFFER_TOKEN;
	OutBuffers[0].cbBuffer = sizeof(dwType);

	sbout.cBuffers = 1;
	sbout.pBuffers = OutBuffers;
	sbout.ulVersion = SECBUFFER_VERSION;

	for (;;)
	{
		ss = ApplyControlToken(&hCtx, &sbout);
		if (FAILED(ss))
			return -1;


		DWORD           dwSSPIFlags;
		DWORD           dwSSPIOutFlags;
		dwSSPIFlags = ISC_REQ_SEQUENCE_DETECT | ISC_REQ_REPLAY_DETECT | ISC_REQ_CONFIDENTIALITY | ISC_RET_EXTENDED_ERROR | ISC_REQ_ALLOCATE_MEMORY | ISC_REQ_STREAM;

		OutBuffers[0].pvBuffer = NULL;
		OutBuffers[0].BufferType = SECBUFFER_TOKEN;
		OutBuffers[0].cbBuffer = 0;
		sbout.cBuffers = 1;
		sbout.pBuffers = OutBuffers;
		sbout.ulVersion = SECBUFFER_VERSION;

		ss = AcceptSecurityContext(&hCred, &hCtx, NULL, dwSSPIFlags, SECURITY_NATIVE_DREP, NULL, &sbout, &dwSSPIOutFlags, 0);
		if (FAILED(ss))
			return -1;

		PBYTE           pbMessage;
		DWORD           cbMessage;
		pbMessage = (BYTE *)(OutBuffers[0].pvBuffer);
		cbMessage = OutBuffers[0].cbBuffer;

		if (pbMessage != NULL && cbMessage != 0)
		{
			int rval = 0; //ssend_p((char*)pbMessage, cbMessage);
			FreeContextBuffer(pbMessage);
			return rval;
		}
		break;
	}
	return 1;
}