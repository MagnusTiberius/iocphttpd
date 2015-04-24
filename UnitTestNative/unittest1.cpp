#include "stdafx.h"
#include "CppUnitTest.h"
#include "ScannerA.h"
#include "RequestParser.h"
#include "HttpResponse.h"
#include "HttpResponse.h"
#include "HttpUrlRoute.h"

extern "C" {
#include "wparser.tab.h"
}

extern void ww_parse_it();

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestNative
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			ScannerA scanner;
			scanner.Input("GET /test HTTP/1.1\r\n\r\n");
			bool b1 = scanner.Accept("G");
			scanner.Next();
			bool b2 = scanner.Accept("E");
			scanner.Next();
			bool b3 = scanner.Accept("T");
		}

		TEST_METHOD(TestMethod2)
		{
			ScannerA scanner;
			scanner.Input("GET /test HTTP/1.1\r\n\r\n");
			CHAR *c1 = scanner.AcceptRun("GET");
			CHAR *d1 = _strdup(c1);
			printf("%s/n", d1);
			scanner.SkipEmpty();
			CHAR *c2 = scanner.AcceptRun("\/ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_");
			CHAR *d2 = _strdup(c2);
			printf("%s/n", d2);
			scanner.SkipEmpty();
			CHAR *c3 = scanner.AcceptRun("\/ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_.0123456789");
			CHAR *d3 = _strdup(c3);
			printf("%s/n", d3);
			free(d1);
			free(d2);
			free(d3);
		}


		//TEST_METHOD(TestMethod3)
		//{
		//	ww_parse_it();
		//}

		TEST_METHOD(TestMethod4)
		{
			std::string str_value = "	GET /tutorials/other/top-20-mysql-best-practices/ HTTP/1.1 \n\
				Host: net.tutsplus.com \n\
				User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729) \n\
				Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8 \n\
				Accept-Language: en-us,en;q=0.5 \n\
				Accept-Encoding: gzip,deflateAccept-Encoding: gzip,deflate; \n\
				Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7 \n\
				Keep-Alive: 300 \n\
				Connection: keep-alive \n\
				Cookie: PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120 \n\
				Pragma: no-cache \n\
				Cache-Control: no-cache \n\
				\n\
				shdf hsfjksh dfjk sdh fjkshdf jkshf jksdhf jksdfh jksdfh jksdfh sdjkfh djf \n\
				";

			RequestParser parser;

			parser.Input(str_value.c_str());
			parser.Parse();
		}

		TEST_METHOD(TestMethod5)
		{
			std::string str_value = "	GET /foo.php?first_name=John&last_name=Doe&action=Submit HTTP/1.1 \n\
				Host: net.tutsplus.com \n\
				User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729) \n\
				Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8 \n\
				Accept-Language: en-us,en;q=0.5 \n\
				Accept-Encoding: gzip,deflateAccept-Encoding: gzip,deflate; \n\
				Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7 \n\
				Keep-Alive: 300 \n\
				Connection: keep-alive \n\
				Cookie: PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120 \n\
				Pragma: no-cache \n\
				Cache-Control: no-cache \n\
				\n\
				shdf hsfjksh dfjk sdh fjkshdf jkshf jksdhf jksdfh jksdfh jksdfh sdjkfh djf \n\
				";

			RequestParser parser;

			parser.Input(str_value.c_str());
			parser.Parse();
		}

		TEST_METHOD(TestMethod6)
		{
			std::string str_value = "	POST /foo.php HTTP/1.1 \n\
				Host: net.tutsplus.com \n\
				User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729) \n\
				Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8 \n\
				Accept-Language: en-us,en;q=0.5 \n\
				Accept-Encoding: gzip,deflateAccept-Encoding: gzip,deflate; \n\
				Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7 \n\
				Keep-Alive: 300 \n\
				Connection: keep-alive \n\
				Cookie: PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120 \n\
				Pragma: no-cache \n\
				Cache-Control: no-cache \n\
				Content-Type: application/x-www-form-urlencoded \n \
				Content-Length: 43 \n\
				\nfirst_name=John&last_name=Doe&action=Submit";

			RequestParser parser;

			parser.Input(str_value.c_str());
			parser.Parse();
		}

		
		TEST_METHOD(TestMethod7)
		{
			std::string str_value = "	GET /static/img/intel_desktop_roadmap.jpg HTTP/1.1 \n\
				Host: net.tutsplus.com \n\
				User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729) \n\
				Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8 \n\
				Accept-Language: en-us,en;q=0.5 \n\
				Accept-Encoding: gzip,deflateAccept-Encoding: gzip,deflate; \n\
				Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7 \n\
				Keep-Alive: 300 \n\
				Connection: keep-alive \n\
				Cookie: PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120 \n\
				Pragma: no-cache \n\
				Cache-Control: no-cache \n\
				Content-Type: application/x-www-form-urlencoded \n \
				Content-Length: 43 \n\
				\nfirst_name=John&last_name=Doe&action=Submit";

			RequestParser parser;

			parser.Input(str_value.c_str());
			parser.Parse();
		}

		TEST_METHOD(TestMethod8)
		{
			std::string str_value = "	GET /favicon.ico HTTP/1.1 \n\
				Host: localhost:5150 \n \
				Connection: keep-alive \n \
				User-Agent: Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/42.0.2311.90 Safari/537.36 \n \
				Accept: */* \n \
				Referer: http://localhost:5150/test \n \
				Accept-Encoding: gzip, deflate, sdch \n \
				Accept-Language: en-US,en;q=0.8 \n \
				\n \
								    ";
			RequestParser parser;

			parser.Input(str_value.c_str());
			parser.Parse();
		}

		TEST_METHOD(TestMethod9)
		{

			HttpResponse httpResponse;
			byte *s;
			long len;
			s = httpResponse.GetStaticContent2("C:\\www\\index.html", &len);
			s = httpResponse.GetStaticContent2("C:\\www\\init.html", &len);
			s = httpResponse.GetStaticContent2("C:\\www\\index.html", &len);
		}

		TEST_METHOD(TestMethod10)
		{
			HttpUrlRoute httpUrlRoute;
			httpUrlRoute.AddRoute("/json/testtwo", NULL);
			httpUrlRoute.AddRoute("/user/profile/<id:[0-9]+>/", NULL);

			if (httpUrlRoute.HasUrlParams("/user/profile/42344/"))
			{

			}
		}

	};
}

/*

Reference:
1) http://code.tutsplus.com/tutorials/http-headers-for-dummies--net-8039

*/