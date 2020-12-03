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
#include "timinglib_libparser.tab.hh"

using namespace Timinglib;

#undef  YY_DECL
#define YY_DECL int my_yylex(YYSTYPE *yylval_param, \
   yyscan_t yyscanner, Timinglib::LibAnalysis &libAnalysis)

void
msgOutput(LibAnalysis &libAnalysis, si2drSeverityT sev, const std::string &msg)
{
   si2drErrorT err;
   si2drMessageHandlerT MsgPrinter;
   MsgPrinter = libAnalysis.si2drPIGetMessageHandler(&err); /* the printer is in another file! */
   (libAnalysis.*MsgPrinter)(sev, kSI2DR_NO_ERROR, 
            (char *)(msg.c_str()), 
            &err);
}

%}

%option reentrant
%option noyywrap
%option bison-bridge
%option nounput
%option never-interactive
/*%option bison-locations*/
%x comment
%x stringx
%x include

%%
%{ /** Code executed at the beginning of yylex **/
   yylval = yylval_param;
   Timinglib::scandata *sd = libAnalysis.getScandata();
   Timinglib::LibStrtab *strtab = libAnalysis.getStrtab();
   for (int i=0; i < MAX_INCLUDE_DEPTH; i++)
   {
      if (sd->include_stack[i] == nullptr)
         sd->include_stack[i] = new YY_BUFFER_STATE;
   }
%}

\+  {sd->lline = sd->lineno;libAnalysis.setTok(); return PLUS;}
\-  {sd->lline = sd->lineno;libAnalysis.setTok(); return MINUS;}
\*  {sd->lline = sd->lineno;libAnalysis.setTok(); return MULT;}
\/  {sd->lline = sd->lineno;libAnalysis.setTok(); return DIV;}
\,	{sd->lline = sd->lineno;libAnalysis.setTok(); return COMMA;}
\;	{sd->lline = sd->lineno;libAnalysis.setTok(); return SEMI;}
\(	{sd->lline = sd->lineno;libAnalysis.setTok(); return LPAR;}
\)	{sd->lline = sd->lineno;libAnalysis.setTok(); return RPAR;}
\=  {sd->lline = sd->lineno;libAnalysis.setTok(); return EQ;}
\{	{
      if(sd->lline != sd->lineno)
      {
         std::string str = stringFormat("line %d: Opening Curly must be on same line as group declaration!", sd->lineno);
         msgOutput(libAnalysis, kSI2DR_SEVERITY_WARN, str);
      }
      libAnalysis.setTok();
      return LCURLY;
   }

\}[ \t]*\;?	{sd->lline = sd->lineno;libAnalysis.setTok(); return RCURLY;}
[ \t]?\:	{
      sd->lline = sd->lineno;
      if( yyleng == 1 && !sd->tight_colon_ok )
      {
         std::string str = stringFormat("space must precede Colon (:) at line %d", sd->lineno);
         msgOutput(libAnalysis, kSI2DR_SEVERITY_ERR, str);
      }
      libAnalysis.setTok(); 
      return COLON;
   }

include_file[ \t]*\(	BEGIN(include);

[-+]?([0-9]+\.?[0-9]*([Ee][-+]?[0-9]+)?|[0-9]*\.[0-9]*([Ee][-+]?[0-9]+)?) {
	sd->lline = sd->lineno; 
	if(strchr(yytext,'.') || strchr(yytext,'E') || strchr(yytext,'e')) 
	{
      xnumber x;
      x.type = 1;
      x.floatnum = strtod(yytext,(char**)NULL);
		yylval->num = x;
	} 
	else 
	{
      xnumber x;
      x.type = 0;
      x.intnum = strtol(yytext,(char**)NULL, 10);
		yylval->num = x; 
	}
   libAnalysis.setTok();
	return NUM;
}


[A-Za-z!@#$%^&_+\|~\?:][A-Za-z0-9!@#$%^&_+\|~\?:]*[\<\{\[\(][-0-9:]+[\]\}\>\)]		{
 sd->lline = sd->lineno;
 if( !strncmp(yytext,"values(",7) )
 {
   /* ugh -- a values() with a single unquoted number in it! let's translate it into a values with a single quoted value instead! */
   char *ident = strtab->timinglibStrtableEnterString(sd->master_string_table, (char*)("values")); /* OLD WAY: (char*)malloc(7); */
   char *str /* OLD WAY: = (char*)malloc(strlen(yytext)-4) */;
   /* strcpy(ident,"values");  OLD allocation method */
   yylval->str = ident;
   str = strtab->timinglibStrtableEnterString(sd->master_string_table, yytext+7);
   /* OLD WAY: strcpy(str,yytext+7); 
	           str[strlen(str)-1] = 0; */
   libAnalysis.addToken(LPAR, 0, 0, 0.0, 0);
   libAnalysis.addToken(STRING, 0, 0, 0.0, str);
   libAnalysis.addToken(RPAR, 0, 0, 0.0, 0);
   libAnalysis.setTok(); return IDENT;
 }
 else
 {
   /* OLD:  char *str = (char*)malloc(strlen(yytext)+1);
            strcpy(str,yytext)*/ ;  
	yylval->str = /* OLD: str  NEW: */ strtab->timinglibStrtableEnterString(sd->master_string_table, yytext);
   libAnalysis.setTok(); return STRING;
 }
}

"define" {sd->lline = sd->lineno;libAnalysis.setTok(); return KW_DEFINE;}
"define_group" {sd->lline = sd->lineno;libAnalysis.setTok(); return KW_DEFINE_GROUP;}
[Tt][Rr][Uu][Ee]   {sd->lline = sd->lineno;libAnalysis.setTok(); return KW_TRUE;}
[Ff][Aa][Ll][Ss][Ee]  {sd->lline = sd->lineno;libAnalysis.setTok(); return KW_FALSE;}
\\?\n	{sd->lineno++;}
\\[ \t]+\n	{
      std::string str = stringFormat("%s:%d -- Continuation char followed by spaces or tabs!", sd->curr_file, sd->lineno);
      msgOutput(libAnalysis, kSI2DR_SEVERITY_ERR, str);
      sd->lineno++;
   }

\r  {}
\t  {}
" "	{}

[a-zA-Z0-9!@#$%^&_+\|~\?<>\.\-]+ {
   sd->lline = sd->lineno;
   yylval->str = /* OLD: myStrdup(yytext) NEW: */ strtab->timinglibStrtableEnterString(sd->master_string_table, yytext);
   libAnalysis.setTok();
   return IDENT;
   }


"/*"	{ 
   BEGIN(comment); 
   if( !sd->tok_encountered && sd->token_comment_buf[0] )
      strcat(sd->token_comment_buf,"\n");
	else if( sd->tok_encountered && sd->token_comment_buf2[0] )
       strcat(sd->token_comment_buf2,"\n"); 
}

\"		sd->string_buf_ptr = sd->string_buf; BEGIN(stringx);

<comment>[^*\n]*        /* eat anything that's not a '*' */ {
   if( sd->tok_encountered ) 
      strcat(sd->token_comment_buf2,yytext); 
   else 
      strcat(sd->token_comment_buf,yytext);
}

<comment>"*"+[^*/\n]*   /* eat up '*'s not followed by '/'s */{
   if( sd->tok_encountered ) 
      strcat(sd->token_comment_buf2,yytext); 
   else 
      strcat(sd->token_comment_buf,yytext);
}

<comment>\n             {
   ++sd->lineno;
   if( sd->tok_encountered ) 
      strcat(sd->token_comment_buf2,yytext); 
   else 
      strcat(sd->token_comment_buf,"\n");
}

<comment>"*"+"/"	BEGIN(INITIAL);

<stringx>\"	{ 
   BEGIN(INITIAL); 
   *sd->string_buf_ptr = 0;
	yylval->str = strtab->timinglibStrtableEnterString(sd->master_string_table, sd->string_buf);
   /* OLD: myStrdup(string_buf); */ libAnalysis.setTok(); 
   return STRING; 
}

<stringx>\n { 
   std::string str = stringFormat("line %d: unterminated string constant-- use a back-slash to continue a string to the next line.", sd->lineno++);
   msgOutput(libAnalysis, kSI2DR_SEVERITY_ERR, str); 
	BEGIN(INITIAL); 
   *sd->string_buf_ptr = 0;
	yylval->str = strtab->timinglibStrtableEnterString(sd->master_string_table, sd->string_buf);
   /* OLD: myStrdup(string_buf);*/ libAnalysis.setTok(); 
   return STRING;
}

<stringx>\\\n 	{ *(sd->string_buf_ptr)++ = '\\'; *(sd->string_buf_ptr)++ = '\n'; sd->lineno++;}
<stringx>\\. 	{ *(sd->string_buf_ptr)++ = '\\'; *(sd->string_buf_ptr)++ = yytext[1];}
<stringx>[^\\\n\"]+		{ char *yptr = yytext; while (*yptr) *(sd->string_buf_ptr)++ = *yptr++; }

<include>[ \t]* {}
<include>[^ \t\n);]+	{  
                 if ( sd->include_stack_index >= MAX_INCLUDE_DEPTH )
                  {
                     std::string str = stringFormat("Includes nested too deeply! An included file cannot include another file!" );
                     msgOutput(libAnalysis, kSI2DR_SEVERITY_ERR, str);
                     exit( 1 );
                  }
				      libAnalysis.cleanFileName(yytext,sd->filenamebuf);
}

<include>")"		{}
<include>";"		{ 
   FILE *yyin_save = yyin; 
   Timinglib::struct_nl *nlp;
   ((YY_BUFFER_STATE*)(sd->include_stack))[sd->include_stack_index++] = YY_CURRENT_BUFFER;
   /*include_stack[sd->include_stack_index++] = YY_CURRENT_BUFFER;*/
	std::string str = stringFormat("Including file %s.", sd->filenamebuf);
   msgOutput(libAnalysis, kSI2DR_SEVERITY_NOTE, str);
   yyin = fopen( sd->filenamebuf, "r" );
	if ( ! yyin )
   {
      
      std::string str = stringFormat("Couldn't find the include file: %s; ignoring the Include directive!", sd->filenamebuf);
      msgOutput(libAnalysis, kSI2DR_SEVERITY_ERR, str);
      sd->include_stack_index--;
      yyin = yyin_save;
      BEGIN(INITIAL);
   }
   else
   {
      yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE, yyg), yyg);
      nlp = (Timinglib::struct_nl *)calloc(sizeof(Timinglib::struct_nl),1);
      nlp->fname = /*OLD: (char*)malloc(strlen(filenamebuf)+1); NEW:*/ strtab->timinglibStrtableEnterString(sd->master_string_table, sd->filenamebuf);
                  /*OLD: strcpy(nlp->fname,filenamebuf); */
      nlp->next = sd->file_name_list;
      sd->file_name_list = nlp;
      sd->curr_file_save = sd->curr_file;
      sd->curr_file = nlp->fname;
      sd->save_lineno = sd->lineno;
      sd->save_lline = sd->lline;
      sd->lineno = 1;
      sd->lline = 1;
      BEGIN(INITIAL);
   }
}

<<EOF>>  {
      if ( --sd->include_stack_index < 0 )
      {
         yyterminate();
      }

      else
      {
         yy_delete_buffer( YY_CURRENT_BUFFER, yyg);
         yy_switch_to_buffer(((YY_BUFFER_STATE *)(sd->include_stack))[sd->include_stack_index], yyg);
         /*yy_switch_to_buffer(include_stack[sd->include_stack_index] );*/
         sd->lineno = sd->save_lineno;
         sd->lline = sd->save_lline;
         sd->curr_file = sd->curr_file_save;
      }
}
%%

int yylex(YYSTYPE *yylval_param, \
   yyscan_t yyscanner, Timinglib::LibAnalysis &libAnalysis)
{
   if( !libAnalysis.tokenQEmpty() )
      {
         void *p = (void *)yylval_param;
         return libAnalysis.injectedToken(p);
      }
	   else
		   return my_yylex(yylval_param, yyscanner, libAnalysis);
}

namespace Timinglib
{
   void LibSyn::__libScanBegin(FILE *fp)
   {
      yylex_init(&scanner_);
      yyrestart (fp, scanner_);
   }
   int LibSyn::__libParse()
   {
      if (analysis_ == nullptr)
         return 2;
      return  yyparse(scanner_, *analysis_);;
   }
   void LibSyn::__libScanEnd(FILE *fp)
   {
      if (scanner_ != nullptr)
         yylex_destroy(scanner_);
      scanner_ = nullptr;
   }
}
