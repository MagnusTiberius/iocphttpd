// iocphttpd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "IocpHttpd.h"

#include "HParser.h"


int _tmain(int argc, _TCHAR* argv[])
{


	IocpHttpd server;
	server.Start();
	return 0;
}

