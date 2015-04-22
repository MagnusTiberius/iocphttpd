// iocphttpl.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "iocphttpl.h"


// This is an example of an exported variable
IOCPHTTPL_API int niocphttpl=0;

// This is an example of an exported function.
IOCPHTTPL_API int fniocphttpl(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see iocphttpl.h for the class definition
Ciocphttpl::Ciocphttpl()
{
	return;
}
