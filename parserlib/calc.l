/* Mini Calculator */
/* calc.lex */

%{
#include "stdafx.h"
#include "calc.tab.h"
int yyerror(char *s);
//int yylineno = 1;
%}

digit		[0-9]
int_const	{digit}+
identifier		[a-zA-Z]+[0-9]
method			[GET|POST]

%%

{int_const}			{ yylval.int_val = atoi(yytext); return INTEGER_LITERAL; }
"+"					{ yylval.identifier_tok = _strdup(yytext); return PLUS; }
"*"					{ yylval.identifier_tok = _strdup(yytext); return MULT; }
{identifier}		{ yylval.int_val = _strdup(yytext); return IDENTIFIER; }
{method}			{ yylval.int_val = _strdup(yytext); return METHOD; }

[ \t]*		{}
[\n]		{ yylineno++;	}

.		{ yylval.int_val = atoi(yytext); 	}

%%

int yywrap() {
	return 1;
}