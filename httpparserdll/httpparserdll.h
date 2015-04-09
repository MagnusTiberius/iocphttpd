// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the HTTPPARSERDLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// HTTPPARSERDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef HTTPPARSERDLL_EXPORTS
#define HTTPPARSERDLL_API __declspec(dllexport)
#else
#define HTTPPARSERDLL_API __declspec(dllimport)
#endif

// This class is exported from the httpparserdll.dll
class HTTPPARSERDLL_API Chttpparserdll {
public:
	Chttpparserdll(void);
	// TODO: add your methods here.
};

extern HTTPPARSERDLL_API int nhttpparserdll;

HTTPPARSERDLL_API int fnhttpparserdll(void);
