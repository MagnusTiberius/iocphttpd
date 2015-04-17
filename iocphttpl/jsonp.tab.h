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

#ifndef YY_JP_JSONP_TAB_H_INCLUDED
# define YY_JP_JSONP_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef JPDEBUG
# if defined YYDEBUG
#  if YYDEBUG
#   define JPDEBUG 1
#  else
#   define JPDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define JPDEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined JPDEBUG */
#if JPDEBUG
extern int jpdebug;
#endif

/* Tokens.  */
#ifndef JPTOKENTYPE
# define JPTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum jptokentype {
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
     JPPLUS = 275,
     JPMULT = 276
   };
#endif


#if ! defined JPSTYPE && ! defined JPSTYPE_IS_DECLARED
typedef union JPSTYPE
{
/* Line 2058 of yacc.c  */
#line 33 "jsonp.y"

	int		int_val;
	char*	str_val;


/* Line 2058 of yacc.c  */
#line 92 "jsonp.tab.h"
} JPSTYPE;
# define JPSTYPE_IS_TRIVIAL 1
# define jpstype JPSTYPE /* obsolescent; will be withdrawn */
# define JPSTYPE_IS_DECLARED 1
#endif


#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int jpparse (void *YYPARSE_PARAM);
#else
int jpparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int jpparse (void);
#else
int jpparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_JP_JSONP_TAB_H_INCLUDED  */
