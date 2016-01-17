// netiocp.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "netiocp.h"


// This is an example of an exported variable
NETIOCP_API int nnetiocp=0;

// This is an example of an exported function.
NETIOCP_API int fnnetiocp(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see netiocp.h for the class definition
Cnetiocp::Cnetiocp()
{
	return;
}
