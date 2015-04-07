#include "HParser.h"

#define yyconst const

extern  int yyparse(void);
extern  int GetHttpUrl(char *action, size_t siz);
//extern  int yyerror(char *s);
//extern  int yyerror(std::string s);
//extern  int yylex(void);
//extern  int yywrap(void);

typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_scan_string(yyconst char *yy_str);
extern void yy_delete_buffer(YY_BUFFER_STATE b);

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
		int res = yyparse();
	}

	void HParser::Parse(char* content)
	{
		printf("HParser::Parse() \n");
		YY_BUFFER_STATE  my_string_buffer = yy_scan_string(content);
		int res = yyparse();
		yy_delete_buffer(my_string_buffer);
		GetHttpUrl(m_Url, DATA_BUFSIZE);
	}

	const char* HParser::GetUrl()
	{
		return m_Url;
	}

}