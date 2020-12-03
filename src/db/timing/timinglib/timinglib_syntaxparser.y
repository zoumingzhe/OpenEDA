
/* bison input file */
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
#include "timinglib_libhash.h"

typedef void* yyscan_t;
}

%code provides {
#undef  YY_DECL
#define YY_DECL int syntax_lex(SYNTAX_STYPE *yylval_param, \
   yyscan_t yyscanner, Timinglib::LibAnalysis &libAnalysis)
YY_DECL;

void syntax_error(yyscan_t scanner, Timinglib::LibAnalysis &, const char *str);
}

%{
#include "timinglib_structs.h"
#include "timinglib_libanalysis.h"

#define SD (libAnalysis.getScandata())
#define STRTAB (libAnalysis.getStrtab())

%}

%union {
	char *str;
	double num;
	void *voidptr;
	Timinglib::libsynt_attr_type attrtype;
	Timinglib::libsynt_float_constraint *float_constr;
	Timinglib::libsynt_int_constraint *int_constr;
	Timinglib::libsynt_string_enum *str_enum;
	Timinglib::libsynt_argument *arg;
	Timinglib::libsynt_attribute_info *attr;
	Timinglib::libsynt_name_constraints nameconstr;
	Timinglib::libsynt_group_info *group;
	Timinglib::libsynt_head *head;
	Timinglib::libsynt_technology *tech;
}

%define api.pure full
%define api.prefix {syntax_}
%define api.token.prefix {SYNTAX_}
%parse-param {yyscan_t yyscanner}
%parse-param { Timinglib::LibAnalysis &libAnalysis }
%lex-param {yyscan_t yyscanner}
%lex-param   { Timinglib::LibAnalysis &libAnalysis }

%token COMMA SEMI LPAR RPAR LCURLY RCURLY COLON LBRACK RBRACK
%token KW_FLOAT KW_STRING KW_ENUM KW_UNK_ARGS KW_INTEGER KW_VIRTATTR KW_SHORT KW_OR KW_AND KW_BOOLEAN COLONEQ KW_LIST GREATERTHAN LESSTHAN DOTS

%token <num> NUM
%token <str> STRING IDENT

%type <nameconstr> namelist
%type <arg> arglist
%type <head> arg_or_namelist head
%type <str_enum> string_enum
%type <attr> simple_attr complex_attr
%type <float_constr> float_constraint
%type <int_constr> int_constraint
%type <group> group
%type <tech> file

%type <str>	s_or_i

%%
file	: group { SD->libsynt_techs[SD->libsynt_tech_count++].lib = $1; }
		;

group	: head LCURLY {libAnalysis.syntaxPushGroup($1);} statements RCURLY {$$ = SD->gsinfo[SD->gsindex-1];libAnalysis.syntaxPopGroup();}
        | head LCURLY {libAnalysis.syntaxPushGroup($1);} RCURLY {Timinglib::si2drObjectIdT toid;  $$ = SD->gsinfo[SD->gsindex-1]; Timinglib::LibHash::timinglibHashLookup(SD->libsynt_groups, SD->gsinfo[SD->gsindex-1]->type, &toid);
                                                if( toid.v1 != (void*)NULL )$$->ref =(Timinglib::libsynt_group_info*)toid.v1;
                                                else {printf("Error: line %d: Couldn't find group %s\n",$1->lineno,$1->ident); } libAnalysis.syntaxPopGroup();}
		;


statements 	: statement {}
		 	| statements statement {}
			;


statement 	: simple_attr {Timinglib::si2drObjectIdT toid; toid.v1 = (void*)$1;Timinglib::LibHash::timinglibHashEnterOid(SD->gsinfo[SD->gsindex-1]->attr_hash, $1->name, toid);$1->next = SD->gsinfo[SD->gsindex-1]->attr_list; SD->gsinfo[SD->gsindex-1]->attr_list = $1; }
			| complex_attr {Timinglib::si2drObjectIdT toid;toid.v1 = (void*)$1;Timinglib::LibHash::timinglibHashEnterOid(SD->gsinfo[SD->gsindex-1]->attr_hash, $1->name, toid);$1->next = SD->gsinfo[SD->gsindex-1]->attr_list; SD->gsinfo[SD->gsindex-1]->attr_list = $1;}
			| group  {Timinglib::si2drObjectIdT toid;toid.v1 = (void*)$1;Timinglib::LibHash::timinglibHashEnterOid(SD->gsinfo[SD->gsindex-1]->group_hash, $1->type,toid);$1->next = SD->gsinfo[SD->gsindex-1]->group_list; SD->gsinfo[SD->gsindex-1]->group_list = $1;}
			;

simple_attr	: IDENT COLON KW_VIRTATTR SEMI{ $$ = libAnalysis.syntaxMakeSimple($1,Timinglib::kSYNTAX_ATTRTYPE_VIRTUAL,0);} 
            | IDENT COLON KW_STRING SEMI{ $$ = libAnalysis.syntaxMakeSimple($1,Timinglib::kSYNTAX_ATTRTYPE_STRING,0);} 
            | IDENT COLON KW_STRING string_enum SEMI{ $$ = libAnalysis.syntaxMakeSimple($1,Timinglib::kSYNTAX_ATTRTYPE_ENUM, $4);} 
            | IDENT COLON KW_FLOAT float_constraint  SEMI{ $$ = libAnalysis.syntaxMakeSimple($1,Timinglib::kSYNTAX_ATTRTYPE_FLOAT,$4);}
            | IDENT COLON KW_FLOAT SEMI{ $$ = libAnalysis.syntaxMakeSimple($1,Timinglib::kSYNTAX_ATTRTYPE_FLOAT,0);}
            | IDENT COLON KW_ENUM string_enum SEMI{ $$ = libAnalysis.syntaxMakeSimple($1,Timinglib::kSYNTAX_ATTRTYPE_ENUM, $4);} 
            | IDENT COLON KW_SHORT SEMI{ $$ = libAnalysis.syntaxMakeSimple($1,Timinglib::kSYNTAX_ATTRTYPE_INT,0);} 
            | IDENT COLON KW_SHORT int_constraint SEMI{ $$ = libAnalysis.syntaxMakeSimple($1,Timinglib::kSYNTAX_ATTRTYPE_INT,$4);} 
            | IDENT COLON KW_INTEGER SEMI{ $$ = libAnalysis.syntaxMakeSimple($1,Timinglib::kSYNTAX_ATTRTYPE_INT,0);} 
            | IDENT COLON KW_INTEGER int_constraint SEMI{ $$ = libAnalysis.syntaxMakeSimple($1,Timinglib::kSYNTAX_ATTRTYPE_INT,$4);} 
            | IDENT COLON KW_BOOLEAN SEMI{ $$ = libAnalysis.syntaxMakeSimple($1,Timinglib::kSYNTAX_ATTRTYPE_BOOLEAN,0);} 
            ;

string_enum : LPAR s_or_i_list RPAR {
         int i; 
         $$ = (Timinglib::libsynt_string_enum*)calloc(sizeof(Timinglib::libsynt_string_enum),1); $$->array = (char**)calloc(sizeof(char*),SD->enumcount+1);
         for(i=0;i<SD->enumcount; i++)
         {
                $$->array[i] = SD->enumlist[i];
         } 
         $$->size = SD->enumcount;
        }
        ;

s_or_i_list : s_or_i { SD->enumcount=0; SD->enumlist[SD->enumcount++] = $1;}
            | s_or_i_list COMMA s_or_i { SD->enumlist[SD->enumcount++] = $3;}
            | s_or_i_list COMMA KW_OR s_or_i { SD->enumlist[SD->enumcount++] = $4;}
            ;


float_constraint : LPAR GREATERTHAN NUM RPAR {$$ = (Timinglib::libsynt_float_constraint*)calloc(sizeof(Timinglib::libsynt_float_constraint),1); $$->greater_than = $3; $$->greater_specd =1; }
                 | LPAR LESSTHAN NUM RPAR {$$ = (Timinglib::libsynt_float_constraint*)calloc(sizeof(Timinglib::libsynt_float_constraint),1); $$->less_than = $3; $$->less_specd = 1; }
                 | LPAR GREATERTHAN NUM KW_AND LESSTHAN NUM RPAR {$$ = (Timinglib::libsynt_float_constraint*)calloc(sizeof(Timinglib::libsynt_float_constraint),1);
                                                 $$->less_than = $6; $$->less_specd = 1;$$->greater_than = $3; $$->greater_specd =1;}
                 | COLONEQ NUM {$$ = (Timinglib::libsynt_float_constraint*)calloc(sizeof(Timinglib::libsynt_float_constraint),1); $$->default_value = $2; $$->default_specd =1;}
                 | LPAR GREATERTHAN NUM RPAR COLONEQ NUM {$$ = (Timinglib::libsynt_float_constraint*)calloc(sizeof(Timinglib::libsynt_float_constraint),1); $$->default_value = $6; $$->default_specd =1; $$->greater_than = $3; $$->greater_specd =1; }
                 | LPAR LESSTHAN NUM RPAR COLONEQ NUM {$$ = (Timinglib::libsynt_float_constraint*)calloc(sizeof(Timinglib::libsynt_float_constraint),1); $$->default_value = $6; $$->default_specd =1;$$->less_than = $3; $$->less_specd = 1; }
                 | LPAR GREATERTHAN NUM KW_AND LESSTHAN NUM RPAR COLONEQ NUM {$$ = (Timinglib::libsynt_float_constraint*)calloc(sizeof(Timinglib::libsynt_float_constraint),1); $$->default_value = $9;
                                                 $$->less_than = $6; $$->less_specd = 1;$$->greater_than = $3; $$->greater_specd =1; $$->default_specd =1;}
                 | COLONEQ s_or_i {$$ = (Timinglib::libsynt_float_constraint*)calloc(sizeof(Timinglib::libsynt_float_constraint),1); $$->refd_default = $2; $$->ref_default_specd =1;}
                 | LPAR GREATERTHAN NUM RPAR COLONEQ s_or_i {$$ = (Timinglib::libsynt_float_constraint*)calloc(sizeof(Timinglib::libsynt_float_constraint),1); $$->refd_default = $6;  $$->ref_default_specd =1;$$->greater_than = $3; $$->greater_specd =1; }
                 | LPAR LESSTHAN NUM RPAR COLONEQ s_or_i {$$ = (Timinglib::libsynt_float_constraint*)calloc(sizeof(Timinglib::libsynt_float_constraint),1); $$->refd_default = $6; $$->ref_default_specd =1;$$->less_than = $3; $$->less_specd = 1; }
                 | LPAR GREATERTHAN NUM KW_AND LESSTHAN NUM RPAR COLONEQ s_or_i {$$ = (Timinglib::libsynt_float_constraint*)calloc(sizeof(Timinglib::libsynt_float_constraint),1); $$->refd_default = $9;
                                                 $$->less_than = $6; $$->less_specd = 1;$$->greater_than = $3; $$->greater_specd =1; $$->ref_default_specd =1;}
                 ;

int_constraint :  LPAR GREATERTHAN NUM RPAR {$$ = (Timinglib::libsynt_int_constraint*)calloc(sizeof(Timinglib::libsynt_int_constraint),1); $$->greater_than = $3; $$->greater_specd =1; }
               | LPAR LESSTHAN NUM RPAR {$$ = (Timinglib::libsynt_int_constraint*)calloc(sizeof(Timinglib::libsynt_int_constraint),1); $$->less_than = $3; $$->less_specd = 1; }
               | LPAR GREATERTHAN NUM KW_AND LESSTHAN NUM RPAR {$$ = (Timinglib::libsynt_int_constraint*)calloc(sizeof(Timinglib::libsynt_int_constraint),1);
                                                                $$->less_than = $6; $$->less_specd = 1;$$->greater_than = $3; $$->greater_specd =1;}
               | COLONEQ NUM {$$ = (Timinglib::libsynt_int_constraint*)calloc(sizeof(Timinglib::libsynt_int_constraint),1); $$->default_value = $2; $$->default_specd =1;}
               | LPAR GREATERTHAN NUM RPAR COLONEQ NUM {$$ = (Timinglib::libsynt_int_constraint*)calloc(sizeof(Timinglib::libsynt_int_constraint),1); $$->default_value = $6; $$->default_specd =1; $$->greater_than = $3; $$->greater_specd =1; }
               | LPAR LESSTHAN NUM RPAR COLONEQ NUM {$$ = (Timinglib::libsynt_int_constraint*)calloc(sizeof(Timinglib::libsynt_int_constraint),1); $$->default_value = $6; $$->default_specd =1;$$->less_than = $3; $$->less_specd = 1; }
               | LPAR GREATERTHAN NUM KW_AND LESSTHAN NUM RPAR COLONEQ NUM {$$ = (Timinglib::libsynt_int_constraint*)calloc(sizeof(Timinglib::libsynt_int_constraint),1); $$->default_value = $9;
                                                                            $$->less_than = $6; $$->less_specd = 1;$$->greater_than = $3; $$->greater_specd =1; $$->default_specd =1;}
               | COLONEQ s_or_i {$$ = (Timinglib::libsynt_int_constraint*)calloc(sizeof(Timinglib::libsynt_int_constraint),1); $$->refd_default = $2; $$->ref_default_specd =1;}
               | LPAR GREATERTHAN NUM RPAR COLONEQ s_or_i {$$ = (Timinglib::libsynt_int_constraint*)calloc(sizeof(Timinglib::libsynt_int_constraint),1); $$->refd_default = $6;  $$->ref_default_specd =1;$$->greater_than = $3; $$->greater_specd =1; }
               | LPAR LESSTHAN NUM RPAR COLONEQ s_or_i {$$ = (Timinglib::libsynt_int_constraint*)calloc(sizeof(Timinglib::libsynt_int_constraint),1); $$->refd_default = $6; $$->ref_default_specd =1;$$->less_than = $3; $$->less_specd = 1; }
               | LPAR GREATERTHAN NUM KW_AND LESSTHAN NUM RPAR COLONEQ s_or_i {$$ = (Timinglib::libsynt_int_constraint*)calloc(sizeof(Timinglib::libsynt_int_constraint),1); $$->refd_default = $9;
                                                                      $$->less_than = $6; $$->less_specd = 1;$$->greater_than = $3; $$->greater_specd =1; $$->ref_default_specd =1;}
;



complex_attr 	: head  SEMI  { $$= libAnalysis.syntaxMakeComplex($1);}
				;

head	: IDENT LPAR arg_or_namelist RPAR { $$ = $3; $$->ident = $1; $$->lineno = SD->lineno;}
        | KW_SHORT LPAR arg_or_namelist RPAR { $$ = $3; $$->ident = (char*)("short"); $$->lineno = SD->lineno;}
		;


arg_or_namelist : namelist {
                        $$ = (Timinglib::libsynt_head *)calloc(sizeof(Timinglib::libsynt_head),1); 
                        $$->namecons = $1;
                }
		| arglist {
                        $$ = (Timinglib::libsynt_head *)calloc(sizeof(Timinglib::libsynt_head),1); 
                        $$->arglist = $1;
                }
		;

namelist :  {$$ = Timinglib::kSYNTAX_GNAME_NONE;}
		 | IDENT {$$ = Timinglib::kSYNTAX_GNAME_ONE;}
		 | LBRACK IDENT RBRACK {$$ = Timinglib::kSYNTAX_GNAME_NONE_OR_ONE;}
		 | IDENT LBRACK COMMA DOTS COMMA IDENT RBRACK {$$ = Timinglib::kSYNTAX_GNAME_ONE_OR_MORE;}
		 | IDENT COMMA IDENT {$$ = Timinglib::kSYNTAX_GNAME_TWO;}
		 | IDENT COMMA IDENT COMMA IDENT {$$ = Timinglib::kSYNTAX_GNAME_THREE;}
		 ;

arglist : KW_STRING { $$=(Timinglib::libsynt_argument *)calloc(sizeof(Timinglib::libsynt_argument),1); $$->type = Timinglib::kSYNTAX_ATTRTYPE_STRING;}
        | KW_INTEGER {$$=(Timinglib::libsynt_argument *)calloc(sizeof(Timinglib::libsynt_argument),1); $$->type = Timinglib::kSYNTAX_ATTRTYPE_INT;}
        | KW_FLOAT {$$=(Timinglib::libsynt_argument *)calloc(sizeof(Timinglib::libsynt_argument),1); $$->type = Timinglib::kSYNTAX_ATTRTYPE_FLOAT;}
        | KW_UNK_ARGS {$$=(Timinglib::libsynt_argument *)calloc(sizeof(Timinglib::libsynt_argument),1); $$->type = Timinglib::kSYNTAX_ATTRTYPE_COMPLEX_UNKNOWN;}
        | KW_LIST {$$=(Timinglib::libsynt_argument *)calloc(sizeof(Timinglib::libsynt_argument),1); $$->type = Timinglib::kSYNTAX_ATTRTYPE_COMPLEX_LIST;}
        | KW_VIRTATTR {$$=(Timinglib::libsynt_argument *)calloc(sizeof(Timinglib::libsynt_argument),1); $$->type = Timinglib::kSYNTAX_ATTRTYPE_VIRTUAL;}
        | arglist COMMA KW_STRING {
                Timinglib::libsynt_argument *x= (Timinglib::libsynt_argument *)calloc(sizeof(Timinglib::libsynt_argument),1),*y; 
                x->type = Timinglib::kSYNTAX_ATTRTYPE_STRING;
                for(y=$1;y;y=y->next)
                {
                        if( !y->next)
                        {
                                y->next=x;break;
                        }
                }
                $$=$1;
        }
        | arglist COMMA KW_INTEGER {Timinglib::libsynt_argument *x= (Timinglib::libsynt_argument *)calloc(sizeof(Timinglib::libsynt_argument),1),*y; x->type = Timinglib::kSYNTAX_ATTRTYPE_INT;for(y=$1;y;y=y->next){if( !y->next){y->next=x;break;}}$$=$1;}
        | arglist COMMA KW_FLOAT {Timinglib::libsynt_argument *x= (Timinglib::libsynt_argument *)calloc(sizeof(Timinglib::libsynt_argument),1),*y; x->type = Timinglib::kSYNTAX_ATTRTYPE_FLOAT;for(y=$1;y;y=y->next){if( !y->next){y->next=x;break;}}$$=$1;}
        | arglist COMMA KW_LIST {Timinglib::libsynt_argument *x= (Timinglib::libsynt_argument *)calloc(sizeof(Timinglib::libsynt_argument),1),*y; x->type = Timinglib::kSYNTAX_ATTRTYPE_COMPLEX_LIST;for(y=$1;y;y=y->next){if( !y->next){y->next=x;break;}}$$=$1;}
        | arglist COMMA KW_VIRTATTR {Timinglib::libsynt_argument *x= (Timinglib::libsynt_argument *)calloc(sizeof(Timinglib::libsynt_argument),1),*y; x->type = Timinglib::kSYNTAX_ATTRTYPE_VIRTUAL;for(y=$1;y;y=y->next){if( !y->next){y->next=x;break;}}$$=$1;}
        ;



s_or_i  : STRING {$$ = $1;}
		| IDENT {$$=$1;}
		;

%%

void syntax_error(yyscan_t scanner, Timinglib::LibAnalysis &libAnalysis, const char *str){
   Timinglib::scandata *sd = libAnalysis.getScandata();
   fprintf(stderr,"%s line number %d\n", str, sd->lineno);
   return;
}
