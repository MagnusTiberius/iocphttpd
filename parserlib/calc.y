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

%token	<int_val>			DIGITS
%token	<identifier_tok>	IDENTIFIER METHODACTION DIGIT HEADERELEM URLPATH METHODVER ID2
%type	<int_val>			exp
%type	<identifier_tok>	line 
%left	PLUS
%left	MULT

%%

input	: /* empty */
		| input line 
		;

line	: exp { cout << "Result: " << $1 << endl; }
		| line1
		| line8
		| line17 
		;

exp		: DIGITS	{ $$ = $1; }
		| exp PLUS exp	{ $$ = $1 + $3; }
		| exp MULT exp	{ $$ = $1 * $3; }
		;


line1	: METHODACTION URLPATH METHODVER { printf("line1 seen\n"); }
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
