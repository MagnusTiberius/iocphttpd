/* Mini Calculator */
/* calc.y */

%{
#include "stdafx.h"
#include "AstHttp.h"


int yyerror(char *s);
int yylex(void);

%}

%union{
  int		int_val;
  char*		identifier_tok;
};

%start	input 

%token	<int_val>			DIGITS
%token	<identifier_tok>	IDENTIFIER METHODACTION DIGIT HEADERELEM URLPATH METHODVER ID2 URLPARAMS HOST
							CONNECTION USERAGENT
%type	<int_val>			exp
%type	<lpHttpdoc>	line 
%left	PLUS
%left	MULT

%%

input	: /* empty */
		| input line 
		;

line	: exp { cout << "Result: " << $1 << endl; }
		| line1 { printf("line1 seen\n"); }
		| line8
		| line17 
		| host { printf("HOST seen\n"); }
		| connection { printf("CONNECTION seen\n"); }
		| useragent { printf("USERAGENT seen\n"); }
		;

exp		: DIGITS	{ $$ = $1; }
		| exp PLUS exp	{ $$ = $1 + $3; }
		| exp MULT exp	{ $$ = $1 * $3; }
		;


line1	: METHODACTION    
		| line1 URLPATH		{ SetUrl($2, &g_Httpdoc); }
		| line1 URLPARAMS   { SetUrlParams($2, &g_Httpdoc); }
		| line1 METHODVER   { SetMethodVersion($2, &g_Httpdoc); }
		;

line8	: DIGITS IDENTIFIER { printf("line seen\n"); }
		;


line17  : ID2 
		| MULT
		| line17 ":" ID2
		| line17 "/" ID2
		| line17 PLUS ID2
		| line17 "," ID2
		| line17 ";" ID2
		| line17 "=" ID2
		| line17 "/" MULT
		;

host	: HOST
		| host ID2
		| host ":" ID2
		;

connection	: CONNECTION
			| connection ID2
			;

useragent   : USERAGENT
			| useragent ID2 
			| useragent MULT
			| useragent ":" ID2
			| useragent "/" ID2
			| useragent PLUS ID2
			| useragent "," ID2
			| useragent ";" ID2
			| useragent "=" ID2
			| useragent "/" MULT
			;

%%



int yyerror(string s)
{
  extern int yylineno;	// defined and maintained in lex.c
  extern char *yytext;	// defined and maintained in lex.c
  
  cerr << "PARSER sERROR: " << s << " at symbol \"" << yytext;
  cerr << "\" on line " << yylineno << endl;
  //exit(1);
  return 1;
}

int yyerror(char *s)
{
  return yyerror(string(s));
}
