// httpparserdll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "httpparserdll.h"


// This is an example of an exported variable
HTTPPARSERDLL_API int nhttpparserdll=0;

// This is an example of an exported function.
HTTPPARSERDLL_API int fnhttpparserdll(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see httpparserdll.h for the class definition
Chttpparserdll::Chttpparserdll()
{
	return;
}
