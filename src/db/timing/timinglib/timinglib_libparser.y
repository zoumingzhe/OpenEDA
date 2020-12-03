%code requires {
	
/******************************************************************************
    Copyright (c) 1996-2005 Synopsys, Inc.    ALL RIGHTS RESERVED

  The contents of this file are subject to the restrictions and limitations
  set forth in the SYNOPSYS Open Source License Version 1.0  (the "License");
  you may not use this file except in compliance with such restrictions
  and limitations. You may obtain instructions on how to receive a copy of
  the License at

  http://www.synopsys.com/partners/tapin/tapinprogram.html.

  Software distributed by Original Contributor under the License is
  distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either
  expressed or implied. See the License for the specific language governing
  rights and limitations under the License.

******************************************************************************/

#include <string.h>
#include "timinglib_si2dr.h"
#include "timinglib_libanalysis.h"

struct xnumber
{
	int type; /* 0=int, 1=float */
	int intnum;
	double floatnum;
};
typedef struct xnumber xnumber;

typedef void* yyscan_t;

}

%code provides {
#undef  YY_DECL
#define YY_DECL int yylex(YYSTYPE *yylval_param, \
   yyscan_t yyscanner, Timinglib::LibAnalysis &libAnalysis)
YY_DECL;

void yyerror(yyscan_t scanner, Timinglib::LibAnalysis &, const char *str);
}

%{
#include "timinglib_structs.h"
#include "timinglib_libanalysis.h"

#define SD (libAnalysis.getScandata())
#define STRTAB (libAnalysis.getStrtab())

%}

%union {
    char *str;
    xnumber num;
    Timinglib::timinglib_group *group;
    Timinglib::timinglib_attribute *attr;
    Timinglib::timinglib_attribute_value *val;
    Timinglib::timinglib_define *def;
    Timinglib::timinglib_head *head;
    Timinglib::si2drExprT *expr;
}

%define api.pure full
%parse-param {yyscan_t yyscanner}
%parse-param { Timinglib::LibAnalysis &libAnalysis }
%lex-param {yyscan_t yyscanner}
%lex-param   { Timinglib::LibAnalysis &libAnalysis }

%token COMMA SEMI LPAR RPAR LCURLY RCURLY COLON KW_DEFINE KW_DEFINE_GROUP KW_TRUE KW_FALSE PLUS MINUS MULT DIV EQ
%token UNARY

%token <num> NUM
%token <str> STRING IDENT

%left PLUS MINUS
%left MULT DIV
%right UNARY
%left LPAR RPAR

%type <group>     group file statements statement
%type <def>       define define_group
%type <val> param_list attr_val attr_val_expr
%type <str>   s_or_i
%type <head> head
%type <expr> expr

%%

file : {SD->lineno = 1; SD->syntax_errors= 0;} group {}
     ;

group	: head LCURLY {libAnalysis.pushGroup($1);} statements RCURLY {libAnalysis.popGroup($1);}
        | head LCURLY {libAnalysis.pushGroup($1);} RCURLY {libAnalysis.popGroup($1);}
		;


statements 	: statement {}
		 	| statements statement  {}
			;


statement 	: simple_attr {}
			| complex_attr {}
			| define {}
			| define_group {}
			| group  {}
			;

simple_attr	: IDENT COLON attr_val_expr { libAnalysis.makeSimple($1,$3);} SEMI
			| IDENT COLON attr_val_expr { libAnalysis.makeSimple($1,$3);}
            | IDENT EQ    attr_val_expr { libAnalysis.makeSimple($1,$3);libAnalysis.si2drSimpleAttrSetIsVar(SD->curr_attr,&SD->err); } SEMI
			;

complex_attr 	: head  SEMI  {libAnalysis.makeComplex($1);}
                | head  {libAnalysis.makeComplex($1);}
				;

head	: IDENT LPAR {SD->tight_colon_ok =1;} param_list RPAR { $$ = (Timinglib::timinglib_head*)calloc(sizeof(Timinglib::timinglib_head),1); $$->name = $1; $$->list = $4; $$->lineno = SD->lineno;$$->filename = SD->curr_file; SD->tight_colon_ok =0;}
        | IDENT LPAR RPAR            { $$ = (Timinglib::timinglib_head*)calloc(sizeof(Timinglib::timinglib_head),1); $$->name = $1; $$->list = 0; $$->lineno = SD->lineno;$$->filename = SD->curr_file;}
		;


param_list  : attr_val {$$=$1;}
            | param_list COMMA attr_val
              {
				  Timinglib::timinglib_attribute_value *v;
				  for(v=$1; v; v=v->next)
				  {
					  if(!v->next)
					  {
						  v->next = $3;
						  break;
					  }
				  }
				  $$ = $1;
			  }
            | param_list attr_val
              {
				  Timinglib::timinglib_attribute_value *v;
				  for(v=$1; v; v=v->next)
				  {
					  if(!v->next)
					  {
						  v->next = $2;
						  break;
					  }
				  }
				  $$ = $1;
			  }
			;

define 	: KW_DEFINE LPAR s_or_i COMMA s_or_i COMMA s_or_i RPAR SEMI  
		{SD->curr_def = libAnalysis.si2drGroupCreateDefine(SD->gs[SD->gsindex-1],$3,$5,libAnalysis.convertVt($7),&SD->err);libAnalysis.si2drObjectSetLineNo(SD->curr_def,SD->lineno,&SD->err);libAnalysis.si2drObjectSetFileName(SD->curr_def,SD->curr_file,&SD->err);
		if( SD->token_comment_buf[0] ) { libAnalysis.si2drDefineSetComment(SD->curr_def, SD->token_comment_buf,&SD->err); SD->token_comment_buf[0]=0;} 
		if( SD->token_comment_buf2[0] )	{strcpy(SD->token_comment_buf, SD->token_comment_buf2);SD->token_comment_buf2[0] = 0;}
		SD->tok_encountered = 0;
		}
		;


define_group : KW_DEFINE_GROUP LPAR s_or_i COMMA s_or_i RPAR SEMI
            {SD->curr_def = libAnalysis.si2drGroupCreateDefine(SD->gs[SD->gsindex-1],$3,$5,Timinglib::kSI2DR_UNDEFINED_VALUETYPE,&SD->err);libAnalysis.si2drObjectSetLineNo(SD->curr_def,SD->lineno,&SD->err);libAnalysis.si2drObjectSetFileName(SD->curr_def,SD->curr_file,&SD->err);
			if( SD->token_comment_buf[0] ) { libAnalysis.si2drDefineSetComment(SD->curr_def, SD->token_comment_buf,&SD->err); SD->token_comment_buf[0]=0;} 
			if( SD->token_comment_buf2[0] )	{strcpy(SD->token_comment_buf, SD->token_comment_buf2);SD->token_comment_buf2[0] = 0;}
			SD->tok_encountered = 0;
			}
		;

s_or_i  : STRING {$$ = $1;}
		| IDENT {$$=$1;}
		;

attr_val : NUM { $$= (Timinglib::timinglib_attribute_value*)calloc(sizeof(Timinglib::timinglib_attribute_value),1);
	 		/* I get back a floating point number... not a string, and I have to 
        		tell if it's an integer, without using any math lib funcs? */
				if( $1.type == 0 )
				{
					$$->type = Timinglib::kTIMINGLIB__VAL_INT;
					$$->u.int_val = (int) $1.intnum;
				}
				else
				{
					$$->type = Timinglib::kTIMINGLIB__VAL_DOUBLE;
					$$->u.double_val = $1.floatnum;
				}
            }
		 |  s_or_i
               {
				   $$= (Timinglib::timinglib_attribute_value*)calloc(sizeof(Timinglib::timinglib_attribute_value),1);
				   $$->type = Timinglib::kTIMINGLIB__VAL_STRING;
				   $$->u.string_val = $1;
			   }
		 |  s_or_i  COLON s_or_i 
               {
				   char *x;
				   $$= (Timinglib::timinglib_attribute_value*)calloc(sizeof(Timinglib::timinglib_attribute_value),1);
				   $$->type = Timinglib::kTIMINGLIB__VAL_STRING;
				   x = (char*)alloca(strlen($1) + strlen($3) + 2); /* get a scratchpad */
				   sprintf(x, "%s:%s", $1,$3);
				   $$->u.string_val = STRTAB->timinglibStrtableEnterString(SD->master_string_table, x); /* scratchpad goes away after this */
			   }
         | KW_TRUE
               {
				   $$= (Timinglib::timinglib_attribute_value*)calloc(sizeof(Timinglib::timinglib_attribute_value),1);
				   $$->type = Timinglib::kTIMINGLIB__VAL_BOOLEAN;
				   $$->u.bool_val = 1;
			   }
         | KW_FALSE
               {
				   $$= (Timinglib::timinglib_attribute_value*)calloc(sizeof(Timinglib::timinglib_attribute_value),1);
				   $$->type = Timinglib::kTIMINGLIB__VAL_BOOLEAN;
				   $$->u.bool_val = 0;
			   }
		 ;

attr_val_expr : /* NUM { $$= (Timinglib::timinglib_attribute_value*)calloc(sizeof(Timinglib::timinglib_attribute_value),1);
                 $$->type = Timinglib::kTIMINGLIB__VAL_DOUBLE;
				 $$->u.double_val = $1;
               }  I'm going to put nums thru the expr stuff

			   |  */ STRING
               {
				   $$= (Timinglib::timinglib_attribute_value*)calloc(sizeof(Timinglib::timinglib_attribute_value),1);
				   $$->type = Timinglib::kTIMINGLIB__VAL_STRING;
				   $$->u.string_val = $1;
			   }
         | KW_TRUE
               {
				   $$= (Timinglib::timinglib_attribute_value*)calloc(sizeof(Timinglib::timinglib_attribute_value),1);
				   $$->type = Timinglib::kTIMINGLIB__VAL_BOOLEAN;
				   $$->u.bool_val = 1;
			   }
         | KW_FALSE
               {
				   $$= (Timinglib::timinglib_attribute_value*)calloc(sizeof(Timinglib::timinglib_attribute_value),1);
				   $$->type = Timinglib::kTIMINGLIB__VAL_BOOLEAN;
				   $$->u.bool_val = 0;
			   }
         | expr
               {
				   /* all the if/else if's are to reduce the total number of exprs to a minimum */
				   if( $1->type == Timinglib::kSI2DR_EXPR_VAL && $1->valuetype == Timinglib::kSI2DR_FLOAT64 && !$1->left && !$1->right )
				   {
					   $$= (Timinglib::timinglib_attribute_value*)calloc(sizeof(Timinglib::timinglib_attribute_value),1);
					   $$->type = Timinglib::kTIMINGLIB__VAL_DOUBLE;
					   $$->u.double_val = $1->u.d;
					   /* printf("EXPR->double %g \n", $1->u.d); */
					   libAnalysis.si2drExprDestroy($1,&SD->err);
				   }
				   else if( $1->type == Timinglib::kSI2DR_EXPR_VAL && $1->valuetype == Timinglib::kSI2DR_INT32 && !$1->left && !$1->right )
				   {
					   $$= (Timinglib::timinglib_attribute_value*)calloc(sizeof(Timinglib::timinglib_attribute_value),1);
					   $$->type = Timinglib::kTIMINGLIB__VAL_INT;
					   $$->u.int_val = $1->u.i;
					   /* printf("EXPR->int - %d \n", $1->u.i); */
					   libAnalysis.si2drExprDestroy($1,&SD->err);
				   }
				   else if( $1->type == Timinglib::kSI2DR_EXPR_OP_SUB && $1->left && !$1->right 
							&& $1->left->valuetype == Timinglib::kSI2DR_FLOAT64 && !$1->left->left && !$1->left->right )
				   {
					   $$= (Timinglib::timinglib_attribute_value*)calloc(sizeof(Timinglib::timinglib_attribute_value),1);
					   $$->type = Timinglib::kTIMINGLIB__VAL_DOUBLE;
					   $$->u.double_val = -$1->left->u.d;
					   /* printf("EXPR->double - %g \n", $1->u.d); */
					   libAnalysis.si2drExprDestroy($1,&SD->err);
				   }
				   else if( $1->type == Timinglib::kSI2DR_EXPR_OP_SUB && $1->left && !$1->right 
							&& $1->left->valuetype == Timinglib::kSI2DR_INT32 && !$1->left->left && !$1->left->right )
				   {
					   $$= (Timinglib::timinglib_attribute_value*)calloc(sizeof(Timinglib::timinglib_attribute_value),1);
					   $$->type = Timinglib::kTIMINGLIB__VAL_INT;
					   $$->u.int_val = -$1->left->u.i;
					   /* printf("EXPR->double - %g \n", $1->u.d); */
					   libAnalysis.si2drExprDestroy($1,&SD->err);
				   }
				   else if( $1->type == Timinglib::kSI2DR_EXPR_OP_ADD && $1->left && !$1->right 
							&& $1->left->valuetype == Timinglib::kSI2DR_FLOAT64 && !$1->left->left && !$1->left->right )
				   {
					   $$= (Timinglib::timinglib_attribute_value*)calloc(sizeof(Timinglib::timinglib_attribute_value),1);
					   $$->type = Timinglib::kTIMINGLIB__VAL_DOUBLE;
					   $$->u.double_val = $1->left->u.d;
					   /* printf("EXPR->double + %g \n", $1->u.d); */
					   libAnalysis.si2drExprDestroy($1,&SD->err);
				   }
				   else if( $1->type == Timinglib::kSI2DR_EXPR_OP_ADD && $1->left && !$1->right 
							&& $1->left->valuetype == Timinglib::kSI2DR_INT32 && !$1->left->left && !$1->left->right )
				   {
					   $$= (Timinglib::timinglib_attribute_value*)calloc(sizeof(Timinglib::timinglib_attribute_value),1);
					   $$->type = Timinglib::kTIMINGLIB__VAL_INT;
					   $$->u.int_val = $1->left->u.i;
					   /* printf("EXPR->double + %g \n", $1->u.d); */
					   libAnalysis.si2drExprDestroy($1,&SD->err);
				   }
				   else if( $1->type == Timinglib::kSI2DR_EXPR_VAL && $1->valuetype == Timinglib::kSI2DR_STRING && !$1->left && !$1->right 
						/* && ( strcmp($1->u.s,"VDD") && strcmp($1->u.s,"VSS")  )  I'm getting complaints about excluding VSS and VDD, so.... they'll not be exprs any more it they are all alone */ )
				   {  /* uh, do we need to exclude all but VSS and VDD ? no! */
					/* The only way a string would turned into an expr, is if it were parsed
						as an IDENT -- so no quotes will ever be seen... */
					   $$= (Timinglib::timinglib_attribute_value*)calloc(sizeof(Timinglib::timinglib_attribute_value),1);
					   $$->type = Timinglib::kTIMINGLIB__VAL_STRING;
					   $$->u.string_val = $1->u.s;
					   /* printf("EXPR->string = %s \n", $1->u.s); */
					   libAnalysis.si2drExprDestroy($1,&SD->err);
				   }
				   else
				   {
					   $$= (Timinglib::timinglib_attribute_value*)calloc(sizeof(Timinglib::timinglib_attribute_value),1);
					   $$->type = Timinglib::kTIMINGLIB__VAL_EXPR;
					   $$->u.expr_val = $1;
					   /* printf("left EXPR alone\n"); */
				   }
			   }
		 ;

expr     : expr PLUS expr
           {
			   Timinglib::si2drErrorT err;
			   
			   $$ = libAnalysis.si2drCreateExpr(Timinglib::kSI2DR_EXPR_OP_ADD,&err);
			   $$->left = $1;
			   $$->right = $3;
           }
         | expr MINUS expr
           {
			   Timinglib::si2drErrorT err;
			   
			   $$ = libAnalysis.si2drCreateExpr(Timinglib::kSI2DR_EXPR_OP_SUB,&err);
			   $$->left = $1;
			   $$->right = $3;
           }
         | expr MULT expr
           {
			   Timinglib::si2drErrorT err;
			   
			   $$ = libAnalysis.si2drCreateExpr(Timinglib::kSI2DR_EXPR_OP_MUL,&err);
			   $$->left = $1;
			   $$->right = $3;
           }
         | expr DIV  expr
           {
			   Timinglib::si2drErrorT err;
			   
			   $$ = libAnalysis.si2drCreateExpr(Timinglib::kSI2DR_EXPR_OP_DIV,&err);
			   $$->left = $1;
			   $$->right = $3;
           }
         | LPAR expr RPAR
           {
			   Timinglib::si2drErrorT err;
			   
			   $$ = libAnalysis.si2drCreateExpr(Timinglib::kSI2DR_EXPR_OP_PAREN,&err);
			   $$->left = $2;
			   $$->right = 0;
           }
         | MINUS expr %prec UNARY
           {
			   Timinglib::si2drErrorT err;
			   
			   $$ = libAnalysis.si2drCreateExpr(Timinglib::kSI2DR_EXPR_OP_SUB,&err);
			   $$->left = $2;
			   $$->right = 0;
           }
         | PLUS  expr %prec UNARY
           {
			   Timinglib::si2drErrorT err;
			   
			   $$ = libAnalysis.si2drCreateExpr(Timinglib::kSI2DR_EXPR_OP_ADD,&err);
			   $$->left = $2;
			   $$->right = 0;
           }
         | NUM
           {
			   Timinglib::si2drErrorT err;
			   $$ = libAnalysis.si2drCreateExpr(Timinglib::kSI2DR_EXPR_VAL,&err);
			   $$->left = 0;
			   $$->right = 0;
			   if( $1.type == 0 )
				{
					$$->valuetype = Timinglib::kSI2DR_INT32;
					$$->u.i = $1.intnum;
				}
				else
				{
					$$->valuetype = Timinglib::kSI2DR_FLOAT64;
					$$->u.d = $1.floatnum;
				}
			   
           }
         | IDENT
           {
			   Timinglib::si2drErrorT err;
			   
			   $$ = libAnalysis.si2drCreateExpr(Timinglib::kSI2DR_EXPR_VAL,&err);
			   $$->valuetype = Timinglib::kSI2DR_STRING;
			   $$->u.s = $1;
			   $$->left = 0;
			   $$->right = 0;
           }
         ;
%%


void yyerror(yyscan_t scanner, Timinglib::LibAnalysis &libAnalysis, const char *str)
{
	Timinglib::scandata *sd = libAnalysis.getScandata();
    Timinglib::si2drErrorT err;
	
	Timinglib::si2drMessageHandlerT MsgPrinter;

	MsgPrinter = libAnalysis.si2drPIGetMessageHandler(&err); 

	sprintf(SD->PB,"%s file: %s, line number %d", str, SD->curr_file, SD->lineno);
	(libAnalysis.*MsgPrinter)(Timinglib::kSI2DR_SEVERITY_ERR, Timinglib::kSI2DR_SYNTAX_ERROR, 
				  SD->PB, 
				  &err);
	
	SD->syntax_errors++;
}