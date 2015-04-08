#include "Jsonp.h"

#define yyconst const

#define yyparse jsonlibparse
#define yylval jsonliblval

extern  int yyparse(void);

typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern YY_BUFFER_STATE jsonlib_scan_string(yyconst char *yy_str);
extern void jsonlib_delete_buffer(YY_BUFFER_STATE b);

Jsonp::Jsonp()
{
}


Jsonp::~Jsonp()
{
}


void Jsonp::Parse(char* content)
{
	YY_BUFFER_STATE  my_string_buffer = jsonlib_scan_string(content);
	int res = yyparse();
	jsonlib_delete_buffer(my_string_buffer);

}