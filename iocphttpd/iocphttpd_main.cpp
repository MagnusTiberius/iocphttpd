// iocphttpd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "IocpHttpd.h"

#include "HParser.h"
#include "Jsonp.h"

int _tmain(int argc, _TCHAR* argv[])
{

	Jsonp jsonp;
	jsonp.Parse("{ \"test\":123456 }");



	IocpHttpd server;
	server.Start();
	return 0;
}

