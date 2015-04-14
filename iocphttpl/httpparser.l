
%{

#include "stdafx.h"
#include "httpparser.tab.h"

#define YYSTYPE HPSTYPE

//#define yystype hpstype
//#define YYSTYPE PARSERLIBSTYPE
//
//
//int yyerror(char *s);
//int yylineno = 1;
%}
%option reentrant
%option bison-bridge
%s ASTATE BSTATE

digit			[0-9]
digits			{digit}+
methodaction    (GET|POST)
identifier		[a-zA-Z]+[0-9]+[a-zA-Z0-9\-]
urlpath         \/[a-zA-Z]+[\/\_\-a-zA-Z.]+
methodver		HTTP\/[0-9].[0-9]
identifier2		([a-zA-z]+|[a-zA-z\-]+[a-zA-z\-]+)
equal			\=
question        \?
ampersand       \&
propname        {identifier2}:
comma			\,
semicol			\;
colon			\:
fslash          \/
period			\.
openpar			\(
closepar		\)

%%

{digits}			{	yyg->yylval_r->identifier_tok = _strdup(yytext); 
						//printf("Lexer::digits:=>%s\n",yytext); 
						return DIGITS; }
"+"					{	yyg->yylval_r->identifier_tok = _strdup(yytext); 
						return PLUS; }
"*"					{	yyg->yylval_r->identifier_tok = _strdup(yytext); 
						return MULT; }
{identifier}		{	yyg->yylval_r->identifier_tok = _strdup(yytext); 
						//printf("Lexer::identifier\n"); 
						return IDENTIFIER; }
{digit}				{	yyg->yylval_r->identifier_tok = _strdup(yytext); 
						//printf("Lexer::digit\n"); 
						return DIGIT; }

{methodaction}		{	BEGIN ASTATE; 
						yyg->yylval_r->identifier_tok = _strdup(yytext); 
						//printf("Lexer::methodaction:%s\n",yytext); 
						return METHODACTION; 
					}

<ASTATE>{urlpath}	{ yyg->yylval_r->identifier_tok = _strdup(yytext); 
					  //printf("Lexer::urlpath:%s\n",yytext); 
					  return URLPATH; 
					}
<ASTATE>{equal}	{ yyg->yylval_r->identifier_tok = _strdup(yytext); 
					  //printf("Lexer::EQUAL:%s\n",yytext); 
					  return EQUAL; 
					}
<ASTATE>{question}	{ yyg->yylval_r->identifier_tok = _strdup(yytext); 
					  //printf("Lexer::question:%s\n",yytext); 
					  return QUESTION; 
					}

<ASTATE>{ampersand}	{ yyg->yylval_r->identifier_tok = _strdup(yytext); 
					  //printf("Lexer::ampersand:%s\n",yytext); 
					  return AMPERSAND; 
					}


<ASTATE>{digits}	{ yyg->yylval_r->int_val = atoi(yytext); return DIGITS; }

<ASTATE>{identifier2}	{ yyg->yylval_r->identifier_tok = _strdup(yytext); 
					  //printf("Lexer::identifier2:%s\n",yytext); 
					  return ID2; 
					}


<ASTATE>{methodver}	{ BEGIN 0; 
					  yyg->yylval_r->identifier_tok = _strdup(yytext); 
					 //printf("Lexer::methodver:=%s\n",yytext); 
					  return METHODVER; 
					}

{identifier2}		{ yyg->yylval_r->identifier_tok = _strdup(yytext); 
					  //printf("Lexer::identifier2:=%s\n",yytext); 
					  return ID2; 
					}

{propname}			{ yyg->yylval_r->identifier_tok = _strdup(yytext); 
					  //printf("Lexer::PROPNAME:=%s\n",yytext); 
					  return PROPNAME; 
					}

{comma}				{ yyg->yylval_r->identifier_tok = _strdup(yytext); 
					  //printf("Lexer::COMMA:=%s\n",yytext); 
					  return COMMA; 
					}

{semicol}			{ yyg->yylval_r->identifier_tok = _strdup(yytext); 
					  //printf("Lexer::SEMICOL:=%s\n",yytext); 
					  return SEMICOL; 
					}

{colon}				{ yyg->yylval_r->identifier_tok = _strdup(yytext); 
					  //printf("Lexer::COLON:=%s\n",yytext); 
					  return COLON; 
					}

{fslash}			{ yyg->yylval_r->identifier_tok = _strdup(yytext); 
					  //printf("Lexer::FSLASH:=%s\n",yytext); 
					  return FSLASH; 
					}

{period}			{ yyg->yylval_r->identifier_tok = _strdup(yytext); 
					  //printf("Lexer::PERIOD:=%s\n",yytext); 
					  return PERIOD; 
					}

{openpar}			{ yyg->yylval_r->identifier_tok = _strdup(yytext); 
					  //printf("Lexer::OPENPAR:=%s\n",yytext); 
					  return OPENPAR; 
					}

{closepar}			{ yyg->yylval_r->identifier_tok = _strdup(yytext); 
					  //printf("Lexer::CLOSEPAR:=%s\n",yytext); 
					  return CLOSEPAR; 
					}




[ \t]*		{}
[\n]		{ yylineno++; yyg->yylval_r->identifier_tok = _strdup(yytext);  /*printf("Lexer::newline:\n");*/ return NEWLINE;	}

.			{ yyg->yylval_r->int_val = atoi(yytext); 	}

%%
 
int yywrap() {
	return 1;
}

