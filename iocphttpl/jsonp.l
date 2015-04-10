
%{

#include "stdafx.h"
#include "jsonp.tab.h"

int jperror(char *s);
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

{digits}		{ jplval.int_val = atoi(yytext); return INTEGER_LITERAL; }
"+"				{ jplval.str_val = _strdup(yytext); return JPPLUS; }
"*"				{ jplval.str_val = _strdup(yytext); return JPMULT; }
{opencurly}		{ jplval.str_val = _strdup(yytext); printf("opencurly\n"); return OPENCURLY; }
{closecurly}	{ jplval.str_val = _strdup(yytext); printf("closecurly\n"); return CLOSECURLY; }
{opensqrbrk}	{ jplval.str_val = _strdup(yytext); printf("OPENSQRBRK\n"); return OPENSQRBRK; }
{closesqrbrk}	{ jplval.str_val = _strdup(yytext); printf("CLOSESQRBRK\n"); return CLOSESQRBRK; }
{commasep}		{ jplval.str_val = _strdup(yytext); printf("COMMASEP\n"); return COMMASEP; }
{string}		{ jplval.str_val = _strdup(yytext); printf("STRINGVAL==%s\n",yytext); return STRINGVAL; }
{colonsep}		{ jplval.str_val = _strdup(yytext); printf("COLONSEP\n"); return COLONSEP; }
{ctrue}			{ jplval.str_val = _strdup(yytext); return CTRUE; }
{cfalse}		{ jplval.str_val = _strdup(yytext); return CFALSE; }
{cnull}			{ jplval.str_val = _strdup(yytext); return CNULL; }
{intnum}		{ jplval.str_val = _strdup(yytext); printf("intnum==%s\n",yytext); return INTNUM; }
{fract}			{ jplval.str_val = _strdup(yytext); printf("fract==%s\n",yytext); return FRACTNUM; }
{exponent}		{ jplval.str_val = _strdup(yytext); printf("exponent==%s\n",yytext); return EXPONENT; }
{fourhex}		{ jplval.str_val = _strdup(yytext); printf("fourhex==%s\n",yytext); return FOURHEX; }




[ \t]*		{}
[\n]		{ jplineno++;	}

.		{ jplval.int_val = atoi(yytext); 	}

%%

int yywrap() {
	return 1;
}