
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INT = 258,
     VOID = 259,
     FLOAT = 260,
     CASE = 261,
     IF = 262,
     ELSE = 263,
     FOR = 264,
     ROF = 265,
     CONTINUE = 266,
     BREAK = 267,
     RETURN = 268,
     FI = 269,
     PROGRAM = 270,
     WRITE = 271,
     STRING = 272,
     READ = 273,
     _BEGIN = 274,
     END = 275,
     FUNCTION = 276,
     IDENTIFIER = 277,
     STRINGLITERAL = 278,
     INTLITERAL = 279,
     FLOATLITERAL = 280,
     ASSIGN_OP = 281,
     ADD_OP = 282,
     MINUS_OP = 283,
     MUL_OP = 284,
     DIV_OP = 285,
     EQ_OP = 286,
     NEQ_OP = 287,
     LE_OP = 288,
     GT_OP = 289,
     LBT_OP = 290,
     RBR_OP = 291,
     SC_OP = 292,
     LTE_OP = 293,
     GTE_OP = 294
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 37 "src/parser.y"

	float fval;
	int iVal;
	char * str;



/* Line 1676 of yacc.c  */
#line 99 "parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


