/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

#ifndef YY_FRONTEND_VERILOG_YY_VERILOG_TAB_H_INCLUDED
# define YY_FRONTEND_VERILOG_YY_VERILOG_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef FRONTEND_VERILOG_YYDEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define FRONTEND_VERILOG_YYDEBUG 1
#  else
#   define FRONTEND_VERILOG_YYDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define FRONTEND_VERILOG_YYDEBUG 1
# endif /* ! defined YYDEBUG */
#endif  /* ! defined FRONTEND_VERILOG_YYDEBUG */
#if FRONTEND_VERILOG_YYDEBUG
extern int frontend_verilog_yydebug;
#endif
/* "%code requires" blocks.  */
#line 232 "/home/haoqs/code/open-edi/src/parser/verilog/frontends/verilog/verilog_parser.y" /* yacc.c:1909  */

#include <map>
#include <string>
#include "frontends/verilog/verilog_frontend.h"

#line 58 "verilog.tab.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef FRONTEND_VERILOG_YYTOKENTYPE
# define FRONTEND_VERILOG_YYTOKENTYPE
  enum frontend_verilog_yytokentype
  {
    TOK_STRING = 258,
    TOK_ID = 259,
    TOK_CONSTVAL = 260,
    TOK_REALVAL = 261,
    TOK_PRIMITIVE = 262,
    TOK_SVA_LABEL = 263,
    TOK_SPECIFY_OPER = 264,
    TOK_MSG_TASKS = 265,
    TOK_BASE = 266,
    TOK_BASED_CONSTVAL = 267,
    TOK_UNBASED_UNSIZED_CONSTVAL = 268,
    TOK_USER_TYPE = 269,
    TOK_PKG_USER_TYPE = 270,
    TOK_ASSERT = 271,
    TOK_ASSUME = 272,
    TOK_RESTRICT = 273,
    TOK_COVER = 274,
    TOK_FINAL = 275,
    ATTR_BEGIN = 276,
    ATTR_END = 277,
    DEFATTR_BEGIN = 278,
    DEFATTR_END = 279,
    TOK_MODULE = 280,
    TOK_ENDMODULE = 281,
    TOK_PARAMETER = 282,
    TOK_LOCALPARAM = 283,
    TOK_DEFPARAM = 284,
    TOK_PACKAGE = 285,
    TOK_ENDPACKAGE = 286,
    TOK_PACKAGESEP = 287,
    TOK_INTERFACE = 288,
    TOK_ENDINTERFACE = 289,
    TOK_MODPORT = 290,
    TOK_VAR = 291,
    TOK_WILDCARD_CONNECT = 292,
    TOK_INPUT = 293,
    TOK_OUTPUT = 294,
    TOK_INOUT = 295,
    TOK_WIRE = 296,
    TOK_WAND = 297,
    TOK_WOR = 298,
    TOK_REG = 299,
    TOK_LOGIC = 300,
    TOK_INTEGER = 301,
    TOK_SIGNED = 302,
    TOK_ASSIGN = 303,
    TOK_PLUS_ASSIGN = 304,
    TOK_ALWAYS = 305,
    TOK_INITIAL = 306,
    TOK_ALWAYS_FF = 307,
    TOK_ALWAYS_COMB = 308,
    TOK_ALWAYS_LATCH = 309,
    TOK_BEGIN = 310,
    TOK_END = 311,
    TOK_IF = 312,
    TOK_ELSE = 313,
    TOK_FOR = 314,
    TOK_WHILE = 315,
    TOK_REPEAT = 316,
    TOK_DPI_FUNCTION = 317,
    TOK_POSEDGE = 318,
    TOK_NEGEDGE = 319,
    TOK_OR = 320,
    TOK_AUTOMATIC = 321,
    TOK_CASE = 322,
    TOK_CASEX = 323,
    TOK_CASEZ = 324,
    TOK_ENDCASE = 325,
    TOK_DEFAULT = 326,
    TOK_FUNCTION = 327,
    TOK_ENDFUNCTION = 328,
    TOK_TASK = 329,
    TOK_ENDTASK = 330,
    TOK_SPECIFY = 331,
    TOK_IGNORED_SPECIFY = 332,
    TOK_ENDSPECIFY = 333,
    TOK_SPECPARAM = 334,
    TOK_SPECIFY_AND = 335,
    TOK_IGNORED_SPECIFY_AND = 336,
    TOK_GENERATE = 337,
    TOK_ENDGENERATE = 338,
    TOK_GENVAR = 339,
    TOK_REAL = 340,
    TOK_SYNOPSYS_FULL_CASE = 341,
    TOK_SYNOPSYS_PARALLEL_CASE = 342,
    TOK_SUPPLY0 = 343,
    TOK_SUPPLY1 = 344,
    TOK_TO_SIGNED = 345,
    TOK_TO_UNSIGNED = 346,
    TOK_POS_INDEXED = 347,
    TOK_NEG_INDEXED = 348,
    TOK_PROPERTY = 349,
    TOK_ENUM = 350,
    TOK_TYPEDEF = 351,
    TOK_RAND = 352,
    TOK_CONST = 353,
    TOK_CHECKER = 354,
    TOK_ENDCHECKER = 355,
    TOK_EVENTUALLY = 356,
    TOK_INCREMENT = 357,
    TOK_DECREMENT = 358,
    TOK_UNIQUE = 359,
    TOK_PRIORITY = 360,
    TOK_STRUCT = 361,
    TOK_PACKED = 362,
    TOK_UNSIGNED = 363,
    TOK_INT = 364,
    TOK_BYTE = 365,
    TOK_SHORTINT = 366,
    TOK_UNION = 367,
    TOK_OR_ASSIGN = 368,
    TOK_XOR_ASSIGN = 369,
    TOK_AND_ASSIGN = 370,
    TOK_SUB_ASSIGN = 371,
    OP_LOR = 372,
    OP_LAND = 373,
    OP_NOR = 374,
    OP_XNOR = 375,
    OP_NAND = 376,
    OP_EQ = 377,
    OP_NE = 378,
    OP_EQX = 379,
    OP_NEX = 380,
    OP_LE = 381,
    OP_GE = 382,
    OP_SHL = 383,
    OP_SHR = 384,
    OP_SSHL = 385,
    OP_SSHR = 386,
    OP_POW = 387,
    OP_CAST = 388,
    UNARY_OPS = 389,
    FAKE_THEN = 390
  };
#endif

/* Value type.  */
#if ! defined FRONTEND_VERILOG_YYSTYPE && ! defined FRONTEND_VERILOG_YYSTYPE_IS_DECLARED

union FRONTEND_VERILOG_YYSTYPE
{
#line 238 "/home/haoqs/code/open-edi/src/parser/verilog/frontends/verilog/verilog_parser.y" /* yacc.c:1909  */

	std::string *string;
	struct YOSYS_NAMESPACE_PREFIX AST::AstNode *ast;
	YOSYS_NAMESPACE_PREFIX dict<YOSYS_NAMESPACE_PREFIX RTLIL::IdString, YOSYS_NAMESPACE_PREFIX AST::AstNode*> *al;
	struct specify_target *specify_target_ptr;
	struct specify_triple *specify_triple_ptr;
	struct specify_rise_fall *specify_rise_fall_ptr;
	bool boolean;
	char ch;

#line 217 "verilog.tab.h" /* yacc.c:1909  */
};

typedef union FRONTEND_VERILOG_YYSTYPE FRONTEND_VERILOG_YYSTYPE;
# define FRONTEND_VERILOG_YYSTYPE_IS_TRIVIAL 1
# define FRONTEND_VERILOG_YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined FRONTEND_VERILOG_YYLTYPE && ! defined FRONTEND_VERILOG_YYLTYPE_IS_DECLARED
typedef struct FRONTEND_VERILOG_YYLTYPE FRONTEND_VERILOG_YYLTYPE;
struct FRONTEND_VERILOG_YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define FRONTEND_VERILOG_YYLTYPE_IS_DECLARED 1
# define FRONTEND_VERILOG_YYLTYPE_IS_TRIVIAL 1
#endif



int frontend_verilog_yyparse (void);

#endif /* !YY_FRONTEND_VERILOG_YY_VERILOG_TAB_H_INCLUDED  */
