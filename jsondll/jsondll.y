%define api.prefix jsondll

%{

#include "stdafx.h"


#define yyerror jsondllerror
#define yylex jsondlllex
#define yyparse jsondllparse
//#define yylval jsondlllval
#define yychar jsondllchar
#define yynerrs jsondllnerrs

//#define yystype jsondllstype


int jsondllerror(char *s);
int jsondllerror(string s);
int jsondlllex(void);



%}

%union{
	int		int_val;
	char*	str_val;
}

%start	input

%token	<str_val>	OPENCURLY CLOSECURLY OPENSQRBRK CLOSESQRBRK COMMASEP STRINGVAL COLONSEP ANUMBER
					CTRUE CFALSE CNULL INTNUM FRACTNUM FLOATNUM EXPONENT FOURHEX
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
			| anarray
			;

jsonobject  : OPENCURLY members CLOSECURLY { printf(" OPENCURLY members CLOSECURLY\n"); }
			;

members     : pair
			| members COMMASEP pair
            ;

anarray     : OPENSQRBRK arrmembers CLOSESQRBRK
            ;

arrmembers  : avalue
			| arrmembers COMMASEP avalue
            ;


pair		: STRINGVAL COLONSEP avalue
			;

avalue		: STRINGVAL		{ printf("STRINGVAL\n"); }
			| anumber		{ printf("anumber\n"); }
			| jsonobject    { printf("jsonobject\n"); }
			| anarray
			| CTRUE
			| CFALSE
			| CNULL
			| pair
			| FOURHEX
			;

anumber		: INTNUM				{ printf("INTNUM\n"); }
			| FRACTNUM				{ printf("FRACTNUM\n"); }
			| anumber  FRACTNUM		{ printf("anumber  FRACTNUM\n"); }
			| anumber  EXPONENT     { printf("anumber  EXPONENT\n"); }
			| anumber  INTNUM		{ printf("anumber  INTNUM\n"); }
			;

%%
int jsondllerror(string s)
{
	extern int jsondlllineno;	// defined and maintained in lex.c
	extern char *jsondlltext;	// defined and maintained in lex.c

	cerr << "ERROR: " << s << " at symbol \"" << jsondlltext;
	cerr << "\" on line " << jsondlllineno << endl;
	//exit(1);
	return 0;
}
int jsondllerror(char *s)
{
	return jsondllerror(string(s));
}




