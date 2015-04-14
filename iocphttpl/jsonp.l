
%{

#include "stdafx.h"
#include "jsonp.tab.h"

#define YYSTYPE JPSTYPE

int jperror(char *s);
//int yylineno = 1;
%}

%option reentrant
%option bison-bridge

plussign    \+
minsign		\-
digit		[0-9]
digits		{digit}+
exponent    [Ee]
intnum      [\-\+]*{digits}
fract       [\-\+]*.{digits}
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
fourhex     \\[\u\U][0-9][0-9][0-9][0-9]
cchar       ({cchar1}|{fourhex})
cchars      ({cchar}|{cchar}+)
string      ({doublequote}{doublequote}|{doublequote}{cchars}{doublequote})

ctrue       (true|TRUE) 
cfalse      (false|FALSE) 
cnull       (null|NULL) 

%%

{digits}		{ yyg->yylval_r->int_val = atoi(yytext); return INTEGER_LITERAL; }
"+"				{ yyg->yylval_r->str_val = _strdup(yytext); return JPPLUS; }
"*"				{ yyg->yylval_r->str_val = _strdup(yytext); return JPMULT; }
{opencurly}		{ yyg->yylval_r->str_val = _strdup(yytext); printf("opencurly\n"); return OPENCURLY; }
{closecurly}	{ yyg->yylval_r->str_val = _strdup(yytext); printf("closecurly\n"); return CLOSECURLY; }
{opensqrbrk}	{ yyg->yylval_r->str_val = _strdup(yytext); printf("OPENSQRBRK\n"); return OPENSQRBRK; }
{closesqrbrk}	{ yyg->yylval_r->str_val = _strdup(yytext); printf("CLOSESQRBRK\n"); return CLOSESQRBRK; }
{commasep}		{ yyg->yylval_r->str_val = _strdup(yytext); printf("COMMASEP\n"); return COMMASEP; }
{string}		{ yyg->yylval_r->str_val = _strdup(yytext); printf("STRINGVAL==%s\n",yytext); return STRINGVAL; }
{colonsep}		{ yyg->yylval_r->str_val = _strdup(yytext); printf("COLONSEP\n"); return COLONSEP; }
{ctrue}			{ yyg->yylval_r->str_val = _strdup(yytext); return CTRUE; }
{cfalse}		{ yyg->yylval_r->str_val = _strdup(yytext); return CFALSE; }
{cnull}			{ yyg->yylval_r->str_val = _strdup(yytext); return CNULL; }
{intnum}		{ yyg->yylval_r->str_val = _strdup(yytext); printf("intnum==%s\n",yytext); return INTNUM; }
{fract}			{ yyg->yylval_r->str_val = _strdup(yytext); printf("fract==%s\n",yytext); return FRACTNUM; }
{exponent}		{ yyg->yylval_r->str_val = _strdup(yytext); printf("exponent==%s\n",yytext); return EXPONENT; }
{fourhex}		{ yyg->yylval_r->str_val = _strdup(yytext); printf("fourhex==%s\n",yytext); return FOURHEX; }




[ \t]*		{}
[\n]		{ yylineno++;	}

.		{ yyg->yylval_r->int_val = atoi(yytext); 	}

%%

int yywrap() {
	return 1;
}