// iocpservice.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "iocpservice_i.h"


using namespace ATL;

#include <stdio.h>

class CiocpserviceModule : public ATL::CAtlServiceModuleT< CiocpserviceModule, IDS_SERVICENAME >
{
public :
	DECLARE_LIBID(LIBID_iocpserviceLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_IOCPSERVICE, "{6A0E45FD-5E33-4C60-8DA9-18202963F667}")
		HRESULT InitializeSecurity() throw()
	{
		// TODO : Call CoInitializeSecurity and provide the appropriate security settings for your service
		// Suggested - PKT Level Authentication, 
		// Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY 
		// and an appropriate Non NULL Security Descriptor.

		return S_OK;
	}
	};

CiocpserviceModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

