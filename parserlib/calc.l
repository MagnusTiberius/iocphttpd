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

%%

{int_const}	{ yylval.int_val = atoi(yytext); return INTEGER_LITERAL; }
"+"		{ yylval.op_val = _strdup(yytext); return PLUS; }
"*"		{ yylval.op_val = _strdup(yytext); return MULT; }

[ \t]*		{}
[\n]		{ yylineno++;	}

.		{ std::cerr << "SCANNER "; yyerror(""); exit(1);	}

%%

int yywrap() {
	return 1;
}