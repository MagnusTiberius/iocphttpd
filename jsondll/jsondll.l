
%{

#include "stdafx.h"
#include "jsondll.tab.h"

#define yylval jsondlllval


int yyerror(char *s);
//int yylineno = 1;
%}

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

{digits}		{ yylval.int_val = atoi(yytext); return INTEGER_LITERAL; }
"+"				{ yylval.str_val = _strdup(yytext); return PLUS; }
"*"				{ yylval.str_val = _strdup(yytext); return MULT; }
{opencurly}		{ yylval.str_val = _strdup(yytext); printf("opencurly\n"); return OPENCURLY; }
{closecurly}	{ yylval.str_val = _strdup(yytext); printf("closecurly\n"); return CLOSECURLY; }
{opensqrbrk}	{ yylval.str_val = _strdup(yytext); printf("OPENSQRBRK\n"); return OPENSQRBRK; }
{closesqrbrk}	{ yylval.str_val = _strdup(yytext); printf("CLOSESQRBRK\n"); return CLOSESQRBRK; }
{commasep}		{ yylval.str_val = _strdup(yytext); printf("COMMASEP\n"); return COMMASEP; }
{string}		{ yylval.str_val = _strdup(yytext); printf("STRINGVAL==%s\n",yytext); return STRINGVAL; }
{colonsep}		{ yylval.str_val = _strdup(yytext); printf("COLONSEP\n"); return COLONSEP; }
{ctrue}			{ yylval.str_val = _strdup(yytext); return CTRUE; }
{cfalse}		{ yylval.str_val = _strdup(yytext); return CFALSE; }
{cnull}			{ yylval.str_val = _strdup(yytext); return CNULL; }
{intnum}		{ yylval.str_val = _strdup(yytext); printf("intnum==%s\n",yytext); return INTNUM; }
{fract}			{ yylval.str_val = _strdup(yytext); printf("fract==%s\n",yytext); return FRACTNUM; }
{exponent}		{ yylval.str_val = _strdup(yytext); printf("exponent==%s\n",yytext); return EXPONENT; }
{fourhex}		{ yylval.str_val = _strdup(yytext); printf("fourhex==%s\n",yytext); return FOURHEX; }




[ \t]*		{}
[\n]		{ yylineno++;	}

.		{ yylval.int_val = atoi(yytext); 	}

%%

int yywrap() {
	return 1;
}