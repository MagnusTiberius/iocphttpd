// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the NETIOCP_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// NETIOCP_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef NETIOCP_EXPORTS
#define NETIOCP_API __declspec(dllexport)
#else
#define NETIOCP_API __declspec(dllimport)
#endif

namespace NETIOCP
{

	// This class is exported from the netiocp.dll
	class NETIOCP_API Cnetiocp {
	public:
		Cnetiocp(void);
		// TODO: add your methods here.
	};

	extern NETIOCP_API int nnetiocp;

	NETIOCP_API int fnnetiocp(void);


}