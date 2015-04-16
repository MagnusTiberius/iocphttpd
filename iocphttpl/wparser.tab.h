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

#ifndef YY_WW_WPARSER_TAB_H_INCLUDED
# define YY_WW_WPARSER_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef WWDEBUG
# if defined YYDEBUG
#  if YYDEBUG
#   define WWDEBUG 1
#  else
#   define WWDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define WWDEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined WWDEBUG */
#if WWDEBUG
extern int wwdebug;
#endif

/* Tokens.  */
#ifndef WWTOKENTYPE
# define WWTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum wwtokentype {
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


#if ! defined WWSTYPE && ! defined WWSTYPE_IS_DECLARED
typedef union WWSTYPE
{
/* Line 2058 of yacc.c  */
#line 25 "wparser.y"

  int		int_val;
  char*		identifier_tok;


/* Line 2058 of yacc.c  */
#line 104 "wparser.tab.h"
} WWSTYPE;
# define WWSTYPE_IS_TRIVIAL 1
# define wwstype WWSTYPE /* obsolescent; will be withdrawn */
# define WWSTYPE_IS_DECLARED 1
#endif


#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int wwparse (void *YYPARSE_PARAM);
#else
int wwparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int wwparse (void);
#else
int wwparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_WW_WPARSER_TAB_H_INCLUDED  */
