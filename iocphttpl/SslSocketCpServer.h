#pragma once
// Link to ws2_32.lib
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#include "stdafx.h"

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpUrlRoute.h"
#include "HttpTemplate.h"


#pragma warning(disable:4996)
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Crypt32.lib")
#pragma comment(lib,"Secur32.lib")
#pragma comment(lib,"version.lib")
#pragma comment(lib,"Shlwapi.lib")
#pragma comment(lib,"shell32.lib")
#pragma comment(lib,"Advapi32.lib")



//#define DATA_BUFSIZE 8192

class IOCPHTTPL_API SslSocketCpServer : public HttpUrlRoute
{
public:
	SslSocketCpServer();
	~SslSocketCpServer();

	SslSocketCpServer(int PortNum);

	void Start(int PortNum);

	typedef struct
	{
		OVERLAPPED Overlapped;
		WSABUF DataBuf;
		CHAR Buffer[DATA_BUFSIZE];
		byte *LPBuffer;
		vector<byte> byteBuffer;
		DWORD BytesSEND;
		DWORD BytesRECV;
	} PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;

	typedef struct
	{
		SOCKET Socket;
	} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

	typedef /*static*/ void(*LPSTATICFUNC)(HttpRequest *httpRequest, HttpResponse *httpResponse);

	int Start();

	HANDLE GetCompletionPort();
	virtual void EvalGet(HttpRequest *httpRequest, HttpResponse *httpResponse);
	virtual void EvalPost(HttpRequest *httpRequest, HttpResponse *httpResponse);
	virtual void Dispatch(HttpRequest *httpRequest, HttpResponse *httpResponse);
	virtual void EvalStatic(HttpRequest *httpRequest, HttpResponse *httpResponse);
	virtual void UrlNotFound(HttpRequest *httpRequest, HttpResponse *httpResponse);
	virtual void EvalHasUrlParams(HttpRequest *httpRequest, HttpResponse *httpResponse);

	HANDLE ghMutex;

	PCCERT_CONTEXT CreateOurCertificate();
	int ServerInit(bool NoLoop);
	int ServerOff();

private:
	static DWORD WINAPI ServerWorkerThread(LPVOID CompletionPortID);
	HANDLE CompletionPort;
	int m_PortNum = PORT;
	bool FileExist(const TCHAR *fileName);

	PCCERT_CONTEXT OurCertificate;
	SCHANNEL_CRED m_SchannelCred;
	bool IsExternalCert;
	HCERTSTORE hCS;
	CredHandle hCred;
	CtxtHandle hCtx;
	TimeStamp tsExpiry;
	SecBufferDesc sbin;
	SecBufferDesc sbout;

};


