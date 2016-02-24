#pragma once
#include "HttpHeader.h"
//#include "HParser.h"
#include "Enums.h"
#include "HttpTemplate.h"
#include "RequestParser.h"

namespace HTTPMODULE2
{


	class HttpRequest : public HttpTemplate
	{
	public:
		HttpRequest();
		~HttpRequest();

		void Parse(char *content);
		char* GetUrl();
		MethodType GetMethod();
		const char* GetContent();

		std::vector<std::string> urlParams;

		bool IsValid(char* input);

	private:
		//headerparser::HParser hParser;
		HttpHeader httpHeader;
		RequestParser requestParser;
	};


}