

%{

#include "stdafx.h"
#include "jsonp.tab.h"

int yyerror(char *s);
//int yylineno = 1;
%}

digit		[0-9]
int_const	{digit}+
anumber     ((-)[0-9]+(.[0-9]+)((-)[eE][0-9]+)|(-)([0-9]+).[0-9]+((-)[eE][0-9]+))
opencurly   \{
closecurly  \}
opensqrbrk  \[
closesqrbrk \]
doublequote \"
revsolidus  \\
solidus     \/
backspace   \b
formfeed    \f
newline     \n
carriageret \r
horiztab    \t
commasep    \,
colonsep    \:
cspecial    [!@#$%^&_=]
cchar1      [[a-zA-Z0-9]|{revsolidus}|{solidus}|{backspace}|{formfeed}|{newline}|{carriageret}|{horiztab}|{doublequote}|{cspecial}]
fourhex     \u[0-9][0-9][0-9][0-9]
cchar       ({cchar1}|{fourhex})
cchars      ({cchar}|{cchar}+)
string      ({doublequote}{doublequote}|{doublequote}{cchars}{doublequote})

ctrue       (true|TRUE) 
cfalse      (false|FALSE) 
cnull       (null|NULL) 

%%

{int_const}		{ yylval.int_val = atoi(yytext); return INTEGER_LITERAL; }
"+"				{ yylval.str_val = _strdup(yytext); return PLUS; }
"*"				{ yylval.str_val = _strdup(yytext); return MULT; }
{opencurly}		{ yylval.str_val = _strdup(yytext); return OPENCURLY; }
{closecurly}	{ yylval.str_val = _strdup(yytext); return CLOSECURLY; }
{opensqrbrk}	{ yylval.str_val = _strdup(yytext); return OPENSQRBRK; }
{closesqrbrk}	{ yylval.str_val = _strdup(yytext); return CLOSESQRBRK; }
{commasep}		{ yylval.str_val = _strdup(yytext); return COMMASEP; }
{string}		{ yylval.str_val = _strdup(yytext); return STRINGVAL; }
{colonsep}		{ yylval.str_val = _strdup(yytext); return COLONSEP; }
{anumber}		{ yylval.str_val = _strdup(yytext); return ANUMBER; }
{ctrue}			{ yylval.str_val = _strdup(yytext); return CTRUE; }
{cfalse}		{ yylval.str_val = _strdup(yytext); return CFALSE; }
{cnull}			{ yylval.str_val = _strdup(yytext); return CNULL; }


[ \t]*		{}
[\n]		{ jsonliblineno++;	}

.		{ std::cerr << "SCANNER "; yyerror(""); exit(1);	}

%%