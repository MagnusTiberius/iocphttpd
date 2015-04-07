
%{
#include "stdafx.h"

int yyerror(char *s);
int yylex(void);

%}

%union{
	int		int_val;
	char*	str_val;
}

%start	input

%token	<str_val>	OPENCURLY CLOSECURLY OPENSQRBRK CLOSESQRBRK COMMASEP STRINGVAL COLONSEP ANUMBER
					CTRUE CFALSE CNULL
%token	<int_val>	INTEGER_LITERAL 
%type	<int_val>	exp
%left	PLUS
%left	MULT

%%

input		:		/* empty */
			| exp{ cout << "Result: " << $1 << endl; }
			| jsonvalue
			;

exp			:		INTEGER_LITERAL{ $$ = $1; }
			| exp PLUS exp{ $$ = $1 + $3; }
			| exp MULT exp{ $$ = $1 * $3; }
			;

jsonvalue	: jsonobject
			;

jsonobject  : OPENCURLY members CLOSECURLY
			;

members     : pair
			| members COMMASEP pair
            ;

anarray     : OPENSQRBRK avalue CLOSESQRBRK
            ;

pair		: STRINGVAL COLONSEP avalue
			;

avalue		: STRINGVAL
			| ANUMBER
			| jsonobject
			| anarray
			| CTRUE
			| CFALSE
			| CNULL
			;

%%
int yyerror(string s)
{
	extern int yylineno;	// defined and maintained in lex.c
	extern char *yytext;	// defined and maintained in lex.c

	cerr << "ERROR: " << s << " at symbol \"" << yytext;
	cerr << "\" on line " << yylineno << endl;
	exit(1);
}
int yyerror(char *s)
{
	return yyerror(string(s));
}
int yylex()
{
	return 0;
}