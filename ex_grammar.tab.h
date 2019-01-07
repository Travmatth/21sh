/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     WORD = 258,
     ASSIGNMENT_WORD = 259,
     NAME = 260,
     NEWLINE = 261,
     IO_NUMBER = 262,
     AND_IF = 263,
     OR_IF = 264,
     DSEMI = 265,
     DLESS = 266,
     DGREAT = 267,
     LESSAND = 268,
     GREATAND = 269,
     LESSGREAT = 270,
     DLESSDASH = 271,
     CLOBBER = 272,
     If = 273,
     Then = 274,
     Else = 275,
     Elif = 276,
     Fi = 277,
     Do = 278,
     Done = 279,
     Case = 280,
     Esac = 281,
     While = 282,
     Until = 283,
     For = 284,
     Lbrace = 285,
     Rbrace = 286,
     Bang = 287,
     In = 288
   };
#endif
/* Tokens.  */
#define WORD 258
#define ASSIGNMENT_WORD 259
#define NAME 260
#define NEWLINE 261
#define IO_NUMBER 262
#define AND_IF 263
#define OR_IF 264
#define DSEMI 265
#define DLESS 266
#define DGREAT 267
#define LESSAND 268
#define GREATAND 269
#define LESSGREAT 270
#define DLESSDASH 271
#define CLOBBER 272
#define If 273
#define Then 274
#define Else 275
#define Elif 276
#define Fi 277
#define Do 278
#define Done 279
#define Case 280
#define Esac 281
#define While 282
#define Until 283
#define For 284
#define Lbrace 285
#define Rbrace 286
#define Bang 287
#define In 288




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

