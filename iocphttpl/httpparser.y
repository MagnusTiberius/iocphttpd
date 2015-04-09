%define api.prefix hp

%{
#include "stdafx.h"
#include "httpparserast.h"

#define yyerror hperror
#define yylex hplex

//#define yyparse hpparse
//#define yylval hplval
//#define yychar hpchar
//#define yynerrs hpnerrs
//#define yystype hpstype
//#define YYSTYPE PARSERLIBSTYPE


int hperror(string s);
int hperror(char *s);
int hplex(void);

//int yyerror(char *s);
//int yyerror(string s);
//int yylex(void);

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
%type	<identifier_tok>	delimiters property_name property

%type	<lpHttpdoc>	line 
/* %left	PLUS
%left	MULT */

%%

input	: /* empty */
		| input line 
		;

line	: line1 NEWLINE			{ printf("line1 seen\n"); }
		| property NEWLINE		{ printf("property_item seen\n"); }
		| NEWLINE				{ printf("content should follow after this marker.\n"); }
		;


line1	: METHODACTION		{ SetMethodAction($1, &g_Httpdoc); }
		| line1 URLPATH		{ SetUrl($2, &g_Httpdoc); }
		| line1 urlparams   { printf("urlparams whole seen\n"); }
		| line1 METHODVER   { SetMethodVersion($2, &g_Httpdoc); }
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
  extern int hplineno;	// defined and maintained in lex.c
  extern char *hptext;	// defined and maintained in lex.c
  
  cerr << "PARSER sERROR: " << s << " at symbol \"" << hptext;
  cerr << "\" on line " << hplineno << endl;
  //exit(1);
  return 1;
}

int hperror(char *s)
{
  return hperror(string(s));
}

/*
int hplex(void)
{
	return 0;
}

int yylex(void)
{
	return 0;
}

int yyerror(string s)
{
  extern int hplineno;	// defined and maintained in lex.c
  extern char *hptext;	// defined and maintained in lex.c
  
  cerr << "PARSER sERROR: " << s << " at symbol \"" << hptext;
  cerr << "\" on line " << hplineno << endl;
  //exit(1);
  return 1;
}

int yyerror(char *s)
{
  return yyerror(string(s));
}
*/