
%{
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
#include <fstream>

#include "timinglib_libanalysis.h"
#include "timinglib_libsyn.h"
#include "timinglib_si2dr.h"
#include "timinglib_syntaxparser.tab.hh"

using namespace Timinglib;

#undef  YY_DECL
#define YY_DECL int syntax_lex(SYNTAX_STYPE *yylval_param, \
   yyscan_t yyscanner, Timinglib::LibAnalysis &libAnalysis)
#define YYSTYPE SYNTAX_STYPE

%}

%option prefix="syntax_"
%option reentrant
%option noyywrap
%option bison-bridge
/*%option bison-locations*/
%x comment
%%
%{
  yylval = yylval_param;
  Timinglib::scandata *sd = libAnalysis.getScandata();
%}

\,	{return SYNTAX_COMMA;}
\;	{return SYNTAX_SEMI;}
\(	{return SYNTAX_LPAR;}
\)	{return SYNTAX_RPAR;}
\[	{return SYNTAX_LBRACK;}
\]	{return SYNTAX_RBRACK;}
\{	{return SYNTAX_LCURLY;}
\}[ 	]*\;?	{return SYNTAX_RCURLY;}
\:	{return SYNTAX_COLON;}
float 					{return SYNTAX_KW_FLOAT;}
string 					{return SYNTAX_KW_STRING;}
enum 					{return SYNTAX_KW_ENUM;}
"<unknown_arguments>" 	{return SYNTAX_KW_UNK_ARGS;}
integer {return SYNTAX_KW_INTEGER;}
int {return SYNTAX_KW_INTEGER;}
"virtual_attribute" {return SYNTAX_KW_VIRTATTR;}
short {return SYNTAX_KW_SHORT;}
or {return SYNTAX_KW_OR;}
and {return SYNTAX_KW_AND;}
boolean {return SYNTAX_KW_BOOLEAN;}
list {return SYNTAX_KW_LIST;}
"..." {return SYNTAX_DOTS;}
":=" {return SYNTAX_COLONEQ;}
">" {return SYNTAX_GREATERTHAN;}
"<" {return SYNTAX_LESSTHAN;}

\n	{sd->lineno++;}
\r  {}
\t  {}
" "	{}

\"[^"]*\"		{char *p = (char *)malloc(strlen(yytext)); yytext[strlen(yytext)-1]=0; libAnalysis.searchStringForLinefeeds(yytext); strcpy(p,yytext+1); yylval->str = p;return SYNTAX_STRING;}
\"		|
\"[^"]*		{fprintf(stderr,"No closing quote for quoted string begun in line %d\n",sd->lineno);}

"/*"	BEGIN(comment);

<comment>[^*\n]*        /* eat anything that's not a '*' */
<comment>"*"+[^*/\n]*   /* eat up '*'s not followed by '/'s */
<comment>\n             ++(sd->lineno);
<comment>"*"+"/"	BEGIN(INITIAL);


[-+]?[0-9]*\.?[0-9]*([Ee][-+]?[0-9]+)?	{yylval->num = strtod(yytext,(char**)NULL); return SYNTAX_NUM;}

[-a-zA-Z0-9!@#$%^&_+=|~?<>.]+ {char *p = (char *)malloc(strlen(yytext)+1); strcpy(p,yytext);  yylval->str = p;return SYNTAX_IDENT;}

%%
/*  \/\*.*\*\/    {printf("Line %d: Comment found: %s\n", sd->lineno, yytext);} */

namespace Timinglib
{
   void LibSyn::__descScanBegin(FILE *fp)
   {
      syntax_lex_init(&scanner_);
      syntax_restart (fp, scanner_);
   }
   int LibSyn::__descParse()
   {
      if (analysis_ == nullptr)
         return 2;
      return syntax_parse(scanner_, *analysis_);;
   }
   void LibSyn::__descScanEnd(FILE *fp)
   {
      if (scanner_ != nullptr)
         syntax_lex_destroy(scanner_);
      scanner_ = nullptr;
   }
}
