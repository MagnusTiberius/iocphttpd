#include "HParser.h"


extern  int yyparse(void);
//extern  int yyerror(char *s);
//extern  int yyerror(std::string s);
//extern  int yylex(void);
//extern  int yywrap(void);

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

}