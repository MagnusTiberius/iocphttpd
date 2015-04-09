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

#ifndef YY_HP_HTTPPARSER_TAB_H_INCLUDED
# define YY_HP_HTTPPARSER_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef HPDEBUG
# if defined YYDEBUG
#  if YYDEBUG
#   define HPDEBUG 1
#  else
#   define HPDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define HPDEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined HPDEBUG */
#if HPDEBUG
extern int hpdebug;
#endif

/* Tokens.  */
#ifndef HPTOKENTYPE
# define HPTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum hptokentype {
     IDENTIFIER = 258,
     METHODACTION = 259,
     DIGIT = 260,
     HEADERELEM = 261,
     URLPATH = 262,
     METHODVER = 263,
     ID2 = 264,
     URLPARAMS = 265,
     HOST = 266,
     EQUAL = 267,
     CONNECTION = 268,
     USERAGENT = 269,
     QUESTION = 270,
     AMPERSAND = 271,
     ANYTYPE = 272,
     ACCEPT = 273,
     CACHECONTROL = 274,
     ACCEPTENCODING = 275,
     ACCEPTLANG = 276,
     DIGITS = 277,
     NEWLINE = 278,
     PROPNAME = 279,
     COMMA = 280,
     SEMICOL = 281,
     COLON = 282,
     FSLASH = 283,
     PERIOD = 284,
     OPENPAR = 285,
     CLOSEPAR = 286,
     PLUS = 287,
     MULT = 288
   };
#endif


#if ! defined HPSTYPE && ! defined HPSTYPE_IS_DECLARED
typedef union HPSTYPE
{
/* Line 2058 of yacc.c  */
#line 28 "httpparser.y"

  int		int_val;
  char*		identifier_tok;


/* Line 2058 of yacc.c  */
#line 104 "httpparser.tab.h"
} HPSTYPE;
# define HPSTYPE_IS_TRIVIAL 1
# define hpstype HPSTYPE /* obsolescent; will be withdrawn */
# define HPSTYPE_IS_DECLARED 1
#endif

extern HPSTYPE hplval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int hpparse (void *YYPARSE_PARAM);
#else
int hpparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int hpparse (void);
#else
int hpparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_HP_HTTPPARSER_TAB_H_INCLUDED  */
