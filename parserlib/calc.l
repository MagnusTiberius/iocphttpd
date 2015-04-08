
%{

#include "stdafx.h"
#include "calc.tab.h"



//int yyerror(char *s);
//int yylineno = 1;
%}

%s ASTATE BSTATE

digit			[0-9]
digits			{digit}+
methodaction    (GET|POST)
identifier		[a-zA-Z]+[0-9]+[a-zA-Z0-9\-]
urlpath         \/[a-zA-Z]+[\/a-zA-Z.]+
methodver		HTTP\/[0-9].[0-9]
identifier2		([a-zA-z]+|[a-zA-z]+[a-zA-z\-]+)
equal			\=
question        \?
ampersand       \&
host			Host:
accept			Accept:
connection		Connection:
useragent		User-Agent:
anytype         \*\/\*

%%

{digits}			{ yylval.identifier_tok = _strdup(yytext); printf("Lexer::digits:=>%s\n",yytext); return DIGITS; }
"+"					{ yylval.identifier_tok = _strdup(yytext); return PLUS; }
"*"					{ yylval.identifier_tok = _strdup(yytext); return MULT; }
{identifier}		{ yylval.identifier_tok = _strdup(yytext); printf("Lexer::identifier\n"); return IDENTIFIER; }
{digit}				{ yylval.identifier_tok = _strdup(yytext); printf("Lexer::digit\n"); return DIGIT; }

{methodaction}		{	BEGIN ASTATE; 
						yylval.identifier_tok = _strdup(yytext); 
						printf("Lexer::methodaction:%s\n",yytext); 
						return METHODACTION; 
					}

<ASTATE>{urlpath}	{ yylval.identifier_tok = _strdup(yytext); 
					  printf("Lexer::urlpath:%s\n",yytext); 
					  return URLPATH; 
					}
<ASTATE>{equal}	{ yylval.identifier_tok = _strdup(yytext); 
					  printf("Lexer::EQUAL:%s\n",yytext); 
					  return EQUAL; 
					}
<ASTATE>{question}	{ yylval.identifier_tok = _strdup(yytext); 
					  printf("Lexer::question:%s\n",yytext); 
					  return QUESTION; 
					}

<ASTATE>{ampersand}	{ yylval.identifier_tok = _strdup(yytext); 
					  printf("Lexer::ampersand:%s\n",yytext); 
					  return AMPERSAND; 
					}


<ASTATE>{digits}	{ yylval.int_val = atoi(yytext); return DIGITS; }

<ASTATE>{identifier2}	{ yylval.identifier_tok = _strdup(yytext); 
					  printf("Lexer::identifier2:%s\n",yytext); 
					  return ID2; 
					}


<ASTATE>{methodver}	{ BEGIN 0; 
					  yylval.identifier_tok = _strdup(yytext); 
					  printf("Lexer::methodver:=%s\n",yytext); 
					  return METHODVER; 
					}

{identifier2}		{ yylval.identifier_tok = _strdup(yytext); 
					  printf("Lexer::identifier2:=%s\n",yytext); 
					  return ID2; 
					}

{host}				{	yylval.identifier_tok = _strdup(yytext); 
						printf("Lexer::Host:=%s\n",yytext); 
						return HOST; 
					}

{connection}		{	yylval.identifier_tok = _strdup(yytext); 
						printf("Lexer::Connection:=%s\n",yytext); 
						return CONNECTION; 
					}

{anytype}			{	yylval.identifier_tok = _strdup(yytext); 
						printf("Lexer::anytype:=%s\n",yytext); 
						return ANYTYPE; 
					}

{accept}			{	yylval.identifier_tok = _strdup(yytext); 
						printf("Lexer::accept:=%s\n",yytext); 
						return ACCEPT; 
					}

{useragent}			{	yylval.identifier_tok = _strdup(yytext); 
						printf("Lexer::useragent:=%s\n",yytext); 
						return USERAGENT; 
					}



[ \t]*		{}
[\n]		{ yylineno++; yylval.identifier_tok = _strdup(yytext);  printf("Lexer::newline:\n"); 	}

.		{ yylval.int_val = atoi(yytext); 	}

%%

int yywrap() {
	return 1;
}

