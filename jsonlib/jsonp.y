
%{

#include "stdafx.h"

#define yyerror jsonliberror
#define yylex jsonliblex
#define yyparse jsonlibparse
#define yylval jsonliblval
#define yychar jsonlibchar
#define yynerrs jsonlibnerrs
//#define yystype jsonlibstype


int jsonliberror(char *s);
int jsonliberror(string s);
int jsonliblex(void);



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
int jsonliberror(string s)
{
	extern int jsonliblineno;	// defined and maintained in lex.c
	extern char *jsonlibtext;	// defined and maintained in lex.c

	cerr << "ERROR: " << s << " at symbol \"" << jsonlibtext;
	cerr << "\" on line " << jsonliblineno << endl;
	//exit(1);
	return 0;
}
int jsonliberror(char *s)
{
	return jsonliberror(string(s));
}




