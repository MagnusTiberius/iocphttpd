// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the HTTPMODULE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// HTTPMODULE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef HTTPMODULE_EXPORTS
#define HTTPMODULE_API __declspec(dllexport)
#else
#define HTTPMODULE_API __declspec(dllimport)
#endif

// This class is exported from the httpmodule.dll
class HTTPMODULE_API Chttpmodule {
public:
	Chttpmodule(void);
	// TODO: add your methods here.
};

extern HTTPMODULE_API int nhttpmodule;

HTTPMODULE_API int fnhttpmodule(void);
