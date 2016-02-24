// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the IOCPHTTPL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// IOCPHTTPL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#include "stdafx.h"

namespace IOCPHTTPL
{

	// This class is exported from the iocphttpl.dll
	class IOCPHTTPL_API Ciocphttpl {
	public:
		Ciocphttpl(void);
		// TODO: add your methods here.
	};

	extern IOCPHTTPL_API int niocphttpl;

	IOCPHTTPL_API int fniocphttpl(void);

}