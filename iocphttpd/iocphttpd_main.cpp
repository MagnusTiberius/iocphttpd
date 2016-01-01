// iocphttpd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "IocpHttpd.h"

//#include "HParser.h"
#include "Jsonp.h"

int _tmain(int argc, _TCHAR* argv[])
{

	Jsonp jsonp;
	jsonp.Parse("{ \"\\u3333\" : .888, \"\\u2121\" : -.777}");
	//jsonp.Parse("{ \"test1\" : 123456E-2 , \"test2\" : -123456E-2, \"test3\" : 123456.78E-2, \"test4\" : -123456.78E-2 }");
	//jsonp.Parse("{ \"test1\" : 123456E2, \"test2\" : -123456E2, \"test3\" : 123456.78E2, \"test4\" : -123456.78E2 }");
	//jsonp.Parse("{ \"test1\" : 123456, \"test2\" : -123456, \"test3\" : 123456.78, \"test4\" : -123456.78 }");
	//jsonp.Parse("{ \"tesw1\" : .999, \"tesw2\" : -.9999}");
	//jsonp.Parse("[ \"abc\", 99999 ] ");
	//jsonp.Parse("{ \"arr2\" : [ \"abc\", 99999 ] }");
	//jsonp.Parse("[ \"abc\", 99999, { \"fgh\" : 1234, \"www\" : 53556  } ] ");
	//jsonp.Parse("[ \"abc\", 99999, \"obj1\" : { \"fgh\" : 1234, \"www\" : 53556  }, 234234234, \"wertwtwet\" ] ");



	IocpHttpd server;
	server.Start(8080);
	return 0;
}

