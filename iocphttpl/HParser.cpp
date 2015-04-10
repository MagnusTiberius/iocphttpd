#include "HParser.h"

#define yyconst const

//#define yyparse parserlibparse
//#define yylval parserliblval

extern  int yyparse(void);
extern  int GetHttpUrl(char *action, size_t siz);
extern  int GetHttpMethod(char *method, size_t siz);




//extern  int yyerror(char *s);
//extern  int yyerror(std::string s);
//extern  int yylex(void);
//extern  int yywrap(void);

typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern YY_BUFFER_STATE hp_scan_string(yyconst char *yy_str);
extern void hp_delete_buffer(YY_BUFFER_STATE b);

namespace headerparser
{

	HParser::HParser()
	{
	}


	HParser::~HParser()
	{
	}

	void HParser::Parse()
	{
		printf("HParser::Parse() \n");
		int res = hpparse();
	}

	void HParser::Parse(char* content)
	{
		printf("HParser::Parse()\n %s \n", content);
		YY_BUFFER_STATE  my_string_buffer = hp_scan_string(content);
		int res = hpparse();
		hp_delete_buffer(my_string_buffer);
	}

	char* HParser::GetUrl()
	{
		GetHttpUrl(m_Url, DATA_BUFSIZE);
		return m_Url;
	}

	const char* HParser::GetMethod()
	{
		GetHttpMethod(m_Method, DATA_BUFSIZE);
		return m_Method;
	}


}