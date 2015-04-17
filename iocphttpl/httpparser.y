%define api.prefix hp
%define api.pure
%{
#include "stdafx.h"
#include "httpparserast.h"
#include "httpparser.tab.h"

void* hpscanner;
#define hpparm hpscanner


//#define YYPARSE_PARAM hpparm
#define YYLEX_PARAM &hpparm


int hperror(string s);
int hperror(char *s);

typedef void* yyscan_t;
extern int hplex (YYSTYPE * yylval_param ,yyscan_t yyscanner);
//extern int hplex (YYSTYPE * yylval_param );

%}

%union{
  int		int_val;
  char*		identifier_tok;
};

%start	input 

%token	<int_val>			
%token	<identifier_tok>	IDENTIFIER METHODACTION DIGIT HEADERELEM URLPATH METHODVER ID2 URLPARAMS HOST EQUAL
							CONNECTION USERAGENT QUESTION AMPERSAND ANYTYPE ACCEPT CACHECONTROL
							ACCEPTENCODING ACCEPTLANG DIGITS NEWLINE 
							PROPNAME COMMA SEMICOL COLON FSLASH PERIOD OPENPAR CLOSEPAR
							PLUS MULT
%type	<identifier_tok>	delimiters property_name property urlpath2

%type	<lpHttpdoc>	line 
/* %left	PLUS
%left	MULT */

%%

input	: /* empty */
		| input line 
		;

line	: line1 NEWLINE			{ /*printf("line1 seen\n");*/ }
		| property NEWLINE		{ /*printf("property_item seen\n");*/ }
		| NEWLINE				{ /*printf("content should follow after this marker.\n");*/ }
		;


line1	: METHODACTION		{ SetMethodAction($1, &g_Httpdoc); }
		| line1 urlpath2	{ SetUrl($2, &g_Httpdoc); }
		| line1 urlparams   { printf("urlparams whole seen\n"); }
		| line1 METHODVER   { SetMethodVersion($2, &g_Httpdoc); }
		;

urlpath2 : FSLASH
		| URLPATH
		;

urlparams : QUESTION								{ printf("urlparams 1 seen\n"); }
		  | urlparams IDENTIFIER EQUAL IDENTIFIER	{ printf("urlparams 2 seen\n"); }
		  | urlparams ID2 EQUAL ID2					{ printf("urlparams 3 seen\n"); }
		  | urlparams ID2 EQUAL DIGITS				{ printf("urlparams 4 seen\n"); }
		  | urlparams AMPERSAND						{ printf("urlparams 5 seen\n"); }
		  ;


property	: property_name				{  $$ = $1; }
			| property ANYTYPE			{  $$ = $2; }
			| property ID2				{  $$ = $2; }
			| property IDENTIFIER		{  $$ = $2; }
			| property delimiters		{  $$ = $2; }
			| property DIGITS			{  $$ = $2; }
			| property PROPNAME			{  $$ = $2; }
			; 


delimiters    : COMMA				{  $$ = $1; }
			  | SEMICOL				{  $$ = $1; }	
			  | COLON				{  $$ = $1; }	
			  | FSLASH				{  $$ = $1; }
			  | PLUS				{  $$ = $1; }
			  | PERIOD				{  $$ = $1; }
			  | OPENPAR				{  $$ = $1; }
			  | CLOSEPAR			{  $$ = $1; }
			  | MULT				{  $$ = $1; }
			  ;

property_name   : CACHECONTROL     {  $$ = $1; }
				| ACCEPTENCODING   {  $$ = $1; }
				| ACCEPTLANG       {  $$ = $1; }
				| PROPNAME		   {  $$ = $1; }
				;

%%



int hperror(string s)
{
  //extern int hplineno;	// defined and maintained in lex.c
  //extern char *hptext;	// defined and maintained in lex.c
  
  //cerr << "PARSER sERROR: " << s << " at symbol \"" << hptext;
  //cerr << "\" on line " << hplineno << endl;
  return 1;
}

int hperror(char *s)
{
  return hperror(string(s));
}
