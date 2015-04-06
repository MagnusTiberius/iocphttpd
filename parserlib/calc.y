/* Mini Calculator */
/* calc.y */

%{
#include "stdafx.h"





int yyerror(char *s);
int yylex(void);



%}

%union{
  int		int_val;
  char*		identifier_tok;
};

%start	input 

%token	<int_val>	INTEGER_LITERAL
%token	<identifier_tok>	IDENTIFIER METHOD
%type	<int_val>	exp
%left	PLUS
%left	MULT

%%

input:		/* empty */
		| exp	{ cout << "Result: " << $1 << endl; }
		;

exp:		INTEGER_LITERAL	{ $$ = $1; }
		| exp PLUS exp	{ $$ = $1 + $3; }
		| exp MULT exp	{ $$ = $1 * $3; }
		;

%%



int yyerror(string s)
{
  extern int yylineno;	// defined and maintained in lex.c
  extern char *yytext;	// defined and maintained in lex.c
  
  cerr << "ERROR: " << s << " at symbol \"" << yytext;
  cerr << "\" on line " << yylineno << endl;
  //exit(1);
  return 1;
}

int yyerror(char *s)
{
  return yyerror(string(s));
}
