#include "Jsonp.h"

#define yyconst const

#define yyparse jpparse
#define yylval jplval

extern  int yyparse(void);

typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern YY_BUFFER_STATE jp_scan_string(yyconst char *yy_str);
extern void jp_delete_buffer(YY_BUFFER_STATE b);

Jsonp::Jsonp()
{
}


Jsonp::~Jsonp()
{
}


void Jsonp::Parse(char* content)
{
	YY_BUFFER_STATE  my_string_buffer = jp_scan_string(content);
	int res = jpparse();
	jp_delete_buffer(my_string_buffer);

}