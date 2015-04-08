
%{

#include "stdafx.h"
#include "calc.tab.h"

//#define yystype parserlibstype
//#define YYSTYPE PARSERLIBSTYPE


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
propname        {identifier2}:
comma			\,
semicol			\;
colon			\:
fslash          \/
period			\.
openpar			\(
closepar		\)

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

{propname}			{ yylval.identifier_tok = _strdup(yytext); 
					  printf("Lexer::PROPNAME:=%s\n",yytext); 
					  return PROPNAME; 
					}

{comma}				{ yylval.identifier_tok = _strdup(yytext); 
					  printf("Lexer::COMMA:=%s\n",yytext); 
					  return COMMA; 
					}

{semicol}			{ yylval.identifier_tok = _strdup(yytext); 
					  printf("Lexer::SEMICOL:=%s\n",yytext); 
					  return SEMICOL; 
					}

{colon}				{ yylval.identifier_tok = _strdup(yytext); 
					  printf("Lexer::COLON:=%s\n",yytext); 
					  return COLON; 
					}

{fslash}			{ yylval.identifier_tok = _strdup(yytext); 
					  printf("Lexer::FSLASH:=%s\n",yytext); 
					  return FSLASH; 
					}

{period}			{ yylval.identifier_tok = _strdup(yytext); 
					  printf("Lexer::PERIOD:=%s\n",yytext); 
					  return PERIOD; 
					}

{openpar}			{ yylval.identifier_tok = _strdup(yytext); 
					  printf("Lexer::OPENPAR:=%s\n",yytext); 
					  return OPENPAR; 
					}

{closepar}			{ yylval.identifier_tok = _strdup(yytext); 
					  printf("Lexer::CLOSEPAR:=%s\n",yytext); 
					  return CLOSEPAR; 
					}




[ \t]*		{}
[\n]		{ yylineno++; yylval.identifier_tok = _strdup(yytext);  printf("Lexer::newline:\n"); return NEWLINE;	}

.			{ yylval.int_val = atoi(yytext); 	}

%%
 
int yywrap() {
	return 1;
}

