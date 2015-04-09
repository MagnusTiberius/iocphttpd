/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_JSONDLL_JSONDLL_TAB_H_INCLUDED
# define YY_JSONDLL_JSONDLL_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef JSONDLLDEBUG
# if defined YYDEBUG
#  if YYDEBUG
#   define JSONDLLDEBUG 1
#  else
#   define JSONDLLDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define JSONDLLDEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined JSONDLLDEBUG */
#if JSONDLLDEBUG
extern int jsondlldebug;
#endif

/* Tokens.  */
#ifndef JSONDLLTOKENTYPE
# define JSONDLLTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum jsondlltokentype {
     OPENCURLY = 258,
     CLOSECURLY = 259,
     OPENSQRBRK = 260,
     CLOSESQRBRK = 261,
     COMMASEP = 262,
     STRINGVAL = 263,
     COLONSEP = 264,
     ANUMBER = 265,
     CTRUE = 266,
     CFALSE = 267,
     CNULL = 268,
     INTNUM = 269,
     FRACTNUM = 270,
     FLOATNUM = 271,
     EXPONENT = 272,
     FOURHEX = 273,
     INTEGER_LITERAL = 274,
     PLUS = 275,
     MULT = 276
   };
#endif


#if ! defined JSONDLLSTYPE && ! defined JSONDLLSTYPE_IS_DECLARED
typedef union JSONDLLSTYPE
{
/* Line 2058 of yacc.c  */
#line 26 "jsondll.y"

	int		int_val;
	char*	str_val;


/* Line 2058 of yacc.c  */
#line 92 "jsondll.tab.h"
} JSONDLLSTYPE;
# define JSONDLLSTYPE_IS_TRIVIAL 1
# define jsondllstype JSONDLLSTYPE /* obsolescent; will be withdrawn */
# define JSONDLLSTYPE_IS_DECLARED 1
#endif

extern JSONDLLSTYPE jsondlllval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int jsondllparse (void *YYPARSE_PARAM);
#else
int jsondllparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int jsondllparse (void);
#else
int jsondllparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_JSONDLL_JSONDLL_TAB_H_INCLUDED  */
