// httpmodule.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "httpmodule.h"


// This is an example of an exported variable
HTTPMODULE_API int nhttpmodule=0;

// This is an example of an exported function.
HTTPMODULE_API int fnhttpmodule(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see httpmodule.h for the class definition
Chttpmodule::Chttpmodule()
{
	return;
}
