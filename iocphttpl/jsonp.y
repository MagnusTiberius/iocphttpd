%define api.prefix jp
%define api.pure
%{

#include "stdafx.h"
#include "jsonp.tab.h"

//#define yyerror jperror
//#define yylex jplex
//#define yyparse jpparse
//#define yychar jpchar
//#define yynerrs jpnerrs

//#define YYPARSE_PARAM jpparm
//#define YYLEX_PARAM jpparm



int jperror(char *s);
int jperror(string s);

typedef void* yyscan_t;
//int jplex (YYSTYPE * yylval_param ,yyscan_t yyscanner);
int jplex (YYSTYPE * yylval_param);


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
%left	JPPLUS
%left	JPMULT

%%

input		:		/* empty */
			| exp{ cout << "Result: " << $1 << endl; }
			| jsonvalue
			;

exp			:		INTEGER_LITERAL{ $$ = $1; }
			| exp JPPLUS exp{ $$ = $1 + $3; }
			| exp JPMULT exp{ $$ = $1 * $3; }
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
int jperror(string s)
{
	extern int jplineno;	// defined and maintained in lex.c
	extern char *jptext;	// defined and maintained in lex.c

	cerr << "ERROR: " << s << " at symbol \"" << jptext;
	cerr << "\" on line " << jplineno << endl;
	//exit(1);
	return 0;
}
int jperror(char *s)
{
	return jperror(string(s));
}




