%{
 /*
 * @file spef_parser.l
 * @date 2020-11-02
 * @brief
 *
 * Copyright (C) 2020 NIIC EDA
 *
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 *
 * of the BSD license.  See the LICENSE file for details.
 */
#include <string.h>
#include <fstream>
#include <string>
#include "db/timing/spef/spef_reader.h"
#include "util/message.h"
#include "spef_parser.yacc.hh"

#undef  YY_DECL
#define YY_DECL int spef_lex(SPEF_STYPE *yylval_param, yyscan_t yyscanner, SpefReader::SpefReader *spefReader)
#define YYSTYPE SPEF_STYPE

%}

%option prefix="spef_"
%option reentrant
%option noyywrap
%option bison-bridge
%option nounput
%option never-interactive

%x COMMENT
%x QUOTE

UPPER    [A-Z]
LOWER    [a-z]
ALPHA    {UPPER}|{LOWER}
DIGIT    [0-9]
POS_SIGN "+"
NEG_SIGN "-"
SIGN     {POS_SIGN}|{NEG_SIGN}
INTEGER  {SIGN}?{DIGIT}+
DECIMAL  {SIGN}?{DIGIT}+"."{DIGIT}*
FRACTION {SIGN}?"."{DIGIT}+
RADIX    {DECIMAL}|{FRACTION}|{INTEGER}
EXP_CHAR [eE]
EXP      {RADIX}{EXP_CHAR}{INTEGER}
FLOAT    {DECIMAL}|{FRACTION}|{EXP}
NUMBER   {INTEGER}|{FLOAT}
POS_INTEGER  {DIGIT}+
POS_DECIMAL  {DIGIT}+"."{DIGIT}*
POS_FRACTION "."{DIGIT}+
POS_RADIX {POS_INTEGER}|{POS_DECIMAL}|{POS_FRACTION}
POS_EXP   {POS_RADIX}{EXP_CHAR}{INTEGER}
POS_FLOAT {POS_DECIMAL}|{POS_FRACTION}|{POS_EXP}
POS_NUMBER {POS_INTEGER}|{POS_FLOAT}

HCHAR   "."|"/"|":"|"|"
HIER_DELIM {HCHAR}
SPECIAL_CHAR "!"|"#"|"$"|"%"|"&"|"'"|"("|")"|"*"|"+"|","|"-"|"."|"/"|":"|";"|"<"|"="|">"|"?"|"@"|"["|"\\"|"]"|"^"|"`"|"{"|"|"|"}"|"~"
ESCAPED_CHAR_SET {SPECIAL_CHAR}|\"
ESCAPED_CHAR \\{ESCAPED_CHAR_SET}
IDENTIFIER_CHAR {ESCAPED_CHAR}|{ALPHA}|{DIGIT}|"_"
IDENTIFIER {IDENTIFIER_CHAR}+
PREFIX_BUS_DELIM "["|"{"|"("|"<"|":"|"."
SUFFIX_BUS_DELIM "]"|"}"|")"|">"
BIT_IDENTIFIER {IDENTIFIER}({PREFIX_BUS_DELIM}{POS_INTEGER}{SUFFIX_BUS_DELIM}?)*
PARTIAL_PATH {IDENTIFIER}{HIER_DELIM}
PATH ({HIER_DELIM}?{BIT_IDENTIFIER})|({HIER_DELIM}?{PARTIAL_PATH}+{BIT_IDENTIFIER})
WHITE_SPACE   [ \t]
QSTRING_CHAR {SPECIAL_CHAR}|{ALPHA}|{DIGIT}|{WHITE_SPACE}|"_"
QSTRING \"{QSTRING_CHAR}+\"
NAME {QSTRING}|{IDENTIFIER}
PHYSICAL_NAME {NAME}
PARTIAL_PHYSICAL_REF {HIER_DELIM}{PHYSICAL_NAME}
PHYSICAL_REF {PHYSICAL_NAME}{PARTIAL_PHYSICAL_REF}+
INDEX "*"{POS_INTEGER}

%%

%{
yylval = yylval_param;
%}


"/*"                    { BEGIN(COMMENT); }
<COMMENT>[^*\n]*        /* eat anything that's not a '*' */
<COMMENT>"*"+[^*/\n]*   /* eat up '*'s not followed by '/'s */
<COMMENT>\n             { spefReader->incrLineNo(); }
<COMMENT>"*"+"/"        { BEGIN(INITIAL); }
<COMMENT><<EOF>>        { open_edi::util::message->issueMsg(
                              open_edi::util::kError, "Unterminated comment"); 
                          BEGIN(INITIAL); 
			  yyterminate(); 
			}

"//".*\n        { spefReader->incrLineNo(); }
[ \t\r]*        {}
\n              { spefReader->incrLineNo(); }
"\""            { BEGIN(QUOTE); }
<QUOTE>[ \t\r]+ /* eat anything that's not a '\n' */
<QUOTE>\n       { spefReader->incrLineNo(); }
<QUOTE>"\\".    { spefReader->recordChar(yytext[1]); }
<QUOTE>"\""     { BEGIN(INITIAL); 
		  yylval->string = spefReader->getRecordStr(); 
                  return QSTRING; }
<QUOTE>.        { spefReader->recordChar(yytext[0]); }
<QUOTE><<EOF>>  { open_edi::util::message->issueMsg(
                              open_edi::util::kError, "Unterminated comment");
                  BEGIN(INITIAL); 
		  yyterminate(); }
"*SPEF"         { return SPEF; }
"*DESIGN"       { return DESIGN; }
"*DATE"     /* Don't save it for now */
"*VENDOR"   /* Don't save it for now */
"*PROGRAM"  /* Don't save it for now */
"*VERSION"  /* Don't save it for now */
"*DESIGN_FLOW" { return DESIGN_FLOW; }
"*DIVIDER"     { return DIVIDER; }
"*DELIMITER"   { return DELIMITER; }
"*BUS_DELIMITER" { return BUS_DELIMITER; }
"*T_UNIT" { return T_UNIT; }
"*C_UNIT" { return C_UNIT; }
"*R_UNIT" { return R_UNIT; }
"*L_UNIT" { return L_UNIT; }
"*POWER_NETS"  { return POWER_NETS; }
"*GROUND_NETS" { return GROUND_NETS; }
"*NAME_MAP"    { return NAME_MAP; }
"*PORTS"       { return PORTS; }
"*PHYSICAL_PORTS" { return PHYSICAL_PORTS; }
"*C" { return COORDINATE; }
"*L" { return CAPLOAD; }
"*S" { return SLEW; }
"*D" { return CELL; }
"*DEFINE" { return DEFINE; }
"*PDEFINE" { return PDEFINE; }
"*VARIATION_PARAMETERS" { return VARI_PARA; }
"*D_NET" { return D_NET; }
"*V"     { return RCONF; }
"*CONN"  { return CONN; }
"*P" { return PPORT; }
"*I" { return PINT; }
"*N" { return NINT; }
"*CAP" { return CAP; }
"*SC"  { return SC; }
"*RES" { return RES; }
"*INDUC"  { return INDUC; }
"*R_NET"  { return R_NET; }
"*DRIVER" { return DRVPIN; }
"*CELL"   { return DRVCELL; }
"*C2_R1_C1" { return C2_R1_C1; }
"*LOADS"    { return LOADS; }
"*RC" { return RC; }
"*Q"  { return QPOLE; }
"*K"  { return RESIDUE; }
"*D_PNET" { return D_PNET; }
"*END"    { return END; }
"*R_PNET" { return R_PNET; }

{INTEGER} { yylval->integer = atoi(yytext); return INTEGER; }

{FLOAT}   { yylval->value = static_cast<float>(atof(yytext)); return FLOAT; }

{IDENTIFIER}   { yylval->string = spefReader->stringCopy(yytext); return IDENTIFIER; }

{PATH} { yylval->string = spefReader->stringCopy(yytext); return NAME; }

{INDEX} { yylval->string = spefReader->stringCopy(yytext); return INDEX; }

{HCHAR} { char ch = yytext[0]; return ((int) ch); }

.       { return ((int) yytext[0]); }

%%

namespace SpefReader
{
   void SpefReader::__spef_parse_begin(FILE *fp)
   {
      spef_lex_init(&scanner_);
      spef_restart(fp, scanner_);
   }
   int SpefReader::__spef_parse()
   {
      return  spef_parse(scanner_, this);
   }
   void SpefReader::__spef_parse_end(FILE *fp)
   {
      if (scanner_ != nullptr)
         spef_lex_destroy(scanner_);
      scanner_ = nullptr;
   }
}
