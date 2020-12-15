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
#include "db/timing/timinglib/timinglib_libanalysis.h"

#include <unistd.h>

#include <fstream>
#include <list>
#include <string>

#include "db/timing/timinglib/attr_enum.h"
#include "db/timing/timinglib/group_enum.h"
#include "db/timing/timinglib/timinglib_libbuilder.h"
#include "db/timing/timinglib/timinglib_libhash.h"
#include "db/timing/timinglib/timinglib_si2dr.h"
#include "timinglib_libparser.tab.hh"
#include "util/util.h"

typedef struct libGroupMap {
    char *name;
    Timinglib::group_enum type;
} libGroupMap;
typedef struct libAttrMap {
    char *name;
    Timinglib::attr_enum type;
} libAttrMap;

extern const struct libGroupMap *lookup_group_name(register const char *str,
                                                   register size_t len);

extern const struct libAttrMap *lookup_attr_name(register const char *str,
                                                 register size_t len);

namespace Timinglib {
si2drObjectIdT LibAnalysis::nulloid_ = {0, 0};
LibAnalysis::LibAnalysis(LibBuilder *libbuilder) {
    sd_ = new scandata;
    memset(sd_, 0, sizeof(scandata));
    strtab_ = new Timinglib::LibStrtab();
    libbuilder_ = libbuilder;
}
LibAnalysis::~LibAnalysis() {
    si2drErrorT err;
    si2drPIQuit(&err);
    delete (sd_);
    sd_ = nullptr;
    delete strtab_;
    strtab_ = nullptr;
}
void LibAnalysis::setLibertyFileName(const char *filename) {
    if (sd_ != nullptr) sd_->curr_file = const_cast<char *>(filename);
}
void LibAnalysis::setLibertyParseLogStr(std::string *parseLogStr) {
    parseLogStr_ = parseLogStr;
}
scandata *LibAnalysis::getScandata(void) { return sd_; }
Timinglib::LibStrtab *LibAnalysis::getStrtab(void) { return strtab_; }
char *LibAnalysis::searchStringForLinefeeds(char *str) {
    char *s;
    s = str;
    while (*s) {
        if (*s++ == '\n') {
            sd_->lineno++;
            if (*(s - 2) != '\\') {
                printf(
                    "Warning: line %d: String constant spanning input lines "
                    "does not use continuation character.\n",
                    sd_->lineno);
            }
        }
    }
    return str;
}
void LibAnalysis::generateAttrEnumHeader() {
    printf("Generating attr_enum.h...\n");
    FILE *outH = fopen("attr_enum.h", "w");
    if (!outH) {
        printf("Couldn't open attr_enum.h. Fix the problem and rerun.\n");
        return;
    }
    fprintf(outH, "#ifndef ATTR_ENUM_H_\n");
    fprintf(outH, "#define ATTR_ENUM_H_\n\n");
    fprintf(outH, "namespace Timinglib {\n");
    fprintf(outH, "\n\ntypedef enum\n{\n");
    timinglib_hash_bucket *hb;
    int i = 1;
    std::list<std::string> strList;
    for (hb = sd_->libsynt_attrs->all_list; hb; hb = hb->all_next) {
        strList.emplace_back(hb->name);
    }
    strList.sort();
    for (auto &str : strList) {
        fprintf(outH,
                "kTIMINGLIB_ATTRENUM_%s                    = "
                "%d,\n",
                str.c_str(), i++);
    }
    fprintf(outH, "} attr_enum;\n\n");
    fprintf(outH, "} // namespace Timinglib\n\n");
    fprintf(outH, "#endif // ATTR_ENUM_H_");
    fclose(outH);
}
void LibAnalysis::generateGroupEnumHeader() {
    printf("Generating group_enum.h...\n");
    FILE *outH = fopen("group_enum.h", "w");
    if (!outH) {
        printf("Couldn't open group_enum.h. Fix the problem and rerun.\n");
        return;
    }
    fprintf(outH, "#ifndef GROUP_ENUM_H_\n");
    fprintf(outH, "#define GROUP_ENUM_H_\n\n");
    fprintf(outH, "namespace Timinglib {\n");
    fprintf(outH, "\n\ntypedef enum\n{\n");
    timinglib_hash_bucket *hb;
    int i = 1;
    std::list<std::string> strList;
    for (hb = sd_->libsynt_allgroups->all_list; hb; hb = hb->all_next) {
        strList.emplace_back(hb->name);
    }
    strList.sort();
    for (auto &str : strList) {
        fprintf(outH,
                "kTIMINGLIB_GROUPENUM_%s                    = "
                "%d,\n",
                str.c_str(), i++);
    }
    fprintf(outH, "} group_enum;\n\n");
    fprintf(outH, "} // namespace Timinglib\n\n");
    fprintf(outH, "#endif // GROUP_ENUM_H_");
    fclose(outH);
}
void LibAnalysis::generateSyntaxDeclsHAndCPP() {
    printf(
        "Generating timinglib_syntaxdecls.cpp and "
        "timinglib_syntaxdecls.h...\n");
    FILE *outC = fopen("timinglib_syntaxdecls.cpp", "w");
    FILE *outH = fopen("timinglib_syntaxdecls.h", "w");
    if (!outC || !outH) {
        printf(
            "Couldn't open 'timinglib_syntaxdecls.cpp' or"
            " 'timinglib_syntaxdecls.h';Solve the problem and re-run.\n");
        return;
    }
    fprintf(outH, "#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_SYNTAXDECLS_H_\n");
    fprintf(outH,
            "#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_SYNTAXDECLS_H_"
            "\n\n");
    fprintf(outH, "namespace Timinglib {\n");
    fprintf(outC, "#include \"timinglib_structs.h\"\n");
    fprintf(outC, "#include \"timinglib_syntaxdecls.h\"\n");
    fprintf(outC, "namespace Timinglib {\n");
    for (int i = 0; i < sd_->libsynt_tech_count; i++) {
        fprintf(outH, "\n\n/* Technology: %s       */\n\n",
                sd_->libsynt_techs[i].name);
        fprintf(outC, "\n\n/* Technology: %s       */\n\n",
                sd_->libsynt_techs[i].name);
        __dumpGroup(sd_->libsynt_techs[i].lib, outC, outH);
        LibHash::dumpGroupHash(sd_->libsynt_techs[i].group_hash, outC, outH);
    }
    fprintf(outH, "extern libsynt_technology libsynt_techs[%d];\n",
            sd_->libsynt_tech_count);
    fprintf(outC, "libsynt_technology libsynt_techs[%d] = {\n",
            sd_->libsynt_tech_count);
    for (int i = 0; i < sd_->libsynt_tech_count; i++) {
        fprintf(
            outC, "{(char*)(\"%s\"), %s, %s },\n", sd_->libsynt_techs[i].name,
            LibHash::makeRep("group", sd_->libsynt_techs[i].lib).c_str(),
            LibHash::makeRep("ht", sd_->libsynt_techs[i].group_hash).c_str());
    }
    fprintf(outC, "};\n");
    fprintf(outH, "extern int libsynt_tech_num;\n");
    fprintf(outC, "int libsynt_tech_num = %d;\n", sd_->libsynt_tech_count);
    fprintf(outH, "} // namespace Timinglib\n");
    fprintf(outH,
            "#endif "
            "// SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_SYNTAXDECLS_H_\n");
    fprintf(outC, "}\n");
    fclose(outC);
    fclose(outH);
}
void LibAnalysis::generateAttrAndGroupLookup() {
    printf("Generating group_lookup...\n");
    FILE *outC = fopen("group_lookup", "w");
    if (!outC) {
        printf("Couldn't open group_lookup. Fix the problem and rerun.\n");
        return;
    }
    fprintf(
        outC,
        "%%{\n#include \"group_enum.h\"\n\nusing namespace Timinglib;\n%%}\n");
    fprintf(
        outC,
        "struct libGroupMap { const char *name; group_enum type; };\n%%%%\n");
    timinglib_hash_bucket *hb;
    std::list<std::string> groupStrList;
    for (hb = sd_->libsynt_allgroups->all_list; hb; hb = hb->all_next) {
        groupStrList.emplace_back(hb->name);
    }
    groupStrList.sort();
    for (auto &str : groupStrList) {
        fprintf(outC, "%s,  kTIMINGLIB_GROUPENUM_%s\n", str.c_str(),
                str.c_str());
    }
    fprintf(outC, "%%%%\n");
    fclose(outC);

    printf("Generating attr_lookup...\n");
    outC = fopen("attr_lookup", "w");
    if (!outC) {
        printf("Couldn't open attr_lookup. Fix the problem and rerun.\n");
        return;
    }
    fprintf(
        outC,
        "%%{\n#include \"attr_enum.h\"\n\nusing namespace Timinglib;\n%%}\n");
    fprintf(
        outC,
        "struct libGroupMap { const char *name; attr_enum type; };\n%%%%\n");
    std::list<std::string> attrStrList;
    for (hb = sd_->libsynt_attrs->all_list; hb; hb = hb->all_next) {
        attrStrList.emplace_back(hb->name);
    }
    attrStrList.sort();
    for (auto &str : attrStrList) {
        fprintf(outC, "%s,    kTIMINGLIB_ATTRENUM_%s\n", str.c_str(),
                str.c_str());
    }
    fprintf(outC, "%%%%\n");
    fclose(outC);
}
int LibAnalysis::tokenQEmpty(void) {
    if (sd_->token_q_front == sd_->token_q_rear)
        return 1;
    else
        return 0;
}
void LibAnalysis::__removeToken(int *retval, int *set_num, int *intnum,
                                double *floatnum, char **str) {
    if (tokenQEmpty()) return;
    if (sd_->token_q_front == 9)
        sd_->token_q_front = 0;
    else
        sd_->token_q_front++;
    *retval = sd_->token_q_buf[sd_->token_q_front].retval;
    *set_num = sd_->token_q_buf[sd_->token_q_front].set_num;
    *floatnum = sd_->token_q_buf[sd_->token_q_front].floatnum;
    *intnum = sd_->token_q_buf[sd_->token_q_front].floatnum;
    if (sd_->token_q_buf[sd_->token_q_front].str) {
        *str = sd_->token_q_buf[sd_->token_q_front].str;
    }
}
void LibAnalysis::addToken(int retval, int set_num, int intnum, double floatnum,
                           char *str) {
    if (sd_->token_q_rear == 9)
        sd_->token_q_rear = 0;
    else
        sd_->token_q_rear++;
    if (sd_->token_q_rear == sd_->token_q_front) {
        printf("Token Queue Stack Overflow-- notify program maintainers!\n");
        return;
    }
    sd_->token_q_buf[sd_->token_q_rear].str = str;
    sd_->token_q_buf[sd_->token_q_rear].retval = retval;
    sd_->token_q_buf[sd_->token_q_rear].floatnum = floatnum;
    sd_->token_q_buf[sd_->token_q_rear].intnum = intnum;
    sd_->token_q_buf[sd_->token_q_rear].set_num = set_num;
}
int LibAnalysis::injectedToken(void *val) {
    YYSTYPE *lval = static_cast<YYSTYPE *>(val);
    char *str;
    double num;
    int intnum;
    int retval;
    int set_num;
    __removeToken(&retval, &set_num, &intnum, &num, &str);
    if (set_num == -1) {
        xnumber x;
        x.type = 0;
        x.intnum = intnum;
        lval->num = x;
    } else if (set_num == 1) {
        xnumber x;
        x.type = 0;
        x.floatnum = num;
        lval->num = x;
    } else if (str) {
        lval->str = str;
    }
    return retval;
}
void LibAnalysis::setTok(void) {
    if (sd_->token_comment_buf[0]) sd_->tok_encountered++;
}
void LibAnalysis::pushGroup(Timinglib::timinglib_head *h) {
    Timinglib::timinglib_attribute_value *v, *vn;
    si2drErrorT err;
    si2drMessageHandlerT MsgPrinter;
    Timinglib::group_enum ge;
    size_t pb_size = 8000;
    MsgPrinter =
        si2drPIGetMessageHandler(&err); /* the printer is in another file! */
    if (sd_->gsindex == 0) {
        sd_->gs[sd_->gsindex] = __si2drPICreateGroup(0, h->name, &err);
    } else {
        sd_->gs[sd_->gsindex] = __si2drGroupCreateGroup(
            sd_->gs[sd_->gsindex - 1], 0, h->name, &err);
    }
    if (sd_->token_comment_buf[0]) {
        __si2drGroupSetComment(
            sd_->gs[sd_->gsindex], sd_->token_comment_buf,
            &err); /* heaven help us if there's more than 100K of comment! */
        sd_->token_comment_buf[0] = 0;
        sd_->tok_encountered = 0;
        if (sd_->token_comment_buf2[0]) {
            size_t size = 10240;
            snprintf(sd_->token_comment_buf, size, "%s",
                     sd_->token_comment_buf2);
        }
        sd_->token_comment_buf2[0] = 0;
    }
    ge = __si2drGroupGetID(sd_->gs[sd_->gsindex], &err);
    if (err == kSI2DR_OBJECT_ALREADY_EXISTS &&
        (ge != kTIMINGLIB_GROUPENUM_internal_power)) {
        snprintf(
            sd_->PB, pb_size,
            "%s:%d: The group name %s is already being used in this context.",
            sd_->curr_file, sd_->lineno, h->name);
        (this->*MsgPrinter)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR, sd_->PB,
                            &err);
    }
    if (err == kSI2DR_INVALID_NAME) {
        snprintf(sd_->PB, pb_size, "%s:%d: The group name \"%s\" is invalid.",
                 sd_->curr_file, sd_->lineno, h->name);
        (this->*MsgPrinter)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR, sd_->PB,
                            &err);
    }
    sd_->gsindex++;
    si2drObjectSetLineNo(sd_->gs[sd_->gsindex - 1], h->lineno, &err);
    si2drObjectSetFileName(sd_->gs[sd_->gsindex - 1], h->filename, &err);
    if (libbuilder_) libbuilder_->beginGroup(h);
    for (v = h->list; v; v = vn) {
        if (v->type != kTIMINGLIB__VAL_STRING) {
            char buf[1000];
            if (v->type == kTIMINGLIB__VAL_INT) {
                snprintf(buf, pb_size, "%d", v->u.int_val);
            } else {
                snprintf(buf, pb_size, "%.12g", v->u.double_val);
            }
            __si2drGroupAddName(sd_->gs[sd_->gsindex - 1], buf, &err);
            if (err == kSI2DR_OBJECT_ALREADY_EXISTS &&
                (ge != kTIMINGLIB_GROUPENUM_internal_power) &&
                (ge != kTIMINGLIB_GROUPENUM_vector) &&
                (ge != kTIMINGLIB_GROUPENUM_ccs_timing_base_curve)) {
                snprintf(sd_->PB, pb_size,
                         "%s:%d: The group name %s is already being used in "
                         "this context.",
                         sd_->curr_file, sd_->lineno, buf);
                (this->*MsgPrinter)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR,
                                    sd_->PB, &err);
            }
            if (err == kSI2DR_INVALID_NAME) {
                snprintf(sd_->PB, pb_size,
                         "%s:%d: The group name \"%s\" is invalid.",
                         sd_->curr_file, sd_->lineno, buf);
                (this->*MsgPrinter)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR,
                                    sd_->PB, &err);
            }
        } else {
            __si2drGroupAddName(sd_->gs[sd_->gsindex - 1], v->u.string_val,
                                &err);
            if (err == kSI2DR_OBJECT_ALREADY_EXISTS &&
                (ge != kTIMINGLIB_GROUPENUM_internal_power) &&
                (ge != kTIMINGLIB_GROUPENUM_vector) &&
                (ge != kTIMINGLIB_GROUPENUM_library) &&
                (ge != kTIMINGLIB_GROUPENUM_intrinsic_resistance) &&
                (ge != kTIMINGLIB_GROUPENUM_va_compact_ccs_rise) &&
                (ge != kTIMINGLIB_GROUPENUM_va_compact_ccs_fall) &&
                (ge != kTIMINGLIB_GROUPENUM_va_receiver_capacitance1_fall) &&
                (ge != kTIMINGLIB_GROUPENUM_va_receiver_capacitance2_fall) &&
                (ge != kTIMINGLIB_GROUPENUM_va_receiver_capacitance1_rise) &&
                (ge != kTIMINGLIB_GROUPENUM_va_receiver_capacitance2_rise) &&
                (ge != kTIMINGLIB_GROUPENUM_va_rise_constraint) &&
                (ge != kTIMINGLIB_GROUPENUM_va_fall_constraint) && (ge != 0)) {
                snprintf(sd_->PB, pb_size,
                         "%s:%d: The group name %s is already being used in "
                         "this context.",
                         sd_->curr_file, sd_->lineno, v->u.string_val);
                (this->*MsgPrinter)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR,
                                    sd_->PB, &err);
            }
            if (err == kSI2DR_INVALID_NAME) {
                snprintf(sd_->PB, pb_size,
                         "%s:%d: The group name \"%s\" is invalid. It will not "
                         "be added to the database\n",
                         sd_->curr_file, sd_->lineno, v->u.string_val);
                (this->*MsgPrinter)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR,
                                    sd_->PB, &err);
            }
        }
        vn = v->next;
        free(v);
    }
}
void LibAnalysis::popGroup(timinglib_head *h) {
    sd_->gsindex--;
    free(h);
    if (libbuilder_) libbuilder_->endGroup();
}
void LibAnalysis::makeComplex(timinglib_head *h) {
    Timinglib::timinglib_attribute_value *v, *vn;
    sd_->curr_attr = __si2drGroupCreateAttr(sd_->gs[sd_->gsindex - 1], h->name,
                                            kSI2DR_COMPLEX, &sd_->err);
    if (sd_->token_comment_buf[0]) {
        __si2drAttrSetComment(sd_->curr_attr, sd_->token_comment_buf,
                              &sd_->err);
        sd_->token_comment_buf[0] = 0;
        sd_->tok_encountered = 0;
    }
    si2drObjectSetLineNo(sd_->curr_attr, h->lineno, &sd_->err);
    si2drObjectSetFileName(sd_->curr_attr, h->filename, &sd_->err);
    if (libbuilder_) libbuilder_->buildAttribute(h->name, h->list);
    for (v = h->list; v; v = vn) {
        if (v->type == kTIMINGLIB__VAL_BOOLEAN)
            __si2drComplexAttrAddBooleanValue(
                sd_->curr_attr, static_cast<si2drBooleanT>(v->u.bool_val),
                &sd_->err);
        else if (v->type == kTIMINGLIB__VAL_STRING)
            __si2drComplexAttrAddStringValue(
                sd_->curr_attr, static_cast<si2drStringT>(v->u.string_val),
                &sd_->err);
        else if (v->type == kTIMINGLIB__VAL_DOUBLE)
            __si2drComplexAttrAddFloat64Value(sd_->curr_attr, v->u.double_val,
                                              &sd_->err);
        else
            __si2drComplexAttrAddInt32Value(sd_->curr_attr, v->u.int_val,
                                            &sd_->err);
        vn = v->next;
        free(v);
    }
    free(h);
}
void LibAnalysis::makeSimple(char *name, timinglib_attribute_value *v) {
    sd_->curr_attr = __si2drGroupCreateAttr(sd_->gs[sd_->gsindex - 1], name,
                                            kSI2DR_SIMPLE, &sd_->err);
    if (sd_->token_comment_buf[0]) {
        __si2drAttrSetComment(sd_->curr_attr, sd_->token_comment_buf,
                              &sd_->err);
        sd_->token_comment_buf[0] = 0;
        sd_->tok_encountered = 0;
    }
    si2drObjectSetLineNo(sd_->curr_attr, sd_->lineno, &sd_->err);
    si2drObjectSetFileName(sd_->curr_attr, sd_->curr_file, &sd_->err);
    if (v->type == kTIMINGLIB__VAL_BOOLEAN)
        __si2drSimpleAttrSetBooleanValue(
            sd_->curr_attr, static_cast<si2drBooleanT>(v->u.bool_val),
            &sd_->err);
    else if (v->type == kTIMINGLIB__VAL_EXPR)
        __si2drSimpleAttrSetExprValue(sd_->curr_attr,
                                      static_cast<si2drExprT *>(v->u.expr_val),
                                      &sd_->err);
    else if (v->type == kTIMINGLIB__VAL_STRING)
        __si2drSimpleAttrSetStringValue(
            sd_->curr_attr, static_cast<si2drStringT>(v->u.string_val),
            &sd_->err);
    else if (v->type == kTIMINGLIB__VAL_DOUBLE)
        __si2drSimpleAttrSetFloat64Value(sd_->curr_attr, v->u.double_val,
                                         &sd_->err);
    else
        __si2drSimpleAttrSetInt32Value(sd_->curr_attr, v->u.int_val, &sd_->err);
    if (libbuilder_) libbuilder_->buildAttribute(name, v);
    free(v);
}
void LibAnalysis::syntaxPushGroup(libsynt_head *h) {
    si2drObjectIdT toid, noid;
    sd_->gsinfo[sd_->gsindex] = static_cast<libsynt_group_info *>(
        calloc(sizeof(libsynt_group_info), 1));
    toid.v1 = static_cast<void *>(sd_->gsinfo[sd_->gsindex]);
    sd_->gsinfo[sd_->gsindex]->lineno = h->lineno;
    sd_->gsinfo[sd_->gsindex]->type = h->ident;
    sd_->gsinfo[sd_->gsindex]->name_constraints = h->namecons;
    sd_->gsinfo[sd_->gsindex]->attr_hash =
        LibHash::timinglibHashCreateHashTable(41, 1, 0);
    sd_->gsinfo[sd_->gsindex]->group_hash =
        LibHash::timinglibHashCreateHashTable(41, 1, 0);
    LibHash::timinglibHashLookup(sd_->libsynt_groups, h->ident, &noid);
    if (noid.v1 == static_cast<void *>(NULL))
        LibHash::timinglibHashEnterOid(sd_->libsynt_groups, h->ident, toid);
    LibHash::timinglibHashLookup(sd_->libsynt_allgroups, h->ident, &noid);
    if (noid.v1 == static_cast<void *>(NULL))
        LibHash::timinglibHashEnterOid(sd_->libsynt_allgroups, h->ident, toid);
    sd_->gsindex++;
}
void LibAnalysis::syntaxPopGroup(void) { sd_->gsindex--; }
libsynt_attribute_info *LibAnalysis::syntaxMakeComplex(libsynt_head *h) {
    si2drObjectIdT toid, noid;
    libsynt_attribute_info *x = static_cast<libsynt_attribute_info *>(
        calloc(sizeof(libsynt_attribute_info), 1));
    x->name = h->ident;
    toid.v1 = static_cast<void *>(x);
    if (h->arglist->type == kSYNTAX_ATTRTYPE_COMPLEX_UNKNOWN)
        x->type = kSYNTAX_ATTRTYPE_COMPLEX_UNKNOWN;
    else if (h->arglist->type == kSYNTAX_ATTRTYPE_COMPLEX_LIST)
        x->type = kSYNTAX_ATTRTYPE_COMPLEX_LIST;
    else
        x->type = kSYNTAX_ATTRTYPE_COMPLEX;
    x->u.args = h->arglist;
    LibHash::timinglibHashLookup(sd_->libsynt_attrs, h->ident, &noid);
    if (noid.v1 == static_cast<void *>(NULL))
        LibHash::timinglibHashEnterOid(sd_->libsynt_attrs, h->ident, toid);
    return x;
}
libsynt_attribute_info *LibAnalysis::syntaxMakeSimple(char *name,
                                                      libsynt_attr_type type,
                                                      void *constraint_ptr) {
    si2drObjectIdT toid, noid;
    libsynt_attribute_info *x = static_cast<libsynt_attribute_info *>(
        calloc(sizeof(libsynt_attribute_info), 1));
    x->name = name;
    x->type = type;
    toid.v1 = static_cast<void *>(x);
    switch (type) {
        case kSYNTAX_ATTRTYPE_STRING:
            break;
        case kSYNTAX_ATTRTYPE_VIRTUAL:
            break;
        case kSYNTAX_ATTRTYPE_ENUM:
            x->u.stringenum =
                static_cast<libsynt_string_enum *>(constraint_ptr);
            break;
        case kSYNTAX_ATTRTYPE_FLOAT:
            x->u.floatcon =
                static_cast<libsynt_float_constraint *>(constraint_ptr);
            break;
        case kSYNTAX_ATTRTYPE_INT:
            x->u.intcon = static_cast<libsynt_int_constraint *>(constraint_ptr);
            break;
        case kSYNTAX_ATTRTYPE_BOOLEAN:
            break;
        default:
            break;
    }
    LibHash::timinglibHashLookup(sd_->libsynt_attrs, x->name, &noid);
    if (noid.v1 == static_cast<void *>(NULL))
        LibHash::timinglibHashEnterOid(sd_->libsynt_attrs, x->name, toid);
    return x;
}
bool LibAnalysis::isLibertySyntaxValid() {
    bool bOk = true;
    si2drErrorT err;
    si2drGroupsIdT groups = __si2drPIGetGroups(&err);
    si2drGroupIdT group;
    while (!__si2drObjectIsNull((group = __si2drIterNextGroup(groups, &err)),
                                &err)) {
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_NOTE, kSI2DR_NO_ERROR,
                            si2drStringT("Checking the database..."), &err);

        __si2drCheckLibertyLibrary(group, &err);
        if (err == kSI2DR_NO_ERROR) {
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_NOTE, kSI2DR_NO_ERROR,
                                si2drStringT("Passed."), &err);
        } else {
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_NOTE, kSI2DR_NO_ERROR,
                                si2drStringT("Errors detected."), &err);
            bOk = false;
        }
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_NOTE, kSI2DR_NO_ERROR,
                            si2drStringT("Checking the database "
                                         "finished."),
                            &err);
    }
    __si2drIterQuit(groups, &err);

    if (!bOk) return bOk;

    int cellcount = 0;
    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_NOTE, kSI2DR_NO_ERROR,
                        si2drStringT("Stats:"), &err);
    groups = __si2drPIGetGroups(&err);
    while (!__si2drObjectIsNull((group = __si2drIterNextGroup(groups, &err)),
                                &err)) {
        si2drNamesIdT gnames;
        si2drStringT gname;
        si2drGroupsIdT gs2;
        si2drGroupIdT g2;
        gnames = __si2drGroupGetNames(group, &err);
        gname = __si2drIterNextName(gnames, &err);
        __si2drIterQuit(gnames, &err);
        if (!gname) gname = si2drStringT("<NONAME>");
        std::string str = stringFormat("Library name: %s.", gname);
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_NOTE, kSI2DR_NO_ERROR,
                            si2drStringT(str.c_str()), &err);
        gs2 = __si2drGroupGetGroups(group, &err);
        while (!__si2drObjectIsNull((g2 = __si2drIterNextGroup(gs2, &err)),
                                    &err)) {
            si2drStringT gt = __si2drGroupGetGroupType(g2, &err);
            if (!strcmp(gt, "cell")) cellcount++;
        }
        __si2drIterQuit(gs2, &err);
        str = stringFormat("Cell count: %d.", cellcount);
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_NOTE, kSI2DR_NO_ERROR,
                            si2drStringT(str.c_str()), &err);
    }
    __si2drIterQuit(groups, &err);
    // if( strtab_tots )
    // strtab_->printStrtabStats();
    // (this->*si2ErrMsg_)(kSI2DR_SEVERITY_NOTE, kSI2DR_NO_ERROR,
    //                    si2drStringT("Quitting.."), &err);
    // (this->*si2ErrMsg_)(kSI2DR_SEVERITY_NOTE, kSI2DR_NO_ERROR,
    //                    si2drStringT("Done...."), &err);

    return bOk;
}
bool LibAnalysis::dumpLibFile(const char *const filename,
                              bool clearFileContent /*=true*/) {
    si2drGroupsIdT groups;
    si2drGroupIdT group;
    si2drErrorT err;

    FILE *of = nullptr;
    if (clearFileContent)
        of = fopen(filename, "w");
    else
        of = fopen(filename, "a+");
    if (of == nullptr) {
        std::string str =
            stringFormat("Could not open %s for writing.", filename);
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_NO_ERROR,
                            si2drStringT(str.c_str()), &err);
        return false;
    }

    std::string str = stringFormat("Write file: %s...", filename);
    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_NOTE, kSI2DR_NO_ERROR,
                        si2drStringT(str.c_str()), &err);

    groups = __si2drPIGetGroups(&err);

    while (!__si2drObjectIsNull((group = __si2drIterNextGroup(groups, &err)),
                                &err)) {
        si2drNamesIdT gnames;
        si2drStringT gname;

        gnames = __si2drGroupGetNames(group, &err);
        gname = __si2drIterNextName(gnames, &err);
        __si2drIterQuit(gnames, &err);

        __si2drWriteLibertyFile(of, group, &err);
    }
    __si2drIterQuit(groups, &err);

    fclose(of);

    str = stringFormat("Write file: %s finished.", filename);
    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_NOTE, kSI2DR_NO_ERROR,
                        si2drStringT(str.c_str()), &err);

    return true;
}
void LibAnalysis::cleanFileName(char *dirty, char *clean) {
    /* clean (no quotes, no spaces) the name;
   then make sure it exists in the current dir or the path; if
   it exists in the path, plug in the path component into the name,
   so the later fopen will find it */
    char cleanbuf[4096], *p, anotherbuf[4096];
    size_t size = 4096;
    p = dirty;
    while (*p && (*p == '"' || *p == ' ' || *p == '\t')) {
        p++;
    }
    snprintf(cleanbuf, size, "%s", p);
    p = cleanbuf + strlen(cleanbuf) - 1;
    while (*p && p > cleanbuf && (*p == '"' || *p == ' ' || *p == '\t'))
        *p-- = 0;
    if (access(cleanbuf, F_OK) == 0) {
        snprintf(clean, size, "%s", cleanbuf);
        return;
    } else {
        /* hmmm, not in the current directory? */
        char enbuf[20256];
        char *en = getenv("TIMINGLIB_PARSER_SEARCH_PATH");
        if (!en) {
            snprintf(clean, size, "%s", cleanbuf);
            return;
        } else {
            char *q;
            snprintf(enbuf, size, "%s:", en);
            q = enbuf;
            while (q && *q && (p = strchr(q, ':'))) {
                *p = 0;
                if (p > enbuf && (*(p - 1) == '/' || *(p - 1) == '\\'))
                    *(p - 1) = 0;
                snprintf(anotherbuf, size, "%s/%s", q, cleanbuf);
                if (access(anotherbuf, F_OK) == 0) {
                    snprintf(clean, size, "%s", anotherbuf);
                    return;
                }
                q = p + 1;
            }
        }
    }
}
si2drValueTypeT LibAnalysis::convertVt(char *type) {
    if (!strcmp(type, "string")) return kSI2DR_STRING;
    if (!strcmp(type, "integer")) return kSI2DR_INT32;
    if (!strcmp(type, "float")) return kSI2DR_FLOAT64;
    if (!strcmp(type, "boolean")) return kSI2DR_BOOLEAN;
    return kSI2DR_UNDEFINED_VALUETYPE;
}
std::string LibAnalysis::__oidString(si2drObjectIdT oid) {
    char buf[500];
    size_t size = 500;
    if (oid.v1 == 0 && oid.v2 == 0)
        snprintf(buf, size, "%s", "nulloid_");
    else
        snprintf(buf, size, "OID_%s_%llx",
                 toString((si2drObjectTypeT)(
                     reinterpret_cast<std::intptr_t>(oid.v1))),
                 reinterpret_cast<uint64_t>(oid.v2));
    return std::string(buf);
}
std::string LibAnalysis::__iterString(si2drIterIdT iter) {
    iterat *y = static_cast<iterat *>(iter);
    char buf[500];
    char *itype = 0;
    switch (y->owner) {
        case kITER_NAME:
            itype = const_cast<char *>("NAME");
            break;
        case kITER_VAL:
            itype = const_cast<char *>("VAL");
            break;
        case kITER_GROUP:
            itype = const_cast<char *>("GROUP");
            break;
        case kITER_ATTR:
            itype = const_cast<char *>("ATTR");
            break;
        case kITER_DEF:
            itype = const_cast<char *>("DEFINE");
            break;
        case kITER_NOTHING:
            itype = const_cast<char *>("NOTHING");
            break;
        default:
            break;
    }
    snprintf(buf, sizeof(buf), "ITER_%s_%llx", itype,
             reinterpret_cast<uint64_t>(iter));
    // sprintf(buf[index], "ITER_%s_%x", itype, static_cast<unsigned
    // int>(iter));
    return std::string(buf);
}
const char *LibAnalysis::__itypeString(si2drIterIdT iter) {
    iterat *y = static_cast<iterat *>(iter);
    switch (y->owner) {
        case kITER_NAME:
            return "si2drNamesIdT";
            break;
        case kITER_VAL:
            return "si2drValuesIdT";
            break;
        case kITER_GROUP:
            return "si2drGroupsIdT";
            break;
        case kITER_ATTR:
            return "si2drAttrsIdT";
            break;
        case kITER_DEF:
            return "si2drDefinesIdT";
            break;
        default:
            return "si2drObjectsIdT";
            break;
    }
    return "";
}
void LibAnalysis::__traceCheck(si2drObjectIdT oid) {
    fprintf(tracefile1_, "\t\tcheckResults(%s, err, __FILE__,__LINE__);\n",
            __oidString(oid).c_str());
    fflush(tracefile1_);
}
void LibAnalysis::__outinitOid(si2drObjectIdT oid) {
    fprintf(
        tracefile2_, "%s %s;\n",
        toString((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(oid.v2))),
        __oidString(oid).c_str());
}
void LibAnalysis::__outinitIter(si2drIterIdT iter) {
    fprintf(tracefile2_, "%s %s;\n", __itypeString(iter),
            __iterString(iter).c_str());
}
void LibAnalysis::__incTracecount(void) {
    tracecount_++;
    trace_grand_total_++;
    if (tracecount_ > 1200) {
        tracefunc_++;
        fprintf(tracefile1_, "}\n\n\n");
        if ((tracefunc_ % 6) == 0) {
            char buf[100];
            tracefilecount_++;
            fclose(tracefile1_);
            char tracefbase[500];
            snprintf(buf, sizeof(buf), "%s%02d.c", tracefbase, tracefilecount_);
            tracefile1_ = fopen(buf, "w");
            if (!tracefile1_) {
                printf("Tracing turned off due to file open problems\n");
                trace_ = 0;
                return;
            } else {
                fprintf(tracefile1_, "#include <stdio.h>\n\n");
                fprintf(tracefile1_, "#include \"timinglib_si2dr.h\"\n");
                fprintf(tracefile1_, "#include \"oiddecls.h\"\n\n\n");
                fprintf(tracefile1_,
                        "extern void checkResults(si2drObjectIdT result, "
                        "si2drErrorT err, char *fname, int lineno);\n\n");
            }
        }
        fprintf(tracefile1_, "\n\nvoid func%d(void)\n{\n", tracefunc_);
        fprintf(tracefile1_, "\t/* output decls used later */\n");
        fprintf(tracefile1_, "\tsi2drValueTypeT type;\n");
        fprintf(tracefile1_, "\tsi2drInt32T     intgr;\n");
        fprintf(tracefile1_, "\tsi2drFloat64T   float64;\n");
        fprintf(tracefile1_, "\tsi2drStringT    string;\n");
        fprintf(tracefile1_, "\tsi2drStringT    name_string;\n");
        fprintf(tracefile1_, "\tsi2drStringT    allowed_groups;\n");
        fprintf(tracefile1_, "\tsi2drStringT    valtype;\n");
        fprintf(tracefile1_, "\tsi2drBooleanT   boolval;\n");
        fprintf(tracefile1_, "\tsi2drErrorT     err;\n");
        fprintf(tracefile1_,
                "\tsi2drObjectIdT  nulloid_ = __si2drPIGetNullId(&err);\n");
        tracecount_ = 0;
    }
}
si2drVoidT LibAnalysis::__si2drPISetDebugMode(si2drErrorT *err) {
    timinglib_debug_mode_ = 1;
    *err = kSI2DR_NO_ERROR;
    if (trace_) {
        fprintf(tracefile1_, "\n\t__si2drPISetDebugMode(&err);\n\n");
        __incTracecount();
    }
}
si2drVoidT LibAnalysis::__si2drPIUnSetDebugMode(si2drErrorT *err) {
    timinglib_debug_mode_ = 0;
    *err = kSI2DR_NO_ERROR;
    if (trace_) {
        fprintf(tracefile1_, "\n\t__si2drPIUnSetDebugMode(&err);\n\n");
        __incTracecount();
    }
}
si2drBooleanT LibAnalysis::__si2drPIGetDebugMode(si2drErrorT *err) {
    *err = kSI2DR_NO_ERROR;
    if (trace_) {
        fprintf(tracefile1_, "\n\t__si2drPIGetDebugMode(&err);\n\n");
        __incTracecount();
    }
    if (timinglib_debug_mode_ != 0)
        return SI2DR_TRUE;
    else
        return SI2DR_FALSE;
}
si2drVoidT LibAnalysis::si2drPISetNocheckMode(si2drErrorT *err) {
    timinglib_nocheck_mode_ = 1;
    *err = kSI2DR_NO_ERROR;
    if (trace_) {
        fprintf(tracefile1_, "\n\tsi2drPISetNocheckMode(&err);\n\n");
        __incTracecount();
    }
}
si2drVoidT LibAnalysis::si2drPIUnSetNocheckMode(si2drErrorT *err) {
    timinglib_nocheck_mode_ = 0;
    *err = kSI2DR_NO_ERROR;
    if (trace_) {
        fprintf(tracefile1_, "\n\tsi2drPIUnSetNocheckMode(&err);\n\n");
        __incTracecount();
    }
}
si2drBooleanT LibAnalysis::__si2drPIGetNocheckMode(si2drErrorT *err) {
    *err = kSI2DR_NO_ERROR;
    if (trace_) {
        fprintf(tracefile1_, "\n\t__si2drPIGetNocheckMode(&err);\n\n");
        __incTracecount();
    }
    if (timinglib_nocheck_mode_ != 0)
        return SI2DR_TRUE;
    else
        return SI2DR_FALSE;
}
si2drVoidT LibAnalysis::__si2drDefaultMessageHandler(si2drSeverityT sev,
                                                     si2drErrorT errToPrint,
                                                     si2drStringT auxText,
                                                     si2drErrorT *err) {
    si2drErrorT err1;
    char *sevstr, *errt = __si2drPIGetErrorText(errToPrint, &err1);
    std::string str = "";
    switch (sev) {
        case kSI2DR_SEVERITY_NOTE:
            sevstr = const_cast<char *>("INFO");
            str = stringFormat("%s: %s\n", sevstr, auxText);
            // open_edi::util::message->issueMsg("TIMINGLIB", 14,
            // open_edi::util::kInfo, auxText);
            break;
        case kSI2DR_SEVERITY_WARN:
            sevstr = const_cast<char *>("WARNING");
            str = stringFormat("%s: %s\n", sevstr, auxText);
            // open_edi::util::message->issueMsg("TIMINGLIB", 14,
            // open_edi::util::kWarn, auxText);
            break;
        case kSI2DR_SEVERITY_ERR:
            sevstr = const_cast<char *>("ERROR");
            if (auxText) {
                if (!strcmp(errt, "")) {
                    str = stringFormat("%s: %s\n", sevstr, auxText);
                    open_edi::util::message->issueMsg(
                        "TIMINGLIB", 14, open_edi::util::kError, auxText);
                } else {
                    str = stringFormat("%s: %s (%s)\n", sevstr, auxText, errt);
                    open_edi::util::message->issueMsg(
                        "TIMINGLIB", 15, open_edi::util::kError, auxText, errt);
                }
            } else {
                str = stringFormat("%s: %s\n", sevstr, errt);
                open_edi::util::message->issueMsg("TIMINGLIB", 14,
                                                  open_edi::util::kError, errt);
            }
            break;
    }
    if (str != "" && parseLogStr_ != nullptr) {
        *parseLogStr_ += str;
        // printf("%s", str.c_str());
    }

    *err = kSI2DR_NO_ERROR;
}
si2drVoidT LibAnalysis::__si2drPISetMessageHandler(si2drMessageHandlerT handler,
                                                   si2drErrorT *err) {
    si2ErrMsg_ = handler;
    *err = kSI2DR_NO_ERROR;
}
si2drMessageHandlerT LibAnalysis::si2drPIGetMessageHandler(si2drErrorT *err) {
    *err = kSI2DR_NO_ERROR;
    return si2ErrMsg_;
}
si2drVoidT LibAnalysis::__si2drPISetTraceMode(si2drStringT fname,
                                              si2drErrorT *err) {
    char fnamet[1000];
    char fnameth[1000];
    char tracefbase[500];
    snprintf(tracefbase, sizeof(tracefbase), "%s", fname);
    snprintf(fnamet, sizeof(fnamet), "%s00.c", fname);
    snprintf(fnameth, sizeof(fnameth), "%s_oiddecls.h", fname);
    tracefile1_ = fopen(fnamet, "w");
    tracefile2_ = fopen(fnameth, "w");
    if (!tracefile1_ || !tracefile2_) {
        trace_ = 0;
        *err = kSI2DR_TRACE_FILES_CANNOT_BE_OPENED;
    } else {
        fprintf(tracefile1_, "#include <stdio.h>\n\n");
        fprintf(tracefile1_, "#include \"timinglib_si2dr.h\"\n");
        fprintf(tracefile1_, "#include \"%s_oiddecls.h\"\n\n\n", fname);
        fprintf(tracefile1_, "si2drObjectIdT nulloid_;\n\n");
        fprintf(tracefile1_,
                "void checkResults(si2drObjectIdT result, si2drErrorT err, "
                "char *fname, int lineno)\n");
        fprintf(tracefile1_, "{\n");
        fprintf(tracefile1_, "    si2drErrorT err3;\n");
        fprintf(tracefile1_, "    \n");
        fprintf(tracefile1_, "    if( err == kSI2DR_NO_ERROR )\n");
        fprintf(tracefile1_, "    {\n");
        fprintf(tracefile1_,
                "        if( !__si2drObjectIsUsable(result,&err3) )\n");
        fprintf(tracefile1_, "        {\n");
        fprintf(tracefile1_,
                "            printf(\"Unusable OID returned from function at "
                "\\\"%%s\\\", line %%d\\n\", fname, lineno);\n");
        fprintf(tracefile1_, "        }\n");
        fprintf(tracefile1_, "    }\n");
        fprintf(tracefile1_, "}\n\n");
        fprintf(tracefile1_, "void func0(void)\n{\n");
        fprintf(tracefile1_, "\t/* output decls used later */\n");
        fprintf(tracefile1_, "\tsi2drErrorT     err;\n");
        fprintf(tracefile1_, "\tsi2drValueTypeT type;\n");
        fprintf(tracefile1_, "\tsi2drInt32T     intgr;\n");
        fprintf(tracefile1_, "\tsi2drFloat64T   float64;\n");
        fprintf(tracefile1_, "\tsi2drStringT    string;\n");
        fprintf(tracefile1_, "\tsi2drStringT    name_string;\n");
        fprintf(tracefile1_, "\tsi2drStringT    allowed_groups;\n");
        fprintf(tracefile1_, "\tsi2drStringT    valtype;\n");
        fprintf(tracefile1_, "\tsi2drBooleanT   boolval;\n");
        fprintf(tracefile1_, "\tsi2drObjectIdT  nulloid_;\n");
        fprintf(tracefile1_, "\tnulloid_ = __si2drPIGetNullId(&err);\n\n");
        trace_ = 1;
        *err = kSI2DR_NO_ERROR;
    }
}
si2drVoidT LibAnalysis::__si2drPIUnSetTraceMode(si2drErrorT *err) {
    int i, j, k;
    for (i = k = 0; i < tracefilecount_; i++) {
        for (j = 0; j < 6; j++) {
            fprintf(tracefile1_, "extern void func%d(void);\n", k++);
        }
    }
    fprintf(tracefile1_, "}\n\n\nint main(int argc, char **argv)\n{\n");
    for (i = 0; i <= tracefunc_; i++) {
        fprintf(tracefile1_, "\tfunc%d();\n", i);
    }
    fprintf(tracefile1_, "\treturn 0;\n}\n");
    fclose(tracefile1_);
    fclose(tracefile2_);
    *err = kSI2DR_NO_ERROR;
}
si2drBooleanT LibAnalysis::__si2drPIGetTraceMode(si2drErrorT *err) {
    *err = kSI2DR_NO_ERROR;
    if (timinglib_trace_mode_CFP_ != 0)
        return SI2DR_TRUE;
    else
        return SI2DR_FALSE;
}
/* here are the implementations of the functions described in timinglib_si2dr.h
 */
si2drGroupIdT LibAnalysis::__si2drPICreateGroup(si2drStringT name,
                                                si2drStringT group_type,
                                                si2drErrorT *err) {
    if (strtab_ == nullptr || sd_ == nullptr) return nulloid_;
    timinglib_hash_table *&master_group_hash = sd_->master_group_hash;
    timinglib_group *&master_group_list = sd_->master_group_list;
    timinglib_strtable *&master_string_table = sd_->master_string_table;
    si2drGroupIdT retoid, toid;
    const libGroupMap *lgm;
    timinglib_group *g;
    char nameb[1500], *nnb = 0;
    // int nbl=0;
    int npl = strlen(group_type) + 4;
    if (name) {
        snprintf(nameb, sizeof(nameb), "%s||||%s", group_type, name);
        // nbl = strlen(nameb);
    }
    if (!timinglib_nocheck_mode_) {
        if (!master_group_hash) {
            *err = kSI2DR_PIINIT_NOT_CALLED;
            return nulloid_;
        }
        if (name && *name) {
            LibHash::timinglibHashLookup(master_group_hash, name, &toid);
            if (toid.v1 != static_cast<void *>(0)) {
                *err = kSI2DR_OBJECT_ALREADY_EXISTS;
                if (timinglib_debug_mode_) {
                    si2drErrorT err2;
                    (this->*si2ErrMsg_)(
                        kSI2DR_SEVERITY_ERR, kSI2DR_OBJECT_ALREADY_EXISTS,
                        si2drStringT("__si2drPICreateGroup: "), &err2);
                }
                return nulloid_;
            }
        }
    }
    g = static_cast<timinglib_group *>(calloc(sizeof(timinglib_group), 1));
    retoid.v1 = reinterpret_cast<void *>(kSI2DR_GROUP);
    retoid.v2 = static_cast<void *>(g);
    if (trace_) {
        __outinitOid(retoid);
        fprintf(tracefile1_,
                "\n\t%s = __si2drPICreateGroup(\"%s\", \"%s\"), &err);\n\n",
                __oidString(retoid).c_str(), name, group_type);
        __traceCheck(retoid);
        __incTracecount();
    }
    g->names = static_cast<timinglib_name_list *>(
        calloc(sizeof(timinglib_name_list), 1));
    g->last_name = g->names;
    g->type =
        strtab_->timinglibStrtableEnterString(master_string_table, group_type);
    g->names->prefix_len = npl;
    lgm = lookup_group_name(group_type, strlen(group_type));
    if (lgm) g->EVAL = lgm->type;
    if (name && *name) {
        /* OLD:
            nnb = (char*)malloc(nbl+1);
            strcpy(nnb,nameb);
            g->names->name = nnb;
            NEW: */
        nnb = g->names->name =
            strtab_->timinglibStrtableEnterString(master_string_table, nameb);
    } else {
        g->names->name = 0;
    }
    /* set up hash tables */
    g->attr_hash = LibHash::timinglibHashCreateHashTable(503, 1, 0);
    g->define_hash = LibHash::timinglibHashCreateHashTable(53, 1, 0);
    g->group_hash = LibHash::timinglibHashCreateHashTable(2011, 1, 0);
    /* link into the master list */
    g->next = master_group_list;
    master_group_list = g;
    if (name && *name) {
        /* put this name(s) into the master hash tab */
        LibHash::timinglibHashEnterOid(master_group_hash, nnb, retoid);
    }
    *err = kSI2DR_NO_ERROR;
    return retoid;
}
si2drAttrIdT LibAnalysis::__si2drGroupCreateAttr(si2drGroupIdT group,
                                                 si2drStringT name,
                                                 si2drAttrTypeT type,
                                                 si2drErrorT *err) {
    if (strtab_ == nullptr || sd_ == nullptr) return nulloid_;
    timinglib_strtable *master_string_table = sd_->master_string_table;
    si2drAttrIdT retoid, toid;
    const libAttrMap *lam;
    timinglib_group *g = static_cast<timinglib_group *>(group.v2);
    timinglib_attribute *attr;
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(
                reinterpret_cast<std::intptr_t>(group.v1))) != kSI2DR_GROUP) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drGroupCreateAttr:"), &err2);
            }
            return nulloid_;
        }
        if (g->attr_hash == static_cast<timinglib_hash_table *>(NULL)) {
            *err = kSI2DR_UNUSABLE_OID;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_UNUSABLE_OID,
                                    si2drStringT("__si2drGroupCreateAttr:"),
                                    &err2);
            }
            return nulloid_;
        }
        if (name && *name) {
            LibHash::timinglibHashLookup(g->attr_hash, name, &toid);
            /* [20060809:nanda] Allow multiple define_group attributes */
            /* if( toid.v1 != (void*)0 ) */
            if (toid.v1 != static_cast<void *>(0) &&
                strcmp(name, "define_group") && strcmp(name, "curve_y") &&
                strcmp(name, "library_features") && strcmp(name, "vector") &&
                strcmp(name, "voltage_map") && strcmp(name, "power_rail")) {
                *err = kSI2DR_OBJECT_ALREADY_EXISTS;
                if (timinglib_debug_mode_) {
                    si2drErrorT err2;
                    (this->*si2ErrMsg_)(
                        kSI2DR_SEVERITY_ERR, kSI2DR_OBJECT_ALREADY_EXISTS,
                        si2drStringT("__si2drGroupCreateAttr:"), &err2);
                }
                return nulloid_;
            }
        } else {
            *err = kSI2DR_INVALID_NAME;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_NAME,
                                    si2drStringT("__si2drGroupCreateAttr:"),
                                    &err2);
            }
            return nulloid_;
        }
        if (type != kSI2DR_SIMPLE && type != kSI2DR_COMPLEX) {
            *err = kSI2DR_INVALID_ATTRTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_ATTRTYPE,
                    si2drStringT("__si2drGroupCreateAttr:"), &err2);
            }
            return nulloid_;
        }
    }
    attr = static_cast<timinglib_attribute *>(
        calloc(sizeof(timinglib_attribute), 1));
    retoid.v1 = reinterpret_cast<void *>(kSI2DR_ATTR);
    retoid.v2 = static_cast<void *>(attr);
    if (trace_) {
        __outinitOid(retoid);
        fprintf(tracefile1_,
                "\n\t%s = __si2drGroupCreateAttr(%s,\"%s\", %s, &err);\n\n",
                __oidString(retoid).c_str(), __oidString(group).c_str(), name,
                toString(type));
        __traceCheck(retoid);
        __incTracecount();
    }
    attr->owner = g;
    attr->name =
        strtab_->timinglibStrtableEnterString(master_string_table, name);
    lam = lookup_attr_name(name, strlen(name));
    if (lam) attr->EVAL = lam->type;
    switch (type) {
        case kSI2DR_SIMPLE:
            attr->type = kTIMINGLIB__SIMPLE;
            break;
        case kSI2DR_COMPLEX:
            attr->type = kTIMINGLIB__COMPLEX;
            break;
    }
    LibHash::timinglibHashEnterOid(
        g->attr_hash, attr->name,
        retoid); /* fix via chrisj's posting: attr->name instead of name */
    /* nanda also spotted this */
    /* link the attr struct into the lists */
    if (g->attr_last) {
        g->attr_last->next = attr;
        g->attr_last = attr;
    } else {
        g->attr_last = attr;
        g->attr_list = attr;
    }
    *err = kSI2DR_NO_ERROR;
    return retoid;
}
/* Functions to handle comments */
si2drStringT LibAnalysis::__si2drGroupGetComment(si2drGroupIdT group,
                                                 si2drErrorT *err) {
    timinglib_group *g = static_cast<timinglib_group *>(group.v2);
    return static_cast<si2drStringT>(g->comment);
}
si2drVoidT LibAnalysis::__si2drGroupSetComment(si2drGroupIdT group,
                                               si2drStringT comment,
                                               si2drErrorT *err) {
    if (strtab_ == nullptr || sd_ == nullptr) return;
    timinglib_strtable *master_string_table = sd_->master_string_table;
    timinglib_group *g = static_cast<timinglib_group *>(group.v2);
    g->comment =
        strtab_->timinglibStrtableEnterString(master_string_table, comment);
}
si2drStringT LibAnalysis::__si2drAttrGetComment(si2drAttrIdT attr,
                                                si2drErrorT *err) {
    timinglib_attribute *a = static_cast<timinglib_attribute *>(attr.v2);
    return static_cast<si2drStringT>(a->comment);
}
si2drVoidT LibAnalysis::__si2drAttrSetComment(si2drAttrIdT attr,
                                              si2drStringT comment,
                                              si2drErrorT *err) {
    if (strtab_ == nullptr || sd_ == nullptr) return;
    timinglib_strtable *master_string_table = sd_->master_string_table;
    timinglib_attribute *a = static_cast<timinglib_attribute *>(attr.v2);
    a->comment =
        strtab_->timinglibStrtableEnterString(master_string_table, comment);
}
si2drStringT LibAnalysis::__si2drDefineGetComment(si2drDefineIdT def,
                                                  si2drErrorT *err) {
    timinglib_define *d = static_cast<timinglib_define *>(def.v2);
    return static_cast<si2drStringT>(d->comment);
}
si2drVoidT LibAnalysis::si2drDefineSetComment(si2drDefineIdT def,
                                              si2drStringT comment,
                                              si2drErrorT *err) {
    if (strtab_ == nullptr || sd_ == nullptr) return;
    timinglib_strtable *master_string_table = sd_->master_string_table;
    timinglib_define *d = static_cast<timinglib_define *>(def.v2);
    d->comment =
        strtab_->timinglibStrtableEnterString(master_string_table, comment);
}
/* These two functions unpublished because the ID will change as the standard
evolves, and I'm using it purely as a speedup. */
group_enum LibAnalysis::__si2drGroupGetID(si2drGroupIdT group,
                                          si2drErrorT *err) {
    timinglib_group *g = static_cast<timinglib_group *>(group.v2);
    return group_enum(g->EVAL);
}
attr_enum LibAnalysis::__si2drAttrGetID(si2drAttrIdT attr, si2drErrorT *err) {
    timinglib_attribute *a = static_cast<timinglib_attribute *>(attr.v2);
    return attr_enum(a->EVAL);
}
si2drAttrTypeT LibAnalysis::__si2drAttrGetAttrType(si2drAttrIdT attr,
                                                   si2drErrorT *err) {
    timinglib_attribute *x = static_cast<timinglib_attribute *>(attr.v2);
    if (trace_) {
        fprintf(tracefile1_, "\n\t__si2drAttrGetAttrType(%s,&err);\n\n",
                __oidString(attr).c_str());
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_ &&
        (si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(
            reinterpret_cast<std::intptr_t>(attr.v1))) != kSI2DR_ATTR) {
        *err = kSI2DR_INVALID_OBJECTTYPE;
        if (timinglib_debug_mode_) {
            si2drErrorT err2;
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                                si2drStringT("__si2drAttrGetAttrType:"), &err2);
        }
        return static_cast<si2drAttrTypeT>(0);
    }
    *err = kSI2DR_NO_ERROR;
    switch (x->type) {
        case kTIMINGLIB__SIMPLE:
            return kSI2DR_SIMPLE;
        case kTIMINGLIB__COMPLEX:
            return kSI2DR_COMPLEX;
    }
    *err = kSI2DR_INVALID_OBJECTTYPE;
    return static_cast<si2drAttrTypeT>(0);
}
si2drVoidT LibAnalysis::__si2drComplexAttrAddInt32Value(si2drAttrIdT attr,
                                                        si2drInt32T intgr,
                                                        si2drErrorT *err) {
    timinglib_attribute *x = static_cast<timinglib_attribute *>(attr.v2);
    timinglib_attribute_value *y;
    if (trace_) {
        fprintf(tracefile1_,
                "\n\t__si2drComplexAttrAddInt32Value(%s,%d,&err);\n\n",
                __oidString(attr).c_str(), static_cast<int>(intgr));
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(
                reinterpret_cast<std::intptr_t>(attr.v1))) != kSI2DR_ATTR) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drComplexAttrAddInt32Value:"), &err2);
            }
            return;
        }
        if (x->type != kTIMINGLIB__COMPLEX) {
            *err = kSI2DR_INVALID_ATTRTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_ATTRTYPE,
                    si2drStringT("__si2drComplexAttrAddInt32Value:"), &err2);
            }
            return;
        }
    }
    y = static_cast<timinglib_attribute_value *>(
        calloc(sizeof(timinglib_attribute_value), 1));
    y->type = kTIMINGLIB__VAL_INT;
    y->u.int_val = intgr;
    /* link the attr struct into the lists */
    if (x->last_value) {
        x->last_value->next = y;
        x->last_value = y;
    } else {
        x->last_value = y;
        x->value = y;
    }
    *err = kSI2DR_NO_ERROR;
}
si2drVoidT LibAnalysis::__si2drComplexAttrAddStringValue(si2drAttrIdT attr,
                                                         si2drStringT string,
                                                         si2drErrorT *err) {
    if (strtab_ == nullptr || sd_ == nullptr) return;
    timinglib_strtable *master_string_table = sd_->master_string_table;
    timinglib_attribute *x = static_cast<timinglib_attribute *>(attr.v2);
    timinglib_attribute_value *y;
    if (trace_) {
        fprintf(tracefile1_,
                "\n\t__si2drComplexAttrAddStringValue(%s,\"%s\",&err);\n\n",
                __oidString(attr).c_str(), string);
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(
                reinterpret_cast<std::intptr_t>(attr.v1))) != kSI2DR_ATTR) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drComplexAttrAddStringValue:"), &err2);
            }
            return;
        }
        if (x->type != kTIMINGLIB__COMPLEX) {
            *err = kSI2DR_INVALID_ATTRTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_ATTRTYPE,
                    si2drStringT("__si2drComplexAttrAddStringValue:"), &err2);
            }
            return;
        }
    }
    y = static_cast<timinglib_attribute_value *>(
        calloc(sizeof(timinglib_attribute_value), 1));
    y->type = kTIMINGLIB__VAL_STRING;
    y->u.string_val =
        strtab_->timinglibStrtableEnterString(master_string_table, string);
    /* link the attr struct into the lists */
    if (x->last_value) {
        x->last_value->next = y;
        x->last_value = y;
    } else {
        x->last_value = y;
        x->value = y;
    }
    *err = kSI2DR_NO_ERROR;
}
si2drVoidT LibAnalysis::__si2drComplexAttrAddBooleanValue(si2drAttrIdT attr,
                                                          si2drBooleanT boolval,
                                                          si2drErrorT *err) {
    timinglib_attribute *x = static_cast<timinglib_attribute *>(attr.v2);
    timinglib_attribute_value *y;
    if (trace_) {
        fprintf(tracefile1_,
                "\n\t__si2drComplexAttrAddBooleanValue(%s,%d,&err);\n\n",
                __oidString(attr).c_str(), boolval);
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(
                reinterpret_cast<std::intptr_t>(attr.v1))) != kSI2DR_ATTR) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drComplexAttrAddBooleanValue:"), &err2);
            }
            return;
        }
        if (x->type != kTIMINGLIB__COMPLEX) {
            *err = kSI2DR_INVALID_ATTRTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_ATTRTYPE,
                    si2drStringT("__si2drComplexAttrAddBooleanValue:"), &err2);
            }
            return;
        }
    }
    y = static_cast<timinglib_attribute_value *>(
        calloc(sizeof(timinglib_attribute_value), 1));
    y->type = kTIMINGLIB__VAL_BOOLEAN;
    y->u.int_val = boolval;
    /* link the attr struct into the lists */
    if (x->last_value) {
        x->last_value->next = y;
        x->last_value = y;
    } else {
        x->last_value = y;
        x->value = y;
    }
    *err = kSI2DR_NO_ERROR;
}
si2drVoidT LibAnalysis::__si2drComplexAttrAddFloat64Value(si2drAttrIdT attr,
                                                          si2drFloat64T float64,
                                                          si2drErrorT *err) {
    timinglib_attribute *x = static_cast<timinglib_attribute *>(attr.v2);
    timinglib_attribute_value *y;
    if (trace_) {
        fprintf(tracefile1_,
                "\n\t__si2drComplexAttrAddFloat64Value(%s,%g,&err);\n\n",
                __oidString(attr).c_str(), float64);
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(
                reinterpret_cast<std::intptr_t>(attr.v1))) != kSI2DR_ATTR) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drComplexAttrAddFloat64Value:"), &err2);
            }
            return;
        }
        if (x->type != kTIMINGLIB__COMPLEX) {
            *err = kSI2DR_INVALID_ATTRTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_ATTRTYPE,
                    si2drStringT("__si2drComplexAttrAddFloat64Value:"), &err2);
            }
            return;
        }
    }
    y = static_cast<timinglib_attribute_value *>(
        calloc(sizeof(timinglib_attribute_value), 1));
    y->type = kTIMINGLIB__VAL_DOUBLE;
    y->u.double_val = float64;
    /* link the attr struct into the lists */
    if (x->last_value) {
        x->last_value->next = y;
        x->last_value = y;
    } else {
        x->last_value = y;
        x->value = y;
    }
    *err = kSI2DR_NO_ERROR;
}
/* many thanks to Stefan Zager for reporting that AddExprValue was not
available. (it is now!). */
si2drVoidT LibAnalysis::__si2drComplexAttrAddExprValue(si2drAttrIdT attr,
                                                       si2drExprT *expr,
                                                       si2drErrorT *err) {
    timinglib_attribute *x = static_cast<timinglib_attribute *>(attr.v2);
    timinglib_attribute_value *y;
    if (trace_) {
        fprintf(tracefile1_,
                "\n\t__si2drComplexAttrAddExprValue(%s,%llx,&err);\n\n",
                __oidString(attr).c_str(), reinterpret_cast<uint64_t>(expr));
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(
                reinterpret_cast<std::intptr_t>(attr.v1))) != kSI2DR_ATTR) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drComplexAttrAddExprValue:"), &err2);
            }
            return;
        }
        if (x->type != kTIMINGLIB__COMPLEX) {
            *err = kSI2DR_INVALID_ATTRTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_ATTRTYPE,
                    si2drStringT("__si2drComplexAttrAddExprValue:"), &err2);
            }
            return;
        }
    }
    y = static_cast<timinglib_attribute_value *>(
        calloc(sizeof(timinglib_attribute_value), 1));
    y->type = kTIMINGLIB__VAL_EXPR;
    y->u.expr_val = expr;
    /* link the attr struct into the lists */
    if (x->last_value) {
        x->last_value->next = y;
        x->last_value = y;
    } else {
        x->last_value = y;
        x->value = y;
    }
    *err = kSI2DR_NO_ERROR;
}
si2drValuesIdT LibAnalysis::__si2drComplexAttrGetValues(si2drAttrIdT attr,
                                                        si2drErrorT *err) {
    timinglib_attribute *x = static_cast<timinglib_attribute *>(attr.v2);
    iterat *y;
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(
                reinterpret_cast<std::intptr_t>(attr.v1))) != kSI2DR_ATTR) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drComplexAttrGetValues:"), &err2);
            }
            return 0;
        }
        if (x->type != kTIMINGLIB__COMPLEX) {
            *err = kSI2DR_INVALID_ATTRTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_ATTRTYPE,
                    si2drStringT("__si2drComplexAttrGetValues:"), &err2);
            }
            return 0;
        }
    }
    *err = kSI2DR_NO_ERROR;
    l__iter_val_count_++;
    y = static_cast<iterat *>(calloc(sizeof(iterat), 1));
    y->owner = kITER_VAL;
    y->next = x->value;
    if (trace_) {
        __outinitIter((si2drValuesIdT)y);
        fprintf(
            tracefile1_, "\n\t%s = __si2drComplexAttrGetValues(%s, &err);\n\n",
            __iterString((si2drIterIdT)y).c_str(), __oidString(attr).c_str());
        __incTracecount();
    }
    *err = kSI2DR_NO_ERROR;
    return static_cast<si2drValuesIdT>(y);
}
si2drVoidT LibAnalysis::__si2drIterNextComplexValue(
    si2drValuesIdT iter, si2drValueTypeT *type, si2drInt32T *intgr,
    si2drFloat64T *float64, si2drStringT *string, si2drBooleanT *boolval,
    si2drExprT **expr, si2drErrorT *err) {
    iterat *y = static_cast<iterat *>(iter);
    timinglib_attribute_value *z;
    if (trace_) {
        fprintf(tracefile1_,
                "\n\t__si2drIterNextComplexValue(%s,&type,&intgr,&float64,&"
                "string,&boolval,&expr,&err);\n\n",
                __iterString(iter).c_str());
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if (y->owner != kITER_VAL) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drIterNextComplexValue:"), &err2);
            }
            *intgr = 0;
            *expr = 0;
            *float64 = 0.0;
            *string = 0;
            *boolval = static_cast<si2drBooleanT>(0);
            return;
        }
    }
    if (y->next == static_cast<timinglib_attribute_value *>(NULL)) {
        *type = kSI2DR_UNDEFINED_VALUETYPE;
        *intgr = 0;
        *expr = 0;
        *float64 = 0.0;
        *string = 0;
        *boolval = static_cast<si2drBooleanT>(0);
        *err = kSI2DR_NO_ERROR;
        return;
    }
    z = static_cast<timinglib_attribute_value *>(y->next);
    switch (z->type) {
        case kTIMINGLIB__VAL_STRING:
            *type = kSI2DR_STRING;
            *string = z->u.string_val;
            *intgr = 0;
            *float64 = 0.0;
            *boolval = static_cast<si2drBooleanT>(0);
            break;
        case kTIMINGLIB__VAL_EXPR:
            *type = kSI2DR_EXPR;
            *expr = static_cast<si2drExprT *>(z->u.expr_val);
            *string = 0;
            *intgr = 0;
            *float64 = 0.0;
            *boolval = static_cast<si2drBooleanT>(0);
            break;
        case kTIMINGLIB__VAL_DOUBLE:
            *type = kSI2DR_FLOAT64;
            *expr = 0;
            *string = 0;
            *intgr = 0;
            *float64 = z->u.double_val;
            *boolval = static_cast<si2drBooleanT>(0);
            break;
        case kTIMINGLIB__VAL_BOOLEAN:
            *type = kSI2DR_BOOLEAN;
            *string = 0;
            *expr = 0;
            *intgr = 0;
            *float64 = 0.0;
            *boolval = static_cast<si2drBooleanT>(z->u.int_val);
            break;
        case kTIMINGLIB__VAL_INT:
            *type = kSI2DR_INT32;
            *string = 0;
            *expr = 0;
            *intgr = z->u.int_val;
            *float64 = 0.0;
            *boolval = static_cast<si2drBooleanT>(0);
            break;
        case kTIMINGLIB__VAL_UNDEFINED:
            *type = kSI2DR_UNDEFINED_VALUETYPE;
            *intgr = 0;
            *float64 = 0.0;
            *string = 0;
            *expr = 0;
            *boolval = static_cast<si2drBooleanT>(0);
            *err = kSI2DR_NO_ERROR;
            break;
    }
    y->next = z->next;
    *err = kSI2DR_NO_ERROR;
}
si2drValueTypeT LibAnalysis::__si2drComplexValGetValueType(
    si2drAttrComplexValIdT attr, si2drErrorT *err) {
    timinglib_attribute_value *z =
        static_cast<timinglib_attribute_value *>(attr.v2);
    *err = kSI2DR_NO_ERROR;
    if (!z) {
        return kSI2DR_UNDEFINED_VALUETYPE;
    }
    switch (z->type) {
        case kTIMINGLIB__VAL_STRING:
            return kSI2DR_STRING;
        case kTIMINGLIB__VAL_EXPR:
            return kSI2DR_EXPR;
        case kTIMINGLIB__VAL_DOUBLE:
            return kSI2DR_FLOAT64;
        case kTIMINGLIB__VAL_BOOLEAN:
            return kSI2DR_BOOLEAN;
        case kTIMINGLIB__VAL_INT:
            return kSI2DR_INT32;
        case kTIMINGLIB__VAL_UNDEFINED:
        default:
            return kSI2DR_UNDEFINED_VALUETYPE;
    }
}
si2drInt32T LibAnalysis::__si2drComplexValGetInt32Value(
    si2drAttrComplexValIdT attr, si2drErrorT *err) {
    timinglib_attribute_value *z =
        static_cast<timinglib_attribute_value *>(attr.v2);
    if (z->type != kTIMINGLIB__VAL_INT) {
        *err = kSI2DR_INVALID_OBJECTTYPE;
        if (timinglib_debug_mode_) {
            si2drErrorT err2;
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                                si2drStringT("__si2drComplexValGetInt32Value:"),
                                &err2);
        }
        return 0;
    }
    *err = kSI2DR_NO_ERROR;
    return z->u.int_val;
}
si2drFloat64T LibAnalysis::__si2drComplexValGetFloat64Value(
    si2drAttrComplexValIdT attr, si2drErrorT *err) {
    timinglib_attribute_value *z =
        static_cast<timinglib_attribute_value *>(attr.v2);
    if (z->type != kTIMINGLIB__VAL_DOUBLE) {
        *err = kSI2DR_INVALID_OBJECTTYPE;
        if (timinglib_debug_mode_) {
            si2drErrorT err2;
            (this->*si2ErrMsg_)(
                kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                si2drStringT("__si2drComplexValGetFloat64Value:"), &err2);
        }
        return 0;
    }
    *err = kSI2DR_NO_ERROR;
    return z->u.double_val;
}
si2drStringT LibAnalysis::__si2drComplexValGetStringValue(
    si2drAttrComplexValIdT attr, si2drErrorT *err) {
    timinglib_attribute_value *z =
        static_cast<timinglib_attribute_value *>(attr.v2);
    if (z->type != kTIMINGLIB__VAL_STRING) {
        *err = kSI2DR_INVALID_OBJECTTYPE;
        if (timinglib_debug_mode_) {
            si2drErrorT err2;
            (this->*si2ErrMsg_)(
                kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                si2drStringT("__si2drComplexValGetStringValue:"), &err2);
        }
        return 0;
    }
    *err = kSI2DR_NO_ERROR;
    return z->u.string_val;
}
si2drBooleanT LibAnalysis::__si2drComplexValGetBooleanValue(
    si2drAttrComplexValIdT attr, si2drErrorT *err) {
    timinglib_attribute_value *z =
        static_cast<timinglib_attribute_value *>(attr.v2);
    if (z->type != kTIMINGLIB__VAL_BOOLEAN) {
        *err = kSI2DR_INVALID_OBJECTTYPE;
        if (timinglib_debug_mode_) {
            si2drErrorT err2;
            (this->*si2ErrMsg_)(
                kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                si2drStringT("__si2drComplexValGetBooleanValue:"), &err2);
        }
        return static_cast<si2drBooleanT>(0);
    }
    *err = kSI2DR_NO_ERROR;
    return static_cast<si2drBooleanT>(z->u.int_val);
}
si2drExprT *LibAnalysis::__si2drComplexValGetExprValue(
    si2drAttrComplexValIdT attr, si2drErrorT *err) {
    timinglib_attribute_value *z =
        static_cast<timinglib_attribute_value *>(attr.v2);
    if (z->type != kTIMINGLIB__VAL_EXPR) {
        *err = kSI2DR_INVALID_OBJECTTYPE;
        if (timinglib_debug_mode_) {
            si2drErrorT err2;
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                                si2drStringT("__si2drComplexValGetExprValue:"),
                                &err2);
        }
        return 0;
    }
    *err = kSI2DR_NO_ERROR;
    return static_cast<si2drExprT *>(z->u.expr_val);
}
si2drAttrComplexValIdT LibAnalysis::__si2drIterNextComplex(si2drValuesIdT iter,
                                                           si2drErrorT *err) {
    si2drAttrComplexValIdT retoid;
    iterat *y = static_cast<iterat *>(iter);
    timinglib_attribute_value *z;
    if (trace_) {
        fprintf(tracefile1_,
                "\n\t__si2drIterNextComplexValue(%s,&type,&intgr,&float64,&"
                "string,&boolval,&expr,&err);\n\n",
                __iterString(iter).c_str());
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if (y->owner != kITER_VAL) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drIterNextComplexValue:"), &err2);
            }
            retoid.v1 = 0;
            retoid.v2 = 0;
            return retoid;
        }
    }
    if (y->next == static_cast<timinglib_attribute_value *>(NULL)) {
        *err = kSI2DR_NO_ERROR;
        retoid.v1 = 0;
        retoid.v2 = 0;
        return retoid;
    }
    z = static_cast<timinglib_attribute_value *>(y->next);
    y->next = z->next;
    retoid.v1 = reinterpret_cast<void *>(kkSI2DR_COMPLEXVAL);
    retoid.v2 = static_cast<void *>(z);
    *err = kSI2DR_NO_ERROR;
    return retoid;
}
si2drValueTypeT LibAnalysis::__si2drSimpleAttrGetValueType(si2drAttrIdT attr,
                                                           si2drErrorT *err) {
    timinglib_attribute *x = static_cast<timinglib_attribute *>(attr.v2);
    if (trace_) {
        fprintf(tracefile1_, "\n\t__si2drSimpleAttrGetValueType (%s,&err);\n\n",
                __oidString(attr).c_str());
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(attr.v1)) !=
            kSI2DR_ATTR) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drSimpleAttrGetValueType:"), &err2);
            }
            return static_cast<si2drValueTypeT>(0);
        }
        if (x->type != kTIMINGLIB__SIMPLE) {
            *err = kSI2DR_INVALID_ATTRTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_ATTRTYPE,
                    si2drStringT("__si2drSimpleAttrGetValueType:"), &err2);
            }
            return static_cast<si2drValueTypeT>(0);
        }
    }
    *err = kSI2DR_NO_ERROR;
    if (x->value == static_cast<timinglib_attribute_value *>(NULL)) {
        return kSI2DR_UNDEFINED_VALUETYPE;
    }
    switch (x->value->type) {
        case kTIMINGLIB__VAL_STRING:
            return kSI2DR_STRING;
        case kTIMINGLIB__VAL_DOUBLE:
            return kSI2DR_FLOAT64;
        case kTIMINGLIB__VAL_BOOLEAN:
            return kSI2DR_BOOLEAN;
        case kTIMINGLIB__VAL_INT:
            return kSI2DR_INT32;
        case kTIMINGLIB__VAL_EXPR:
            return kSI2DR_EXPR;
        default:
            break;
    }
    return kSI2DR_UNDEFINED_VALUETYPE;
}
si2drInt32T LibAnalysis::__si2drSimpleAttrGetInt32Value(si2drAttrIdT attr,
                                                        si2drErrorT *err) {
    timinglib_attribute *x = static_cast<timinglib_attribute *>(attr.v2);
    if (trace_) {
        fprintf(tracefile1_, "\n\t__si2drSimpleAttrGetInt32Value(%s,&err);\n\n",
                __oidString(attr).c_str());
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(attr.v1)) !=
            kSI2DR_ATTR) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drSimpleAttrGetInt32Value:"), &err2);
            }
            return 0;
        }
        if (x->type != kTIMINGLIB__SIMPLE) {
            *err = kSI2DR_INVALID_ATTRTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_ATTRTYPE,
                    si2drStringT("__si2drSimpleAttrGetInt32Value:"), &err2);
            }
            return 0;
        }
        if (x->value == static_cast<timinglib_attribute_value *>(NULL)) {
            *err = kSI2DR_INVALID_VALUE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_VALUE,
                    si2drStringT("__si2drSimpleAttrGetInt32Value:"), &err2);
            }
            return 0;
        }
        if (x->value->type != kTIMINGLIB__VAL_INT) {
            *err = kSI2DR_INVALID_VALUE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_VALUE,
                    si2drStringT("__si2drSimpleAttrGetInt32Value:"), &err2);
            }
            return 0;
        }
    }
    *err = kSI2DR_NO_ERROR;
    return x->value->u.int_val;
}
si2drFloat64T LibAnalysis::__si2drSimpleAttrGetFloat64Value(si2drAttrIdT attr,
                                                            si2drErrorT *err) {
    timinglib_attribute *x = static_cast<timinglib_attribute *>(attr.v2);
    if (trace_) {
        fprintf(tracefile1_,
                "\n\t__si2drSimpleAttrGetFloat64Value(%s,&err);\n\n",
                __oidString(attr).c_str());
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(attr.v1)) !=
            kSI2DR_ATTR) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drSimpleAttrGetFloat64Value:"), &err2);
            }
            return 0;
        }
        if (x->type != kTIMINGLIB__SIMPLE) {
            *err = kSI2DR_INVALID_ATTRTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_ATTRTYPE,
                    si2drStringT("__si2drSimpleAttrGetFloat64Value:"), &err2);
            }
            return 0;
        }
        if (x->value == static_cast<timinglib_attribute_value *>(NULL)) {
            *err = kSI2DR_INVALID_VALUE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_VALUE,
                    si2drStringT("__si2drSimpleAttrGetFloat64Value:"), &err2);
            }
            return 0;
        }
        if (x->value->type != kTIMINGLIB__VAL_DOUBLE) {
            *err = kSI2DR_INVALID_VALUE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_VALUE,
                    si2drStringT("__si2drSimpleAttrGetFloat64Value:"), &err2);
            }
            return 0;
        }
    }
    *err = kSI2DR_NO_ERROR;
    return static_cast<si2drFloat64T>(x->value->u.double_val);
}
si2drStringT LibAnalysis::__si2drSimpleAttrGetStringValue(si2drAttrIdT attr,
                                                          si2drErrorT *err) {
    timinglib_attribute *x = static_cast<timinglib_attribute *>(attr.v2);
    if (trace_) {
        fprintf(tracefile1_,
                "\n\t__si2drSimpleAttrGetStringValue(%s,&err);\n\n",
                __oidString(attr).c_str());
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(attr.v1)) !=
            kSI2DR_ATTR) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drSimpleAttrGetStringValue:"), &err2);
            }
            return 0;
        }
        if (x->type != kTIMINGLIB__SIMPLE) {
            *err = kSI2DR_INVALID_ATTRTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_ATTRTYPE,
                    si2drStringT("__si2drSimpleAttrGetStringValue:"), &err2);
            }
            return 0;
        }
        if (x->value == static_cast<timinglib_attribute_value *>(NULL)) {
            *err = kSI2DR_INVALID_VALUE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_VALUE,
                    si2drStringT("__si2drSimpleAttrGetStringValue:"), &err2);
            }
            return 0;
        }
        if (x->value->type != kTIMINGLIB__VAL_STRING) {
            *err = kSI2DR_INVALID_VALUE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_VALUE,
                    si2drStringT("__si2drSimpleAttrGetStringValue:"), &err2);
            }
            return 0;
        }
    }
    *err = kSI2DR_NO_ERROR;
    return x->value->u.string_val;
}
si2drBooleanT LibAnalysis::__si2drSimpleAttrGetBooleanValue(si2drAttrIdT attr,
                                                            si2drErrorT *err) {
    timinglib_attribute *x = static_cast<timinglib_attribute *>(attr.v2);
    if (trace_) {
        fprintf(tracefile1_,
                "\n\t__si2drSimpleAttrGetBooleanValue(%s,&err);\n\n",
                __oidString(attr).c_str());
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(attr.v1)) !=
            kSI2DR_ATTR) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drSimpleAttrGetBooleanValue:"), &err2);
            }
            return static_cast<si2drBooleanT>(0);
        }
        if (x->type != kTIMINGLIB__SIMPLE) {
            *err = kSI2DR_INVALID_ATTRTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_ATTRTYPE,
                    si2drStringT("__si2drSimpleAttrGetBooleanValue:"), &err2);
            }
            return static_cast<si2drBooleanT>(0);
        }
        if (x->value == static_cast<timinglib_attribute_value *>(NULL)) {
            *err = kSI2DR_INVALID_VALUE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_VALUE,
                    si2drStringT("__si2drSimpleAttrGetBooleanValue:"), &err2);
            }
            return static_cast<si2drBooleanT>(0);
        }
        if (x->value->type != kTIMINGLIB__VAL_BOOLEAN) {
            *err = kSI2DR_INVALID_VALUE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_VALUE,
                    si2drStringT("__si2drSimpleAttrGetBooleanValue:"), &err2);
            }
            return static_cast<si2drBooleanT>(0);
        }
    }
    *err = kSI2DR_NO_ERROR;
    return static_cast<si2drBooleanT>(x->value->u.bool_val);
}
si2drBooleanT LibAnalysis::__si2drSimpleAttrGetIsVar(si2drAttrIdT attr,
                                                     si2drErrorT *err) {
    timinglib_attribute *x = static_cast<timinglib_attribute *>(attr.v2);
    if (trace_) {
        fprintf(tracefile1_, "\n\t__si2drSimpleAttrGetIsVar(%s,&err);\n\n",
                __oidString(attr).c_str());
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(attr.v1)) !=
            kSI2DR_ATTR) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drSimpleAttrGetIsVar:"), &err2);
            }
            return static_cast<si2drBooleanT>(0);
        }
        if (x->type != kTIMINGLIB__SIMPLE) {
            *err = kSI2DR_INVALID_ATTRTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_ATTRTYPE,
                    si2drStringT("__si2drSimpleAttrGetIsVar:"), &err2);
            }
            return static_cast<si2drBooleanT>(0);
        }
    }
    *err = kSI2DR_NO_ERROR;
    return static_cast<si2drBooleanT>(x->is_var);
}
si2drVoidT LibAnalysis::__si2drSimpleAttrSetInt32Value(si2drAttrIdT attr,
                                                       si2drInt32T intgr,
                                                       si2drErrorT *err) {
    timinglib_attribute *x = static_cast<timinglib_attribute *>(attr.v2);
    if (trace_) {
        fprintf(tracefile1_,
                "\n\t__si2drSimpleAttrSetInt32Value(%s,%d,&err);\n\n",
                __oidString(attr).c_str(), static_cast<int>(intgr));
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(attr.v1)) !=
            kSI2DR_ATTR) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drSimpleAttrSetInt32Value:"), &err2);
            }
            return;
        }
        if (x->type != kTIMINGLIB__SIMPLE) {
            *err = kSI2DR_INVALID_ATTRTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_ATTRTYPE,
                    si2drStringT("__si2drSimpleAttrSetInt32Value:"), &err2);
            }
            return;
        }
    }
    if (x->value == static_cast<timinglib_attribute_value *>(NULL)) {
        x->value = static_cast<timinglib_attribute_value *>(
            calloc(sizeof(timinglib_attribute_value), 1));
    }
    x->value->type = kTIMINGLIB__VAL_INT;
    x->value->u.int_val = intgr;
    *err = kSI2DR_NO_ERROR;
}
si2drVoidT LibAnalysis::__si2drSimpleAttrSetBooleanValue(si2drAttrIdT attr,
                                                         si2drBooleanT boolval,
                                                         si2drErrorT *err) {
    timinglib_attribute *x = static_cast<timinglib_attribute *>(attr.v2);
    if (trace_) {
        fprintf(tracefile1_,
                "\n\t__si2drSimpleAttrSetBooleanValue(%s,%d,&err);\n\n",
                __oidString(attr).c_str(), boolval);
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(attr.v1)) !=
            kSI2DR_ATTR) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drSimpleAttrSetBooleanValue:"), &err2);
            }
            return;
        }
        if (x->type != kTIMINGLIB__SIMPLE) {
            *err = kSI2DR_INVALID_ATTRTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_ATTRTYPE,
                    si2drStringT("__si2drSimpleAttrSetBooleanValue:"), &err2);
            }
            return;
        }
    }
    if (x->value == static_cast<timinglib_attribute_value *>(NULL)) {
        x->value = static_cast<timinglib_attribute_value *>(
            calloc(sizeof(timinglib_attribute_value), 1));
    }
    x->value->type = kTIMINGLIB__VAL_BOOLEAN;
    x->value->u.bool_val = boolval;
    *err = kSI2DR_NO_ERROR;
}
si2drVoidT LibAnalysis::si2drSimpleAttrSetIsVar(si2drAttrIdT attr,
                                                si2drErrorT *err) {
    timinglib_attribute *x = static_cast<timinglib_attribute *>(attr.v2);
    if (trace_) {
        fprintf(tracefile1_, "\n\tsi2drSimpleAttrSetIsVar(%s,&err);\n\n",
                __oidString(attr).c_str());
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(attr.v1)) !=
            kSI2DR_ATTR) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("si2drSimpleAttrSetIsVar:"), &err2);
            }
            return;
        }
        if (x->type != kTIMINGLIB__SIMPLE) {
            *err = kSI2DR_INVALID_ATTRTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_ATTRTYPE,
                    si2drStringT("si2drSimpleAttrSetIsVar:"), &err2);
            }
            return;
        }
    }
    x->is_var = 1;
    *err = kSI2DR_NO_ERROR;
}
si2drVoidT LibAnalysis::__si2drSimpleAttrSetFloat64Value(si2drAttrIdT attr,
                                                         si2drFloat64T float64,
                                                         si2drErrorT *err) {
    timinglib_attribute *x = static_cast<timinglib_attribute *>(attr.v2);
    if (trace_) {
        fprintf(tracefile1_,
                "\n\t__si2drSimpleAttrSetFloat64Value(%s,%g,&err);\n\n",
                __oidString(attr).c_str(), float64);
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(attr.v1)) !=
            kSI2DR_ATTR) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drSimpleAttrSetFloat64Value:"), &err2);
            }
            return;
        }
        if (x->type != kTIMINGLIB__SIMPLE) {
            *err = kSI2DR_INVALID_ATTRTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_ATTRTYPE,
                    si2drStringT("__si2drSimpleAttrSetFloat64Value:"), &err2);
            }
            return;
        }
    }
    if (x->value == static_cast<timinglib_attribute_value *>(NULL)) {
        x->value = static_cast<timinglib_attribute_value *>(
            calloc(sizeof(timinglib_attribute_value), 1));
    }
    x->value->type = kTIMINGLIB__VAL_DOUBLE;
    x->value->u.double_val = float64;
    *err = kSI2DR_NO_ERROR;
}
si2drVoidT LibAnalysis::__si2drSimpleAttrSetStringValue(si2drAttrIdT attr,
                                                        si2drStringT string,
                                                        si2drErrorT *err) {
    if (strtab_ == nullptr || sd_ == nullptr) return;
    timinglib_strtable *master_string_table = sd_->master_string_table;
    timinglib_attribute *x = static_cast<timinglib_attribute *>(attr.v2);
    if (trace_) {
        fprintf(tracefile1_,
                "\n\t__si2drSimpleAttrSetStringValue(%s,\"%s\",&err);\n\n",
                __oidString(attr).c_str(), string);
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(attr.v1)) !=
            kSI2DR_ATTR) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drSimpleAttrSetStringValue:"), &err2);
            }
            return;
        }
        if (x->type != kTIMINGLIB__SIMPLE) {
            *err = kSI2DR_INVALID_ATTRTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_ATTRTYPE,
                    si2drStringT("__si2drSimpleAttrSetStringValue:"), &err2);
            }
            return;
        }
    }
    if (x->value == static_cast<timinglib_attribute_value *>(NULL)) {
        x->value = static_cast<timinglib_attribute_value *>(
            calloc(sizeof(timinglib_attribute_value), 1));
    }
    x->value->type = kTIMINGLIB__VAL_STRING;
    x->value->u.string_val =
        strtab_->timinglibStrtableEnterString(master_string_table, string);
    *err = kSI2DR_NO_ERROR;
}
si2drDefineIdT LibAnalysis::si2drGroupCreateDefine(
    si2drGroupIdT group, si2drStringT name, si2drStringT allowed_group_name,
    si2drValueTypeT valtype, si2drErrorT *err) {
    if (strtab_ == nullptr || sd_ == nullptr) return nulloid_;
    timinglib_hash_table *master_define_hash = sd_->master_define_hash;
    timinglib_strtable *master_string_table = sd_->master_string_table;
    si2drDefineIdT retoid, toid;
    timinglib_group *g = static_cast<timinglib_group *>(group.v2);
    timinglib_define *d;
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(group.v1)) !=
            kSI2DR_GROUP) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("si2drGroupCreateDefine:"), &err2);
            }
            return nulloid_;
        }
        if (g->attr_hash == static_cast<timinglib_hash_table *>(NULL)) {
            *err = kSI2DR_UNUSABLE_OID;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_UNUSABLE_OID,
                                    si2drStringT("si2drGroupCreateDefine:"),
                                    &err2);
            }
            return nulloid_;
        }
    }
    if (name && *name) {
        int found;
        LibHash::timinglibHashLookup(g->define_hash, name, &toid);
        if (toid.v1 != reinterpret_cast<void *>(0)) {
            char buf[10000], *p, *q;
            d = static_cast<timinglib_define *>(toid.v2);
            snprintf(buf, sizeof(buf), "%s", d->group_type);
            found = 0;
            q = buf;
            if (!strcmp(d->group_type, allowed_group_name)) {
                found = 1;
            }
            if (!found) {
                while ((p = strchr(q, '|'))) {
                    *p = 0;
                    if (!strcmp(q, allowed_group_name)) {
                        found = 1;
                        break;
                    }
                    q = p + 1;
                }
            }
            if (found) {
                *err = kSI2DR_OBJECT_ALREADY_EXISTS;
                if (timinglib_debug_mode_) {
                    si2drErrorT err2;
                    (this->*si2ErrMsg_)(
                        kSI2DR_SEVERITY_ERR, kSI2DR_OBJECT_ALREADY_EXISTS,
                        si2drStringT("si2drGroupCreateDefine:"), &err2);
                }
                return nulloid_;
            } else {
                size_t size =
                    strlen(d->group_type) + strlen(allowed_group_name) + 2;
                char *x = static_cast<char *>(alloca(size));
                snprintf(x, size, "%s|%s", d->group_type, allowed_group_name);
                d->group_type =
                    /* OLD: x NEW: */ strtab_->timinglibStrtableEnterString(
                        master_string_table, x);
                *err = kSI2DR_NO_ERROR;
                return toid; /* the scratchpad is freed at this point */
            }
        }
    } else {
        *err = kSI2DR_INVALID_NAME;
        if (timinglib_debug_mode_) {
            si2drErrorT err2;
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_NAME,
                                si2drStringT("si2drGroupCreateDefine:"), &err2);
        }
        return nulloid_;
    }
    d = static_cast<timinglib_define *>(calloc(sizeof(timinglib_define), 1));
    retoid.v1 = reinterpret_cast<void *>(kSI2DR_DEFINE);
    retoid.v2 = static_cast<void *>(d);
    if (trace_) {
        __outinitOid(retoid);
        fprintf(tracefile1_,
                "\n\t%s = si2drGroupCreateDefine(%s, \"%s\", \"%s\", %s, "
                "&err);\n\n",
                __oidString(retoid).c_str(), __oidString(group).c_str(), name,
                allowed_group_name, toString(valtype));
        __traceCheck(retoid);
        __incTracecount();
    }
    d->name = strtab_->timinglibStrtableEnterString(master_string_table, name);
    d->owner = g;
    d->group_type = strtab_->timinglibStrtableEnterString(master_string_table,
                                                          allowed_group_name);
    switch (valtype) {
        case kSI2DR_INT32:
            d->valtype = kTIMINGLIB__VAL_INT;
            break;
        case kSI2DR_UNDEFINED_VALUETYPE:
        case kSI2DR_MAX_VALUETYPE:
        default:
            d->valtype = kTIMINGLIB__VAL_UNDEFINED;
            break;
        case kSI2DR_STRING:
            d->valtype = kTIMINGLIB__VAL_STRING;
            break;
        case kSI2DR_FLOAT64:
            d->valtype = kTIMINGLIB__VAL_DOUBLE;
            break;
        case kSI2DR_BOOLEAN:
            d->valtype = kTIMINGLIB__VAL_BOOLEAN;
            break;
    }
    LibHash::timinglibHashEnterOid(
        g->define_hash, d->name,
        retoid); /* fix via chrisj's posting: d->name instead of using name */
    LibHash::timinglibHashEnterOid(
        master_define_hash, d->name,
        retoid); /* nanda, btw, also spotted and fixed this */
    /* link the attr struct into the lists */
    if (g->define_last) {
        g->define_last->next = d;
        g->define_last = d;
    } else {
        g->define_last = d;
        g->define_list = d;
    }
    *err = kSI2DR_NO_ERROR;
    return retoid;
}
si2drGroupIdT LibAnalysis::__si2drGroupCreateGroup(si2drGroupIdT group,
                                                   si2drStringT name,
                                                   si2drStringT group_type,
                                                   si2drErrorT *err) {
    if (strtab_ == nullptr || sd_ == nullptr) return nulloid_;
    timinglib_strtable *master_string_table = sd_->master_string_table;
    si2drGroupIdT retoid, toid;
    timinglib_group *g = static_cast<timinglib_group *>(group.v2);
    timinglib_group *ng;
    const libGroupMap *lgm;
    char nameb[1500], *nnb = 0;
    // int nbl=0;
    int dont_addhash = 0;
    int npl = strlen(group_type) + 4;
    if (name) {
        snprintf(nameb, sizeof(nameb), "%s||||%s", group_type, name);
        // nbl = strlen(nameb);
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(group.v1)) !=
            kSI2DR_GROUP) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drGroupCreateGroup:"), &err2);
            }
            return nulloid_;
        }
        if (g->attr_hash == static_cast<timinglib_hash_table *>(NULL)) {
            *err = kSI2DR_UNUSABLE_OID;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_UNUSABLE_OID,
                                    si2drStringT("__si2drGroupCreateGroup:"),
                                    &err2);
            }
            return nulloid_;
        }
        if (group_type == 0 || *group_type == 0) {
            *err = kSI2DR_INVALID_VALUE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_VALUE,
                                    si2drStringT("__si2drGroupCreateGroup:"),
                                    &err2);
            }
            return nulloid_;
        }
    }
    lgm = lookup_group_name(group_type, strlen(group_type));
    if (name && *name) {
        LibHash::timinglibHashLookup(g->define_hash, nameb, &toid);
        if (toid.v1 != reinterpret_cast<void *>(0)) {
            /* disqualify any references here */
            switch (lgm->type) {
                case kTIMINGLIB_GROUPENUM_ff_bank:
                    if (g->names && g->names->next) dont_addhash = 1;
                    break;
                case kTIMINGLIB_GROUPENUM_internal_power:
                case kTIMINGLIB_GROUPENUM_cell_degradation:
                case kTIMINGLIB_GROUPENUM_cell_fall:
                case kTIMINGLIB_GROUPENUM_cell_rise:
                case kTIMINGLIB_GROUPENUM_fall_constraint:
                case kTIMINGLIB_GROUPENUM_fall_propagation:
                case kTIMINGLIB_GROUPENUM_fall_transition:
                case kTIMINGLIB_GROUPENUM_retaining_fall:
                case kTIMINGLIB_GROUPENUM_retaining_rise:
                case kTIMINGLIB_GROUPENUM_rise_constraint:
                case kTIMINGLIB_GROUPENUM_rise_propagation:
                case kTIMINGLIB_GROUPENUM_rise_transition:
                case kTIMINGLIB_GROUPENUM_em_max_toggle_rate:
                case kTIMINGLIB_GROUPENUM_fall_power:
                case kTIMINGLIB_GROUPENUM_power:
                case kTIMINGLIB_GROUPENUM_rise_power:
                case kTIMINGLIB_GROUPENUM_fall_net_delay:
                case kTIMINGLIB_GROUPENUM_fall_transition_degradation:
                case kTIMINGLIB_GROUPENUM_rise_net_delay:
                case kTIMINGLIB_GROUPENUM_rise_transition_degradation:
                case kTIMINGLIB_GROUPENUM_interconnect_delay:
                case kTIMINGLIB_GROUPENUM_scaled_cell:
                    dont_addhash = 1;
                    break;
                default:
                    break;
            }
            if (!dont_addhash) {
                *err = kSI2DR_OBJECT_ALREADY_EXISTS;
                if (timinglib_debug_mode_) {
                    si2drErrorT err2;
                    (this->*si2ErrMsg_)(
                        kSI2DR_SEVERITY_ERR, kSI2DR_OBJECT_ALREADY_EXISTS,
                        si2drStringT("__si2drGroupCreateGroup:"), &err2);
                }
                return nulloid_;
            }
        }
    }
    ng = static_cast<timinglib_group *>(calloc(sizeof(timinglib_group), 1));
    if (lgm) ng->EVAL = lgm->type;
    retoid.v1 = reinterpret_cast<void *>(kSI2DR_GROUP);
    retoid.v2 = static_cast<void *>(ng);
    if (trace_) {
        __outinitOid(retoid);
        fprintf(
            tracefile1_,
            "\n\t%s = __si2drGroupCreateGroup(%s, \"%s\", \"%s\"), &err);\n\n",
            __oidString(retoid).c_str(), __oidString(group).c_str(), name,
            group_type);
        __traceCheck(retoid);
        __incTracecount();
    }
    ng->names = static_cast<timinglib_name_list *>(
        calloc(sizeof(timinglib_name_list), 1));
    ng->last_name = ng->names;
    ng->owner = g;
    ng->names->prefix_len = npl;
    if (name && *name) {
        /* OLD:
               nnb = (char*)malloc(nbl+1); */
        /* scope the names by the group type */
        /*
              strcpy(nnb,nameb);
              ng->names->name = nnb;
              NEW: */
        nnb = ng->names->name =
            strtab_->timinglibStrtableEnterString(master_string_table, nameb);
    } else {
        ng->names->name = 0;
    }
    ng->type =
        strtab_->timinglibStrtableEnterString(master_string_table, group_type);
    lgm = lookup_group_name(group_type, strlen(group_type));
    if (lgm) ng->EVAL = lgm->type;
    /* set up hash tables */
    ng->attr_hash = LibHash::timinglibHashCreateHashTable(3, 1, 0);
    ng->define_hash = LibHash::timinglibHashCreateHashTable(3, 1, 0);
    ng->group_hash = LibHash::timinglibHashCreateHashTable(3, 1, 0);
    if (name && *name && !dont_addhash)
        LibHash::timinglibHashEnterOid(g->group_hash, nnb, retoid);
    /* link the attr struct into the lists */
    if (g->group_last) {
        g->group_last->next = ng;
        g->group_last = ng;
    } else {
        g->group_last = ng;
        g->group_list = ng;
    }
    *err = kSI2DR_NO_ERROR;
    return retoid;
}
si2drVoidT LibAnalysis::__si2drGroupAddName(si2drGroupIdT group,
                                            si2drStringT name,
                                            si2drErrorT *err) {
    if (strtab_ == nullptr || sd_ == nullptr) return;
    timinglib_hash_table *master_group_hash = sd_->master_group_hash;
    timinglib_strtable *master_string_table = sd_->master_string_table;
    si2drGroupIdT toid;
    int dont_addhash;
    timinglib_group *g = static_cast<timinglib_group *>(group.v2);
    timinglib_group *gp = g->owner;
    timinglib_name_list *nlp;
    char nameb[1500], *nnb;
    // int nbl;
    int npl = strlen(g->type) + 4;
    snprintf(nameb, sizeof(nameb), "%s||||%s", g->type, name);
    // nbl = strlen(nameb);
    *err = kSI2DR_NO_ERROR;
    if (trace_) {
        fprintf(tracefile1_, "\n\t__si2drGroupAddName(%s,\"%s\",&err);\n\n",
                __oidString(group).c_str(), name);
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(group.v1)) !=
            kSI2DR_GROUP) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drGroupAddName:"), &err2);
            }
            return;
        }
        if (g->attr_hash == static_cast<timinglib_hash_table *>(NULL)) {
            *err = kSI2DR_UNUSABLE_OID;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_UNUSABLE_OID,
                                    si2drStringT("__si2drGroupAddName:"),
                                    &err2);
            }
            return;
        }
    }
    dont_addhash = 0;
    if (name && *name) {
        if (gp)
            LibHash::timinglibHashLookup(gp->group_hash, nameb, &toid);
        else
            LibHash::timinglibHashLookup(master_group_hash, nameb, &toid);
        if (toid.v1 != reinterpret_cast<void *>(0)) {
            /* disqualify any references here */
            switch ((group_enum)g->EVAL) {
                case kTIMINGLIB_GROUPENUM_ff_bank:
                    if (g->names && g->names->next) dont_addhash = 1;
                    break;
                case kTIMINGLIB_GROUPENUM_internal_power:
                case kTIMINGLIB_GROUPENUM_cell_degradation:
                case kTIMINGLIB_GROUPENUM_cell_fall:
                case kTIMINGLIB_GROUPENUM_cell_rise:
                case kTIMINGLIB_GROUPENUM_fall_constraint:
                case kTIMINGLIB_GROUPENUM_fall_propagation:
                case kTIMINGLIB_GROUPENUM_fall_transition:
                case kTIMINGLIB_GROUPENUM_retaining_fall:
                case kTIMINGLIB_GROUPENUM_retaining_rise:
                case kTIMINGLIB_GROUPENUM_rise_constraint:
                case kTIMINGLIB_GROUPENUM_rise_propagation:
                case kTIMINGLIB_GROUPENUM_rise_transition:
                case kTIMINGLIB_GROUPENUM_em_max_toggle_rate:
                case kTIMINGLIB_GROUPENUM_fall_power:
                case kTIMINGLIB_GROUPENUM_power:
                case kTIMINGLIB_GROUPENUM_rise_power:
                case kTIMINGLIB_GROUPENUM_fall_net_delay:
                case kTIMINGLIB_GROUPENUM_fall_transition_degradation:
                case kTIMINGLIB_GROUPENUM_rise_net_delay:
                case kTIMINGLIB_GROUPENUM_rise_transition_degradation:
                case kTIMINGLIB_GROUPENUM_interconnect_delay:
                case kTIMINGLIB_GROUPENUM_scaled_cell:
                    dont_addhash = 1;
                    break;
                default:
                    break;
            }
            if (!dont_addhash) {
                *err = kSI2DR_OBJECT_ALREADY_EXISTS;
                if (timinglib_debug_mode_) {
                    si2drErrorT err2;
                    (this->*si2ErrMsg_)(
                        kSI2DR_SEVERITY_ERR, kSI2DR_OBJECT_ALREADY_EXISTS,
                        si2drStringT("__si2drGroupAddName:"), &err2);
                }
            }
        }
    } else {
        *err = kSI2DR_INVALID_NAME;
        if (timinglib_debug_mode_) {
            si2drErrorT err2;
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_NAME,
                                si2drStringT("__si2drGroupAddName:"), &err2);
        }
        return;
    }
    /* OLD:
    nnb = (char*)malloc(nbl+1);
    strcpy(nnb,nameb);
       NEW:*/
    nnb = strtab_->timinglibStrtableEnterString(master_string_table, nameb);
    if (*err != kSI2DR_OBJECT_ALREADY_EXISTS && !dont_addhash) {
        if (gp)
            LibHash::timinglibHashEnterOid(gp->group_hash, nnb, group);
        else
            LibHash::timinglibHashEnterOid(master_group_hash, nnb, group);
    }

    /* in some special case, the
       only name is not deleted? */
    if (g->names && g->names->name == reinterpret_cast<char *>(NULL)) {
        g->names->name = nnb;
        g->names->prefix_len = npl;
        return;
    }
    nlp = static_cast<timinglib_name_list *>(
        calloc(sizeof(timinglib_name_list), 1));
    nlp->name = nnb;
    nlp->prefix_len = npl;
    if (g->last_name) {
        g->last_name->next = nlp;
        g->last_name = nlp;
    } else {
        g->names = nlp;
        g->last_name = nlp;
    }
}
si2drVoidT LibAnalysis::__si2drGroupDeleteName(si2drGroupIdT group,
                                               si2drStringT name,
                                               si2drErrorT *err) {
    if (sd_ == nullptr) return;
    timinglib_hash_table *master_group_hash = sd_->master_group_hash;
    si2drGroupIdT toid;
    timinglib_group *g = static_cast<timinglib_group *>(group.v2);
    timinglib_group *gp = g->owner;
    timinglib_name_list *nlp, *nlp_last;
    char nameb[1500];
    // int nbl=0;
    snprintf(nameb, sizeof(nameb), "%s||||%s", g->type, name);
    // nbl = strlen(nameb);
    if (trace_) {
        fprintf(tracefile1_, "\n\t__si2drGroupDeleteName(%s,\"%s\",&err);\n\n",
                __oidString(group).c_str(), name);
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(group.v1)) !=
            kSI2DR_GROUP) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drGroupDeleteName:"), &err2);
            }
            return;
        }
        if (g->attr_hash == static_cast<timinglib_hash_table *>(NULL)) {
            *err = kSI2DR_UNUSABLE_OID;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_UNUSABLE_OID,
                                    si2drStringT("__si2drGroupDeleteName:"),
                                    &err2);
            }
            return;
        }
        if (name && *name) {
            if (gp)
                LibHash::timinglibHashLookup(gp->group_hash, nameb, &toid);
            else
                LibHash::timinglibHashLookup(master_group_hash, nameb, &toid);
            if (toid.v1 == reinterpret_cast<void *>(0) || toid.v2 != g) {
                *err = kSI2DR_OBJECT_NOT_FOUND;
                if (timinglib_debug_mode_) {
                    si2drErrorT err2;
                    (this->*si2ErrMsg_)(
                        kSI2DR_SEVERITY_ERR, kSI2DR_OBJECT_NOT_FOUND,
                        si2drStringT("__si2drGroupDeleteName:"), &err2);
                }
                return;
            }
        } else {
            *err = kSI2DR_INVALID_NAME;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_NAME,
                                    si2drStringT("__si2drGroupDeleteName:"),
                                    &err2);
            }
            return;
        }
    }
    /* linear search thru g's namelist for this name and kill it */
    nlp_last = 0;
    for (nlp = g->names; nlp; nlp = nlp->next) {
        if (!strcmp(nlp->name, nameb)) {
            if (!nlp_last) {
                g->names = nlp->next;
            } else {
                nlp_last->next = nlp->next;
            }
            if (nlp == g->last_name) {
                g->last_name = nlp_last;
            }
            if (gp)
                LibHash::timinglibHashDeleteElem(gp->group_hash, nameb);
            else
                LibHash::timinglibHashDeleteElem(master_group_hash, nameb);
            nlp->name = 0;
            nlp->next = 0;
            free(nlp);
            break;
        }
        nlp_last = nlp;
    }
    *err = kSI2DR_NO_ERROR;
}
si2drGroupIdT LibAnalysis::__si2drPIFindGroupByName(si2drStringT name,
                                                    si2drStringT type,
                                                    si2drErrorT *err) {
    if (sd_ == nullptr) return nulloid_;
    timinglib_hash_table *master_group_hash = sd_->master_group_hash;
    si2drGroupIdT retoid;
    char nameb[1500];
    // int nbl=0;
    *err = kSI2DR_NO_ERROR;
    snprintf(nameb, sizeof(nameb), "%s||||%s", type, name);
    // nbl = strlen(nameb);
    if (!timinglib_nocheck_mode_) {
        if (master_group_hash == 0) {
            *err = kSI2DR_INTERNAL_SYSTEM_ERROR;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INTERNAL_SYSTEM_ERROR,
                    si2drStringT("__si2drPIFindGroupByName:"), &err2);
            }
            return nulloid_;
        }
    }
    if (name && *name) {
        LibHash::timinglibHashLookup(master_group_hash, nameb, &retoid);
        if (retoid.v1 == reinterpret_cast<void *>(0)) {
            *err = kSI2DR_OBJECT_NOT_FOUND;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_NOTE, kSI2DR_OBJECT_NOT_FOUND,
                    si2drStringT("__si2drPIFindGroupByName: Object Not Found"),
                    &err2);
            }
            retoid = nulloid_;
        }
    } else {
        *err = kSI2DR_INVALID_NAME;
        if (timinglib_debug_mode_) {
            si2drErrorT err2;
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_NAME,
                                si2drStringT("__si2drPIFindGroupByName:"),
                                &err2);
        }
        retoid = nulloid_;
    }
    if (trace_) {
        if (retoid.v1 == 0 && retoid.v2 == 0) {
            fprintf(tracefile1_,
                    "\n\t__si2drPIFindGroupByName(\"%s\", \"%s\"), &err);\n\n",
                    name, type);
        } else {
            __outinitOid(retoid);
            fprintf(
                tracefile1_,
                "\n\t%s = __si2drPIFindGroupByName(\"%s\", \"%s\"), &err);\n\n",
                __oidString(retoid).c_str(), name, type);
        }
        __traceCheck(retoid);
        __incTracecount();
    }
    return retoid;
}
si2drGroupIdT LibAnalysis::__si2drGroupFindGroupByName(si2drGroupIdT group,
                                                       si2drStringT name,
                                                       si2drStringT type,
                                                       si2drErrorT *err) {
    if (sd_ == nullptr) return nulloid_;
    timinglib_hash_table *master_group_hash = sd_->master_group_hash;
    si2drGroupIdT retoid;
    timinglib_group *g = static_cast<timinglib_group *>(group.v2);
    char nameb[1500];
    snprintf(nameb, sizeof(nameb), "%s||||%s", type, name);
    *err = kSI2DR_NO_ERROR;
    if (!timinglib_nocheck_mode_) {
        if (master_group_hash == 0) {
            *err = kSI2DR_INTERNAL_SYSTEM_ERROR;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INTERNAL_SYSTEM_ERROR,
                    si2drStringT("__si2drGroupFindGroupByName:"), &err2);
            }
            return nulloid_;
        }
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(group.v1)) !=
            kSI2DR_GROUP) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drGroupFindGroupByName:"), &err2);
            }
            return nulloid_;
        }
        if (g->attr_hash == static_cast<timinglib_hash_table *>(NULL)) {
            *err = kSI2DR_UNUSABLE_OID;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_UNUSABLE_OID,
                    si2drStringT("__si2drGroupFindGroupByName:"), &err2);
            }
            return nulloid_;
        }
    }
    *err = kSI2DR_NO_ERROR;
    if (name && *name) {
        LibHash::timinglibHashLookup(g->group_hash, nameb, &retoid);
        if (retoid.v1 == reinterpret_cast<void *>(0)) {
            *err = kSI2DR_OBJECT_NOT_FOUND;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_NOTE, kSI2DR_OBJECT_NOT_FOUND,
                    si2drStringT(
                        "__si2drGroupFindGroupByName: Object Not Found"),
                    &err2);
            }
            retoid = nulloid_;
        }
    } else {
        *err = kSI2DR_INVALID_NAME;
        if (timinglib_debug_mode_) {
            si2drErrorT err2;
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_NAME,
                                si2drStringT("__si2drGroupFindGroupByName:"),
                                &err2);
        }
        retoid = nulloid_;
    }
    if (trace_) {
        if (retoid.v1 == 0 && retoid.v2 == 0) {
            fprintf(tracefile1_,
                    "\n\t__si2drGroupFindGroupByName(%s, \"%s\", \"%s\"), "
                    "&err);\n\n",
                    __oidString(group).c_str(), name, type);
        } else {
            __outinitOid(retoid);
            fprintf(tracefile1_,
                    "\n\t%s = __si2drGroupFindGroupByName(%s, \"%s\", \"%s\"), "
                    "&err);\n\n",
                    __oidString(retoid).c_str(), __oidString(group).c_str(),
                    name, type);
        }
        __traceCheck(retoid);
        __incTracecount();
    }
    return retoid;
}
si2drAttrIdT LibAnalysis::__si2drGroupFindAttrByName(si2drGroupIdT group,
                                                     si2drStringT name,
                                                     si2drErrorT *err) {
    if (sd_ == nullptr) return nulloid_;
    timinglib_hash_table *master_group_hash = sd_->master_group_hash;
    si2drAttrIdT retoid;
    timinglib_group *g = static_cast<timinglib_group *>(group.v2);
    if (!timinglib_nocheck_mode_) {
        if (master_group_hash == 0) {
            *err = kSI2DR_INTERNAL_SYSTEM_ERROR;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INTERNAL_SYSTEM_ERROR,
                    si2drStringT("__si2drGroupFindAttrByName:"), &err2);
            }
            return nulloid_;
        }
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(group.v1)) !=
            kSI2DR_GROUP) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drGroupFindAttrByName:"), &err2);
            }
            return nulloid_;
        }
        if (g->attr_hash == static_cast<timinglib_hash_table *>(NULL)) {
            *err = kSI2DR_UNUSABLE_OID;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_UNUSABLE_OID,
                                    si2drStringT("__si2drGroupFindAttrByName:"),
                                    &err2);
            }
            return nulloid_;
        }
    }
    *err = kSI2DR_NO_ERROR;
    if (name && *name) {
        LibHash::timinglibHashLookup(g->attr_hash, name, &retoid);
        if (retoid.v1 == reinterpret_cast<void *>(0)) {
            *err = kSI2DR_OBJECT_NOT_FOUND;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_OBJECT_NOT_FOUND,
                    si2drStringT("__si2drGroupFindAttrByName:"), &err2);
            }
            retoid = nulloid_;
        }
    } else {
        *err = kSI2DR_INVALID_NAME;
        if (timinglib_debug_mode_) {
            si2drErrorT err2;
            (this->*si2ErrMsg_)(
                kSI2DR_SEVERITY_NOTE, kSI2DR_INVALID_NAME,
                si2drStringT("__si2drGroupFindAttrByName: Attr Not Found"),
                &err2);
        }
        retoid = nulloid_;
    }
    if (trace_) {
        if (retoid.v1 == 0 && retoid.v2 == 0) {
            fprintf(tracefile1_,
                    "\n\t__si2drGroupFindAttrByName(%s, \"%s\"), &err);\n\n",
                    __oidString(group).c_str(), name);
        } else {
            __outinitOid(retoid);
            fprintf(
                tracefile1_,
                "\n\t%s = __si2drGroupFindAttrByName(%s, \"%s\"), &err);\n\n",
                __oidString(retoid).c_str(), __oidString(group).c_str(), name);
        }
        __traceCheck(retoid);
        __incTracecount();
    }
    return retoid;
}
si2drDefineIdT LibAnalysis::__si2drGroupFindDefineByName(si2drGroupIdT group,
                                                         si2drStringT name,
                                                         si2drErrorT *err) {
    if (sd_ == nullptr) return nulloid_;
    timinglib_hash_table *master_group_hash = sd_->master_group_hash;
    si2drDefineIdT retoid;
    timinglib_group *g = static_cast<timinglib_group *>(group.v2);
    if (!timinglib_nocheck_mode_) {
        if (master_group_hash == 0) {
            *err = kSI2DR_INTERNAL_SYSTEM_ERROR;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INTERNAL_SYSTEM_ERROR,
                    si2drStringT("__si2drGroupFindDefineByName:"), &err2);
            }
            return nulloid_;
        }
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(group.v1)) !=
            kSI2DR_GROUP) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drGroupFindDefineByName:"), &err2);
            }
            return nulloid_;
        }
        if (g->attr_hash == static_cast<timinglib_hash_table *>(NULL)) {
            *err = kSI2DR_UNUSABLE_OID;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_UNUSABLE_OID,
                    si2drStringT("__si2drGroupFindDefineByName:"), &err2);
            }
            return nulloid_;
        }
    }
    *err = kSI2DR_NO_ERROR;
    if (name && *name) {
        LibHash::timinglibHashLookup(g->define_hash, name, &retoid);
        if (retoid.v1 == reinterpret_cast<void *>(0)) {
            *err = kSI2DR_OBJECT_NOT_FOUND;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_NOTE, kSI2DR_OBJECT_NOT_FOUND,
                    si2drStringT(
                        "__si2drGroupFindDefineByName: Define Not Found"),
                    &err2);
            }
            retoid = nulloid_;
        }
    } else {
        *err = kSI2DR_INVALID_NAME;
        if (timinglib_debug_mode_) {
            si2drErrorT err2;
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_NAME,
                                si2drStringT("__si2drGroupFindDefineByName:"),
                                &err2);
        }
        retoid = nulloid_;
    }
    if (trace_) {
        if (retoid.v1 == 0 && retoid.v2 == 0) {
            fprintf(tracefile1_,
                    "\n\t__si2drGroupFindDefineByName(%s, \"%s\"), &err);\n\n",
                    __oidString(group).c_str(), name);
        } else {
            __outinitOid(retoid);
            fprintf(
                tracefile1_,
                "\n\t%s = __si2drGroupFindDefineByName(%s, \"%s\"), &err);\n\n",
                __oidString(retoid).c_str(), __oidString(group).c_str(), name);
        }
        __traceCheck(retoid);
        __incTracecount();
    }
    return retoid;
}
si2drDefineIdT LibAnalysis::__si2drPIFindDefineByName(si2drStringT name,
                                                      si2drErrorT *err) {
    if (sd_ == nullptr) return nulloid_;
    timinglib_hash_table *master_group_hash = sd_->master_group_hash;
    timinglib_hash_table *master_define_hash = sd_->master_define_hash;
    si2drDefineIdT retoid;
    if (!timinglib_nocheck_mode_) {
        if (master_group_hash == 0) {
            *err = kSI2DR_INTERNAL_SYSTEM_ERROR;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INTERNAL_SYSTEM_ERROR,
                    si2drStringT("__si2drPIFindDefineByName:"), &err2);
            }
            return nulloid_;
        }
    }
    *err = kSI2DR_NO_ERROR;
    if (name && *name) {
        LibHash::timinglibHashLookup(master_define_hash, name, &retoid);
        if (retoid.v1 == reinterpret_cast<void *>(0)) {
            *err = kSI2DR_OBJECT_NOT_FOUND;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_NOTE, kSI2DR_OBJECT_NOT_FOUND,
                    si2drStringT("__si2drPIFindDefineByName: Define Not Found"),
                    &err2);
            }
            retoid = nulloid_;
        }
    } else {
        *err = kSI2DR_INVALID_NAME;
        if (timinglib_debug_mode_) {
            si2drErrorT err2;
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_NAME,
                                si2drStringT("__si2drPIFindDefineByName:"),
                                &err2);
        }
        retoid = nulloid_;
    }
    if (trace_) {
        if (retoid.v1 == 0 && retoid.v2 == 0) {
            fprintf(tracefile1_,
                    "\n\t__si2drPIFindDefineByName(\"%s\"), &err);\n\n", name);
        } else {
            __outinitOid(retoid);
            fprintf(tracefile1_,
                    "\n\t%s = __si2drPIFindDefineByName(\"%s\"), &err);\n\n",
                    __oidString(retoid).c_str(), name);
        }
        __traceCheck(retoid);
        __incTracecount();
    }
    return retoid;
}
si2drGroupsIdT LibAnalysis::__si2drPIGetGroups(si2drErrorT *err) {
    if (sd_ == nullptr) return si2drGroupsIdT(0);
    timinglib_hash_table *master_group_hash = sd_->master_group_hash;
    timinglib_group *master_group_list = sd_->master_group_list;
    iterat *y;
    if (!timinglib_nocheck_mode_) {
        if (master_group_hash == 0) {
            *err = kSI2DR_INTERNAL_SYSTEM_ERROR;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                    kSI2DR_INTERNAL_SYSTEM_ERROR,
                                    si2drStringT("__si2drPIGetGroups:"), &err2);
            }
            return static_cast<si2drGroupsIdT>(0);
        }
    }
    *err = kSI2DR_NO_ERROR;
    y = static_cast<iterat *>(calloc(sizeof(iterat), 1));
    y->owner = kITER_GROUP;
    y->next = static_cast<void *>(master_group_list);
    l__iter_group_count_++;
    if (trace_) {
        __outinitIter((si2drIterIdT)y);
        fprintf(tracefile1_, "\n\t%s = __si2drPIGetGroups(&err);\n\n",
                __iterString((si2drIterIdT)y).c_str());
        __incTracecount();
    }
    return static_cast<si2drGroupsIdT>(y);
}
si2drGroupsIdT LibAnalysis::__si2drGroupGetGroups(si2drGroupIdT group,
                                                  si2drErrorT *err) {
    if (sd_ == nullptr) return si2drGroupsIdT(0);
    timinglib_hash_table *master_group_hash = sd_->master_group_hash;
    iterat *y;
    timinglib_group *g = static_cast<timinglib_group *>(group.v2);
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(group.v1)) !=
            kSI2DR_GROUP) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drGroupGetGroups:"), &err2);
            }
            return 0;
        }
        if (master_group_hash == 0) {
            *err = kSI2DR_INTERNAL_SYSTEM_ERROR;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INTERNAL_SYSTEM_ERROR,
                    si2drStringT("__si2drGroupGetGroups:"), &err2);
            }
            return static_cast<si2drGroupsIdT>(0);
        }
        if (g->attr_hash == static_cast<timinglib_hash_table *>(NULL)) {
            *err = kSI2DR_UNUSABLE_OID;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_UNUSABLE_OID,
                                    si2drStringT("__si2drGroupGetGroups:"),
                                    &err2);
            }
            return 0;
        }
    }
    *err = kSI2DR_NO_ERROR;
    y = static_cast<iterat *>(calloc(sizeof(iterat), 1));
    y->owner = kITER_GROUP;
    y->next = static_cast<void *>(g->group_list);
    l__iter_group_count_++;
    if (trace_) {
        __outinitIter((si2drIterIdT)y);
        fprintf(tracefile1_, "\n\t%s = __si2drGroupGetGroups(%s, &err);\n\n",
                __iterString((si2drIterIdT)y).c_str(),
                __oidString(group).c_str());
        __incTracecount();
    }
    return static_cast<si2drGroupsIdT>(y);
}
si2drNamesIdT LibAnalysis::__si2drGroupGetNames(si2drGroupIdT group,
                                                si2drErrorT *err) {
    if (sd_ == nullptr) return si2drNamesIdT(0);
    timinglib_hash_table *master_group_hash = sd_->master_group_hash;
    iterat *y;
    timinglib_group *g = static_cast<timinglib_group *>(group.v2);
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(group.v1)) !=
            kSI2DR_GROUP) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drGroupGetNames:"), &err2);
            }
            return 0;
        }
        if (master_group_hash == 0) {
            *err = kSI2DR_INTERNAL_SYSTEM_ERROR;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INTERNAL_SYSTEM_ERROR,
                    si2drStringT("__si2drGroupGetNames:"), &err2);
            }
            return 0;
        }
        if (g->attr_hash == static_cast<timinglib_hash_table *>(NULL)) {
            *err = kSI2DR_UNUSABLE_OID;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_UNUSABLE_OID,
                                    si2drStringT("__si2drGroupGetNames:"),
                                    &err2);
            }
            return 0;
        }
    }
    *err = kSI2DR_NO_ERROR;
    y = static_cast<iterat *>(calloc(sizeof(iterat), 1));
    y->owner = kITER_NAME;
    y->next = static_cast<void *>(g->names);
    l__iter_name_count_++;
    if (trace_) {
        __outinitIter((si2drIterIdT)y);
        fprintf(tracefile1_, "\n\t%s = __si2drGroupGetNames(%s, &err);\n\n",
                __iterString((si2drIterIdT)y).c_str(),
                __oidString(group).c_str());
        __incTracecount();
    }
    return static_cast<si2drNamesIdT>(y);
}
si2drAttrsIdT LibAnalysis::__si2drGroupGetAttrs(si2drGroupIdT group,
                                                si2drErrorT *err) {
    if (sd_ == nullptr) return si2drAttrsIdT(0);
    timinglib_hash_table *master_group_hash = sd_->master_group_hash;
    iterat *y;
    timinglib_group *g = static_cast<timinglib_group *>(group.v2);
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(group.v1)) !=
            kSI2DR_GROUP) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drGroupGetAttrs:"), &err2);
            }
            return 0;
        }
        if (master_group_hash == 0) {
            *err = kSI2DR_INTERNAL_SYSTEM_ERROR;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INTERNAL_SYSTEM_ERROR,
                    si2drStringT("__si2drGroupGetAttrs:"), &err2);
            }
            return 0;
        }
        if (g->attr_hash == static_cast<timinglib_hash_table *>(NULL)) {
            *err = kSI2DR_UNUSABLE_OID;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_UNUSABLE_OID,
                                    si2drStringT("__si2drGroupGetAttrs:"),
                                    &err2);
            }
            return 0;
        }
    }
    *err = kSI2DR_NO_ERROR;
    y = static_cast<iterat *>(calloc(sizeof(iterat), 1));
    y->owner = kITER_ATTR;
    y->next = static_cast<void *>(g->attr_list);
    l__iter_attr_count_++;
    if (trace_) {
        __outinitIter((si2drIterIdT)y);
        fprintf(tracefile1_, "\n\t%s = __si2drGroupGetAttrs(%s, &err);\n\n",
                __iterString((si2drIterIdT)y).c_str(),
                __oidString(group).c_str());
        __incTracecount();
    }
    return static_cast<si2drAttrsIdT>(y);
}
si2drDefinesIdT LibAnalysis::__si2drGroupGetDefines(si2drGroupIdT group,
                                                    si2drErrorT *err) {
    if (sd_ == nullptr) return si2drDefinesIdT(0);
    timinglib_hash_table *master_group_hash = sd_->master_group_hash;
    iterat *y;
    timinglib_group *g = static_cast<timinglib_group *>(group.v2);
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(group.v1)) !=
            kSI2DR_GROUP) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drGroupGetDefines:"), &err2);
            }
            return 0;
        }
        if (master_group_hash == 0) {
            *err = kSI2DR_INTERNAL_SYSTEM_ERROR;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INTERNAL_SYSTEM_ERROR,
                    si2drStringT("__si2drGroupGetDefines:"), &err2);
            }
            return 0;
        }
        if (g->attr_hash == static_cast<timinglib_hash_table *>(NULL)) {
            *err = kSI2DR_UNUSABLE_OID;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_UNUSABLE_OID,
                                    si2drStringT("__si2drGroupGetDefines:"),
                                    &err2);
            }
            return 0;
        }
    }
    *err = kSI2DR_NO_ERROR;
    y = static_cast<iterat *>(calloc(sizeof(iterat), 1));
    y->owner = kITER_DEF;
    l__iter_def_count_++;
    y->next = static_cast<void *>(g->define_list);
    if (trace_) {
        __outinitIter((si2drIterIdT)y);
        fprintf(tracefile1_, "\n\t%s = __si2drGroupGetDefines(%s, &err);\n\n",
                __iterString((si2drIterIdT)y).c_str(),
                __oidString(group).c_str());
        __incTracecount();
    }
    return static_cast<si2drDefinesIdT>(y);
}
si2drGroupIdT LibAnalysis::__si2drIterNextGroup(si2drGroupsIdT iter,
                                                si2drErrorT *err) {
    si2drGroupIdT retoid;
    timinglib_group *z;
    iterat *y = static_cast<iterat *>(iter);
    if (!timinglib_nocheck_mode_) {
        if (y->owner != kITER_GROUP) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drIterNextGroup:"), &err2);
            }
            return nulloid_;
        }
    }
    if (y->next == static_cast<timinglib_group *>(NULL)) {
        *err = kSI2DR_NO_ERROR;
        if (trace_) {
            fprintf(tracefile1_, "\n\t__si2drIterNextGroup(%s, &err);\n\n",
                    __iterString(iter).c_str());
            __incTracecount();
        }
        return nulloid_;
    }
    z = static_cast<timinglib_group *>(y->next);
    y->next = static_cast<void *>(z->next);
    retoid.v1 = reinterpret_cast<void *>(kSI2DR_GROUP);
    retoid.v2 = static_cast<void *>(z);
    if (trace_) {
        __outinitOid(retoid);
        fprintf(tracefile1_, "\n\t%s = __si2drIterNextGroup(%s, &err);\n\n",
                __oidString(retoid).c_str(), __iterString(iter).c_str());
        __traceCheck(retoid);
        __incTracecount();
    }
    *err = kSI2DR_NO_ERROR;
    return retoid;
}
si2drStringT LibAnalysis::__si2drIterNextName(si2drNamesIdT iter,
                                              si2drErrorT *err) {
    timinglib_name_list *z;
    iterat *y = static_cast<iterat *>(iter);
    if (trace_) {
        fprintf(tracefile1_, "\n\t__si2drIterNextName(%s,&err);\n\n",
                __iterString(iter).c_str());
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if (y->owner != kITER_NAME) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drIterNextName:"), &err2);
            }
            return 0;
        }
    }
    if (y->next == static_cast<void *>(NULL)) {
        *err = kSI2DR_NO_ERROR;
        return 0;
    }
    z = static_cast<timinglib_name_list *>(y->next);
    y->next = static_cast<void *>(z->next);
    *err = kSI2DR_NO_ERROR;
    if (z->name)
        return z->name + z->prefix_len;
    else
        return 0;
}
si2drAttrIdT LibAnalysis::__si2drIterNextAttr(si2drAttrsIdT iter,
                                              si2drErrorT *err) {
    si2drAttrIdT retoid;
    timinglib_attribute *z;
    iterat *y = static_cast<iterat *>(iter);
    if (!timinglib_nocheck_mode_) {
        if (y->owner != kITER_ATTR) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drIterNextAttr:"), &err2);
            }
            return nulloid_;
        }
    }
    if (y->next == static_cast<void *>(NULL)) {
        *err = kSI2DR_NO_ERROR;
        if (trace_) {
            fprintf(tracefile1_, "\n\t__si2drIterNextGroup(%s, &err);\n\n",
                    __iterString(iter).c_str());
            __incTracecount();
        }
        return nulloid_;
    }
    z = static_cast<timinglib_attribute *>(y->next);
    y->next = static_cast<void *>(z->next);
    retoid.v1 = reinterpret_cast<void *>(kSI2DR_ATTR);
    retoid.v2 = static_cast<void *>(z);
    if (trace_) {
        __outinitOid(retoid);
        fprintf(tracefile1_, "\n\t%s = __si2drIterNextAttr(%s, &err);\n\n",
                __oidString(retoid).c_str(), __iterString(iter).c_str());
        __traceCheck(retoid);
        __incTracecount();
    }
    *err = kSI2DR_NO_ERROR;
    return retoid;
}
si2drDefineIdT LibAnalysis::__si2drIterNextDefine(si2drDefinesIdT iter,
                                                  si2drErrorT *err) {
    si2drDefineIdT retoid;
    timinglib_define *z;
    iterat *y = static_cast<iterat *>(iter);
    if (!timinglib_nocheck_mode_) {
        if (y->owner != kITER_DEF) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drIterNextDefine:"), &err2);
            }
            return nulloid_;
        }
    }
    if (y->next == static_cast<void *>(NULL)) {
        *err = kSI2DR_NO_ERROR;
        if (trace_) {
            fprintf(tracefile1_, "\n\t__si2drIterNextDefine(%s, &err);\n\n",
                    __iterString(iter).c_str());
            __incTracecount();
        }
        return nulloid_;
    }
    z = static_cast<timinglib_define *>(y->next);
    y->next = static_cast<void *>(z->next);
    retoid.v1 = reinterpret_cast<void *>(kSI2DR_DEFINE);
    retoid.v2 = static_cast<void *>(z);
    if (trace_) {
        __outinitOid(retoid);
        fprintf(tracefile1_, "\n\t%s = __si2drIterNextDefine(%s, &err);\n\n",
                __oidString(retoid).c_str(), __iterString(iter).c_str());
        __traceCheck(retoid);
        __incTracecount();
    }
    *err = kSI2DR_NO_ERROR;
    return retoid;
}
si2drVoidT LibAnalysis::__si2drIterQuit(si2drIterIdT iter, si2drErrorT *err) {
    iterat *y = static_cast<iterat *>(iter);
    if (trace_) {
        fprintf(tracefile1_, "\n\t__si2drIterQuit(%s,&err);\n\n",
                __iterString(iter).c_str());
        __incTracecount();
    }
    switch (y->owner) {
        case kITER_GROUP:
            l__iter_group_count_--;
            break;
        case kITER_ATTR:
            l__iter_attr_count_--;
            break;
        case kITER_DEF:
            l__iter_def_count_--;
            break;
        case kITER_NAME:
            l__iter_name_count_--;
            break;
        case kITER_VAL:
            l__iter_val_count_--;
            break;
        default:
            break;
    }
    y->next = static_cast<void *>(0);
    y->owner = kITER_NOTHING;
    free(y);
    *err = kSI2DR_NO_ERROR;
}
si2drVoidT LibAnalysis::__si2drObjectDelete(si2drObjectIdT object,
                                            si2drErrorT *err) {
    if (sd_ == nullptr) return;
    timinglib_hash_table *master_group_hash = sd_->master_group_hash;
    timinglib_group *master_group_list = sd_->master_group_list;
    timinglib_group *g, *gp, *gg, *lgg;
    timinglib_attribute *a, *aa, *laa;
    timinglib_attribute_value *av, *av2;
    timinglib_define *d, *dd, *ldd;
    timinglib_name_list *nl;
    si2drAttrIdT attr;
    si2drDefineIdT define;
    si2drGroupIdT group;
    if (trace_) {
        fprintf(tracefile1_, "\n\t__si2drObjectDelete(%s,&err);\n\n",
                __oidString(object).c_str());
        __incTracecount();
    }
    *err = kSI2DR_NO_ERROR;
    attr.v1 = reinterpret_cast<void *>(kSI2DR_ATTR);
    define.v1 = reinterpret_cast<void *>(kSI2DR_DEFINE);
    group.v1 = reinterpret_cast<void *>(kSI2DR_GROUP);
    switch ((si2drObjectTypeT)((reinterpret_cast<std::intptr_t>(object.v1)))) {
        case kSI2DR_GROUP:
            g = static_cast<timinglib_group *>(object.v2);
            gp = g->owner;
            /* delete all attrs */
            while (g->attr_list) {
                attr.v2 = static_cast<void *>(g->attr_list);
                __si2drObjectDelete(attr, err);
            }
            g->attr_list = 0;
            /* delete all defines */
            while (g->define_list) {
                define.v2 = static_cast<void *>(g->define_list);
                __si2drObjectDelete(define, err);
            }
            g->define_list = 0;
            /* delete all subgroups */
            while (g->group_list) {
                group.v2 = static_cast<void *>(g->group_list);
                __si2drObjectDelete(group, err);
            }
            g->group_list = 0;
            /* get rid of the hash tabs */
            LibHash::timinglibHashDestroyHashTable(g->attr_hash);
            g->attr_hash = 0;
            LibHash::timinglibHashDestroyHashTable(g->define_hash);
            g->define_hash = 0;
            LibHash::timinglibHashDestroyHashTable(g->group_hash);
            g->group_hash = 0;
            lgg = static_cast<timinglib_group *>(NULL);
            if (gp == static_cast<timinglib_group *>(NULL)) {
                /* unlink me from my parent hash table */
                for (nl = g->names; nl;) {
                    struct timinglib_name_list *nl2 = nl->next;
                    LibHash::timinglibHashDeleteElem(master_group_hash,
                                                     nl->name);
                    nl->next = 0;
                    free(nl);
                    nl = nl2;
                }
                g->names = 0;
                g->last_name = 0;
                /* unlink me from my parent's list */
                for (gg = master_group_list; gg; gg = gg->next) {
                    if (gg == g) {
                        if (lgg == static_cast<timinglib_group *>(NULL)) {
                            /* first guy in the list */
                            master_group_list = gg->next;
                        } else {
                            lgg->next = gg->next;
                        }
                        break;
                    }
                    lgg = gg;
                }
            } else {
                /* unlink me from my parent hash table */
                for (nl = g->names; nl;) {
                    struct timinglib_name_list *nl2 = nl->next;
                    LibHash::timinglibHashDeleteElem(gp->group_hash, nl->name);
                    nl->next = 0;
                    free(nl);
                    nl = nl2;
                }
                g->names = 0;
                g->last_name = 0;
                /* unlink me from my parent's list */
                for (gg = gp->group_list; gg; gg = gg->next) {
                    if (gg == g) {
                        if (lgg == static_cast<timinglib_group *>(NULL)) {
                            /* first guy in the list */
                            gp->group_list = gg->next;
                            if (gp->group_last == g) gp->group_last = 0;
                        } else {
                            lgg->next = gg->next;
                            if (gp->group_last == g) gp->group_last = lgg;
                        }
                        break;
                    }
                    lgg = gg;
                }
            }
            free(g);
            break;
        case kSI2DR_ATTR:
            a = static_cast<timinglib_attribute *>(object.v2);
            gp = a->owner;
            LibHash::timinglibHashDeleteElem(gp->attr_hash, a->name);
            laa = static_cast<timinglib_attribute *>(0);
            for (aa = gp->attr_list; aa; aa = aa->next) {
                if (aa == a) {
                    if (laa == static_cast<timinglib_attribute *>(NULL)) {
                        /* first guy in the list */
                        gp->attr_list = aa->next;
                        if (gp->attr_last == a) gp->attr_last = 0;
                    } else {
                        laa->next = aa->next;
                        if (gp->attr_last == a) gp->attr_last = laa;
                    }
                    break;
                }
                laa = aa;
            }
            /* free up substructure */
            for (av = a->value; av;) {
                av2 = av->next;
                av->next = 0;
                free(av);
                av = av2;
            }
            free(a);
            break;
        case kSI2DR_DEFINE:
            d = static_cast<timinglib_define *>(object.v2);
            gp = d->owner;
            LibHash::timinglibHashDeleteElem(gp->define_hash, d->name);
            ldd = static_cast<timinglib_define *>(0);
            for (dd = gp->define_list; dd; dd = dd->next) {
                if (dd == d) {
                    if (ldd == static_cast<timinglib_define *>(NULL)) {
                        /* first guy in the list */
                        gp->define_list = dd->next;
                        if (gp->define_last == d) gp->define_last = 0;
                    } else {
                        ldd->next = dd->next;
                        if (gp->define_last == d) gp->define_last = ldd;
                    }
                    break;
                }
                ldd = dd;
            }
            d->next = 0;
            free(d);
            break;
        default:
            *err = kSI2DR_INVALID_OBJECTTYPE;
            return;
    }
}
si2drStringT LibAnalysis::__si2drPIGetErrorText(si2drErrorT errorCode,
                                                si2drErrorT *err) {
    *err = kSI2DR_NO_ERROR;
    switch (errorCode) {
        case kSI2DR_NO_ERROR:
            return si2drStringT("");
        case kSI2DR_INTERNAL_SYSTEM_ERROR:
            return si2drStringT("Internal System Error");
        case kSI2DR_SYNTAX_ERROR:
            return si2drStringT("Syntax Error Encountered");
        case kSI2DR_INVALID_VALUE:
            return si2drStringT("Invalid Value");
        case kSI2DR_INVALID_NAME:
            return si2drStringT("Invalid Name");
        case kSI2DR_INVALID_OBJECTTYPE:
            return si2drStringT("Invalid Object Type");
        case kSI2DR_INVALID_ATTRTYPE:
            return si2drStringT("Invalid Attribute Type");
        case kSI2DR_UNUSABLE_OID:
            return si2drStringT("Oid Not Usable. Did you run PIInit?");
        case kSI2DR_OBJECT_ALREADY_EXISTS:
            return si2drStringT("Object Name Already Being Used.");
        case kSI2DR_OBJECT_NOT_FOUND:
            return si2drStringT("Object Not Found");
        case kSI2DR_TRACE_FILES_CANNOT_BE_OPENED:
            return si2drStringT("Trace Files cannot be opened.");
        case kSI2DR_PIINIT_NOT_CALLED:
            return si2drStringT("PIInit() not yet called.");
        case kSI2DR_SEMANTIC_ERROR:
            return si2drStringT("Semantic Error found in data.");
        case kSI2DR_REFERENCE_ERROR:
            return si2drStringT("Reference Error found in data.");
        default:
            break;
    }
    return si2drStringT("?");
}
si2drObjectIdT LibAnalysis::__si2drPIGetNullId(si2drErrorT *err) {
    *err = kSI2DR_NO_ERROR;
    return nulloid_;
}
si2drVoidT LibAnalysis::si2drPIInit(si2drErrorT *err) {
    if (strtab_ == nullptr || sd_ == nullptr) return;
    timinglib_hash_table *&master_group_hash = sd_->master_group_hash;
    timinglib_strtable *&master_string_table = sd_->master_string_table;
    timinglib_hash_table *&master_define_hash = sd_->master_define_hash;
    /* set up the master hash */
    if (trace_) {
        fprintf(tracefile1_, "\n\tsi2drPIInit(&err);\n\n");
        __incTracecount();
    }
    master_group_hash = LibHash::timinglibHashCreateHashTable(53, 1, 0);
    master_define_hash = LibHash::timinglibHashCreateHashTable(53, 1, 0);
    master_string_table =
        strtab_->timinglibStrtableCreateStrtable(129235, 1024 * 1024, 0);
    /* 1 meg of string storage space, and 129000 or so slot table. */
    si2ErrMsg_ = &LibAnalysis::__si2drDefaultMessageHandler;
    *err = kSI2DR_NO_ERROR;
}
si2drVoidT LibAnalysis::si2drPIQuit(si2drErrorT *err) {
    if (strtab_ == nullptr || sd_ == nullptr) return;
    timinglib_hash_table *master_group_hash = sd_->master_group_hash;
    timinglib_strtable *master_string_table = sd_->master_string_table;
    timinglib_hash_table *master_define_hash = sd_->master_define_hash;
    si2drGroupsIdT groups;
    si2drGroupIdT group;
    if (trace_) {
        fprintf(tracefile1_, "\n\tsi2drPIQuit(&err);\n\n");
        __incTracecount();
    }
    /* free up any groups laying around */
    groups = __si2drPIGetGroups(err);
    if (groups != nullptr) {
        while (!__si2drObjectIsNull((group = __si2drIterNextGroup(groups, err)),
                                    err)) {
            __si2drObjectDelete(group, err);
        }
        __si2drIterQuit(groups, err);
    }

    /* destroy the master hash */
    LibHash::timinglibHashDestroyHashTable(master_group_hash);
    LibHash::timinglibHashDestroyHashTable(master_define_hash);
    master_group_hash = 0;
    master_define_hash = 0;
    if (l__iter_group_count_ != 0 || l__iter_name_count_ != 0 ||
        l__iter_val_count_ != 0 || l__iter_attr_count_ != 0 ||
        l__iter_def_count_ != 0) {
        si2drErrorT err2;
        char tbuf[1000];
        (this->*si2ErrMsg_)(
            kSI2DR_SEVERITY_WARN, kSI2DR_NO_ERROR,
            si2drStringT("si2drPIQuit: Poor Coding Practice Detected--\n"
                         "For each Iterator create, an IterQuit function must "
                         "be called\n"),
            &err2);
        if (l__iter_group_count_ != 0) {
            si2drErrorT err2;
            snprintf(
                tbuf, sizeof(tbuf),
                "si2drPIQuit: GetGroups called %d more times than IterQuit\n",
                l__iter_group_count_);
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_WARN, kSI2DR_NO_ERROR, tbuf,
                                &err2);
        }
        if (l__iter_name_count_ != 0) {
            si2drErrorT err2;
            snprintf(
                tbuf, sizeof(tbuf),
                "si2drPIQuit: GetNames called %d more times than IterQuit\n",
                l__iter_name_count_);
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_WARN, kSI2DR_NO_ERROR, tbuf,
                                &err2);
        }
        if (l__iter_val_count_ != 0) {
            si2drErrorT err2;
            snprintf(
                tbuf, sizeof(tbuf),
                "si2drPIQuit: GetValues called %d more times than IterQuit\n",
                l__iter_val_count_);
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_WARN, kSI2DR_NO_ERROR, tbuf,
                                &err2);
        }
        if (l__iter_attr_count_ != 0) {
            si2drErrorT err2;
            snprintf(
                tbuf, sizeof(tbuf),
                "si2drPIQuit: GetAttrs called %d more times than IterQuit\n",
                l__iter_attr_count_);
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_WARN, kSI2DR_NO_ERROR, tbuf,
                                &err2);
        }
        if (l__iter_def_count_ != 0) {
            si2drErrorT err2;
            snprintf(
                tbuf, sizeof(tbuf),
                "si2drPIQuit: GetDefines called %d more times than IterQuit\n",
                l__iter_def_count_);
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_WARN, kSI2DR_NO_ERROR, tbuf,
                                &err2);
        }
    }
    strtab_->timinglibStrtableDestroyStrtable(master_string_table);
    master_string_table = 0;
    /*mallocPrintTotals();*/
    *err = kSI2DR_NO_ERROR;
}
si2drObjectTypeT LibAnalysis::__si2drObjectGetObjectType(si2drObjectIdT object,
                                                         si2drErrorT *err) {
    si2drObjectTypeT ret;
    ret = (si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(object.v1));
    if (trace_) {
        fprintf(tracefile1_, "\n\t__si2drObjectGetObjectType(%s,&err);\n\n",
                __oidString(object).c_str());
        __incTracecount();
    }
    *err = kSI2DR_NO_ERROR;
    return ret;
}
si2drBooleanT LibAnalysis::__si2drObjectIsNull(si2drObjectIdT object,
                                               si2drErrorT *err) {
    if (trace_) {
        fprintf(tracefile1_, "\n\t__si2drObjectIsNull(%s,&err);\n\n",
                __oidString(object).c_str());
        __incTracecount();
    }
    *err = kSI2DR_NO_ERROR;
    if (object.v2 == static_cast<void *>(NULL))
        return SI2DR_TRUE;
    else
        return SI2DR_FALSE;
}
si2drBooleanT LibAnalysis::__si2drObjectIsSame(si2drObjectIdT object1,
                                               si2drObjectIdT object2,
                                               si2drErrorT *err) {
    if (trace_) {
        fprintf(tracefile1_, "\n\t__si2drObjectIsSame(%s,%s,&err);\n\n",
                __oidString(object1).c_str(), __oidString(object2).c_str());
        __incTracecount();
    }
    *err = kSI2DR_NO_ERROR;
    if (object1.v1 == object2.v1 && object1.v2 == object2.v2)
        return SI2DR_TRUE;
    else
        return SI2DR_FALSE;
}
si2drBooleanT LibAnalysis::__si2drObjectIsUsable(si2drObjectIdT object,
                                                 si2drErrorT *err) {
    if (trace_) {
        fprintf(tracefile1_, "\n\t__si2drObjectIsUsable(%s,&err);\n\n",
                __oidString(object).c_str());
        __incTracecount();
    }
    *err = kSI2DR_NO_ERROR;
    if (object.v2 == static_cast<void *>(NULL)) return SI2DR_FALSE;
    if (object.v1 == reinterpret_cast<void *>(kSI2DR_GROUP) ||
        object.v1 == reinterpret_cast<void *>(kSI2DR_ATTR) ||
        object.v1 == reinterpret_cast<void *>(kSI2DR_DEFINE))
        return SI2DR_TRUE;
    else
        return SI2DR_FALSE;
}
si2drVoidT LibAnalysis::si2drObjectSetFileName(si2drObjectIdT object,
                                               si2drStringT filename,
                                               si2drErrorT *err) {
    timinglib_group *g;
    timinglib_attribute *a;
    timinglib_define *d;
    if (trace_) {
        fprintf(tracefile1_, "\n\tsi2drObjectSetFileName(%s,\"%s\",&err);\n\n",
                __oidString(object).c_str(), filename);
        __incTracecount();
    }
    *err = kSI2DR_NO_ERROR;
    switch ((si2drObjectTypeT)((reinterpret_cast<std::intptr_t>(object.v1)))) {
        case kSI2DR_GROUP:
            g = static_cast<timinglib_group *>(object.v2);
            g->filename = filename;
            break;
        case kSI2DR_ATTR:
            a = static_cast<timinglib_attribute *>(object.v2);
            a->filename = filename;
            break;
        case kSI2DR_DEFINE:
            d = static_cast<timinglib_define *>(object.v2);
            d->filename = filename;
            break;
        default:
            *err = kSI2DR_INVALID_OBJECTTYPE;
            break;
    }
}
si2drVoidT LibAnalysis::si2drObjectSetLineNo(si2drObjectIdT object,
                                             si2drInt32T lineno,
                                             si2drErrorT *err) {
    timinglib_group *g;
    timinglib_attribute *a;
    timinglib_define *d;
    if (trace_) {
        fprintf(tracefile1_, "\n\tsi2drObjectSetLineNo(%s,%d,&err);\n\n",
                __oidString(object).c_str(), static_cast<int>(lineno));
        __incTracecount();
    }
    *err = kSI2DR_NO_ERROR;
    switch ((si2drObjectTypeT)((reinterpret_cast<std::intptr_t>(object.v1)))) {
        case kSI2DR_GROUP:
            g = static_cast<timinglib_group *>(object.v2);
            g->lineno = lineno;
            break;
        case kSI2DR_ATTR:
            a = static_cast<timinglib_attribute *>(object.v2);
            a->lineno = lineno;
            break;
        case kSI2DR_DEFINE:
            d = static_cast<timinglib_define *>(object.v2);
            d->lineno = lineno;
            break;
        default:
            *err = kSI2DR_INVALID_OBJECTTYPE;
            break;
    }
}
si2drInt32T LibAnalysis::__si2drObjectGetLineNo(si2drObjectIdT object,
                                                si2drErrorT *err) {
    timinglib_group *g;
    timinglib_attribute *a;
    timinglib_define *d;
    if (trace_) {
        fprintf(tracefile1_, "\n\tsi2drObjectSetLineNo(%s,&err);\n\n",
                __oidString(object).c_str());
        __incTracecount();
    }
    *err = kSI2DR_NO_ERROR;
    switch ((si2drObjectTypeT)((reinterpret_cast<std::intptr_t>(object.v1)))) {
        case kSI2DR_GROUP:
            g = static_cast<timinglib_group *>(object.v2);
            return g->lineno;
        case kSI2DR_ATTR:
            a = static_cast<timinglib_attribute *>(object.v2);
            return a->lineno;
        case kSI2DR_DEFINE:
            d = static_cast<timinglib_define *>(object.v2);
            return d->lineno;
        default:
            *err = kSI2DR_INVALID_OBJECTTYPE;
            return -1;
    }
}
si2drStringT LibAnalysis::__si2drObjectGetFileName(si2drObjectIdT object,
                                                   si2drErrorT *err) {
    timinglib_group *g;
    timinglib_attribute *a;
    timinglib_define *d;
    if (trace_) {
        fprintf(tracefile1_, "\n\tsi2drObjectSetLineNo(%s,&err);\n\n",
                __oidString(object).c_str());
        __incTracecount();
    }
    *err = kSI2DR_NO_ERROR;
    switch ((si2drObjectTypeT)((reinterpret_cast<std::intptr_t>(object.v1)))) {
        case kSI2DR_GROUP:
            g = static_cast<timinglib_group *>(object.v2);
            return g->filename;
        case kSI2DR_ATTR:
            a = static_cast<timinglib_attribute *>(object.v2);
            return a->filename;
        case kSI2DR_DEFINE:
            d = static_cast<timinglib_define *>(object.v2);
            return d->filename;
        default:
            *err = kSI2DR_INVALID_OBJECTTYPE;
            return reinterpret_cast<char *>(-1);
    }
}
si2drVoidT LibAnalysis::__si2drReadLibertyFile(char *filename,
                                               si2drErrorT *err) {
    if (sd_ == nullptr) return;
    /*
    timinglib_strtable *master_string_table = sd_->master_string_table;
    extern FILE *timinglib_pcarser2_in;
    int in_trace = 0;
    char comm[500];
    char *curr_file = sd_->curr_file;
    extern int syntax_errors;
    curr_file = filename;
    *err = kSI2DR_NO_ERROR;
    if( trace_ )
    {
            fprintf(tracefile1_,"\n\t__si2drReadLibertyFile(\"%s\",&err);\n\n",
   filename); __incTracecount(); in_trace = 1; trace_ = 0;
    }
    si2drPISetNocheckMode(err);
    if( !strcmp( filename+strlen(filename)-4, ".bz2") )
    {
            sprintf(comm,"bzip2 -cd %s", filename);
            timinglib_parser2_in = popen(comm,"r");
            if( !timinglib_parser2_in )
            {
                    si2drErrorT err2;
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
   kSI2DR_INVALID_NAME, si2drStringT("si2dreadLibertyFile: Could not execute
   bzip2!"), &err2); *err = kSI2DR_INVALID_NAME; perror("timinglib_parse");
                    return ;
            }
    }
    else if( !strcmp( filename+strlen(filename)-4, ".zip") || !strcmp(
   filename+strlen(filename)-4, ".ZIP"))
    {
            sprintf(comm,"unzip -cqq %s", filename);
            timinglib_parser2_in = popen(comm,"r");
            if( !timinglib_parser2_in )
            {
                    si2drErrorT err2;
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
   kSI2DR_INVALID_NAME, si2drStringT("si2dreadLibertyFile: Could not execute
   unzip!"), &err2); *err = kSI2DR_INVALID_NAME; perror("timinglib_parse");
                    return;
            }
    }
    else if( !strcmp( filename+strlen(filename)-3, ".gz") || !strcmp(
   filename+strlen(filename)-2, ".Z"))
    {
            sprintf(comm,"gzip -cd %s", filename);
            timinglib_parser2_in = popen(comm,"r");
            if( !timinglib_parser2_in )
            {
                    si2drErrorT err2;
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
   kSI2DR_INVALID_NAME, si2drStringT("si2dreadLibertyFile: Could not execute
   gzip!"), &err2); *err = kSI2DR_INVALID_NAME; perror("timinglib_parse");
                    return;
            }
    }
    else
            timinglib_parser2_in = fopen(filename,"r");
    if( timinglib_parser2_in == static_cast<FILE *>(NULL) )
    {
            *err = kSI2DR_INVALID_NAME;
            perror("timinglib_parse");
            return;
    }
    timinglibParserParse();
    if( strcmp( filename+strlen(filename)-3, ".gz")
            && strcmp( filename+strlen(filename)-2, ".Z")
            && strcmp( filename+strlen(filename)-4, ".bz2")
            && strcmp( filename+strlen(filename)-4, ".ZIP")
            && strcmp( filename+strlen(filename)-4, ".zip") )
            fclose(timinglib_parser2_in);
    else
            pclose(timinglib_parser2_in);
    si2drPIUnSetNocheckMode(err);
    if( in_trace )
            trace_ =1;
    if( syntax_errors )
            *err = kSI2DR_SYNTAX_ERROR;
    */
}
int LibAnalysis::__libNameNeedsToBeQuoted(char *name) {
    char *s = name;
    if (*s >= '0' && *s <= '9') return 1;
    while (*s) {
        if (*s == ' ') return 1;
        if ((*s < '0') || (*s > '9' && *s < 'A') || (*s > 'Z' && *s < 'a') ||
            (*s > 'z'))
            return 1;
        s++;
    }
    return 0;
}
char *LibAnalysis::__exprString(si2drExprT *e) {
    if (strtab_ == nullptr || sd_ == nullptr) return const_cast<char *>("");
    timinglib_strtable *master_string_table = sd_->master_string_table;
    char *buf1 = NULL;
    char tbuf[10000];
    char *lefts = 0, *rights = 0;
    if (e->left) lefts = __exprString(e->left);
    if (e->right) rights = __exprString(e->right);
    switch (e->type) {
        case kSI2DR_EXPR_VAL:
            switch (e->valuetype) {
                case kSI2DR_STRING:
                    snprintf(tbuf, sizeof(tbuf), "%s", e->u.s);
                    /* OLD:
                                    buf1 = (char*)malloc(strlen(tbuf)+1);
                                    strcpy(buf1,tbuf);
                                    NEW: */
                    buf1 = strtab_->timinglibStrtableEnterString(
                        master_string_table, tbuf);
                    break;
                case kSI2DR_FLOAT64:
                    snprintf(tbuf, sizeof(tbuf), "%g", e->u.d);
                    /* OLD:
                                    buf1 = (char*)malloc(strlen(tbuf)+1);
                                    strcpy(buf1,tbuf);
                                    NEW: */
                    buf1 = strtab_->timinglibStrtableEnterString(
                        master_string_table, tbuf);
                    break;
                case kSI2DR_INT32:
                    snprintf(tbuf, sizeof(tbuf), "%ld", e->u.i);
                    /* OLD:
                                    buf1 = (char*)malloc(strlen(tbuf)+1);
                                    strcpy(buf1,tbuf);
                                    NEW: */
                    buf1 = strtab_->timinglibStrtableEnterString(
                        master_string_table, tbuf);
                    break;
                default:
                    break;
            }
            break;
        case kSI2DR_EXPR_OP_ADD:
            if (rights)
                snprintf(tbuf, sizeof(tbuf), "%s + %s", lefts, rights);
            else
                snprintf(tbuf, sizeof(tbuf), "+%s", lefts);
            /* OLD:
                    buf1 = (char*)malloc(strlen(tbuf)+1);
                    strcpy(buf1,tbuf);
                    NEW: */
            buf1 = strtab_->timinglibStrtableEnterString(master_string_table,
                                                         tbuf);
            /* Don't do mallocs anymore on strings... OLD:
                    free(lefts);
                    if( rights )
                            free(rights);
                    */
            break;
        case kSI2DR_EXPR_OP_SUB:
            if (rights)
                snprintf(tbuf, sizeof(tbuf), "%s - %s", lefts, rights);
            else
                snprintf(tbuf, sizeof(tbuf), "-%s", lefts);
            /* OLD:
                    buf1 = (char*)malloc(strlen(tbuf)+1);
                    strcpy(buf1,tbuf);
                    NEW: */
            buf1 = strtab_->timinglibStrtableEnterString(master_string_table,
                                                         tbuf);
            /* OLD:
                    free(lefts);
                    if( rights )
                            free(rights);
                    */
            break;
        case kSI2DR_EXPR_OP_MUL:
            snprintf(tbuf, sizeof(tbuf), "%s * %s", lefts, rights);
            /* OLD:
                    buf1 = (char*)malloc(strlen(tbuf)+1);
                    strcpy(buf1,tbuf);
                    NEW: */
            buf1 = strtab_->timinglibStrtableEnterString(master_string_table,
                                                         tbuf);
            /* OLD:
                    free(lefts);
                    free(rights);
                    */
            break;
        case kSI2DR_EXPR_OP_DIV:
            snprintf(tbuf, sizeof(tbuf), "%s / %s", lefts, rights);
            /* OLD:
                    buf1 = (char*)malloc(strlen(tbuf)+1);
                    strcpy(buf1,tbuf);
                    NEW: */
            buf1 = strtab_->timinglibStrtableEnterString(master_string_table,
                                                         tbuf);
            /* OLD:
                    free(lefts);
                    free(rights);
                    */
            break;
        case kSI2DR_EXPR_OP_PAREN:
            snprintf(tbuf, sizeof(tbuf), "(%s)", lefts);
            /* OLD:
                    buf1 = (char*)malloc(strlen(tbuf)+1);
                    strcpy(buf1,tbuf);
                    NEW: */
            buf1 = strtab_->timinglibStrtableEnterString(master_string_table,
                                                         tbuf);
            /* OLD:
                    free(lefts);
                    */
            break;
        case kSI2DR_EXPR_OP_LOG2:
            break;
        case kSI2DR_EXPR_OP_LOG10:
            break;
        case kSI2DR_EXPR_OP_EXP:
            break;
    }
    return buf1;
}
void LibAnalysis::__libWriteGroup(FILE *of, si2drGroupIdT group, char *indent) {
    char indent2[200];
    si2drErrorT err;
    si2drStringT str;
    si2drNamesIdT names;
    si2drGroupsIdT groups;
    si2drAttrsIdT attrs;
    si2drDefinesIdT defs;
    si2drGroupIdT g1;
    si2drAttrIdT attr;
    si2drDefineIdT def;
    int first;
    snprintf(indent2, sizeof(indent2), "%s\t", indent);
    if ((str = __si2drGroupGetComment(group, &err)) && str[0] != 0) {
        fprintf(of, "%s/* %s */\n", indent, str);
    }
    first = 1;
    fprintf(of, "%s%s (", indent, __si2drGroupGetGroupType(group, &err));
    names = __si2drGroupGetNames(group, &err);
    for (str = __si2drIterNextName(names, &err); str && str[0];
         str = __si2drIterNextName(names, &err)) {
        if (!first) fprintf(of, ",");
        if (__libNameNeedsToBeQuoted(str))
            fprintf(of, "\"%s\"", str);
        else
            fprintf(of, "%s", str);
        first = 0;
    }
    __si2drIterQuit(names, &err);
    fprintf(of, ") {\n");
    /* print the defines */
    defs = __si2drGroupGetDefines(group, &err);
    for (def = __si2drIterNextDefine(defs, &err);
         !__si2drObjectIsNull(def, &err);
         def = __si2drIterNextDefine(defs, &err)) {
        si2drStringT nam, gnam;
        si2drValueTypeT valtype;
        char *vt;
        if ((str = __si2drDefineGetComment(def, &err)) && str[0] != 0) {
            fprintf(of, "%s/* %s */\n", indent2, str);
        }
        si2drDefineGetInfo(def, &nam, &gnam, &valtype, &err);
        switch (valtype) {
            case kSI2DR_STRING:
                vt = const_cast<char *>("string");
                break;
            case kSI2DR_FLOAT64:
                vt = const_cast<char *>("float");
                break;
            case kSI2DR_INT32:
                vt = const_cast<char *>("integer");
                break;
            case kSI2DR_BOOLEAN:
                vt = const_cast<char *>("boolean");
                break;
            default:
                break;
        }
        if (valtype != kSI2DR_UNDEFINED_VALUETYPE) {
            char *p, *q;
            char strTmp[100] = {0};
            q = gnam;
            while ((p = strchr(q, '|'))) {
                strncpy(strTmp, q, p - q);
                fprintf(of, "%sdefine(%s,%s,%s);\n", indent2, nam, strTmp, vt);
                memset(strTmp, 0, sizeof(strTmp));
                q = p + 1;
            }
            fprintf(of, "%sdefine(%s,%s,%s);\n", indent2, nam, q, vt);
        } else {
            char *p, *q;
            char strTmp[100] = {0};
            q = gnam;
            while ((p = strchr(q, '|'))) {
                strncpy(strTmp, q, p - q);
                fprintf(of, "%sdefineGroup(%s,%s);\n", indent2, nam, strTmp);
                memset(strTmp, 0, sizeof(strTmp));
                q = p + 1;
            }
            fprintf(of, "%sdefineGroup(%s,%s);\n", indent2, nam, q);
        }
    }
    __si2drIterQuit(defs, &err);
    /* print the attrs */
    attrs = __si2drGroupGetAttrs(group, &err);
    for (attr = __si2drIterNextAttr(attrs, &err);
         !__si2drObjectIsNull(attr, &err);
         attr = __si2drIterNextAttr(attrs, &err)) {
        si2drAttrTypeT at = __si2drAttrGetAttrType(attr, &err);
        si2drValuesIdT vals;
        si2drValueTypeT vtype;
        si2drInt32T intgr;
        si2drFloat64T float64;
        si2drStringT string, nam = __si2drAttrGetName(attr, &err);
        si2drBooleanT boolval;
        si2drBooleanT is_var;
        si2drExprT *exprp;
        int first;
        if ((str = __si2drAttrGetComment(attr, &err)) && str[0] != 0) {
            fprintf(of, "%s/* %s */\n", indent2, str);
        }
        if (at == kSI2DR_SIMPLE &&
            (!strcmp(nam, "default_operating_conditions") ||
             !strcmp(nam, "default_wire_load") ||
             !strcmp(nam, "default_wire_load_selection")))
            continue;
        fprintf(of, "%s%s", indent2, nam);
        switch (at) {
            case kSI2DR_SIMPLE:
                vtype = __si2drSimpleAttrGetValueType(attr, &err);
                is_var = __si2drSimpleAttrGetIsVar(attr, &err);
                switch (vtype) {
                    case kSI2DR_STRING:
                        if (is_var)
                            fprintf(
                                of, " = \"%s\";\n",
                                __si2drSimpleAttrGetStringValue(attr, &err));
                        else
                            fprintf(
                                of, " : \"%s\";\n",
                                __si2drSimpleAttrGetStringValue(attr, &err));
                        break;
                    case kSI2DR_FLOAT64:
                        if (is_var)
                            fprintf(
                                of, " = %1.6f;\n",
                                __si2drSimpleAttrGetFloat64Value(attr, &err));
                        else if (!strcmp(nam, "key_bit") ||
                                 !strcmp(nam, "address_width") ||
                                 !strcmp(nam, "word_width") ||
                                 !strcmp(nam, "key_seed") ||
                                 !strcmp(nam, "tracks") ||
                                 !strcmp(nam, "divided_by") ||
                                 !strcmp(nam, "multiplied_by") ||
                                 !strcmp(nam, "bit_from") ||
                                 !strcmp(nam, "bit_to") ||
                                 !strcmp(nam, "bit_width"))
                            fprintf(of, " : %d;\n",
                                    static_cast<int>(
                                        __si2drSimpleAttrGetFloat64Value(
                                            attr, &err)));
                        else if (!strcmp(__si2drGroupGetGroupType(group, &err),
                                         "leakage_power") &&
                                 !strcmp(nam, "value"))
                            fprintf(
                                of, " : %1.6e;\n",
                                __si2drSimpleAttrGetFloat64Value(attr, &err));
                        else if (!strcmp(nam, "cell_leakage_power"))
                            fprintf(
                                of, " : %1.6e;\n",
                                __si2drSimpleAttrGetFloat64Value(attr, &err));
                        else if (!strcmp(__si2drGroupGetGroupType(group, &err),
                                         "gate_leakage"))
                            fprintf(
                                of, " : %1.6e;\n",
                                __si2drSimpleAttrGetFloat64Value(attr, &err));
                        else if (!strcmp(__si2drGroupGetGroupType(group, &err),
                                         "pg_current"))
                            fprintf(
                                of, " : %1.6e;\n",
                                __si2drSimpleAttrGetFloat64Value(attr, &err));
                        else if (!strcmp(__si2drGroupGetGroupType(group, &err),
                                         "intrinsic_capacitance"))
                            fprintf(
                                of, " : %1.6e;\n",
                                __si2drSimpleAttrGetFloat64Value(attr, &err));
                        else if (!strcmp(__si2drGroupGetGroupType(group, &err),
                                         "wire_load") &&
                                 !strcmp(nam, "resistance"))
                            fprintf(
                                of, " : %e;\n",
                                __si2drSimpleAttrGetFloat64Value(attr, &err));
                        else
                            fprintf(
                                of, " : %1.6f;\n",
                                __si2drSimpleAttrGetFloat64Value(attr, &err));
                        break;
                    case kSI2DR_INT32:
                        if (is_var)
                            fprintf(
                                of, " = %d;\n",
                                static_cast<int>(__si2drSimpleAttrGetInt32Value(
                                    attr, &err)));
                        else
                            fprintf(
                                of, " : %d;\n",
                                static_cast<int>(__si2drSimpleAttrGetInt32Value(
                                    attr, &err)));
                        break;
                    case kSI2DR_EXPR: {
                        char *es = __exprString(
                            __si2drSimpleAttrGetExprValue(attr, &err));
                        if (is_var)
                            fprintf(of, " = %s;\n", es);
                        else
                            fprintf(of, " : %s;\n", es);
                        /* expr strings in strtable OLD: free(es); */
                    } break;
                    case kSI2DR_BOOLEAN:
                        if (is_var) {
                            if (__si2drSimpleAttrGetBooleanValue(attr, &err))
                                fprintf(of, " = true;\n");
                            else
                                fprintf(of, " = false;\n");
                        } else {
                            if (__si2drSimpleAttrGetBooleanValue(attr, &err))
                                fprintf(of, " : true;\n");
                            else
                                fprintf(of, " : false;\n");
                        }
                        break;
                    default:
                        break;
                }
                break;
            case kSI2DR_COMPLEX:
                vals = __si2drComplexAttrGetValues(attr, &err);
                first = 1;
                fprintf(of, "(");
                for (__si2drIterNextComplexValue(vals, &vtype, &intgr, &float64,
                                                 &string, &boolval, &exprp,
                                                 &err);
                     vtype != kSI2DR_UNDEFINED_VALUETYPE;
                     __si2drIterNextComplexValue(vals, &vtype, &intgr, &float64,
                                                 &string, &boolval, &exprp,
                                                 &err)) {
                    if (!first) fprintf(of, ",");
                    switch (vtype) {
                        case kSI2DR_STRING:
                            /* [20060809:nanda] Workaround for */
                            /* define_group attribute */
                            /* fprintf(of,"\"%s\"", string); */
                            /* if( !first )                  */
                            /* fprintf(of," \\\n%s    ", indent2); */
                            if (!strcmp(nam, "define_group")) {
                                fprintf(of, "%s", string);
                            } else {
                                if (!first) fprintf(of, " \\\n%s    ", indent2);
                                fprintf(of, "\"%s\"", string);
                            }
                            break;
                        case kSI2DR_FLOAT64:
                            if (!strcmp(nam, "curve_y") ||
                                !strcmp(nam, "vector"))
                                fprintf(of, "%d", static_cast<int>(float64));
                            else if (!strcmp(nam, "va_values") ||
                                     !strcmp(nam, "nominal_va_values"))
                                fprintf(of, "%1.6e", float64);
                            else
                                fprintf(of, "%1.6f", float64);
                            break;
                        case kSI2DR_INT32:
                            fprintf(of, "%d", static_cast<int>(intgr));
                            break;
                        case kSI2DR_BOOLEAN:
                            fprintf(of, "%d", boolval);
                            break;
                        default:
                            break;
                    }
                    first = 0;
                }
                __si2drIterQuit(vals, &err);
                fprintf(of, ");\n");
                break;
        }
    }
    __si2drIterQuit(attrs, &err);
    /* print the groups */
    groups = __si2drGroupGetGroups(group, &err);
    for (g1 = __si2drIterNextGroup(groups, &err);
         !__si2drObjectIsNull(g1, &err);
         g1 = __si2drIterNextGroup(groups, &err)) {
        __libWriteGroup(of, g1, indent2);
    }
    __si2drIterQuit(groups, &err);
    /* print the attrs */
    attrs = __si2drGroupGetAttrs(group, &err);
    for (attr = __si2drIterNextAttr(attrs, &err);
         !__si2drObjectIsNull(attr, &err);
         attr = __si2drIterNextAttr(attrs, &err)) {
        si2drAttrTypeT at = __si2drAttrGetAttrType(attr, &err);
        si2drValueTypeT vtype;
        si2drStringT string, nam = __si2drAttrGetName(attr, &err);
        if (at != kSI2DR_SIMPLE ||
            (strcmp(nam, "default_operating_conditions") &&
             strcmp(nam, "default_wire_load") &&
             strcmp(nam, "default_wire_load_selection")))
            continue;
        /* fprintf(of,"%s%s", indent2, nam); */
        if ((str = __si2drAttrGetComment(attr, &err)) && str[0] != 0) {
            fprintf(of, "%s/* %s */\n", indent2, str);
        }
        switch (at) {
            case kSI2DR_SIMPLE:
                vtype = __si2drSimpleAttrGetValueType(attr, &err);
                switch (vtype) {
                    case kSI2DR_STRING:
                        fprintf(of, "%s%s", indent2, nam);
                        if (!strcmp(nam, "vil") || !strcmp(nam, "vih") ||
                            !strcmp(nam, "vimax") || !strcmp(nam, "vimin") ||
                            !strcmp(nam, "vol") || !strcmp(nam, "voh") ||
                            !strcmp(nam, "vomax") || !strcmp(nam, "vomin"))
                            fprintf(of, " : %s;\n",
                                    (string = __si2drSimpleAttrGetStringValue(
                                         attr, &err)));
                        else if (!strcmp(nam, "key_bit") ||
                                 !strcmp(nam, "address_width") ||
                                 !strcmp(nam, "word_width") ||
                                 !strcmp(nam, "key_seed") ||
                                 !strcmp(nam, "tracks") ||
                                 !strcmp(nam, "divided_by") ||
                                 !strcmp(nam, "multiplied_by") ||
                                 !strcmp(nam, "bit_from") ||
                                 !strcmp(nam, "bit_to") ||
                                 !strcmp(nam, "bit_width"))
                            fprintf(of, " : %d;\n",
                                    atoi(__si2drSimpleAttrGetStringValue(
                                        attr, &err)));
                        else
                            fprintf(of, " : \"%s\";\n",
                                    (string = __si2drSimpleAttrGetStringValue(
                                         attr, &err)));
                        break;
                    case kSI2DR_FLOAT64:
                        if (!strcmp(nam, "key_bit") ||
                            !strcmp(nam, "address_width") ||
                            !strcmp(nam, "word_width") ||
                            !strcmp(nam, "key_seed") ||
                            !strcmp(nam, "tracks") ||
                            !strcmp(nam, "divided_by") ||
                            !strcmp(nam, "multiplied_by") ||
                            !strcmp(nam, "bit_from") ||
                            !strcmp(nam, "bit_to") || !strcmp(nam, "bit_width"))
                            fprintf(of, " : %d;\n",
                                    static_cast<int>(
                                        __si2drSimpleAttrGetFloat64Value(
                                            attr, &err)));
                        else if (!strcmp(__si2drGroupGetGroupType(group, &err),
                                         "leakage_power") &&
                                 !strcmp(nam, "value"))
                            fprintf(
                                of, " : %1.6e;\n",
                                __si2drSimpleAttrGetFloat64Value(attr, &err));
                        else if (!strcmp(nam, "cell_leakage_power"))
                            fprintf(
                                of, " : %1.6e;\n",
                                __si2drSimpleAttrGetFloat64Value(attr, &err));
                        else
                            fprintf(
                                of, " : %1.6f;\n",
                                __si2drSimpleAttrGetFloat64Value(attr, &err));
                        break;
                    case kSI2DR_INT32:
                        fprintf(of, " : %d;\n",
                                static_cast<int>(__si2drSimpleAttrGetInt32Value(
                                    attr, &err)));
                        break;
                    case kSI2DR_BOOLEAN:
                        if (__si2drSimpleAttrGetBooleanValue(attr, &err))
                            fprintf(of, " : true;\n");
                        else
                            fprintf(of, " : false;\n");
                        break;
                    default:
                        break;
                }
                break;
            case kSI2DR_COMPLEX:
#ifdef NOMORE
                vals = __si2drComplexAttrGetValues(attr, &err);
                first = 1;
                fprintf(of, "(");
                for (__si2drIterNextComplexValue(vals, &vtype, &intgr, &float64,
                                                 &string, &boolval, &expr,
                                                 &err);
                     vtype != kSI2DR_UNDEFINED_VALUETYPE;
                     __si2drIterNextComplexValue(vals, &vtype, &intgr, &float64,
                                                 &string, &boolval, &expr,
                                                 &err)) {
                    if (!first) fprintf(of, ",");
                    switch (vtype) {
                        case kSI2DR_STRING:
                            fprintf(of, "\"%s\"", string);
                            break;
                        case kSI2DR_FLOAT64:
                            if (!strcmp(nam, "nominal_va_values") ||
                                !strcmp(nam, "va_values"))
                                fprintf(of, "%1.6e", float64);
                            else
                                fprintf(of, "%1.6f", float64);
                            break;
                        case kSI2DR_INT32:
                            fprintf(of, "%d", intgr);
                            break;
                        case kSI2DR_BOOLEAN:
                            fprintf(of, "%d", boolval);
                            break;
                    }
                    first = 0;
                }
                __si2drIterQuit(vals, &err);
                fprintf(of, ");\n");
#endif
                break;
        }
    }
    __si2drIterQuit(attrs, &err);
    fprintf(of, "%s}\n", indent);
}
si2drVoidT LibAnalysis::__si2drWriteLibertyFile(FILE *of, si2drGroupIdT group,
                                                si2drErrorT *err) {
    int in_trace;
    in_trace = 0;
    if (!__si2drObjectIsUsable(group, err)) return;
    if (trace_) {
        fprintf(tracefile1_,
                "\n\t__si2drWriteLibertyFile(\"%lx\",%s,&err);\n\n",
                reinterpret_cast<uint64_t>(of), __oidString(group).c_str());
        __incTracecount();
        in_trace = 1;
        trace_ = 0;
    }
    si2drPISetNocheckMode(err);
    /* call the top level routine */
    __si2drPISetDebugMode(err);
    __libWriteGroup(of, group, const_cast<char *>(""));
    __si2drPIUnSetDebugMode(err);
    if (in_trace) trace_ = 1;
}
si2drVoidT LibAnalysis::__si2drCheckLibertyLibrary(si2drGroupIdT group,
                                                   si2drErrorT *err) {
    int in_trace;
    in_trace = 0;
    if (trace_) {
        fprintf(tracefile1_, "\n\t__si2drCheckLibertyLibrary(%s,&err);\n\n",
                __oidString(group).c_str());
        __incTracecount();
        in_trace = 1;
        trace_ = 0;
    }
    *err = __syntaxCheck(group);
    if (in_trace) trace_ = 1;
}
si2drStringT LibAnalysis::__si2drGroupGetGroupType(si2drGroupIdT group,
                                                   si2drErrorT *err) {
    if (sd_ == nullptr) return 0;
    timinglib_hash_table *master_group_hash = sd_->master_group_hash;
    timinglib_group *g = static_cast<timinglib_group *>(group.v2);
    if (trace_) {
        fprintf(tracefile1_, "\n\t__si2drGroupGetGroupType(%s,&err);\n\n",
                __oidString(group).c_str());
        __incTracecount();
    }
    if (!g) {
        *err = kSI2DR_UNUSABLE_OID;
        return 0;
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(group.v1)) !=
            kSI2DR_GROUP) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            return 0;
        }
        if (master_group_hash == 0) {
            *err = kSI2DR_INTERNAL_SYSTEM_ERROR;
            return 0;
        }
        if (g->attr_hash == static_cast<timinglib_hash_table *>(NULL)) {
            *err = kSI2DR_UNUSABLE_OID;
            return 0;
        }
    }
    return g->type;
}
si2drVoidT LibAnalysis::si2drDefineGetInfo(si2drDefineIdT def,
                                           si2drStringT *name,
                                           si2drStringT *allowed_group_name,
                                           si2drValueTypeT *valtype,
                                           si2drErrorT *err) {
    if (sd_ == nullptr) return;
    timinglib_hash_table *master_group_hash = sd_->master_group_hash;
    timinglib_define *d = static_cast<timinglib_define *>(def.v2);
    if (trace_) {
        fprintf(tracefile1_,
                "\n\tsi2drDefineGetInfo(%s,&name_string, &allowed_groups, "
                "&valtype, &err);\n\n",
                __oidString(def).c_str());
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(def.v1)) !=
            kSI2DR_DEFINE) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            return;
        }
        if (master_group_hash == 0) {
            *err = kSI2DR_INTERNAL_SYSTEM_ERROR;
            return;
        }
    }
    *name = d->name;
    *allowed_group_name = d->group_type;
    switch (d->valtype) {
        case kTIMINGLIB__VAL_STRING:
            *valtype = kSI2DR_STRING;
            break;
        case kTIMINGLIB__VAL_DOUBLE:
            *valtype = kSI2DR_FLOAT64;
            break;
        case kTIMINGLIB__VAL_BOOLEAN:
            *valtype = kSI2DR_BOOLEAN;
            break;
        case kTIMINGLIB__VAL_INT:
            *valtype = kSI2DR_INT32;
            break;
        case kTIMINGLIB__VAL_UNDEFINED:
            *valtype = kSI2DR_UNDEFINED_VALUETYPE;
            break;
        default:
            break;
    }
    *err = kSI2DR_NO_ERROR;
}
si2drStringT LibAnalysis::__si2drDefineGetName(si2drDefineIdT def,
                                               si2drErrorT *err) {
    if (sd_ == nullptr) return si2drStringT("");
    timinglib_hash_table *master_group_hash = sd_->master_group_hash;
    timinglib_define *d = static_cast<timinglib_define *>(def.v2);
    if (trace_) {
        fprintf(tracefile1_, "\n\t__si2drDefineGetName(%s,&err);\n\n",
                __oidString(def).c_str());
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(def.v1)) !=
            kSI2DR_DEFINE) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            return si2drStringT("");
        }
        if (master_group_hash == 0) {
            *err = kSI2DR_INTERNAL_SYSTEM_ERROR;
            return si2drStringT("");
        }
    }
    *err = kSI2DR_NO_ERROR;
    return d->name;
}
si2drStringT LibAnalysis::__si2drDefineGetAllowedGroupName(si2drDefineIdT def,
                                                           si2drErrorT *err) {
    if (sd_ == nullptr) return si2drStringT("");
    timinglib_hash_table *master_group_hash = sd_->master_group_hash;
    timinglib_define *d = static_cast<timinglib_define *>(def.v2);
    if (trace_) {
        fprintf(tracefile1_,
                "\n\t__si2drDefineGetAllowedGroupName(%s,&err);\n\n",
                __oidString(def).c_str());
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(def.v1)) !=
            kSI2DR_DEFINE) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            return si2drStringT("");
        }
        if (master_group_hash == 0) {
            *err = kSI2DR_INTERNAL_SYSTEM_ERROR;
            return si2drStringT("");
        }
    }
    *err = kSI2DR_NO_ERROR;
    return d->group_type;
}
si2drValueTypeT LibAnalysis::__si2drDefineGetValueType(si2drDefineIdT def,
                                                       si2drErrorT *err) {
    if (sd_ == nullptr) return static_cast<si2drValueTypeT>(0);
    timinglib_hash_table *master_group_hash = sd_->master_group_hash;
    timinglib_define *d = static_cast<timinglib_define *>(def.v2);
    si2drValueTypeT valtype;
    if (trace_) {
        fprintf(tracefile1_, "\n\t__si2drDefineGetValueType(%s,&err);\n\n",
                __oidString(def).c_str());
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(def.v1)) !=
            kSI2DR_DEFINE) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            return static_cast<si2drValueTypeT>(0);
        }
        if (master_group_hash == 0) {
            *err = kSI2DR_INTERNAL_SYSTEM_ERROR;
            return static_cast<si2drValueTypeT>(0);
        }
    }
    switch (d->valtype) {
        case kTIMINGLIB__VAL_STRING:
            valtype = kSI2DR_STRING;
            break;
        case kTIMINGLIB__VAL_DOUBLE:
            valtype = kSI2DR_FLOAT64;
            break;
        case kTIMINGLIB__VAL_BOOLEAN:
            valtype = kSI2DR_BOOLEAN;
            break;
        case kTIMINGLIB__VAL_INT:
            valtype = kSI2DR_INT32;
            break;
        default:
        case kTIMINGLIB__VAL_UNDEFINED:
            valtype = kSI2DR_UNDEFINED_VALUETYPE;
            break;
    }
    *err = kSI2DR_NO_ERROR;
    return valtype;
}
si2drObjectIdT LibAnalysis::__si2drObjectGetOwner(si2drObjectIdT object,
                                                  si2drErrorT *err) {
    si2drObjectTypeT ot =
        (si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(object.v1));
    si2drObjectIdT retoid;
    timinglib_group *g;
    timinglib_attribute *a;
    timinglib_define *d;
    if (trace_) {
        fprintf(tracefile1_, "\n\t__si2drObjectGetOwner(%s,&err);\n\n",
                __oidString(object).c_str());
        __incTracecount();
    }
    *err = kSI2DR_NO_ERROR;
    switch (ot) {
        case kSI2DR_GROUP:
            g = static_cast<timinglib_group *>(object.v2);
            retoid.v2 = static_cast<void *>(g->owner);
            retoid.v1 = reinterpret_cast<void *>(kSI2DR_GROUP);
            break;
        case kSI2DR_ATTR:
            a = static_cast<timinglib_attribute *>(object.v2);
            retoid.v2 = static_cast<void *>(a->owner);
            retoid.v1 = reinterpret_cast<void *>(kSI2DR_GROUP);
            break;
        case kSI2DR_DEFINE:
            d = static_cast<timinglib_define *>(object.v2);
            retoid.v2 = static_cast<void *>(d->owner);
            retoid.v1 = reinterpret_cast<void *>(kSI2DR_GROUP);
            break;
        default:
            *err = kSI2DR_INVALID_OBJECTTYPE;
            return nulloid_;
    }
    return retoid;
}
si2drStringT LibAnalysis::__si2drAttrGetName(si2drAttrIdT attr,
                                             si2drErrorT *err) {
    timinglib_attribute *x = static_cast<timinglib_attribute *>(attr.v2);
    if (trace_) {
        fprintf(tracefile1_, "\n\t__si2drAttrGetName(%s,&err);\n\n",
                __oidString(attr).c_str());
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_ &&
        (si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(attr.v1)) !=
            kSI2DR_ATTR) {
        *err = kSI2DR_INVALID_OBJECTTYPE;
        if (timinglib_debug_mode_) {
            si2drErrorT err2;
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                                si2drStringT("__si2drAttrGetName:"), &err2);
        }
        return 0;
    }
    *err = kSI2DR_NO_ERROR;
    return x->name;
}
void LibAnalysis::__createFloatingDefineForCellArea(si2drStringT string) {
    if (sd_ == nullptr) return;
    timinglib_hash_table *master_define_hash = sd_->master_define_hash;
    si2drDefineIdT retoid;
    timinglib_define *d;
    /* now, given this string, let's create a define; */
    d = static_cast<timinglib_define *>(calloc(sizeof(timinglib_define), 1));
    retoid.v1 = reinterpret_cast<void *>(kSI2DR_DEFINE);
    retoid.v2 = static_cast<void *>(d);
    d->name = string;
    d->group_type = const_cast<char *>("cell");
    d->valtype = kTIMINGLIB__VAL_DOUBLE;
    LibHash::timinglibHashEnterOid(master_define_hash, string, retoid);
}
si2drVoidT LibAnalysis::__si2drGroupMoveBefore(si2drGroupIdT groupToMove,
                                               si2drGroupIdT targetGroup,
                                               si2drErrorT *err) {
    if (sd_ == nullptr) return;
    timinglib_hash_table *master_group_hash = sd_->master_group_hash;
    // int in_trace;
    timinglib_group *gm = static_cast<timinglib_group *>(groupToMove.v2);
    timinglib_group *gt = static_cast<timinglib_group *>(targetGroup.v2);
    timinglib_group *gb;
    timinglib_group *gtb;
    timinglib_group *go = gm->owner;
    // in_trace = 0;
    if (trace_) {
        fprintf(tracefile1_, "\n\t__si2drGroupMoveBefore(\"%s\",%s,&err);\n\n",
                __oidString(groupToMove).c_str(),
                __oidString(targetGroup).c_str());
        __incTracecount();
        // in_trace = 1;
        trace_ = 0;
    }
    /* make sure the groups belong to the same owner
       Thanks to Chenzhong Luo of Synopsys for discovering the
       necessity for this and the next check */
    if (gt == gm) {
        *err = kSI2DR_NO_ERROR;
        return;
    }
    if (gm->next == gt) {
        *err = kSI2DR_NO_ERROR;
        return;
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(
                groupToMove.v1)) != kSI2DR_GROUP) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drGroupMoveBefore:"), &err2);
            }
            return;
        }
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(
                targetGroup.v1)) != kSI2DR_GROUP) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drGroupMoveBefore:"), &err2);
            }
            return;
        }
        if (master_group_hash == 0) {
            *err = kSI2DR_INTERNAL_SYSTEM_ERROR;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INTERNAL_SYSTEM_ERROR,
                    si2drStringT("__si2drGroupMoveBefore:"), &err2);
            }
            return;
        }
        if (gm->attr_hash == static_cast<timinglib_hash_table *>(NULL)) {
            *err = kSI2DR_UNUSABLE_OID;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_UNUSABLE_OID,
                                    si2drStringT("__si2drGroupMoveBefore:"),
                                    &err2);
            }
            return;
        }
        if (gt->attr_hash == static_cast<timinglib_hash_table *>(NULL)) {
            *err = kSI2DR_UNUSABLE_OID;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_UNUSABLE_OID,
                                    si2drStringT("__si2drGroupMoveBefore:"),
                                    &err2);
            }
            return;
        }
        if (gm->owner != gt->owner) {
            *err = kSI2DR_REFERENCE_ERROR;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_REFERENCE_ERROR,
                                    si2drStringT("__si2drGroupMoveBefore:"),
                                    &err2);
            }
            return;
        }
    }
    *err = kSI2DR_NO_ERROR;
    if (gm != go->group_list) {
        for (gb = go->group_list; gb && gb->next != gm; gb = gb->next) {
        }
    } else {
        gb = 0;
    }
    if (gt != go->group_list) {
        for (gtb = go->group_list; gtb && gtb->next != gt; gtb = gtb->next) {
        }
    } else {
        gtb = 0;
    }
    if (gb == gm || gtb == gt || (!gtb && !gb)) {
        *err = kSI2DR_INTERNAL_SYSTEM_ERROR;
        if (timinglib_debug_mode_) {
            si2drErrorT err2;
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                kSI2DR_INTERNAL_SYSTEM_ERROR,
                                si2drStringT("__si2drGroupMoveBefore:"), &err2);
        }
        return;
    }
    if (!gb) {
        /* gb is at the front */
        go->group_list = gm->next; /* unlink gm from the list */
        gtb->next = gm; /* attach it the guy in front of gt, unlinking gt */
        gm->next = gt;  /* attach gt after gm */
    } else {
        if (go->group_last == gm) go->group_last = gb;
        if (!gtb) {
            /* the target is at the front of the list */
            gb->next = gm->next; /* gm now unlinked */
            go->group_list =
                gm;        /* now the list points to gm, and gt unlinked */
            gm->next = gt; /* and gt is attached right after gm */
        } else {
            /* neither are at the head of the list */
            gb->next = gm->next; /* gm now unlinked */
            gtb->next = gm;      /* the guy before gt now links to gm */
            gm->next = gt;       /* gm now points to gt as its follower. */
        }
    }
}
si2drVoidT LibAnalysis::__si2drGroupMoveAfter(si2drGroupIdT groupToMove,
                                              si2drGroupIdT targetGroup,
                                              si2drErrorT *err) {
    if (sd_ == nullptr) return;
    timinglib_hash_table *master_group_hash = sd_->master_group_hash;
    // int in_trace;
    timinglib_group *gm = static_cast<timinglib_group *>(groupToMove.v2);
    timinglib_group *gt = static_cast<timinglib_group *>(targetGroup.v2);
    timinglib_group *gb;
    timinglib_group *go = gm->owner;
    // in_trace = 0;
    if (trace_) {
        fprintf(tracefile1_, "\n\t__si2drGroupMoveAfter(\"%s\",%s,&err);\n\n",
                __oidString(groupToMove).c_str(),
                __oidString(targetGroup).c_str());
        __incTracecount();
        // in_trace = 1;
        trace_ = 0;
    }
    /* make sure the groups belong to the same owner */
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(
                groupToMove.v1)) != kSI2DR_GROUP) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drGroupMoveAfter:"), &err2);
            }
            return;
        }
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(
                targetGroup.v1)) != kSI2DR_GROUP) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drGroupMoveAfter:"), &err2);
            }
            return;
        }
        if (master_group_hash == 0) {
            *err = kSI2DR_INTERNAL_SYSTEM_ERROR;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INTERNAL_SYSTEM_ERROR,
                    si2drStringT("__si2drGroupMoveAfter:"), &err2);
            }
            return;
        }
        if (gm->attr_hash == static_cast<timinglib_hash_table *>(NULL)) {
            *err = kSI2DR_UNUSABLE_OID;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_UNUSABLE_OID,
                                    si2drStringT("__si2drGroupMoveAfter:"),
                                    &err2);
            }
            return;
        }
        if (gt->attr_hash == static_cast<timinglib_hash_table *>(NULL)) {
            *err = kSI2DR_UNUSABLE_OID;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_UNUSABLE_OID,
                                    si2drStringT("__si2drGroupMoveAfter:"),
                                    &err2);
            }
            return;
        }
        if (gm->owner != gt->owner) {
            *err = kSI2DR_REFERENCE_ERROR;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_REFERENCE_ERROR,
                                    si2drStringT("__si2drGroupMoveAfter:"),
                                    &err2);
            }
            return;
        }
    }
    *err = kSI2DR_NO_ERROR;
    if (gm != go->group_list) {
        for (gb = go->group_list; gb && gb->next != gm; gb = gb->next) {
        }
    } else {
        gb = 0;
    }
    if (gb == gm) {
        *err = kSI2DR_INTERNAL_SYSTEM_ERROR;
        if (timinglib_debug_mode_) {
            si2drErrorT err2;
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                kSI2DR_INTERNAL_SYSTEM_ERROR,
                                si2drStringT("__si2drGroupMoveAfter:"), &err2);
        }
        return;
    }
    if (gt == gm) return;
    if (gt->next ==
        gm) /* already done. Thanks to Chenzhong Luo for finding a bug here! */
        return;
    if (go->group_last == gt) go->group_last = gm;
    if (!gb) {
        /* gb is at the front */
        go->group_list = gm->next; /* unlink gm from the list */
    } else {
        gb->next = gm->next;
    }
    gm->next = gt->next;
    gt->next = gm; /* attach it the guy in front of gt, unlinking gt */
}
/* expr routines */
si2drExprT *LibAnalysis::__si2drSimpleAttrGetExprValue(si2drAttrIdT attr,
                                                       si2drErrorT *err) {
    timinglib_attribute *x = static_cast<timinglib_attribute *>(attr.v2);
    if (trace_) {
        fprintf(tracefile1_, "\n\t__si2drSimpleAttrGetExprValue(%s,&err);\n\n",
                __oidString(attr).c_str());
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(attr.v1)) !=
            kSI2DR_ATTR) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drSimpleAttrGetExprValue:"), &err2);
            }
            return 0;
        }
        if (x->type != kTIMINGLIB__SIMPLE) {
            *err = kSI2DR_INVALID_ATTRTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_ATTRTYPE,
                    si2drStringT("__si2drSimpleAttrGetExprValue:"), &err2);
            }
            return 0;
        }
        if (x->value == static_cast<timinglib_attribute_value *>(NULL)) {
            *err = kSI2DR_INVALID_VALUE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_VALUE,
                    si2drStringT("__si2drSimpleAttrGetExprValue:"), &err2);
            }
            return 0;
        }
        if (x->value->type != kTIMINGLIB__VAL_EXPR) {
            *err = kSI2DR_INVALID_VALUE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_VALUE,
                    si2drStringT("__si2drSimpleAttrGetExprValue:"), &err2);
            }
            return 0;
        }
    }
    *err = kSI2DR_NO_ERROR;
    return static_cast<si2drExprT *>(x->value->u.expr_val);
}
si2drVoidT LibAnalysis::__si2drSimpleAttrSetExprValue(si2drAttrIdT attr,
                                                      si2drExprT *expr,
                                                      si2drErrorT *err) {
    timinglib_attribute *x = static_cast<timinglib_attribute *>(attr.v2);
    if (trace_) {
        fprintf(tracefile1_,
                "\n\t__si2drSimpleAttrSetExprValue(%s,%llx,&err);\n\n",
                __oidString(attr).c_str(), reinterpret_cast<uint64_t>(expr));
        __incTracecount();
    }
    if (!timinglib_nocheck_mode_) {
        if ((si2drObjectTypeT)(reinterpret_cast<std::intptr_t>(attr.v1)) !=
            kSI2DR_ATTR) {
            *err = kSI2DR_INVALID_OBJECTTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_OBJECTTYPE,
                    si2drStringT("__si2drSimpleAttrSetExprValue:"), &err2);
            }
            return;
        }
        if (x->type != kTIMINGLIB__SIMPLE) {
            *err = kSI2DR_INVALID_ATTRTYPE;
            if (timinglib_debug_mode_) {
                si2drErrorT err2;
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_INVALID_ATTRTYPE,
                    si2drStringT("__si2drSimpleAttrSetExprValue:"), &err2);
            }
            return;
        }
    }
    if (x->value == static_cast<timinglib_attribute_value *>(NULL)) {
        x->value = static_cast<timinglib_attribute_value *>(
            calloc(sizeof(timinglib_attribute_value), 1));
    }
    x->value->type = kTIMINGLIB__VAL_EXPR;
    x->value->u.expr_val = expr;
    *err = kSI2DR_NO_ERROR;
}
si2drVoidT LibAnalysis::si2drExprDestroy(
    si2drExprT *expr, /* recursively free the structures */
    si2drErrorT *err) {
    *err = kSI2DR_NO_ERROR;
    if (expr->left) si2drExprDestroy(expr->left, err);
    if (expr->right) si2drExprDestroy(expr->right, err);
    expr->left = 0;
    expr->right = 0;
    free(expr);
}
si2drExprT *LibAnalysis::si2drCreateExpr(
    si2drExprTypeT type, /* malloc an Expr and return it */
    si2drErrorT *err) {
    si2drExprT *x;
    *err = kSI2DR_NO_ERROR;
    x = static_cast<si2drExprT *>(calloc(sizeof(si2drExprT), 1));
    x->type = type;
    return x;
}
si2drExprT *LibAnalysis::__si2drCreateBooleanValExpr(si2drBooleanT b,
                                                     si2drErrorT *err) {
    si2drExprT *x;
    *err = kSI2DR_NO_ERROR;
    x = static_cast<si2drExprT *>(calloc(sizeof(si2drExprT), 1));
    x->type = kSI2DR_EXPR_VAL;
    x->valuetype = kSI2DR_BOOLEAN;
    x->u.b = b;
    return x;
}
si2drExprT *LibAnalysis::__si2drCreateDoubleValExpr(si2drFloat64T d,
                                                    si2drErrorT *err) {
    si2drExprT *x;
    *err = kSI2DR_NO_ERROR;
    x = static_cast<si2drExprT *>(calloc(sizeof(si2drExprT), 1));
    x->type = kSI2DR_EXPR_VAL;
    x->valuetype = kSI2DR_FLOAT64;
    x->u.d = d;
    return x;
}
si2drExprT *LibAnalysis::__si2drCreateStringValExpr(si2drStringT s,
                                                    si2drErrorT *err) {
    si2drExprT *x;
    *err = kSI2DR_NO_ERROR;
    x = static_cast<si2drExprT *>(calloc(sizeof(si2drExprT), 1));
    x->type = kSI2DR_EXPR_VAL;
    x->valuetype = kSI2DR_STRING;
    x->u.s = s;
    return x;
}
si2drExprT *LibAnalysis::__si2drCreateIntValExpr(si2drInt32T i,
                                                 si2drErrorT *err) {
    si2drExprT *x;
    *err = kSI2DR_NO_ERROR;
    x = static_cast<si2drExprT *>(calloc(sizeof(si2drExprT), 1));
    x->type = kSI2DR_EXPR_VAL;
    x->valuetype = kSI2DR_INT32;
    x->u.i = i;
    return x;
}
si2drExprT *LibAnalysis::__si2drCreateBinaryOpExpr(si2drExprT *left,
                                                   si2drExprTypeT optype,
                                                   si2drExprT *right,
                                                   si2drErrorT *err) {
    si2drExprT *x;
    *err = kSI2DR_NO_ERROR;
    x = static_cast<si2drExprT *>(calloc(sizeof(si2drExprT), 1));
    x->type = optype;
    x->left = left;
    x->right = right;
    return x;
}
si2drExprT *LibAnalysis::__si2drCreateUnaryOpExpr(si2drExprTypeT optype,
                                                  si2drExprT *expr,
                                                  si2drErrorT *err) {
    si2drExprT *x;
    *err = kSI2DR_NO_ERROR;
    x = static_cast<si2drExprT *>(calloc(sizeof(si2drExprT), 1));
    x->type = optype;
    x->left = expr;
    return x;
}
si2drStringT LibAnalysis::si2drExprToString(si2drExprT *expr,
                                            si2drErrorT *err) {
    *err = kSI2DR_NO_ERROR;
    if (expr->type != kSI2DR_EXPR_VAL && expr->type != kSI2DR_EXPR_OP_ADD &&
        expr->type != kSI2DR_EXPR_OP_SUB && expr->type != kSI2DR_EXPR_OP_MUL &&
        expr->type != kSI2DR_EXPR_OP_DIV &&
        expr->type != kSI2DR_EXPR_OP_PAREN &&
        expr->type != kSI2DR_EXPR_OP_LOG2 &&
        expr->type != kSI2DR_EXPR_OP_LOG10 &&
        expr->type != kSI2DR_EXPR_OP_EXP) {
        *err = kSI2DR_INVALID_OBJECTTYPE;
        return si2drStringT("");
    }
    return __exprString(expr);
}
si2drExprTypeT LibAnalysis::__si2drExprGetType(si2drExprT *expr,
                                               si2drErrorT *err) {
    *err = kSI2DR_NO_ERROR;
    if (!expr) {
        *err = kSI2DR_REFERENCE_ERROR;
        return kSI2DR_EXPR_VAL;
    }
    return expr->type;
}
si2drValueTypeT LibAnalysis::__si2drValExprGetValueType(si2drExprT *expr,
                                                        si2drErrorT *err) {
    *err = kSI2DR_NO_ERROR;
    if (!expr) {
        *err = kSI2DR_REFERENCE_ERROR;
        return kSI2DR_MAX_VALUETYPE;
    }
    if (expr->type != kSI2DR_EXPR_VAL) {
        *err = kSI2DR_INVALID_OBJECTTYPE;
        return static_cast<si2drValueTypeT>(0);
    }
    return expr->valuetype;
}
si2drInt32T LibAnalysis::__si2drIntValExprGetInt(si2drExprT *expr,
                                                 si2drErrorT *err) {
    *err = kSI2DR_NO_ERROR;
    if (!expr) {
        *err = kSI2DR_REFERENCE_ERROR;
        return 0;
    }
    if (expr->type != kSI2DR_EXPR_VAL || expr->valuetype != kSI2DR_INT32) {
        *err = kSI2DR_INVALID_OBJECTTYPE;
        return 0;
    }
    return expr->u.i;
}
si2drFloat64T LibAnalysis::__si2drDoubleValExprGetDouble(si2drExprT *expr,
                                                         si2drErrorT *err) {
    *err = kSI2DR_NO_ERROR;
    if (!expr) {
        *err = kSI2DR_REFERENCE_ERROR;
        return 0.0;
    }
    if (expr->type != kSI2DR_EXPR_VAL || expr->valuetype != kSI2DR_FLOAT64) {
        *err = kSI2DR_INVALID_OBJECTTYPE;
        return 0;
    }
    return expr->u.d;
}
si2drBooleanT LibAnalysis::__si2drBooleanValExprGetBoolean(si2drExprT *expr,
                                                           si2drErrorT *err) {
    *err = kSI2DR_NO_ERROR;
    if (!expr) {
        *err = kSI2DR_REFERENCE_ERROR;
        return static_cast<si2drBooleanT>(0);
    }
    if (expr->type != kSI2DR_EXPR_VAL || expr->valuetype != kSI2DR_BOOLEAN) {
        *err = kSI2DR_INVALID_OBJECTTYPE;
        return static_cast<si2drBooleanT>(0);
    }
    return expr->u.b;
}
si2drStringT LibAnalysis::__si2drStringValExprGetString(si2drExprT *expr,
                                                        si2drErrorT *err) {
    *err = kSI2DR_NO_ERROR;
    if (!expr) {
        *err = kSI2DR_REFERENCE_ERROR;
        return 0;
    }
    if (expr->type != kSI2DR_EXPR_VAL || expr->valuetype != kSI2DR_STRING) {
        *err = kSI2DR_INVALID_OBJECTTYPE;
        return 0;
    }
    return expr->u.s;
}
si2drExprT *LibAnalysis::__si2drOpExprGetLeftExpr(
    si2drExprT *expr, /* will apply to Unary & binary Ops */
    si2drErrorT *err) {
    *err = kSI2DR_NO_ERROR;
    if (!expr) {
        *err = kSI2DR_REFERENCE_ERROR;
        return 0;
    }
    if (expr->type == kSI2DR_EXPR_VAL) {
        *err = kSI2DR_INVALID_OBJECTTYPE;
        return 0;
    }
    return expr->left;
}
si2drExprT *LibAnalysis::__si2drOpExprGetRightExpr(si2drExprT *expr,
                                                   si2drErrorT *err) {
    *err = kSI2DR_NO_ERROR;
    if (!expr) {
        *err = kSI2DR_REFERENCE_ERROR;
        return 0;
    }
    if (expr->type != kSI2DR_EXPR_OP_ADD && expr->type != kSI2DR_EXPR_OP_SUB &&
        expr->type != kSI2DR_EXPR_OP_MUL && expr->type != kSI2DR_EXPR_OP_DIV &&
        expr->type != kSI2DR_EXPR_OP_EXP) {
        *err = kSI2DR_INVALID_OBJECTTYPE;
        return 0;
    }
    return expr->right;
}
/* HELPER FUNCTIONS */
LONG_DOUBLE LibAnalysis::__timinglibGetElement(
    struct timinglib_value_data *vd, ...) { /* returns NaN if bounds exceeded */
    va_list ap;
    int indices[7], i;
    int pos;
    va_start(ap, vd);
    for (i = 0; i < vd->dimensions; i++) {
        indices[i] = va_arg(ap, int);
        if (indices[i] < 0 || indices[i] > vd->dim_sizes[i]) {
#ifndef NO_NANL
            return nanl("");
#else
            return 1.0 / 0.0;
#endif
        }
    }
    va_end(ap);
    pos = 0;
    for (i = 0; i < vd->dimensions; i++) {
        pos = vd->dim_sizes[i] * pos + indices[i];
    }
    return vd->values[pos];
}
void LibAnalysis::__timinglibDestroyValueData(struct timinglib_value_data *vd) {
    int i;
    if (vd->values) free(vd->values);
    vd->values = 0;
    if (vd->dim_sizes) free(vd->dim_sizes);
    vd->dim_sizes = 0;
    for (i = 0; i < vd->dimensions; i++) {
        if (vd->index_info) {
            if (vd->index_info[i]) {
                free(vd->index_info[i]);
                vd->index_info[i] = 0;
            }
        }
    }
    if (vd->index_info) {
        free(vd->index_info);
        vd->index_info = 0;
    }
    vd->dimensions = 0;
    free(vd);
}
void LibAnalysis::__timinglibGetIndexInfo(si2drAttrIdT index_x,
                                          struct timinglib_value_data *vd,
                                          int dimno) {
    si2drErrorT err;
    si2drValuesIdT vals;
    si2drValueTypeT vtype;
    si2drInt32T intgr;
    si2drFloat64T float64;
    si2drStringT string;
    si2drBooleanT boolean;
    si2drExprT *expr;
    char *p, *t = NULL;
    int numcount, i2;
    LONG_DOUBLE x;
    if (!__si2drObjectIsNull(index_x, &err)) {
        /* should be a string with a list of comma/space sep'd numbers */
        if (__si2drAttrGetAttrType(index_x, &err) != kSI2DR_COMPLEX) {
            fprintf(stderr,
                    "The 'index_%d' attribute is not a complex attribute.",
                    dimno + 1);
            return; /* this will already be reported */
        }
        vals = __si2drComplexAttrGetValues(index_x, &err);
        __si2drIterNextComplexValue(vals, &vtype, &intgr, &float64, &string,
                                    &boolean, &expr, &err);
        __si2drIterQuit(vals, &err);
        if (vtype == kSI2DR_UNDEFINED_VALUETYPE) {
            fprintf(stderr, "The 'index_%d' attribute has no values.",
                    dimno + 1);
            return;
        }
        if (vtype != kSI2DR_STRING) {
            if (vtype == kSI2DR_FLOAT64) {
                LONG_DOUBLE *pg;
                numcount = 1;
                vd->index_info[dimno] = static_cast<LONG_DOUBLE *>(
                    calloc(sizeof(LONG_DOUBLE), numcount));
                vd->dim_sizes[dimno] = numcount;
                pg = vd->index_info[dimno];
                pg[0] = float64;
                return;
            }
            fprintf(stderr,
                    "The 'index_%d' attribute should be a string or float.\n",
                    dimno + 1);
            return;
        }
        if (!string || !string[0]) {
            fprintf(stderr, "The 'index_%d' attribute is an empty string.",
                    dimno + 1);
            return;
        }
        p = string;
        numcount = 0;
        while ((x = (LONG_DOUBLE)strtod(p, &t)), t != p) {
            numcount++;
            if (t && *t == 0) break;
            /* skip over intervening stuff to the next number */
            while (t && (*t == ',' || *t == ' ' || *t == '\t' || *t == '\n' ||
                         *t == '\r' || *t == '\\')) {
                if (*t == '\\' && *(t + 1) == '\n') /*skip over line escapes */
                    t++;
                t++;
            }
            p = t;
        }
        if (numcount) {
            vd->index_info[dimno] = static_cast<LONG_DOUBLE *>(
                calloc(sizeof(LONG_DOUBLE), numcount));
            vd->dim_sizes[dimno] = numcount;
            p = string;
            i2 = 0;
            while ((x = strtod(p, &t)), t != p) {
                LONG_DOUBLE *pg = vd->index_info[dimno];
                pg[i2] = x;
                if (t && *t == 0) break;
                i2++;
                /* skip over intervening stuff to the next number */
                while (*t == ',' || *t == ' ' || *t == '\t' || *t == '\n' ||
                       *t == '\r' || *t == '\\') {
                    if (*t == '\\' &&
                        *(t + 1) == '\n') /*skip over line escapes */
                        t++;
                    t++;
                }
                p = t;
            }
        }
    }
}
si2drGroupIdT LibAnalysis::__getContainingGroup(si2drGroupIdT group,
                                                si2drStringT type) {
    si2drErrorT err;
    const struct libGroupMap *lgm;
    si2drGroupIdT gup = group;
    group_enum gt = group_enum(0);
    lgm = lookup_group_name(type, strlen(type));
    if (lgm) {
        gt = lgm->type;
    } else {
        fprintf(stderr, "BAD type (%s) handed to __get_containing_group!",
                type);
    }
    while (
        !__si2drObjectIsNull((gup = __si2drObjectGetOwner(gup, &err)), &err)) {
        if (__si2drGroupGetID(gup, &err) == gt) return gup;
    }
    return __si2drPIGetNullId(&err);
}
struct timinglib_value_data *LibAnalysis::__timinglibGetValuesData(
    si2drGroupIdT table_group) {
    si2drErrorT err;
    si2drNamesIdT names;
    group_enum thistype = __si2drGroupGetID(table_group, &err);
    si2drAttrIdT index_1 =
        __si2drGroupFindAttrByName(table_group, si2drStringT("index_1"), &err);
    si2drAttrIdT index_2 =
        __si2drGroupFindAttrByName(table_group, si2drStringT("index_2"), &err);
    si2drAttrIdT index_3 =
        __si2drGroupFindAttrByName(table_group, si2drStringT("index_3"), &err);
    si2drAttrIdT index_4 =
        __si2drGroupFindAttrByName(table_group, si2drStringT("index_4"), &err);
    si2drAttrIdT index_5 =
        __si2drGroupFindAttrByName(table_group, si2drStringT("index_5"), &err);
    si2drAttrIdT index_6 =
        __si2drGroupFindAttrByName(table_group, si2drStringT("index_6"), &err);
    si2drAttrIdT index_7 =
        __si2drGroupFindAttrByName(table_group, si2drStringT("index_7"), &err);
    si2drAttrIdT valuesd =
        __si2drGroupFindAttrByName(table_group, si2drStringT("values"), &err);
    si2drGroupIdT libr, libtemplate;
    si2drValuesIdT vals;
    si2drValueTypeT vtype;
    si2drInt32T intgr;
    si2drFloat64T float64;
    si2drStringT string;
    si2drBooleanT boolean;
    si2drExprT *expr;
    char *p, *t;
    LONG_DOUBLE x;
    struct timinglib_value_data *vd =
        static_cast<struct timinglib_value_data *>(
            calloc(sizeof(struct timinglib_value_data), 1));
    int numels;
    si2drStringT refname;
    /* find the template, fill in any missing index vals! */
    libr = __getContainingGroup(table_group, si2drStringT("library"));
    names = __si2drGroupGetNames(table_group, &err);
    refname = __si2drIterNextName(names, &err);
    __si2drIterQuit(names, &err);
    if (!refname || !strcasecmp(refname, "scalar")) {
        /* scalar is a brand name for a single value */
        numels = 1;
        vd->dimensions = 1;
        vd->dim_sizes = static_cast<int *>(calloc(sizeof(int), vd->dimensions));
        vd->dim_sizes[0] = 1;
        vd->index_info = static_cast<LONG_DOUBLE **>(
            calloc(sizeof(LONG_DOUBLE *), vd->dimensions));
        vd->index_info[0] =
            static_cast<LONG_DOUBLE *>(calloc(sizeof(LONG_DOUBLE), 1));
        vd->index_info[0][0] = 0.000;
    } else {
        switch (thistype) {
            case kTIMINGLIB_GROUPENUM_cell_fall:
            case kTIMINGLIB_GROUPENUM_cell_rise:
            case kTIMINGLIB_GROUPENUM_rise_transition:
            case kTIMINGLIB_GROUPENUM_fall_transition:
            case kTIMINGLIB_GROUPENUM_retaining_fall:
            case kTIMINGLIB_GROUPENUM_retaining_rise:
            case kTIMINGLIB_GROUPENUM_receiver_capacitance1_fall:
            case kTIMINGLIB_GROUPENUM_receiver_capacitance1_rise:
            case kTIMINGLIB_GROUPENUM_receiver_capacitance2_fall:
            case kTIMINGLIB_GROUPENUM_receiver_capacitance2_rise:
            case kTIMINGLIB_GROUPENUM_va_receiver_capacitance2_rise:
            case kTIMINGLIB_GROUPENUM_va_receiver_capacitance2_fall:
            case kTIMINGLIB_GROUPENUM_va_receiver_capacitance1_rise:
            case kTIMINGLIB_GROUPENUM_va_receiver_capacitance1_fall:
            case kTIMINGLIB_GROUPENUM_va_fall_constraint:
            case kTIMINGLIB_GROUPENUM_va_rise_constraint:
            case kTIMINGLIB_GROUPENUM_rise_constraint:
            case kTIMINGLIB_GROUPENUM_fall_constraint:
            case kTIMINGLIB_GROUPENUM_dc_current:
                libtemplate = __si2drGroupFindGroupByName(
                    libr, refname, si2drStringT("lu_table_template"), &err);
                if (__si2drObjectIsNull(libtemplate, &err))
                    libtemplate = __si2drGroupFindGroupByName(
                        libr, refname, si2drStringT("poly_template"), &err);
                break;
            case kTIMINGLIB_GROUPENUM_fall_power:
            case kTIMINGLIB_GROUPENUM_rise_power:
            case kTIMINGLIB_GROUPENUM_internal_power:
            case kTIMINGLIB_GROUPENUM_power:
                libtemplate = __si2drGroupFindGroupByName(
                    libr, refname, si2drStringT("power_lut_template"), &err);
                if (__si2drObjectIsNull(libtemplate, &err))
                    libtemplate = __si2drGroupFindGroupByName(
                        libr, refname, si2drStringT("power_poly_template"),
                        &err);
                break;
            case kTIMINGLIB_GROUPENUM_noise_immunity_above_high:
            case kTIMINGLIB_GROUPENUM_noise_immunity_below_low:
            case kTIMINGLIB_GROUPENUM_noise_immunity_high:
            case kTIMINGLIB_GROUPENUM_noise_immunity_low:
                libtemplate = __si2drGroupFindGroupByName(
                    libr, refname, si2drStringT("noise_lut_template"), &err);
                if (__si2drObjectIsNull(libtemplate, &err))
                    libtemplate = __si2drGroupFindGroupByName(
                        libr, refname, si2drStringT("poly_template"), &err);
                break;
            case kTIMINGLIB_GROUPENUM_propagated_noise_height_below_low:
            case kTIMINGLIB_GROUPENUM_propagated_noise_height_above_high:
            case kTIMINGLIB_GROUPENUM_propagated_noise_height_high:
            case kTIMINGLIB_GROUPENUM_propagated_noise_height_low:
            case kTIMINGLIB_GROUPENUM_propagated_noise_peak_time_ratio_above_high:
            case kTIMINGLIB_GROUPENUM_propagated_noise_peak_time_ratio_below_low:
            case kTIMINGLIB_GROUPENUM_propagated_noise_peak_time_ratio_high:
            case kTIMINGLIB_GROUPENUM_propagated_noise_peak_time_ratio_low:
            case kTIMINGLIB_GROUPENUM_propagated_noise_width_above_high:
            case kTIMINGLIB_GROUPENUM_propagated_noise_width_below_low:
            case kTIMINGLIB_GROUPENUM_propagated_noise_width_high:
            case kTIMINGLIB_GROUPENUM_propagated_noise_width_low:
                libtemplate = __si2drGroupFindGroupByName(
                    libr, refname, si2drStringT("propagation_lut_template"),
                    &err);
                if (__si2drObjectIsNull(libtemplate, &err))
                    libtemplate = __si2drGroupFindGroupByName(
                        libr, refname, si2drStringT("poly_template"), &err);
                break;
            case kTIMINGLIB_GROUPENUM_steady_state_current_high:
            case kTIMINGLIB_GROUPENUM_steady_state_current_low:
                libtemplate = __si2drGroupFindGroupByName(
                    libr, refname, si2drStringT("iv_lut_template"), &err);
                if (__si2drObjectIsNull(libtemplate, &err))
                    libtemplate = __si2drGroupFindGroupByName(
                        libr, refname, si2drStringT("poly_template"), &err);
                break;
            case kTIMINGLIB_GROUPENUM_vector:
                libtemplate = __si2drGroupFindGroupByName(
                    libr, refname, si2drStringT("output_current_template"),
                    &err);
                /* Look for lu_table_template, used by CCS noise */
                if (__si2drObjectIsNull(libtemplate, &err))
                    libtemplate = __si2drGroupFindGroupByName(
                        libr, refname, si2drStringT("lu_table_template"), &err);
                break;
            case kTIMINGLIB_GROUPENUM_em_max_toggle_rate:
                libtemplate = __si2drGroupFindGroupByName(
                    libr, refname, si2drStringT("em_lut_template"), &err);
                break;
            case kTIMINGLIB_GROUPENUM_max_cap:
                libtemplate = __si2drGroupFindGroupByName(
                    libr, refname, si2drStringT("maxcap_lut_template"), &err);
                break;
            case kTIMINGLIB_GROUPENUM_max_trans:
                libtemplate = __si2drGroupFindGroupByName(
                    libr, refname, si2drStringT("maxtrans_lut_template"), &err);
                break;
            default:
                break;
        }
        if (__si2drObjectIsNull(libtemplate, &err)) {
            fprintf(
                stderr,
                "Could not find the template '%s' for the table '%s'. Sorry!\n",
                refname, __si2drGroupGetGroupType(table_group, &err));
        }
        /* fill in any missing index values from the template */
        if (__si2drObjectIsNull(index_1, &err))
            index_1 = __si2drGroupFindAttrByName(libtemplate,
                                                 si2drStringT("index_1"), &err);
        if (__si2drObjectIsNull(index_2, &err))
            index_2 = __si2drGroupFindAttrByName(libtemplate,
                                                 si2drStringT("index_2"), &err);
        if (__si2drObjectIsNull(index_3, &err))
            index_3 = __si2drGroupFindAttrByName(libtemplate,
                                                 si2drStringT("index_3"), &err);
        if (__si2drObjectIsNull(index_4, &err))
            index_4 = __si2drGroupFindAttrByName(libtemplate,
                                                 si2drStringT("index_4"), &err);
        if (__si2drObjectIsNull(index_5, &err))
            index_5 = __si2drGroupFindAttrByName(libtemplate,
                                                 si2drStringT("index_5"), &err);
        if (__si2drObjectIsNull(index_6, &err))
            index_6 = __si2drGroupFindAttrByName(libtemplate,
                                                 si2drStringT("index_6"), &err);
        if (__si2drObjectIsNull(index_7, &err))
            index_7 = __si2drGroupFindAttrByName(libtemplate,
                                                 si2drStringT("index_7"), &err);
        /* find the number of dims */
        if (!__si2drObjectIsNull(index_1, &err)) {
            vd->dimensions = 1;
            if (!__si2drObjectIsNull(index_2, &err)) {
                vd->dimensions = 2;
                if (!__si2drObjectIsNull(index_3, &err)) {
                    vd->dimensions = 3;
                    if (!__si2drObjectIsNull(index_4, &err)) {
                        vd->dimensions = 4;
                        if (!__si2drObjectIsNull(index_5, &err)) {
                            vd->dimensions = 5;
                            if (!__si2drObjectIsNull(index_6, &err)) {
                                vd->dimensions = 6;
                                if (!__si2drObjectIsNull(index_7, &err)) {
                                    vd->dimensions = 7;
                                }
                            }
                        }
                    }
                }
            }
        }
        vd->dim_sizes = static_cast<int *>(calloc(sizeof(int), vd->dimensions));
        vd->index_info = static_cast<LONG_DOUBLE **>(
            calloc(sizeof(LONG_DOUBLE *), vd->dimensions));
        /* now, find and set the sizes of each dim */
        if (!__si2drObjectIsNull(index_1, &err)) {
            __timinglibGetIndexInfo(index_1, vd, 0);
            if (!__si2drObjectIsNull(index_2, &err)) {
                __timinglibGetIndexInfo(index_2, vd, 1);
                if (!__si2drObjectIsNull(index_3, &err)) {
                    __timinglibGetIndexInfo(index_3, vd, 2);
                    if (!__si2drObjectIsNull(index_4, &err)) {
                        __timinglibGetIndexInfo(index_4, vd, 3);
                        if (!__si2drObjectIsNull(index_5, &err)) {
                            __timinglibGetIndexInfo(index_5, vd, 4);
                            if (!__si2drObjectIsNull(index_6, &err)) {
                                __timinglibGetIndexInfo(index_6, vd, 5);
                                if (!__si2drObjectIsNull(index_7, &err)) {
                                    __timinglibGetIndexInfo(index_7, vd, 6);
                                }
                            }
                        }
                    }
                }
            }
        }
        /* now, malloc and fill the values */
        numels = 1;
        if (!__si2drObjectIsNull(index_1, &err)) {
            numels *= vd->dim_sizes[0];
            if (!__si2drObjectIsNull(index_2, &err)) {
                numels *= vd->dim_sizes[1];
                if (!__si2drObjectIsNull(index_3, &err)) {
                    numels *= vd->dim_sizes[2];
                    if (!__si2drObjectIsNull(index_4, &err)) {
                        numels *= vd->dim_sizes[3];
                        if (!__si2drObjectIsNull(index_5, &err)) {
                            numels *= vd->dim_sizes[4];
                            if (!__si2drObjectIsNull(index_6, &err)) {
                                numels *= vd->dim_sizes[5];
                                if (!__si2drObjectIsNull(index_7, &err)) {
                                    numels *= vd->dim_sizes[6];
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    /* Now, fill in the values array from the values attr */
    vd->values =
        static_cast<LONG_DOUBLE *>(calloc(sizeof(LONG_DOUBLE), numels));
    if (!__si2drObjectIsNull(valuesd, &err)) {
        int totalcount = 0;
        int numcount;
        int first = 1;
        /* should be a string with a list of comma/space sep'd numbers */
        if (__si2drAttrGetAttrType(valuesd, &err) != kSI2DR_COMPLEX) {
            fprintf(stderr,
                    "The 'values' attribute is not a complex attribute.");
            return vd; /* this will already be reported */
        }
        vals = __si2drComplexAttrGetValues(valuesd, &err);
        do {
            __si2drIterNextComplexValue(vals, &vtype, &intgr, &float64, &string,
                                        &boolean, &expr, &err);
            if (first && vtype == kSI2DR_UNDEFINED_VALUETYPE) {
                fprintf(stderr, "The 'values' attribute has no values.");
                break;
            }
            if (vtype == kSI2DR_UNDEFINED_VALUETYPE) break;
            first = 0;
            if (vtype != kSI2DR_STRING) {
                fprintf(stderr, "The 'values' attribute should be a string.");
                break;
            }
            if (!string || !string[0]) {
                fprintf(stderr, "The 'values' attribute is an empty string.");
                break;
            }
            p = string;
            numcount = 0;
            while ((x = strtod(p, &t)), t != p) {
                vd->values[totalcount + numcount] = x;
                numcount++;
                if (t && *t == 0) break;
                /* skip over intervening stuff to the next number */
                while (*t == ',' || *t == ' ' || *t == '\t' || *t == '\n' ||
                       *t == '\r' || *t == '\\') {
                    if (*t == '\\' &&
                        *(t + 1) == '\n') /*skip over line escapes */
                        t++;
                    t++;
                }
                p = t;
            }
            totalcount += numcount;
        } while (vtype != kSI2DR_UNDEFINED_VALUETYPE);
        __si2drIterQuit(vals, &err);
        if (totalcount != numels) {
            fprintf(
                stderr,
                "The table 'values' attribute has %d vs. %d expected values\n",
                totalcount, numels);
        }
    } else {
        fprintf(stderr, "The group contains no 'values' attribute.");
    }
    return vd;
}
int LibAnalysis::__isaFormula(char *str) {
    /* a crude approximation at a formula-- the real best solution here would be
   to
       have a general parser for arithmetic expressions; constant arithmetic
   expressions? */
    char *p = str;
    while (*p) {
        if ((*p >= '0' && *p <= '9') || *p == ' ' || *p == '\t' || *p == '*' ||
            *p == '/' || *p == '+' || *p == '-' || *p == 'e' || *p == 'E' ||
            *p == '.')
            p++;
        else if (*p == 'V' && *(p + 1) == 'D' && *(p + 2) == 'D')
            p += 3;
        else
            break;
    }
    if (*p)
        return 0;
    else
        return 1;
}
int LibAnalysis::__numGetPrecision(char *numstr) {
    int digs = 0;
    char *p = numstr;
    while (p && *p) { /* space out to first number */
        if (*p > '0' && *p <= '9') break;
        /* this should skip over non numeric chars,
               '+', '-', '.', '0', etc, until the first num is seen */
        p++;
    }
    while (p && *p) {
        if (*p == '.') {
            p++;
        } else if (*p >= '0' && *p <= '9') {
            digs++;
            p++;
        } else {
            break;
        }
    }
    return digs;
}
void LibAnalysis::__addVarlist(char *name) {
    int found = 0;
    var_list *p;
    for (p = master_var_list_; p; p = p->next) {
        if (!strcmp(name, p->name)) {
            found = 1;
            break;
        }
    }
    if (!found) {
        p = static_cast<var_list *>(calloc(sizeof(var_list), 1));
        p->name = static_cast<char *>(malloc(strlen(name) + 1));
        snprintf(p->name, strlen(name) + 1, "%s", name);
        p->next = master_var_list_;
        master_var_list_ = p;
    }
}
void LibAnalysis::__destroyVarlist(void) {
    var_list *p, *p1;
    for (p = master_var_list_; p; p = p1) {
        if (p->name) free(p->name);
        p->name = 0;
        p1 = p->next;
        free(p);
    }
    master_var_list_ = 0;
}
void LibAnalysis::__printVarList(void) {
    var_list *p;
    si2drErrorT err;
    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_NOTE, kSI2DR_NO_ERROR,
                        si2drStringT("---- var list:"), &err);
    for (p = master_var_list_; p; p = p->next) {
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_NOTE, kSI2DR_NO_ERROR, p->name,
                            &err);
    }
    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_NOTE, kSI2DR_NO_ERROR,
                        si2drStringT("----"), &err);
}
si2drErrorT LibAnalysis::__syntaxCheck(si2drGroupIdT lib) {
    // libsynt_technology (&libsynt_techs)[20] = sd_->libsynt_techs;
    // int libsynt_tech_num = sd_->libsynt_tech_count;
    extern libsynt_technology libsynt_techs[];
    extern int libsynt_tech_num;
    si2drAttrIdT attr;
    si2drStringT gtype;
    si2drErrorT err;
    si2drValuesIdT vals;
    si2drValueTypeT vtype;
    si2drInt32T intgr;
    si2drFloat64T float64;
    si2drStringT string;
    si2drExprT *expr;
    si2drBooleanT boolean;
    int found, first, i, dmln, tcln;
    char model[100], tech[100];
    si2drStringT tcfn;
    si2drStringT dmfn;
    si2drAttrTypeT atype;
    errcount_ = 0;
    /* one thing upon which all others hinge!
       First: this top group must be a library.
       We'll quick scan for tech and model...
    */
    si2ErrMsg_ =
        si2drPIGetMessageHandler(&err); /* the printer is in another file! */
    gtype = __si2drGroupGetGroupType(lib, &err);
    if (strcmp(gtype, "library")) {
        (this->*si2ErrMsg_)(
            kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR,
            si2drStringT(
                "The top level group is not of type 'library'! This error is "
                "so foul, so heinous, so serious, \nso major, so fatal, that "
                "syntax/semantic checking is terminating!"),
            &err);
        return kSI2DR_SYNTAX_ERROR;
    }
    attr = __si2drGroupFindAttrByName(lib, si2drStringT("technology"), &err);
    if (!__si2drObjectIsNull(attr, &err)) {
        atype = __si2drAttrGetAttrType(attr, &err);
        tcln = __si2drObjectGetLineNo(attr, &err);
        tcfn = __si2drObjectGetFileName(attr, &err);
        if (atype != kSI2DR_COMPLEX) {
            char EB[8000];
            snprintf(EB, sizeof(EB),
                     "The technology attribute specified at %s:%d is a simple "
                     "attribute and should be a complex attribute!",
                     __si2drObjectGetFileName(attr, &err),
                     static_cast<int>(__si2drObjectGetLineNo(attr, &err)));
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR, EB,
                                &err);
            errcount_++;
            vtype = __si2drSimpleAttrGetValueType(attr, &err);
            switch (vtype) {
                case kSI2DR_STRING:
                    snprintf(tech, sizeof(tech), "%s",
                             __si2drSimpleAttrGetStringValue(attr, &err));
                    break;
                case kSI2DR_FLOAT64:
                case kSI2DR_INT32:
                case kSI2DR_BOOLEAN:
                case kSI2DR_EXPR:
                    errcount_++;
                    char EB[8000];
                    snprintf(EB, sizeof(EB),
                             "Not only is the technology attribute type wrong, "
                             "so is the value type! It should be a string, but "
                             "there's a value of some other type here!");
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SYNTAX_ERROR, EB, &err);
                    break;
                default:
                    break;
            }
        } else {
            found = 0;
            first = 1;
            vals = __si2drComplexAttrGetValues(attr, &err);
            do {
                __si2drIterNextComplexValue(vals, &vtype, &intgr, &float64,
                                            &string, &boolean, &expr, &err);
                switch (vtype) {
                    case kSI2DR_STRING:
                        snprintf(tech, sizeof(tech), "%s", string);
                        if (first) {
                            first = 0;
                        } else {
                            char EB[8000];
                            snprintf(
                                EB, sizeof(EB),
                                "Found a string argument, but it was not the "
                                "first and only argument of technology!");
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SYNTAX_ERROR, EB, &err);
                            errcount_++;
                        }
                        found = 1;
                        break;
                    case kSI2DR_FLOAT64:
                    case kSI2DR_INT32:
                    case kSI2DR_BOOLEAN:
                        char EB[8000];
                        snprintf(
                            EB, sizeof(EB),
                            "The technology attribute at %s:%d should only "
                            "have one string value; More than one, or "
                            "non-string attributes detected!",
                            __si2drObjectGetFileName(attr, &err),
                            static_cast<int>(
                                __si2drObjectGetLineNo(attr, &err)));
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SYNTAX_ERROR, EB, &err);
                        errcount_++;
                        break;
                    default:
                        break;
                }
            } while (vtype != kSI2DR_UNDEFINED_VALUETYPE);
            __si2drIterQuit(vals, &err);
            if (!found) {
                char EB[8000];
                snprintf(EB, sizeof(EB),
                         "The technology attribute at %s:%d should only have "
                         "one string value; No string value was  detected!",
                         __si2drObjectGetFileName(attr, &err),
                         static_cast<int>(__si2drObjectGetLineNo(attr, &err)));
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR,
                                    EB, &err);
                errcount_++;
            } else {
                char EB[8000];
                snprintf(EB, sizeof(EB), "The technology %s was specified.",
                         tech);
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_NOTE, kSI2DR_NO_ERROR, EB,
                                    &err);
            }
        }
    } else {
        char EB[8000];
        snprintf(EB, sizeof(EB),
                 "No technology attribute specified in the library; the "
                 "technology 'cmos' is assumed.");
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_NOTE, kSI2DR_NO_ERROR, EB, &err);
        snprintf(tech, sizeof(tech), "%s", "cmos");
        tcln = -1;
    }
    attr = __si2drGroupFindAttrByName(lib, si2drStringT("delay_model"), &err);
    if (!__si2drObjectIsNull(attr, &err)) {
        si2drExprT *e;
        dmln = __si2drObjectGetLineNo(attr, &err);
        dmfn = __si2drObjectGetFileName(attr, &err);
        atype = __si2drAttrGetAttrType(attr, &err);
        if (atype != kSI2DR_SIMPLE) {
            char EB[8000];
            snprintf(
                EB, sizeof(EB),
                "The delay_model attribute was not a simple attribute. "
                "Ignoring the specification. 'generic_cmos' will be assumed.");
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_WARN, kSI2DR_NO_ERROR, EB,
                                &err);
            snprintf(model, sizeof(model), "%s", "generic_cmos");
            errcount_++;
        } else {
            vtype = __si2drSimpleAttrGetValueType(attr, &err);
            switch (vtype) {
                case kSI2DR_STRING: {
                    snprintf(model, sizeof(model), "%s",
                             __si2drSimpleAttrGetStringValue(attr, &err));
                    char EB[8000];
                    snprintf(EB, sizeof(EB),
                             "The delay_model specified was %s.", model);
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_NOTE, kSI2DR_NO_ERROR,
                                        EB, &err);
                } break;
                case kSI2DR_FLOAT64:
                case kSI2DR_INT32:
                case kSI2DR_BOOLEAN: {
                    char EB[8000];
                    snprintf(
                        EB, sizeof(EB),
                        "The delay_model attribute at %s:%d was not a "
                        "simple attribute of string type. 'generic_cmos' "
                        "will be assumed.",
                        __si2drObjectGetFileName(attr, &err),
                        static_cast<int>(__si2drObjectGetLineNo(attr, &err)));
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_WARN, kSI2DR_NO_ERROR,
                                        EB, &err);
                    errcount_++;
                    snprintf(model, sizeof(model), "%s", "generic_cmos");
                } break;
                case kSI2DR_EXPR:
                    e = __si2drSimpleAttrGetExprValue(attr, &err);
                    if (e->type == kSI2DR_EXPR_VAL &&
                        e->valuetype == kSI2DR_STRING) {
                        snprintf(model, sizeof(model), "%s", e->u.s);
                        char EB[8000];
                        snprintf(EB, sizeof(EB),
                                 "The delay_model specified was %s.", model);
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_NOTE,
                                            kSI2DR_NO_ERROR, EB, &err);
                    } else {
                        char EB[8000];
                        snprintf(EB, sizeof(EB),
                                 "The delay_model attribute at %s:%d was not a "
                                 "simple attribute of string type. "
                                 "'generic_cmos' will be assumed.",
                                 __si2drObjectGetFileName(attr, &err),
                                 static_cast<int>(
                                     __si2drObjectGetLineNo(attr, &err)));
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_WARN,
                                            kSI2DR_NO_ERROR, EB, &err);
                        errcount_++;
                        snprintf(model, sizeof(model), "%s", "generic_cmos");
                    }
                    break;
                default:
                    break;
            }
        }
    } else {
        (this->*si2ErrMsg_)(
            kSI2DR_SEVERITY_NOTE, kSI2DR_NO_ERROR,
            si2drStringT("No delay_model attribute specified in the library; "
                         "the delay_model 'generic_cmos' is assumed."),
            &err);
        snprintf(model, sizeof(model), "%s", "generic_cmos");
        dmln = -1;
    }
    /* right at this moment, I need to come up with a better convention for
     * naming the desc files.... */
    /* find the cmos/cmos model for now */
    found = 0;
    if (dmln > 0 && tcln > 0 && dmln < tcln) {
        char EB[8000];
        snprintf(EB, sizeof(EB),
                 "The delay_model should be declared after the technology, but "
                 "is not. Check lines %s:%d and %s:%d.",
                 tcfn, tcln, dmfn, dmln);
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR, EB, &err);
        errcount_++;
    }
    if (!strcmp(tech, "cmos")) {
        for (i = 0; i < libsynt_tech_num; i++) {
            if (!strcmp(libsynt_techs[i].name, model)) {
                found = 1;
                break;
            }
        }
    }
    if (!found) {
        for (i = 0; i < libsynt_tech_num; i++) {
            if (!strcmp(libsynt_techs[i].name, tech)) {
                found = 1;
                break;
            }
        }
    }
    if (!found) {
        (this->*si2ErrMsg_)(
            kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR,
            si2drStringT("I cannot check the syntax of this particular "
                         "technology/delay_model combination!"),
            &err);
        (this->*si2ErrMsg_)(
            kSI2DR_SEVERITY_NOTE, kSI2DR_NO_ERROR,
            si2drStringT(
                " Here is a list of technologies/delay_models I can handle:"),
            &err);
        for (i = 0; i < libsynt_tech_num; i++) {
            char EB[8000];
            snprintf(EB, sizeof(EB), "  %s", libsynt_techs[i].name);
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_NOTE, kSI2DR_NO_ERROR, EB,
                                &err);
        }
        return kSI2DR_SYNTAX_ERROR;
    }
    char EB[8000];
    snprintf(EB, sizeof(EB), "Using the %s syntax tables...",
             libsynt_techs[i].name);
    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_NOTE, kSI2DR_NO_ERROR, EB, &err);
    __checkGroupCorrespondence(lib, libsynt_techs[i].lib);
    if (errcount_ > 0)
        return kSI2DR_SYNTAX_ERROR;
    else
        return kSI2DR_NO_ERROR;
}
void LibAnalysis::__enterDefineCellArea(si2drAttrIdT a) {
    si2drErrorT err;
    si2drValuesIdT vals;
    si2drValueTypeT vtype;
    si2drInt32T intgr;
    si2drFloat64T float64;
    si2drStringT string;
    si2drExprT *expr;
    si2drBooleanT boolean;
    /* the object here is to add a define to the database;
       but in such a way so it will not show up if we print the database! */
    /* first, get the first argument */
    /* should be a string with a list of comma/space sep'd numbers */
    if (__si2drAttrGetAttrType(a, &err) != kSI2DR_COMPLEX) {
        return; /* this will already be reported */
    }
    vals = __si2drComplexAttrGetValues(a, &err);
    __si2drIterNextComplexValue(vals, &vtype, &intgr, &float64, &string,
                                &boolean, &expr, &err);
    __si2drIterQuit(vals, &err);
    if (vtype == kSI2DR_UNDEFINED_VALUETYPE) {
        char EB[8000];
        snprintf(EB, sizeof(EB),
                 "%s:%d: The 'define_cell_area' attribute has no values.",
                 __si2drObjectGetFileName(a, &err),
                 static_cast<int>(__si2drObjectGetLineNo(a, &err)));
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR, EB, &err);
        errcount_++;
        return;
    }
    if (vtype != kSI2DR_STRING) {
        char EB[8000];
        snprintf(EB, sizeof(EB),
                 "%s:%d: The 'define_cell_area' attribute should be a string.",
                 __si2drObjectGetFileName(a, &err),
                 static_cast<int>(__si2drObjectGetLineNo(a, &err)));
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR, EB, &err);
        errcount_++;
        return;
    }
    if (!string || !string[0]) {
        char EB[8000];
        snprintf(EB, sizeof(EB),
                 "%s:%d: The 'define_cell_area' attribute is an empty string.",
                 __si2drObjectGetFileName(a, &err),
                 static_cast<int>(__si2drObjectGetLineNo(a, &err)));
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR, EB, &err);
        errcount_++;
        return;
    }
    __createFloatingDefineForCellArea(string);
}
void LibAnalysis::__attributeSpecificChecks(si2drAttrIdT a) {
    si2drErrorT err;
    si2drStringT name = __si2drAttrGetName(a, &err);
    const struct libAttrMap *x = lookup_attr_name(name, strlen(name));
    attr_enum attrtype = x->type;
    switch (attrtype) {
        case kTIMINGLIB_ATTRENUM_input_pins:
            break;
        case kTIMINGLIB_ATTRENUM_fpga_timing_type:
            break;
        case kTIMINGLIB_ATTRENUM_fpga_degenerate_output:
            break;
        case kTIMINGLIB_ATTRENUM_xnf_schnm:
            break;
        case kTIMINGLIB_ATTRENUM_xnf_device_name:
            break;
        case kTIMINGLIB_ATTRENUM_fpga_min_degen_input_size:
            break;
        case kTIMINGLIB_ATTRENUM_fpga_max_degen_input_size:
            break;
        case kTIMINGLIB_ATTRENUM_fpga_lut_output:
            break;
        case kTIMINGLIB_ATTRENUM_fpga_lut_insert_before_sequential:
            break;
        case kTIMINGLIB_ATTRENUM_fpga_family:
            break;
        case kTIMINGLIB_ATTRENUM_fpga_complex_degenerate:
            break;
        case kTIMINGLIB_ATTRENUM_fpga_cell_type:
            break;
        case kTIMINGLIB_ATTRENUM_fpga_bridge_inputs:
            break;
        case kTIMINGLIB_ATTRENUM_fpga_prefer_undegenerated_gates:
            break;
        case kTIMINGLIB_ATTRENUM_fpga_allow_duplicate_degenerated_gates:
            break;
        case kTIMINGLIB_ATTRENUM_force_inverter_removal:
            break;
        case kTIMINGLIB_ATTRENUM_fanout_resistance:
            break;
        case kTIMINGLIB_ATTRENUM_fanout_capacitance:
            break;
        case kTIMINGLIB_ATTRENUM_fanout_area:
            break;
        case kTIMINGLIB_ATTRENUM_wire_load_from_area:
            break;
        case kTIMINGLIB_ATTRENUM_fanout_length:
            break;
        case kTIMINGLIB_ATTRENUM_slope:
            break;
        case kTIMINGLIB_ATTRENUM_resistance:
            break;
        case kTIMINGLIB_ATTRENUM_slower_factor:
            break;
        case kTIMINGLIB_ATTRENUM_faster_factor:
            break;
        case kTIMINGLIB_ATTRENUM_default_power_rail:
            break;
        case kTIMINGLIB_ATTRENUM_vomin:
            break;
        case kTIMINGLIB_ATTRENUM_vomax:
            break;
        case kTIMINGLIB_ATTRENUM_vol:
            break;
        case kTIMINGLIB_ATTRENUM_voh:
            break;
        case kTIMINGLIB_ATTRENUM_power_rail:
            break;
        case kTIMINGLIB_ATTRENUM_voltage:
            break;
        case kTIMINGLIB_ATTRENUM_tree_type:
            break;
        case kTIMINGLIB_ATTRENUM_temperature:
            break;
        case kTIMINGLIB_ATTRENUM_process:
            break;
        case kTIMINGLIB_ATTRENUM_short:
            break;
        case kTIMINGLIB_ATTRENUM_cell_name:
            break;
        case kTIMINGLIB_ATTRENUM_variable_3:
            break;
        case kTIMINGLIB_ATTRENUM_vimin:
            break;
        case kTIMINGLIB_ATTRENUM_vimax:
            break;
        case kTIMINGLIB_ATTRENUM_vil:
            break;
        case kTIMINGLIB_ATTRENUM_vih:
            break;
        case kTIMINGLIB_ATTRENUM_variable_2:
            break;
        case kTIMINGLIB_ATTRENUM_variable_1:
            break;
        case kTIMINGLIB_ATTRENUM_downto:
            break;
        case kTIMINGLIB_ATTRENUM_data_type:
            break;
        case kTIMINGLIB_ATTRENUM_bit_width:
            break;
        case kTIMINGLIB_ATTRENUM_bit_to:
            break;
        case kTIMINGLIB_ATTRENUM_bit_from:
            break;
        case kTIMINGLIB_ATTRENUM_base_type:
            break;
        case kTIMINGLIB_ATTRENUM_table:
            break;
        case kTIMINGLIB_ATTRENUM_force_11:
            break;
        case kTIMINGLIB_ATTRENUM_force_10:
            break;
        case kTIMINGLIB_ATTRENUM_force_01:
            break;
        case kTIMINGLIB_ATTRENUM_force_00:
            break;
        case kTIMINGLIB_ATTRENUM_tracks:
            break;
        case kTIMINGLIB_ATTRENUM_total_track_area:
            break;
        case kTIMINGLIB_ATTRENUM_word_width:
            break;
        case kTIMINGLIB_ATTRENUM_type:
            break;
        case kTIMINGLIB_ATTRENUM_row_address:
            break;
        case kTIMINGLIB_ATTRENUM_column_address:
            break;
        case kTIMINGLIB_ATTRENUM_address_width:
            break;
        case kTIMINGLIB_ATTRENUM_value:
            break;
        case kTIMINGLIB_ATTRENUM_enable_also:
            break;
        case kTIMINGLIB_ATTRENUM_data_in:
            break;
        case kTIMINGLIB_ATTRENUM_shifts:
            break;
        case kTIMINGLIB_ATTRENUM_edges:
            break;
        case kTIMINGLIB_ATTRENUM_multiplied_by:
            break;
        case kTIMINGLIB_ATTRENUM_master_pin:
            break;
        case kTIMINGLIB_ATTRENUM_invert:
            break;
        case kTIMINGLIB_ATTRENUM_duty_cycle:
            break;
        case kTIMINGLIB_ATTRENUM_divided_by:
            break;
        case kTIMINGLIB_ATTRENUM_clock_pin:
            break;
        case kTIMINGLIB_ATTRENUM_preset:
            break;
        case kTIMINGLIB_ATTRENUM_next_state:
            break;
        case kTIMINGLIB_ATTRENUM_clocked_on_also:
            break;
        case kTIMINGLIB_ATTRENUM_clear_preset_var2:
            break;
        case kTIMINGLIB_ATTRENUM_clear_preset_var1:
            break;
        case kTIMINGLIB_ATTRENUM_clear:
            break;
        case kTIMINGLIB_ATTRENUM_bus_type:
            break;
        case kTIMINGLIB_ATTRENUM_tdisable:
            break;
        case kTIMINGLIB_ATTRENUM_edge_type:
            break;
        case kTIMINGLIB_ATTRENUM_rise_pin_resistance:
            break;
        case kTIMINGLIB_ATTRENUM_rise_delay_intercept:
            break;
        case kTIMINGLIB_ATTRENUM_mode:
            break;
        case kTIMINGLIB_ATTRENUM_fall_pin_resistance:
            break;
        case kTIMINGLIB_ATTRENUM_fall_delay_intercept:
            break;
        case kTIMINGLIB_ATTRENUM_when_start:
            break;
        case kTIMINGLIB_ATTRENUM_when_end:
            break;
        case kTIMINGLIB_ATTRENUM_timing_type:
            break;
        case kTIMINGLIB_ATTRENUM_timing_sense:
            break;
        case kTIMINGLIB_ATTRENUM_steady_state_resistance_low_min:
            break;
        case kTIMINGLIB_ATTRENUM_steady_state_resistance_low_max:
            break;
        case kTIMINGLIB_ATTRENUM_steady_state_resistance_high_min:
            break;
        case kTIMINGLIB_ATTRENUM_steady_state_resistance_high_max:
            break;
        case kTIMINGLIB_ATTRENUM_steady_state_resistance_float_min:
            break;
        case kTIMINGLIB_ATTRENUM_steady_state_resistance_float_max:
            break;
        case kTIMINGLIB_ATTRENUM_slope_rise:
            break;
        case kTIMINGLIB_ATTRENUM_slope_fall:
            break;
        case kTIMINGLIB_ATTRENUM_setup_coefficient:
            break;
        case kTIMINGLIB_ATTRENUM_sdf_edges:
            break;
        case kTIMINGLIB_ATTRENUM_sdf_cond_start:
            break;
        case kTIMINGLIB_ATTRENUM_sdf_cond_end:
            break;
        case kTIMINGLIB_ATTRENUM_rise_resistance:
            break;
        case kTIMINGLIB_ATTRENUM_related_output_pin:
            break;
        case kTIMINGLIB_ATTRENUM_related_bus_equivalent:
            break;
        case kTIMINGLIB_ATTRENUM_intrinsic_rise:
            break;
        case kTIMINGLIB_ATTRENUM_intrinsic_fall:
            break;
        case kTIMINGLIB_ATTRENUM_hold_coefficient:
            break;
        case kTIMINGLIB_ATTRENUM_fall_resistance:
            break;
        case kTIMINGLIB_ATTRENUM_edge_rate_sensitivity_r1:
            break;
        case kTIMINGLIB_ATTRENUM_edge_rate_sensitivity_r0:
            break;
        case kTIMINGLIB_ATTRENUM_edge_rate_sensitivity_f1:
            break;
        case kTIMINGLIB_ATTRENUM_edge_rate_sensitivity_f0:
            break;
        case kTIMINGLIB_ATTRENUM_default_timing:
            break;
        case kTIMINGLIB_ATTRENUM_lsi_pad:
            break;
        case kTIMINGLIB_ATTRENUM_fsim_map:
            break;
        case kTIMINGLIB_ATTRENUM_constraint:
            break;
        case kTIMINGLIB_ATTRENUM_sdf_cond:
            break;
        case kTIMINGLIB_ATTRENUM_constraint_low:
            break;
        case kTIMINGLIB_ATTRENUM_constraint_high:
            break;
        case kTIMINGLIB_ATTRENUM_enable:
            break;
        case kTIMINGLIB_ATTRENUM_clocked_on:
            break;
        case kTIMINGLIB_ATTRENUM_address:
            break;
        case kTIMINGLIB_ATTRENUM_when:
            break;
        case kTIMINGLIB_ATTRENUM_switching_together_group:
            break;
        case kTIMINGLIB_ATTRENUM_switching_interval:
            break;
        case kTIMINGLIB_ATTRENUM_rising_together_group:
            break;
        case kTIMINGLIB_ATTRENUM_related_rising_pin:
            break;
        case kTIMINGLIB_ATTRENUM_related_outputs:
            break;
        case kTIMINGLIB_ATTRENUM_related_inputs:
            break;
        case kTIMINGLIB_ATTRENUM_related_input:
            break;
        case kTIMINGLIB_ATTRENUM_related_falling_pin:
            break;
        case kTIMINGLIB_ATTRENUM_falling_together_group:
            break;
        case kTIMINGLIB_ATTRENUM_equal_or_opposite_output:
            break;
        case kTIMINGLIB_ATTRENUM_values:
            break;
        case kTIMINGLIB_ATTRENUM_intermediate_values:
            break;
        case kTIMINGLIB_ATTRENUM_index_3:
            break;
        case kTIMINGLIB_ATTRENUM_index_2:
            break;
        case kTIMINGLIB_ATTRENUM_index_1:
            break;
        case kTIMINGLIB_ATTRENUM_related_pin:
            break;
        case kTIMINGLIB_ATTRENUM_related_bus_pins:
            break;
        case kTIMINGLIB_ATTRENUM_members:
            break;
        case kTIMINGLIB_ATTRENUM_input_map_shift:
            break;
        case kTIMINGLIB_ATTRENUM_x_function:
            break;
        case kTIMINGLIB_ATTRENUM_three_state:
            break;
        case kTIMINGLIB_ATTRENUM_test_output_only:
            break;
        case kTIMINGLIB_ATTRENUM_state_function:
            break;
        case kTIMINGLIB_ATTRENUM_slew_control:
            break;
        case kTIMINGLIB_ATTRENUM_signal_type:
            break;
        case kTIMINGLIB_ATTRENUM_rise_time_before_threshold:
            break;
        case kTIMINGLIB_ATTRENUM_rise_time_after_threshold:
            break;
        case kTIMINGLIB_ATTRENUM_rise_current_slope_before_threshold:
            break;
        case kTIMINGLIB_ATTRENUM_rise_current_slope_after_threshold:
            break;
        case kTIMINGLIB_ATTRENUM_rise_capacitance:
            break;
        case kTIMINGLIB_ATTRENUM_reference_capacitance:
            break;
        case kTIMINGLIB_ATTRENUM_pulling_resistance:
            break;
        case kTIMINGLIB_ATTRENUM_pulling_current:
            break;
        case kTIMINGLIB_ATTRENUM_primary_output:
            break;
        case kTIMINGLIB_ATTRENUM_prefer_tied:
            break;
        case kTIMINGLIB_ATTRENUM_pin_func_type:
            break;
        case kTIMINGLIB_ATTRENUM_output_voltage:
            break;
        case kTIMINGLIB_ATTRENUM_output_signal_level:
            break;
        case kTIMINGLIB_ATTRENUM_nextstate_type:
            break;
        case kTIMINGLIB_ATTRENUM_multicell_pad_pin:
            break;
        case kTIMINGLIB_ATTRENUM_min_transition:
            break;
        case kTIMINGLIB_ATTRENUM_min_pulse_width_low:
            break;
        case kTIMINGLIB_ATTRENUM_min_pulse_width_high:
            break;
        case kTIMINGLIB_ATTRENUM_min_period:
            break;
        case kTIMINGLIB_ATTRENUM_min_fanout:
            break;
        case kTIMINGLIB_ATTRENUM_min_capacitance:
            break;
        case kTIMINGLIB_ATTRENUM_max_transition:
            break;
        case kTIMINGLIB_ATTRENUM_max_time_borrow:
            break;
        case kTIMINGLIB_ATTRENUM_max_fanout:
            break;
        case kTIMINGLIB_ATTRENUM_max_capacitance:
            break;
        case kTIMINGLIB_ATTRENUM_is_pad:
            break;
        case kTIMINGLIB_ATTRENUM_inverted_output:
            break;
        case kTIMINGLIB_ATTRENUM_internal_node:
            break;
        case kTIMINGLIB_ATTRENUM_input_voltage:
            break;
        case kTIMINGLIB_ATTRENUM_input_signal_level:
            break;
        case kTIMINGLIB_ATTRENUM_input_map:
            break;
        case kTIMINGLIB_ATTRENUM_hysteresis:
            break;
        case kTIMINGLIB_ATTRENUM_function:
            break;
        case kTIMINGLIB_ATTRENUM_fault_model:
            break;
        case kTIMINGLIB_ATTRENUM_fanout_load:
            break;
        case kTIMINGLIB_ATTRENUM_fall_time_before_threshold:
            break;
        case kTIMINGLIB_ATTRENUM_fall_time_after_threshold:
            break;
        case kTIMINGLIB_ATTRENUM_fall_current_slope_before_threshold:
            break;
        case kTIMINGLIB_ATTRENUM_fall_current_slope_after_threshold:
            break;
        case kTIMINGLIB_ATTRENUM_fall_capacitance:
            break;
        case kTIMINGLIB_ATTRENUM_edge_rate_rise:
            break;
        case kTIMINGLIB_ATTRENUM_edge_rate_load_rise:
            break;
        case kTIMINGLIB_ATTRENUM_edge_rate_load_fall:
            break;
        case kTIMINGLIB_ATTRENUM_edge_rate_fall:
            break;
        case kTIMINGLIB_ATTRENUM_edge_rate_breakpoint_r1:
            break;
        case kTIMINGLIB_ATTRENUM_edge_rate_breakpoint_r0:
            break;
        case kTIMINGLIB_ATTRENUM_edge_rate_breakpoint_f1:
            break;
        case kTIMINGLIB_ATTRENUM_edge_rate_breakpoint_f0:
            break;
        case kTIMINGLIB_ATTRENUM_driver_type:
            break;
        case kTIMINGLIB_ATTRENUM_drive_current:
            break;
        case kTIMINGLIB_ATTRENUM_direction:
            break;
        case kTIMINGLIB_ATTRENUM_dcm_timing:
            break;
        case kTIMINGLIB_ATTRENUM_connection_class:
            break;
        case kTIMINGLIB_ATTRENUM_complementary_pin:
            break;
        case kTIMINGLIB_ATTRENUM_clock_gate_test_pin:
            break;
        case kTIMINGLIB_ATTRENUM_clock_gate_out_pin:
            break;
        case kTIMINGLIB_ATTRENUM_clock_gate_obs_pin:
            break;
        case kTIMINGLIB_ATTRENUM_clock_gate_enable_pin:
            break;
        case kTIMINGLIB_ATTRENUM_clock_gate_clock_pin:
            break;
        case kTIMINGLIB_ATTRENUM_clock:
            break;
        case kTIMINGLIB_ATTRENUM_capacitance:
            break;
        case kTIMINGLIB_ATTRENUM_rail_connection:
            break;
        case kTIMINGLIB_ATTRENUM_pin_opposite:
            break;
        case kTIMINGLIB_ATTRENUM_pin_equal:
            break;
        case kTIMINGLIB_ATTRENUM_vhdl_name:
            break;
        case kTIMINGLIB_ATTRENUM_use_for_size_only:
            break;
        case kTIMINGLIB_ATTRENUM_single_bit_degenerate:
            break;
        case kTIMINGLIB_ATTRENUM_set_node:
            break;
        case kTIMINGLIB_ATTRENUM_scan_group:
            break;
        case kTIMINGLIB_ATTRENUM_scaling_factors:
            break;
        case kTIMINGLIB_ATTRENUM_preferred:
            break;
        case kTIMINGLIB_ATTRENUM_pad_type:
            break;
        case kTIMINGLIB_ATTRENUM_pad_cell:
            break;
        case kTIMINGLIB_ATTRENUM_observe_node:
            break;
        case kTIMINGLIB_ATTRENUM_mpm_name:
            break;
        case kTIMINGLIB_ATTRENUM_mpm_libname:
            break;
        case kTIMINGLIB_ATTRENUM_map_only:
            break;
        case kTIMINGLIB_ATTRENUM_is_clock_gating_cell:
            break;
        case kTIMINGLIB_ATTRENUM_interface_timing:
            break;
        case kTIMINGLIB_ATTRENUM_handle_negative_constraint:
            break;
        case kTIMINGLIB_ATTRENUM_geometry_print:
            break;
        case kTIMINGLIB_ATTRENUM_dont_use:
            break;
        case kTIMINGLIB_ATTRENUM_dont_touch:
            break;
        case kTIMINGLIB_ATTRENUM_dont_fault:
            break;
        case kTIMINGLIB_ATTRENUM_contention_condition:
            break;
        case kTIMINGLIB_ATTRENUM_clock_gating_integrated_cell:
            break;
        case kTIMINGLIB_ATTRENUM_cell_leakage_power:
            break;
        case kTIMINGLIB_ATTRENUM_cell_footprint:
            break;
        case kTIMINGLIB_ATTRENUM_auxiliary_pad_cell:
            break;
        case kTIMINGLIB_ATTRENUM_area:
            break;
        case kTIMINGLIB_ATTRENUM_technology:
            break;
        case kTIMINGLIB_ATTRENUM_routing_layers:
            break;
        case kTIMINGLIB_ATTRENUM_piece_define:
            break;
        case kTIMINGLIB_ATTRENUM_library_features:
            break;
        case kTIMINGLIB_ATTRENUM_define_cell_area:
            __enterDefineCellArea(a);
            break;
        case kTIMINGLIB_ATTRENUM_capacitive_load_unit:
            break;
        case kTIMINGLIB_ATTRENUM_voltage_unit:
            break;
        case kTIMINGLIB_ATTRENUM_timing_report:
            break;
        case kTIMINGLIB_ATTRENUM_time_unit:
            break;
        case kTIMINGLIB_ATTRENUM_slew_upper_threshold_pct_rise:
            break;
        case kTIMINGLIB_ATTRENUM_slew_upper_threshold_pct_fall:
            break;
        case kTIMINGLIB_ATTRENUM_slew_lower_threshold_pct_rise:
            break;
        case kTIMINGLIB_ATTRENUM_slew_lower_threshold_pct_fall:
            break;
        case kTIMINGLIB_ATTRENUM_slew_derate_from_library:
            break;
        case kTIMINGLIB_ATTRENUM_simulation:
            break;
        case kTIMINGLIB_ATTRENUM_revision:
            break;
        case kTIMINGLIB_ATTRENUM_pulling_resistance_unit:
            break;
        case kTIMINGLIB_ATTRENUM_preferred_output_pad_voltage:
            break;
        case kTIMINGLIB_ATTRENUM_preferred_output_pad_slew_rate_control:
            break;
        case kTIMINGLIB_ATTRENUM_preferred_input_pad_voltage:
            break;
        case kTIMINGLIB_ATTRENUM_piece_type:
            break;
        case kTIMINGLIB_ATTRENUM_output_threshold_pct_rise:
            break;
        case kTIMINGLIB_ATTRENUM_output_threshold_pct_fall:
            break;
        case kTIMINGLIB_ATTRENUM_nom_voltage:
            break;
        case kTIMINGLIB_ATTRENUM_nom_temperature:
            break;
        case kTIMINGLIB_ATTRENUM_nom_process:
            break;
        case kTIMINGLIB_ATTRENUM_lsi_rounding_digit:
            break;
        case kTIMINGLIB_ATTRENUM_lsi_rounding_cutoff:
            break;
        case kTIMINGLIB_ATTRENUM_lsi_pad_rise:
            break;
        case kTIMINGLIB_ATTRENUM_lsi_pad_fall:
            break;
        case kTIMINGLIB_ATTRENUM_leakage_power_unit:
            break;
        case kTIMINGLIB_ATTRENUM_key_version:
            break;
        case kTIMINGLIB_ATTRENUM_key_seed:
            break;
        case kTIMINGLIB_ATTRENUM_key_file:
            break;
        case kTIMINGLIB_ATTRENUM_key_feature:
            break;
        case kTIMINGLIB_ATTRENUM_key_bit:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_wire_res:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_wire_cap:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_slope_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_slope_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_skew_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_skew_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_setup_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_setup_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_rise_transition:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_rise_propagation:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_rise_pin_resistance:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_rise_delay_intercept:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_removal_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_removal_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_recovery_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_recovery_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_pin_cap:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_nochange_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_nochange_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_min_pulse_width_low:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_min_pulse_width_high:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_min_period:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_intrinsic_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_intrinsic_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_internal_power:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_hold_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_hold_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_fall_transition:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_fall_propagation:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_fall_pin_resistance:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_fall_delay_intercept:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_drive_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_drive_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_drive_current:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_cell_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_cell_leakage_power:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_cell_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_volt_cell_degradation:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_wire_res:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_wire_cap:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_slope_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_slope_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_skew_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_skew_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_setup_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_setup_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_rise_transition:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_rise_propagation:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_rise_pin_resistance:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_rise_delay_intercept:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_removal_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_removal_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_recovery_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_recovery_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_pin_cap:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_nochange_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_nochange_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_min_pulse_width_low:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_min_pulse_width_high:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_min_period:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_intrinsic_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_intrinsic_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_internal_power:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_hold_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_hold_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_fall_transition:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_fall_propagation:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_fall_pin_resistance:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_fall_delay_intercept:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_drive_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_drive_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_drive_current:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_cell_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_cell_leakage_power:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_cell_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_temp_cell_degradation:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_wire_res:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_wire_cap:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_slope_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_slope_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_skew_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_skew_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_setup_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_setup_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_rise_transition:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_rise_propagation:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_rise_pin_resistance:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_rise_delay_intercept:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_removal_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_removal_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_recovery_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_recovery_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_pin_rise_cap:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_pin_fall_cap:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_pin_cap:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_nochange_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_nochange_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_min_pulse_width_low:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_min_pulse_width_high:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_min_period:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_intrinsic_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_intrinsic_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_internal_power:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_hold_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_hold_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_fall_transition:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_fall_propagation:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_fall_pin_resistance:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_fall_delay_intercept:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_drive_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_drive_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_drive_current:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_cell_rise:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_cell_leakage_power:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_cell_fall:
            break;
        case kTIMINGLIB_ATTRENUM_k_process_cell_degradation:
            break;
        case kTIMINGLIB_ATTRENUM_input_threshold_pct_rise:
            break;
        case kTIMINGLIB_ATTRENUM_input_threshold_pct_fall:
            break;
        case kTIMINGLIB_ATTRENUM_in_place_swap_mode:
            break;
        case kTIMINGLIB_ATTRENUM_em_temp_degradation_factor:
            break;
        case kTIMINGLIB_ATTRENUM_delay_model:
            break;
        case kTIMINGLIB_ATTRENUM_default_wire_load_selection:
            break;
        case kTIMINGLIB_ATTRENUM_default_wire_load_resistance:
            break;
        case kTIMINGLIB_ATTRENUM_default_wire_load_mode:
            break;
        case kTIMINGLIB_ATTRENUM_default_wire_load_capacitance:
            break;
        case kTIMINGLIB_ATTRENUM_default_wire_load_area:
            break;
        case kTIMINGLIB_ATTRENUM_default_wire_load:
            break;
        case kTIMINGLIB_ATTRENUM_default_slope_rise:
            break;
        case kTIMINGLIB_ATTRENUM_default_slope_fall:
            break;
        case kTIMINGLIB_ATTRENUM_default_setup_coefficient:
            break;
        case kTIMINGLIB_ATTRENUM_default_rise_pin_resistance:
            break;
        case kTIMINGLIB_ATTRENUM_default_rise_delay_intercept:
            break;
        case kTIMINGLIB_ATTRENUM_default_reference_capacitance:
            break;
        case kTIMINGLIB_ATTRENUM_default_rc_rise_coefficient:
            break;
        case kTIMINGLIB_ATTRENUM_default_rc_fall_coefficient:
            break;
        case kTIMINGLIB_ATTRENUM_default_output_pin_rise_res:
            break;
        case kTIMINGLIB_ATTRENUM_default_output_pin_fall_res:
            break;
        case kTIMINGLIB_ATTRENUM_default_output_pin_cap:
            break;
        case kTIMINGLIB_ATTRENUM_default_operating_conditions:
            break;
        case kTIMINGLIB_ATTRENUM_default_min_porosity:
            break;
        case kTIMINGLIB_ATTRENUM_default_max_utilization:
            break;
        case kTIMINGLIB_ATTRENUM_default_max_transition:
            break;
        case kTIMINGLIB_ATTRENUM_default_max_fanout:
            break;
        case kTIMINGLIB_ATTRENUM_default_max_capacitance:
            break;
        case kTIMINGLIB_ATTRENUM_default_leakage_power_density:
            break;
        case kTIMINGLIB_ATTRENUM_default_intrinsic_rise:
            break;
        case kTIMINGLIB_ATTRENUM_default_intrinsic_fall:
            break;
        case kTIMINGLIB_ATTRENUM_default_input_pin_cap:
            break;
        case kTIMINGLIB_ATTRENUM_default_inout_pin_rise_res:
            break;
        case kTIMINGLIB_ATTRENUM_default_inout_pin_fall_res:
            break;
        case kTIMINGLIB_ATTRENUM_default_inout_pin_cap:
            break;
        case kTIMINGLIB_ATTRENUM_default_hold_coefficient:
            break;
        case kTIMINGLIB_ATTRENUM_default_fanout_load:
            break;
        case kTIMINGLIB_ATTRENUM_default_fall_pin_resistance:
            break;
        case kTIMINGLIB_ATTRENUM_default_fall_delay_intercept:
            break;
        case kTIMINGLIB_ATTRENUM_default_edge_rate_breakpoint_r1:
            break;
        case kTIMINGLIB_ATTRENUM_default_edge_rate_breakpoint_r0:
            break;
        case kTIMINGLIB_ATTRENUM_default_edge_rate_breakpoint_f1:
            break;
        case kTIMINGLIB_ATTRENUM_default_edge_rate_breakpoint_f0:
            break;
        case kTIMINGLIB_ATTRENUM_default_connection_class:
            break;
        case kTIMINGLIB_ATTRENUM_default_cell_leakage_power:
            break;
        case kTIMINGLIB_ATTRENUM_date:
            break;
        case kTIMINGLIB_ATTRENUM_current_unit:
            break;
        case kTIMINGLIB_ATTRENUM_comment:
            break;
        case kTIMINGLIB_ATTRENUM_bus_naming_style:
            break;
        default:
            break;
    }
}
char *LibAnalysis::__mystrtod(char *str, char **end) {
    char *x = str, *y;
    while (*x &&
           (*x == ' ' || *x == '\t' || *x == ',' || *x == '\n' || *x == '\\')) {
        x++;
    }
    y = x;
    while (*x && (*x == '+' || *x == '-' || (*x >= '0' && *x <= '9') ||
                  *x == 'E' || *x == 'e' || *x == '.')) {
        x++;
    }
    if (y == x) {
        *end = str;
        return str;
    }
    *end = x;
    return x;
}
int LibAnalysis::__countFloatsInString(char *s) {
    char *p, *t;
    int i = 0;
    p = s;
    while (__mystrtod(p, &t) != p) {
        i++;
        p = t;
    }
    return i;
}
int LibAnalysis::__countStringsInList(si2drAttrIdT attr) {
    si2drErrorT err;
    si2drValuesIdT vals = __si2drComplexAttrGetValues(attr, &err);
    int i = 0;
    si2drValueTypeT type;
    si2drInt32T intgr;
    si2drFloat64T fl;
    si2drStringT str;
    si2drBooleanT boolean;
    si2drExprT *expr;
    while (1) {
        __si2drIterNextComplexValue(vals, &type, &intgr, &fl, &str, &boolean,
                                    &expr, &err);
        if (type != kSI2DR_UNDEFINED_VALUETYPE) {
            i++;
        } else {
            break;
        }
    }
    __si2drIterQuit(vals, &err);
    return i;
}
void LibAnalysis::__checkLuTableTemplateArraysize(si2drGroupIdT group) {
    si2drErrorT err;
    si2drNamesIdT names;
    si2drStringT str;
    si2drGroupIdT rg, rg2;
    names = __si2drGroupGetNames(group, &err);
    str = __si2drIterNextName(names, &err);
    __si2drIterQuit(names, &err);
    if (str && str[0]) {
        char strx[8000];
        snprintf(strx, sizeof(strx), "%s", str);
        if (!strcmp(strx, "scalar")) {
            si2drAttrIdT vs;
            /* should be just one value */
            vs =
                __si2drGroupFindAttrByName(group, si2drStringT("values"), &err);
            if (!__si2drObjectIsNull(vs, &err)) {
                int numstrs = __countStringsInList(vs);
                int numfloats;
                si2drValuesIdT vals;
                si2drValueTypeT type;
                si2drInt32T intgr;
                si2drFloat64T float64;
                si2drStringT string;
                si2drBooleanT boolean;
                si2drExprT *expr;
                vals = __si2drComplexAttrGetValues(vs, &err);
                __si2drIterNextComplexValue(vals, &type, &intgr, &float64,
                                            &string, &boolean, &expr, &err);
                __si2drIterQuit(vals, &err);
                if (type == kSI2DR_UNDEFINED_VALUETYPE) {
                    char EB[8000];
                    snprintf(
                        EB, sizeof(EB), "%s:%d, %s references template %s, so",
                        __si2drObjectGetFileName(group, &err),
                        static_cast<int>(__si2drObjectGetLineNo(group, &err)),
                        __si2drGroupGetGroupType(group, &err), strx);
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                    snprintf(
                        EB, sizeof(EB),
                        "                  the values attribute should "
                        "have one string specified\n                  with "
                        "a number in it, but does not.");
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                    errcount_++;
                }
                if (type != kSI2DR_STRING) {
                    char EB[8000];
                    snprintf(
                        EB, sizeof(EB), "%s:%d, %s references template %s, so",
                        __si2drObjectGetFileName(group, &err),
                        static_cast<int>(__si2drObjectGetLineNo(group, &err)),
                        __si2drGroupGetGroupType(group, &err), strx);
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                    snprintf(
                        EB, sizeof(EB),
                        "                  the values attribute should have "
                        "one string specified\n                  with a number "
                        "in it, but what is there is not a string.");
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                    errcount_++;
                } else {
                    numfloats = __countFloatsInString(string);
                    if (numfloats != 1 || numstrs != 1) {
                        char EB[8000];
                        snprintf(EB, sizeof(EB),
                                 "%s:%d, %s references template %s, but",
                                 __si2drObjectGetFileName(group, &err),
                                 static_cast<int>(
                                     __si2drObjectGetLineNo(group, &err)),
                                 __si2drGroupGetGroupType(group, &err), strx);
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                        snprintf(EB, sizeof(EB),
                                 "                  the values attribute has "
                                 "more than one value or string specified.");
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                        errcount_++;
                    }
                }
            } else {
                char EB[8000];
                snprintf(EB, sizeof(EB),
                         "%s:%d, %s references template %s, but",
                         __si2drObjectGetFileName(group, &err),
                         static_cast<int>(__si2drObjectGetLineNo(group, &err)),
                         __si2drGroupGetGroupType(group, &err), strx);
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR,
                                    EB, &err);
                (this->*si2ErrMsg_)(
                    kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR,
                    si2drStringT("                  the group does "
                                 "not have a values attribute."),
                    &err);
                errcount_++;
            }
            return;
        }
        /* find an lu_table_template with this name */
        rg = __getContainingGroup(group, si2drStringT("library"));
        rg2 = __si2drGroupFindGroupByName(
            rg, strx, si2drStringT("lu_table_template"), &err);
        if (!__si2drObjectIsNull(rg2, &err)) {
            si2drAttrIdT ind1, ind2, ind3;
            si2drAttrIdT vs;
            ind1 = __si2drGroupFindAttrByName(group, si2drStringT("index_1"),
                                              &err);
            ind2 = __si2drGroupFindAttrByName(group, si2drStringT("index_2"),
                                              &err);
            ind3 = __si2drGroupFindAttrByName(group, si2drStringT("index_3"),
                                              &err);
            if (__si2drObjectIsNull(ind1, &err))
                ind1 = __si2drGroupFindAttrByName(rg2, si2drStringT("index_1"),
                                                  &err);
            if (__si2drObjectIsNull(ind2, &err))
                ind2 = __si2drGroupFindAttrByName(rg2, si2drStringT("index_2"),
                                                  &err);
            if (__si2drObjectIsNull(ind3, &err))
                ind3 = __si2drGroupFindAttrByName(rg2, si2drStringT("index_3"),
                                                  &err);
            vs =
                __si2drGroupFindAttrByName(group, si2drStringT("values"), &err);
            if (!__si2drObjectIsNull(ind1, &err) &&
                !__si2drObjectIsNull(ind2, &err) &&
                !__si2drObjectIsNull(ind3, &err)) {
                if (!__si2drObjectIsNull(vs, &err)) {
                    int tpl1nums, tpl2nums, tpl3nums;
                    int numstrs = __countStringsInList(vs);
                    si2drValuesIdT vals;
                    si2drValueTypeT type;
                    si2drInt32T intgr;
                    si2drFloat64T float64;
                    si2drStringT string;
                    si2drBooleanT boolean;
                    si2drExprT *expr;
                    vals = __si2drComplexAttrGetValues(ind1, &err);
                    __si2drIterNextComplexValue(vals, &type, &intgr, &float64,
                                                &string, &boolean, &expr, &err);
                    __si2drIterQuit(vals, &err);
                    if (type == kSI2DR_STRING) {
                        tpl1nums = __countFloatsInString(string);
                        vals = __si2drComplexAttrGetValues(ind2, &err);
                        __si2drIterNextComplexValue(vals, &type, &intgr,
                                                    &float64, &string, &boolean,
                                                    &expr, &err);
                        __si2drIterQuit(vals, &err);
                        if (type == kSI2DR_STRING) {
                            tpl2nums = __countFloatsInString(string);
                            vals = __si2drComplexAttrGetValues(ind3, &err);
                            __si2drIterNextComplexValue(vals, &type, &intgr,
                                                        &float64, &string,
                                                        &boolean, &expr, &err);
                            __si2drIterQuit(vals, &err);
                            if (type == kSI2DR_STRING) {
                                tpl3nums = __countFloatsInString(string);
                                if (numstrs == tpl1nums * tpl2nums) {
                                    /* now, count the nums in each entry, and
                                     * make sure they equal tpl2nums */
                                    int i;
                                    vals =
                                        __si2drComplexAttrGetValues(vs, &err);
                                    for (i = 0; i < numstrs; i++) {
                                        __si2drIterNextComplexValue(
                                            vals, &type, &intgr, &float64,
                                            &string, &boolean, &expr, &err);
                                        if (type == kSI2DR_STRING) {
                                            int numc =
                                                __countFloatsInString(string);
                                            if (numc != tpl3nums) {
                                                char EB[8000];
                                                snprintf(
                                                    EB, sizeof(EB),
                                                    "%s:%d, %s references "
                                                    "template %s,",
                                                    __si2drObjectGetFileName(
                                                        group, &err),
                                                    static_cast<int>(
                                                        __si2drObjectGetLineNo(
                                                            group, &err)),
                                                    __si2drGroupGetGroupType(
                                                        group, &err),
                                                    strx);
                                                (this->*si2ErrMsg_)(
                                                    kSI2DR_SEVERITY_ERR,
                                                    kSI2DR_SEMANTIC_ERROR, EB,
                                                    &err);
                                                snprintf(
                                                    EB, sizeof(EB),
                                                    "                  which "
                                                    "says there should be %d "
                                                    "strings, each with %d "
                                                    "numbers in it in the "
                                                    "values table,",
                                                    tpl1nums * tpl2nums,
                                                    tpl3nums);
                                                (this->*si2ErrMsg_)(
                                                    kSI2DR_SEVERITY_ERR,
                                                    kSI2DR_SEMANTIC_ERROR, EB,
                                                    &err);
                                                snprintf(
                                                    EB, sizeof(EB),
                                                    "                  but in "
                                                    "the group's values "
                                                    "attribute (ending at line "
                                                    "%d) value #%d has %d "
                                                    "numbers.",
                                                    static_cast<int>(
                                                        __si2drObjectGetLineNo(
                                                            vs, &err)),
                                                    i + 1, numc);
                                                (this->*si2ErrMsg_)(
                                                    kSI2DR_SEVERITY_ERR,
                                                    kSI2DR_SEMANTIC_ERROR, EB,
                                                    &err);
                                                errcount_++;
                                            }
                                        } else {
                                            char EB[8000];
                                            snprintf(EB, sizeof(EB),
                                                     "%s:%d, %s references "
                                                     "template %s,",
                                                     __si2drObjectGetFileName(
                                                         group, &err),
                                                     static_cast<int>(
                                                         __si2drObjectGetLineNo(
                                                             group, &err)),
                                                     __si2drGroupGetGroupType(
                                                         group, &err),
                                                     strx);
                                            (this->*si2ErrMsg_)(
                                                kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                                            snprintf(
                                                EB, sizeof(EB),
                                                "                  which says "
                                                "there should be %d strings, "
                                                "each with %d numbers in it in "
                                                "the values table,",
                                                tpl1nums * tpl2nums, tpl3nums);
                                            (this->*si2ErrMsg_)(
                                                kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                                            snprintf(
                                                EB, sizeof(EB),
                                                "                  but in the "
                                                "group's values attribute "
                                                "(ending at line %d) value #%d "
                                                "is not a string.",
                                                static_cast<int>(
                                                    __si2drObjectGetLineNo(
                                                        vs, &err)),
                                                i + 1);
                                            (this->*si2ErrMsg_)(
                                                kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                                            errcount_++;
                                        }
                                    }
                                    __si2drIterQuit(vals, &err);
                                } else {
                                    char EB[8000];
                                    snprintf(
                                        EB, sizeof(EB),
                                        "%s:%d, %s references template %s,",
                                        __si2drObjectGetFileName(group, &err),
                                        static_cast<int>(__si2drObjectGetLineNo(
                                            group, &err)),
                                        __si2drGroupGetGroupType(group, &err),
                                        strx);
                                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                        kSI2DR_SEMANTIC_ERROR,
                                                        EB, &err);
                                    snprintf(
                                        EB, sizeof(EB),
                                        "                  which says there "
                                        "should be %d strings, each with %d "
                                        "numbers in it in the values table,",
                                        tpl1nums * tpl2nums, tpl3nums);
                                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                        kSI2DR_SEMANTIC_ERROR,
                                                        EB, &err);
                                    snprintf(
                                        EB, sizeof(EB),
                                        "                  but the group's "
                                        "values attribute (line %d) has %d "
                                        "strings.",
                                        static_cast<int>(
                                            __si2drObjectGetLineNo(vs, &err)),
                                        numstrs);
                                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                        kSI2DR_SEMANTIC_ERROR,
                                                        EB, &err);
                                    errcount_++;
                                }
                            } else {
                                char EB[8000];
                                snprintf(
                                    EB, sizeof(EB),
                                    "%s:%d, %s references template %s, but",
                                    __si2drObjectGetFileName(group, &err),
                                    static_cast<int>(
                                        __si2drObjectGetLineNo(group, &err)),
                                    __si2drGroupGetGroupType(group, &err),
                                    strx);
                                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                    kSI2DR_SEMANTIC_ERROR, EB,
                                                    &err);
                                snprintf(
                                    EB, sizeof(EB),
                                    "                  the template's "
                                    "index_3 attribute (line %d) does not "
                                    "have a string as a value.",
                                    static_cast<int>(
                                        __si2drObjectGetLineNo(ind2, &err)));
                                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                    kSI2DR_SEMANTIC_ERROR, EB,
                                                    &err);
                                errcount_++;
                            }
                        } else {
                            char EB[8000];
                            snprintf(EB, sizeof(EB),
                                     "%s:%d, %s references template %s, but",
                                     __si2drObjectGetFileName(group, &err),
                                     static_cast<int>(
                                         __si2drObjectGetLineNo(group, &err)),
                                     __si2drGroupGetGroupType(group, &err),
                                     strx);
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                            snprintf(EB, sizeof(EB),
                                     "                  the template's index_2 "
                                     "attribute (line %d) does not have a "
                                     "string as a value.",
                                     static_cast<int>(
                                         __si2drObjectGetLineNo(ind2, &err)));
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                            errcount_++;
                        }
                    } else {
                        char EB[8000];
                        snprintf(EB, sizeof(EB),
                                 "%s:%d, %s references template %s, but",
                                 __si2drObjectGetFileName(group, &err),
                                 static_cast<int>(
                                     __si2drObjectGetLineNo(group, &err)),
                                 __si2drGroupGetGroupType(group, &err), strx);
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                        snprintf(
                            EB, sizeof(EB),
                            "                  the template's index_1 "
                            "attribute (line %d) does not have a string as "
                            "a value.",
                            static_cast<int>(
                                __si2drObjectGetLineNo(ind1, &err)));
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                        errcount_++;
                    }
                } else {
                    char EB[8000];
                    snprintf(
                        EB, sizeof(EB), "%s:%d, %s references template %s, but",
                        __si2drObjectGetFileName(group, &err),
                        static_cast<int>(__si2drObjectGetLineNo(group, &err)),
                        __si2drGroupGetGroupType(group, &err), strx);
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                    snprintf(EB, sizeof(EB),
                             "                  the group does not have a "
                             "values attribute.");
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                    errcount_++;
                }
            } else if (!__si2drObjectIsNull(ind1, &err) &&
                       !__si2drObjectIsNull(ind2, &err) &&
                       __si2drObjectIsNull(ind3, &err)) {
                if (!__si2drObjectIsNull(vs, &err)) {
                    int tpl1nums, tpl2nums;
                    int numstrs = __countStringsInList(vs);
                    si2drValuesIdT vals;
                    si2drValueTypeT type;
                    si2drInt32T intgr;
                    si2drFloat64T float64;
                    si2drStringT string;
                    si2drBooleanT boolean;
                    si2drExprT *expr;
                    vals = __si2drComplexAttrGetValues(ind1, &err);
                    __si2drIterNextComplexValue(vals, &type, &intgr, &float64,
                                                &string, &boolean, &expr, &err);
                    __si2drIterQuit(vals, &err);
                    if (type == kSI2DR_STRING) {
                        tpl1nums = __countFloatsInString(string);
                        vals = __si2drComplexAttrGetValues(ind2, &err);
                        __si2drIterNextComplexValue(vals, &type, &intgr,
                                                    &float64, &string, &boolean,
                                                    &expr, &err);
                        __si2drIterQuit(vals, &err);
                        if (type == kSI2DR_STRING) {
                            tpl2nums = __countFloatsInString(string);
                            if (numstrs == tpl1nums) {
                                /* now, count the nums in each entry, and make
                                 * sure they equal tpl2nums */
                                int i;
                                vals = __si2drComplexAttrGetValues(vs, &err);
                                for (i = 0; i < numstrs; i++) {
                                    __si2drIterNextComplexValue(
                                        vals, &type, &intgr, &float64, &string,
                                        &boolean, &expr, &err);
                                    if (type == kSI2DR_STRING) {
                                        int numc =
                                            __countFloatsInString(string);
                                        if (numc != tpl2nums) {
                                            char EB[8000];
                                            snprintf(EB, sizeof(EB),
                                                     "%s:%d, %s references "
                                                     "template %s,",
                                                     __si2drObjectGetFileName(
                                                         group, &err),
                                                     static_cast<int>(
                                                         __si2drObjectGetLineNo(
                                                             group, &err)),
                                                     __si2drGroupGetGroupType(
                                                         group, &err),
                                                     strx);
                                            (this->*si2ErrMsg_)(
                                                kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                                            snprintf(
                                                EB, sizeof(EB),
                                                "                  which says "
                                                "there should be %d strings, "
                                                "each with %d numbers in it in "
                                                "the values table,",
                                                tpl1nums, tpl2nums);
                                            (this->*si2ErrMsg_)(
                                                kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                                            snprintf(
                                                EB, sizeof(EB),
                                                "                  but in the "
                                                "group's values attribute "
                                                "(ending at line %d) value #%d "
                                                "has %d numbers.",
                                                static_cast<int>(
                                                    __si2drObjectGetLineNo(
                                                        vs, &err)),
                                                i + 1, numc);
                                            (this->*si2ErrMsg_)(
                                                kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                                            errcount_++;
                                        }
                                    } else {
                                        char EB[8000];
                                        snprintf(
                                            EB, sizeof(EB),
                                            "%s:%d, %s references template %s,",
                                            __si2drObjectGetFileName(group,
                                                                     &err),
                                            static_cast<int>(
                                                __si2drObjectGetLineNo(group,
                                                                       &err)),
                                            __si2drGroupGetGroupType(group,
                                                                     &err),
                                            strx);
                                        (this->*si2ErrMsg_)(
                                            kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                                        snprintf(
                                            EB, sizeof(EB),
                                            "                  which says "
                                            "there should be %d strings, "
                                            "each with %d numbers in it in "
                                            "the values table,",
                                            tpl1nums, tpl2nums);
                                        (this->*si2ErrMsg_)(
                                            kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                                        snprintf(
                                            EB, sizeof(EB),
                                            "                  but in the "
                                            "group's values attribute (ending "
                                            "at line %d) value #%d is not a "
                                            "string.",
                                            static_cast<int>(
                                                __si2drObjectGetLineNo(vs,
                                                                       &err)),
                                            i + 1);
                                        (this->*si2ErrMsg_)(
                                            kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                                        errcount_++;
                                    }
                                }
                                __si2drIterQuit(vals, &err);
                            } else {
                                char EB[8000];
                                snprintf(
                                    EB, sizeof(EB),
                                    "%s:%d, %s references template %s,",
                                    __si2drObjectGetFileName(group, &err),
                                    static_cast<int>(
                                        __si2drObjectGetLineNo(group, &err)),
                                    __si2drGroupGetGroupType(group, &err),
                                    strx);
                                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                    kSI2DR_SEMANTIC_ERROR, EB,
                                                    &err);
                                snprintf(EB, sizeof(EB),
                                         "                  which says there "
                                         "should be %d strings, each with %d "
                                         "numbers in it in the values table,",
                                         tpl1nums, tpl2nums);
                                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                    kSI2DR_SEMANTIC_ERROR, EB,
                                                    &err);
                                snprintf(
                                    EB, sizeof(EB),
                                    "                  but the group's values "
                                    "attribute (line %d) has %d strings.",
                                    static_cast<int>(
                                        __si2drObjectGetLineNo(vs, &err)),
                                    numstrs);
                                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                    kSI2DR_SEMANTIC_ERROR, EB,
                                                    &err);
                                errcount_++;
                            }
                        } else {
                            char EB[8000];
                            snprintf(EB, sizeof(EB),
                                     "%s:%d, %s references template %s, but",
                                     __si2drObjectGetFileName(group, &err),
                                     static_cast<int>(
                                         __si2drObjectGetLineNo(group, &err)),
                                     __si2drGroupGetGroupType(group, &err),
                                     strx);
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                            snprintf(EB, sizeof(EB),
                                     "                  the template's index_2 "
                                     "attribute (line %d) does not have a "
                                     "string as a value.",
                                     static_cast<int>(
                                         __si2drObjectGetLineNo(ind2, &err)));
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                            errcount_++;
                        }
                    } else {
                        char EB[8000];
                        snprintf(EB, sizeof(EB),
                                 "%s:%d, %s references template %s, but",
                                 __si2drObjectGetFileName(group, &err),
                                 static_cast<int>(
                                     __si2drObjectGetLineNo(group, &err)),
                                 __si2drGroupGetGroupType(group, &err), strx);
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                        snprintf(
                            EB, sizeof(EB),
                            "                  the template's index_1 "
                            "attribute (line %d) does not have a string as "
                            "a value.",
                            static_cast<int>(
                                __si2drObjectGetLineNo(ind1, &err)));
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                        errcount_++;
                    }
                } else {
                    char EB[8000];
                    snprintf(
                        EB, sizeof(EB), "%s:%d, %s references template %s, but",
                        __si2drObjectGetFileName(group, &err),
                        static_cast<int>(__si2drObjectGetLineNo(group, &err)),
                        __si2drGroupGetGroupType(group, &err), strx);
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                    snprintf(EB, sizeof(EB), "%s",
                             "                  the group does not have a "
                             "values attribute.");
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                    errcount_++;
                }
            } else {
                if (!__si2drObjectIsNull(ind1, &err) &&
                    __si2drObjectIsNull(ind2, &err)) {
                    if (!__si2drObjectIsNull(vs, &err)) {
                        int tpl1nums;
                        int numstrs = __countStringsInList(vs);
                        si2drValuesIdT vals;
                        si2drValueTypeT type;
                        si2drInt32T intgr;
                        si2drFloat64T float64;
                        si2drStringT string;
                        si2drBooleanT boolean;
                        si2drExprT *expr;
                        if (numstrs == 1) {
                            vals = __si2drComplexAttrGetValues(ind1, &err);
                            __si2drIterNextComplexValue(vals, &type, &intgr,
                                                        &float64, &string,
                                                        &boolean, &expr, &err);
                            __si2drIterQuit(vals, &err);
                            if (type == kSI2DR_STRING) {
                                int i;
                                tpl1nums = __countFloatsInString(string);
                                /* now, count the nums in each entry, and make
                                 * sure they equal tpl2nums */
                                vals = __si2drComplexAttrGetValues(vs, &err);
                                for (i = 0; i < numstrs; i++) {
                                    __si2drIterNextComplexValue(
                                        vals, &type, &intgr, &float64, &string,
                                        &boolean, &expr, &err);
                                    if (type == kSI2DR_STRING) {
                                        int numc =
                                            __countFloatsInString(string);
                                        if (numc != tpl1nums) {
                                            char EB[8000];
                                            snprintf(EB, sizeof(EB),
                                                     "%s:%d, %s references "
                                                     "template %s,",
                                                     __si2drObjectGetFileName(
                                                         group, &err),
                                                     static_cast<int>(
                                                         __si2drObjectGetLineNo(
                                                             group, &err)),
                                                     __si2drGroupGetGroupType(
                                                         group, &err),
                                                     strx);
                                            (this->*si2ErrMsg_)(
                                                kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                                            snprintf(
                                                EB, sizeof(EB),
                                                "                  which says "
                                                "there should be 1 string, "
                                                "with %d numbers in it in the "
                                                "values table,",
                                                tpl1nums);
                                            (this->*si2ErrMsg_)(
                                                kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                                            snprintf(
                                                EB, sizeof(EB),
                                                "                  but in "
                                                "the group's values "
                                                "attribute (ending at line "
                                                "%d) has %d numbers.",
                                                static_cast<int>(
                                                    __si2drObjectGetLineNo(
                                                        vs, &err)),
                                                numc);
                                            (this->*si2ErrMsg_)(
                                                kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                                            errcount_++;
                                        }
                                    } else {
                                        char EB[8000];
                                        snprintf(
                                            EB, sizeof(EB),
                                            "%s:%d, %s references template %s,",
                                            __si2drObjectGetFileName(group,
                                                                     &err),
                                            static_cast<int>(
                                                __si2drObjectGetLineNo(group,
                                                                       &err)),
                                            __si2drGroupGetGroupType(group,
                                                                     &err),
                                            strx);
                                        (this->*si2ErrMsg_)(
                                            kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                                        snprintf(EB, sizeof(EB),
                                                 "                  which says "
                                                 "there should be 1 string, "
                                                 "with %d numbers in it in the "
                                                 "values table,",
                                                 tpl1nums);
                                        (this->*si2ErrMsg_)(
                                            kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                                        snprintf(
                                            EB, sizeof(EB),
                                            "                  but in the "
                                            "group's values attribute (ending "
                                            "at line %d) is not a string.",
                                            static_cast<int>(
                                                __si2drObjectGetLineNo(vs,
                                                                       &err)));
                                        (this->*si2ErrMsg_)(
                                            kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                                        errcount_++;
                                    }
                                }
                                __si2drIterQuit(vals, &err);
                            } else {
                                char EB[8000];
                                snprintf(
                                    EB, sizeof(EB),
                                    "%s:%d, %s references template %s, but",
                                    __si2drObjectGetFileName(group, &err),
                                    static_cast<int>(
                                        __si2drObjectGetLineNo(group, &err)),
                                    __si2drGroupGetGroupType(group, &err),
                                    strx);
                                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                    kSI2DR_SEMANTIC_ERROR, EB,
                                                    &err);
                                snprintf(
                                    EB, sizeof(EB),
                                    "                  the template's "
                                    "index_1 attribute (line %d) does not "
                                    "have a string as a value.",
                                    static_cast<int>(
                                        __si2drObjectGetLineNo(ind1, &err)));
                                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                    kSI2DR_SEMANTIC_ERROR, EB,
                                                    &err);
                                errcount_++;
                            }
                        } else {
                            char EB[8000];
                            snprintf(EB, sizeof(EB),
                                     "%s:%d, %s references template %s, and "
                                     "only index_1 is defined, but",
                                     __si2drObjectGetFileName(group, &err),
                                     static_cast<int>(
                                         __si2drObjectGetLineNo(group, &err)),
                                     __si2drGroupGetGroupType(group, &err),
                                     strx);
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                            snprintf(
                                EB, sizeof(EB),
                                "                  the group has more than "
                                "one value item....");
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                            errcount_++;
                        }
                    } else {
                        char EB[8000];
                        snprintf(EB, sizeof(EB),
                                 "%s:%d, %s references template %s, but",
                                 __si2drObjectGetFileName(group, &err),
                                 static_cast<int>(
                                     __si2drObjectGetLineNo(group, &err)),
                                 __si2drGroupGetGroupType(group, &err), strx);
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                        snprintf(EB, sizeof(EB),
                                 "                  the group does not have a "
                                 "values attribute.");
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                        errcount_++;
                    }
                } else if (__si2drObjectIsNull(ind1, &err) &&
                           !__si2drObjectIsNull(ind2, &err)) {
                    char EB[8000];
                    snprintf(
                        EB, sizeof(EB),
                        "%s:%d, %s references template %s, and index_2 was "
                        "defined, but index_1 could not be found",
                        __si2drObjectGetFileName(group, &err),
                        static_cast<int>(__si2drObjectGetLineNo(group, &err)),
                        __si2drGroupGetGroupType(group, &err), strx);
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                    snprintf(
                        EB, sizeof(EB),
                        "                 in the group or the template "
                        "(line %d).",
                        static_cast<int>(__si2drObjectGetLineNo(rg2, &err)));
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                    errcount_++;
                }
                /* else ? */
            }
        } else {
            /* it could be a good 'ol polynomial */
            rg2 = __si2drGroupFindGroupByName(
                rg, strx, si2drStringT("poly_template"), &err);
            if (__si2drObjectIsNull(rg2, &err)) {
                char EB[8000];
                snprintf(EB, sizeof(EB),
                         "%s:%d, %s's reference to template %s: the referenced "
                         "group could not be found",
                         __si2drObjectGetFileName(group, &err),
                         static_cast<int>(__si2drObjectGetLineNo(group, &err)),
                         __si2drGroupGetGroupType(group, &err), strx);
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR,
                                    EB, &err);
                errcount_++;
            }
        }
    }
}
void LibAnalysis::__checkPowerLutTemplateArraysize(si2drGroupIdT group) {
    si2drErrorT err;
    si2drNamesIdT names;
    si2drStringT str;
    si2drGroupIdT rg, rg2;
    names = __si2drGroupGetNames(group, &err);
    str = __si2drIterNextName(names, &err);
    __si2drIterQuit(names, &err);
    if (str && str[0]) {
        char strx[8000];
        snprintf(strx, sizeof(strx), "%s", str);
        if (!strcmp(strx, "scalar")) {
            si2drAttrIdT vs;
            /* should be just one value */
            vs =
                __si2drGroupFindAttrByName(group, si2drStringT("values"), &err);
            if (!__si2drObjectIsNull(vs, &err)) {
                int numstrs = __countStringsInList(vs);
                int numfloats;
                si2drValuesIdT vals;
                si2drValueTypeT type;
                si2drInt32T intgr;
                si2drFloat64T float64;
                si2drStringT string;
                si2drBooleanT boolean;
                si2drExprT *expr;
                vals = __si2drComplexAttrGetValues(vs, &err);
                __si2drIterNextComplexValue(vals, &type, &intgr, &float64,
                                            &string, &boolean, &expr, &err);
                __si2drIterQuit(vals, &err);
                if (type == kSI2DR_UNDEFINED_VALUETYPE) {
                    char EB[8000];
                    snprintf(
                        EB, sizeof(EB), "%s:%d, %s references template %s, so",
                        __si2drObjectGetFileName(group, &err),
                        static_cast<int>(__si2drObjectGetLineNo(group, &err)),
                        __si2drGroupGetGroupType(group, &err), strx);
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                    snprintf(
                        EB, sizeof(EB),
                        "                  the values attribute should "
                        "have one string specified\n                  with "
                        "a number in it, but does not.");
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                    errcount_++;
                }
                if (type != kSI2DR_STRING) {
                    char EB[8000];
                    snprintf(
                        EB, sizeof(EB), "%s:%d, %s references template %s, so",
                        __si2drObjectGetFileName(group, &err),
                        static_cast<int>(__si2drObjectGetLineNo(group, &err)),
                        __si2drGroupGetGroupType(group, &err), strx);
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                    snprintf(
                        EB, sizeof(EB),
                        "                  the values attribute should have "
                        "one string specified\n                  with a number "
                        "in it, but what is there is not a string.");
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                    errcount_++;
                } else {
                    numfloats = __countFloatsInString(string);
                    if (numfloats != 1 || numstrs != 1) {
                        char EB[8000];
                        snprintf(EB, sizeof(EB),
                                 "%s:%d, %s references template %s, but",
                                 __si2drObjectGetFileName(group, &err),
                                 static_cast<int>(
                                     __si2drObjectGetLineNo(group, &err)),
                                 __si2drGroupGetGroupType(group, &err), strx);
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                        snprintf(EB, sizeof(EB),
                                 "                  the values attribute has "
                                 "more than one value or string specified.");
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                        errcount_++;
                    }
                }
            } else {
                char EB[8000];
                snprintf(EB, sizeof(EB),
                         "%s:%d, %s references template %s, but",
                         __si2drObjectGetFileName(group, &err),
                         static_cast<int>(__si2drObjectGetLineNo(group, &err)),
                         __si2drGroupGetGroupType(group, &err), strx);
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR,
                                    EB, &err);
                snprintf(EB, sizeof(EB),
                         "                  the group does not have a values "
                         "attribute.");
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR,
                                    EB, &err);
                errcount_++;
            }
            return;
        }
        /* find an lu_table_template with this name */
        rg = __getContainingGroup(group, si2drStringT("library"));
        rg2 = __si2drGroupFindGroupByName(
            rg, strx, si2drStringT("power_lut_template"), &err);
        if (!__si2drObjectIsNull(rg2, &err)) {
            si2drAttrIdT ind1, ind2, ind3;
            si2drAttrIdT vs;
            ind1 = __si2drGroupFindAttrByName(group, si2drStringT("index_1"),
                                              &err);
            ind2 = __si2drGroupFindAttrByName(group, si2drStringT("index_2"),
                                              &err);
            ind3 = __si2drGroupFindAttrByName(group, si2drStringT("index_3"),
                                              &err);
            if (__si2drObjectIsNull(ind1, &err))
                ind1 = __si2drGroupFindAttrByName(rg2, si2drStringT("index_1"),
                                                  &err);
            if (__si2drObjectIsNull(ind2, &err))
                ind2 = __si2drGroupFindAttrByName(rg2, si2drStringT("index_2"),
                                                  &err);
            if (__si2drObjectIsNull(ind3, &err))
                ind3 = __si2drGroupFindAttrByName(rg2, si2drStringT("index_3"),
                                                  &err);
            vs =
                __si2drGroupFindAttrByName(group, si2drStringT("values"), &err);
            if (!__si2drObjectIsNull(ind1, &err) &&
                !__si2drObjectIsNull(ind2, &err) &&
                !__si2drObjectIsNull(ind3, &err)) {
                if (!__si2drObjectIsNull(vs, &err)) {
                    int tpl1nums, tpl2nums, tpl3nums;
                    int numstrs = __countStringsInList(vs);
                    si2drValuesIdT vals;
                    si2drValueTypeT type;
                    si2drInt32T intgr;
                    si2drFloat64T float64;
                    si2drStringT string;
                    si2drBooleanT boolean;
                    si2drExprT *expr;
                    vals = __si2drComplexAttrGetValues(ind1, &err);
                    __si2drIterNextComplexValue(vals, &type, &intgr, &float64,
                                                &string, &boolean, &expr, &err);
                    __si2drIterQuit(vals, &err);
                    if (type == kSI2DR_STRING) {
                        tpl1nums = __countFloatsInString(string);
                        vals = __si2drComplexAttrGetValues(ind2, &err);
                        __si2drIterNextComplexValue(vals, &type, &intgr,
                                                    &float64, &string, &boolean,
                                                    &expr, &err);
                        __si2drIterQuit(vals, &err);
                        if (type == kSI2DR_STRING) {
                            tpl2nums = __countFloatsInString(string);
                            vals = __si2drComplexAttrGetValues(ind3, &err);
                            __si2drIterNextComplexValue(vals, &type, &intgr,
                                                        &float64, &string,
                                                        &boolean, &expr, &err);
                            __si2drIterQuit(vals, &err);
                            if (type == kSI2DR_STRING) {
                                tpl3nums = __countFloatsInString(string);
                                if (numstrs == tpl1nums * tpl2nums) {
                                    /* now, count the nums in each entry, and
                                     * make sure they equal tpl2nums */
                                    int i;
                                    vals =
                                        __si2drComplexAttrGetValues(vs, &err);
                                    for (i = 0; i < numstrs; i++) {
                                        __si2drIterNextComplexValue(
                                            vals, &type, &intgr, &float64,
                                            &string, &boolean, &expr, &err);
                                        if (type == kSI2DR_STRING) {
                                            int numc =
                                                __countFloatsInString(string);
                                            if (numc != tpl3nums) {
                                                char EB[8000];
                                                snprintf(
                                                    EB, sizeof(EB),
                                                    "%s:%d, %s references "
                                                    "template %s,",
                                                    __si2drObjectGetFileName(
                                                        group, &err),
                                                    static_cast<int>(
                                                        __si2drObjectGetLineNo(
                                                            group, &err)),
                                                    __si2drGroupGetGroupType(
                                                        group, &err),
                                                    strx);
                                                (this->*si2ErrMsg_)(
                                                    kSI2DR_SEVERITY_ERR,
                                                    kSI2DR_SEMANTIC_ERROR, EB,
                                                    &err);
                                                snprintf(
                                                    EB, sizeof(EB),
                                                    "                  which "
                                                    "says there should be %d "
                                                    "strings, each with %d "
                                                    "numbers in it in the "
                                                    "values table,",
                                                    tpl1nums * tpl2nums,
                                                    tpl3nums);
                                                (this->*si2ErrMsg_)(
                                                    kSI2DR_SEVERITY_ERR,
                                                    kSI2DR_SEMANTIC_ERROR, EB,
                                                    &err);
                                                snprintf(
                                                    EB, sizeof(EB),
                                                    "                  but in "
                                                    "the group's values "
                                                    "attribute (ending at line "
                                                    "%d) value #%d has %d "
                                                    "numbers.",
                                                    static_cast<int>(
                                                        __si2drObjectGetLineNo(
                                                            vs, &err)),
                                                    i + 1, numc);
                                                (this->*si2ErrMsg_)(
                                                    kSI2DR_SEVERITY_ERR,
                                                    kSI2DR_SEMANTIC_ERROR, EB,
                                                    &err);
                                                errcount_++;
                                            }
                                        } else {
                                            char EB[8000];
                                            snprintf(EB, sizeof(EB),
                                                     "%s:%d, %s references "
                                                     "template %s,",
                                                     __si2drObjectGetFileName(
                                                         group, &err),
                                                     static_cast<int>(
                                                         __si2drObjectGetLineNo(
                                                             group, &err)),
                                                     __si2drGroupGetGroupType(
                                                         group, &err),
                                                     strx);
                                            (this->*si2ErrMsg_)(
                                                kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                                            snprintf(
                                                EB, sizeof(EB),
                                                "                  which says "
                                                "there should be %d strings, "
                                                "each with %d numbers in it in "
                                                "the values table,",
                                                tpl1nums * tpl2nums, tpl3nums);
                                            (this->*si2ErrMsg_)(
                                                kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                                            snprintf(
                                                EB, sizeof(EB),
                                                "                  but in the "
                                                "group's values attribute "
                                                "(ending at line %d) value #%d "
                                                "is not a string.",
                                                static_cast<int>(
                                                    __si2drObjectGetLineNo(
                                                        vs, &err)),
                                                i + 1);
                                            (this->*si2ErrMsg_)(
                                                kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                                            errcount_++;
                                        }
                                    }
                                    __si2drIterQuit(vals, &err);
                                } else {
                                    char EB[8000];
                                    snprintf(
                                        EB, sizeof(EB),
                                        "%s:%d, %s references template %s,",
                                        __si2drObjectGetFileName(group, &err),
                                        static_cast<int>(__si2drObjectGetLineNo(
                                            group, &err)),
                                        __si2drGroupGetGroupType(group, &err),
                                        strx);
                                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                        kSI2DR_SEMANTIC_ERROR,
                                                        EB, &err);
                                    snprintf(
                                        EB, sizeof(EB),
                                        "                  which says there "
                                        "should be %d strings, each with %d "
                                        "numbers in it in the values table,",
                                        tpl1nums * tpl2nums, tpl3nums);
                                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                        kSI2DR_SEMANTIC_ERROR,
                                                        EB, &err);
                                    snprintf(
                                        EB, sizeof(EB),
                                        "                  but the group's "
                                        "values attribute (line %d) has %d "
                                        "strings.",
                                        static_cast<int>(
                                            __si2drObjectGetLineNo(vs, &err)),
                                        numstrs);
                                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                        kSI2DR_SEMANTIC_ERROR,
                                                        EB, &err);
                                    errcount_++;
                                }
                            } else {
                                char EB[8000];
                                snprintf(
                                    EB, sizeof(EB),
                                    "%s:%d, %s references template %s, but",
                                    __si2drObjectGetFileName(group, &err),
                                    static_cast<int>(
                                        __si2drObjectGetLineNo(group, &err)),
                                    __si2drGroupGetGroupType(group, &err),
                                    strx);
                                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                    kSI2DR_SYNTAX_ERROR, EB,
                                                    &err);
                                snprintf(
                                    EB, sizeof(EB),
                                    "                  the template's "
                                    "index_3 attribute (line %d) does not "
                                    "have a string as a value.",
                                    static_cast<int>(
                                        __si2drObjectGetLineNo(ind2, &err)));
                                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                    kSI2DR_SYNTAX_ERROR, EB,
                                                    &err);
                                errcount_++;
                            }
                        } else {
                            char EB[8000];
                            snprintf(EB, sizeof(EB),
                                     "%s:%d, %s references template %s, but",
                                     __si2drObjectGetFileName(group, &err),
                                     static_cast<int>(
                                         __si2drObjectGetLineNo(group, &err)),
                                     __si2drGroupGetGroupType(group, &err),
                                     strx);
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SYNTAX_ERROR, EB, &err);
                            snprintf(EB, sizeof(EB),
                                     "                  the template's index_2 "
                                     "attribute (line %d) does not have a "
                                     "string as a value.",
                                     static_cast<int>(
                                         __si2drObjectGetLineNo(ind2, &err)));
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SYNTAX_ERROR, EB, &err);
                            errcount_++;
                        }
                    } else {
                        char EB[8000];
                        snprintf(EB, sizeof(EB),
                                 "%s:%d, %s references template %s, but",
                                 __si2drObjectGetFileName(group, &err),
                                 static_cast<int>(
                                     __si2drObjectGetLineNo(group, &err)),
                                 __si2drGroupGetGroupType(group, &err), strx);
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SYNTAX_ERROR, EB, &err);
                        snprintf(
                            EB, sizeof(EB),
                            "                  the template's index_1 "
                            "attribute (line %d) does not have a string as "
                            "a value.",
                            static_cast<int>(
                                __si2drObjectGetLineNo(ind1, &err)));
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SYNTAX_ERROR, EB, &err);
                        errcount_++;
                    }
                } else {
                    char EB[8000];
                    snprintf(
                        EB, sizeof(EB), "%s:%d, %s references template %s, but",
                        __si2drObjectGetFileName(group, &err),
                        static_cast<int>(__si2drObjectGetLineNo(group, &err)),
                        __si2drGroupGetGroupType(group, &err), strx);
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SYNTAX_ERROR, EB, &err);
                    snprintf(EB, sizeof(EB),
                             "                  the group does not have a "
                             "values attribute.");
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SYNTAX_ERROR, EB, &err);
                    errcount_++;
                }
            } else if (!__si2drObjectIsNull(ind1, &err) &&
                       !__si2drObjectIsNull(ind2, &err) &&
                       __si2drObjectIsNull(ind3, &err)) {
                if (!__si2drObjectIsNull(vs, &err)) {
                    int tpl1nums, tpl2nums;
                    int numstrs = __countStringsInList(vs);
                    si2drValuesIdT vals;
                    si2drValueTypeT type;
                    si2drInt32T intgr;
                    si2drFloat64T float64;
                    si2drStringT string;
                    si2drBooleanT boolean;
                    si2drExprT *expr;
                    vals = __si2drComplexAttrGetValues(ind1, &err);
                    __si2drIterNextComplexValue(vals, &type, &intgr, &float64,
                                                &string, &boolean, &expr, &err);
                    __si2drIterQuit(vals, &err);
                    if (type == kSI2DR_STRING) {
                        tpl1nums = __countFloatsInString(string);
                        vals = __si2drComplexAttrGetValues(ind2, &err);
                        __si2drIterNextComplexValue(vals, &type, &intgr,
                                                    &float64, &string, &boolean,
                                                    &expr, &err);
                        __si2drIterQuit(vals, &err);
                        if (type == kSI2DR_STRING) {
                            tpl2nums = __countFloatsInString(string);
                            if (numstrs == tpl1nums) {
                                /* now, count the nums in each entry, and make
                                 * sure they equal tpl2nums */
                                int i;
                                vals = __si2drComplexAttrGetValues(vs, &err);
                                for (i = 0; i < numstrs; i++) {
                                    __si2drIterNextComplexValue(
                                        vals, &type, &intgr, &float64, &string,
                                        &boolean, &expr, &err);
                                    if (type == kSI2DR_STRING) {
                                        int numc =
                                            __countFloatsInString(string);
                                        if (numc != tpl2nums) {
                                            char EB[8000];
                                            snprintf(EB, sizeof(EB),
                                                     "%s:%d, %s references "
                                                     "template %s,",
                                                     __si2drObjectGetFileName(
                                                         group, &err),
                                                     static_cast<int>(
                                                         __si2drObjectGetLineNo(
                                                             group, &err)),
                                                     __si2drGroupGetGroupType(
                                                         group, &err),
                                                     strx);
                                            (this->*si2ErrMsg_)(
                                                kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                                            snprintf(
                                                EB, sizeof(EB),
                                                "                  which says "
                                                "there should be %d strings, "
                                                "each with %d numbers in it in "
                                                "the values table,",
                                                tpl1nums, tpl2nums);
                                            (this->*si2ErrMsg_)(
                                                kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                                            snprintf(
                                                EB, sizeof(EB),
                                                "                  but in the "
                                                "group's values attribute "
                                                "(ending at line %d) value #%d "
                                                "has %d numbers.",
                                                static_cast<int>(
                                                    __si2drObjectGetLineNo(
                                                        vs, &err)),
                                                i + 1, numc);
                                            (this->*si2ErrMsg_)(
                                                kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                                            errcount_++;
                                        }
                                    } else {
                                        char EB[8000];
                                        snprintf(
                                            EB, sizeof(EB),
                                            "%s:%d, %s references template %s,",
                                            __si2drObjectGetFileName(group,
                                                                     &err),
                                            static_cast<int>(
                                                __si2drObjectGetLineNo(group,
                                                                       &err)),
                                            __si2drGroupGetGroupType(group,
                                                                     &err),
                                            strx);
                                        (this->*si2ErrMsg_)(
                                            kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                                        snprintf(
                                            EB, sizeof(EB),
                                            "                  which says "
                                            "there should be %d strings, "
                                            "each with %d numbers in it in "
                                            "the values table,",
                                            tpl1nums, tpl2nums);
                                        (this->*si2ErrMsg_)(
                                            kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                                        snprintf(
                                            EB, sizeof(EB),
                                            "                  but in the "
                                            "group's values attribute (ending "
                                            "at line %d) value #%d is not a "
                                            "string.",
                                            static_cast<int>(
                                                __si2drObjectGetLineNo(vs,
                                                                       &err)),
                                            i + 1);
                                        (this->*si2ErrMsg_)(
                                            kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                                        errcount_++;
                                    }
                                }
                                __si2drIterQuit(vals, &err);
                            } else {
                                char EB[8000];
                                snprintf(
                                    EB, sizeof(EB),
                                    "%s:%d, %s references template %s,",
                                    __si2drObjectGetFileName(group, &err),
                                    static_cast<int>(
                                        __si2drObjectGetLineNo(group, &err)),
                                    __si2drGroupGetGroupType(group, &err),
                                    strx);
                                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                    kSI2DR_SEMANTIC_ERROR, EB,
                                                    &err);
                                snprintf(EB, sizeof(EB),
                                         "                  which says there "
                                         "should be %d strings, each with %d "
                                         "numbers in it in the values table,",
                                         tpl1nums, tpl2nums);
                                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                    kSI2DR_SEMANTIC_ERROR, EB,
                                                    &err);
                                snprintf(
                                    EB, sizeof(EB),
                                    "                  but the group's values "
                                    "attribute (line %d) has %d strings.",
                                    static_cast<int>(
                                        __si2drObjectGetLineNo(vs, &err)),
                                    numstrs);
                                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                    kSI2DR_SEMANTIC_ERROR, EB,
                                                    &err);
                                errcount_++;
                            }
                        } else {
                            char EB[8000];
                            snprintf(EB, sizeof(EB),
                                     "%s:%d, %s references template %s, but",
                                     __si2drObjectGetFileName(group, &err),
                                     static_cast<int>(
                                         __si2drObjectGetLineNo(group, &err)),
                                     __si2drGroupGetGroupType(group, &err),
                                     strx);
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SYNTAX_ERROR, EB, &err);
                            snprintf(EB, sizeof(EB),
                                     "                  the template's index_2 "
                                     "attribute (line %d) does not have a "
                                     "string as a value.",
                                     static_cast<int>(
                                         __si2drObjectGetLineNo(ind2, &err)));
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SYNTAX_ERROR, EB, &err);
                            errcount_++;
                        }
                    } else {
                        char EB[8000];
                        snprintf(EB, sizeof(EB),
                                 "%s:%d, %s references template %s, but",
                                 __si2drObjectGetFileName(group, &err),
                                 static_cast<int>(
                                     __si2drObjectGetLineNo(group, &err)),
                                 __si2drGroupGetGroupType(group, &err), strx);
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SYNTAX_ERROR, EB, &err);
                        snprintf(
                            EB, sizeof(EB),
                            "                  the template's index_1 "
                            "attribute (line %d) does not have a string as "
                            "a value.",
                            static_cast<int>(
                                __si2drObjectGetLineNo(ind1, &err)));
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SYNTAX_ERROR, EB, &err);
                        errcount_++;
                    }
                } else {
                    char EB[8000];
                    snprintf(
                        EB, sizeof(EB), "%s:%d, %s references template %s, but",
                        __si2drObjectGetFileName(group, &err),
                        static_cast<int>(__si2drObjectGetLineNo(group, &err)),
                        __si2drGroupGetGroupType(group, &err), strx);
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                    snprintf(EB, sizeof(EB),
                             "                  the group does not have a "
                             "values attribute.");
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                    errcount_++;
                }
            } else {
                if (!__si2drObjectIsNull(ind1, &err) &&
                    __si2drObjectIsNull(ind2, &err)) {
                    if (!__si2drObjectIsNull(vs, &err)) {
                        int tpl1nums;
                        int numstrs = __countStringsInList(vs);
                        si2drValuesIdT vals;
                        si2drValueTypeT type;
                        si2drInt32T intgr;
                        si2drFloat64T float64;
                        si2drStringT string;
                        si2drBooleanT boolean;
                        si2drExprT *expr;
                        if (numstrs == 1) {
                            vals = __si2drComplexAttrGetValues(ind1, &err);
                            __si2drIterNextComplexValue(vals, &type, &intgr,
                                                        &float64, &string,
                                                        &boolean, &expr, &err);
                            __si2drIterQuit(vals, &err);
                            if (type == kSI2DR_STRING) {
                                int i;
                                tpl1nums = __countFloatsInString(string);
                                /* now, count the nums in each entry, and make
                                 * sure they equal tpl2nums */
                                vals = __si2drComplexAttrGetValues(vs, &err);
                                for (i = 0; i < numstrs; i++) {
                                    __si2drIterNextComplexValue(
                                        vals, &type, &intgr, &float64, &string,
                                        &boolean, &expr, &err);
                                    if (type == kSI2DR_STRING) {
                                        int numc =
                                            __countFloatsInString(string);
                                        if (numc != tpl1nums) {
                                            char EB[8000];
                                            snprintf(EB, sizeof(EB),
                                                     "%s:%d, %s references "
                                                     "template %s,",
                                                     __si2drObjectGetFileName(
                                                         group, &err),
                                                     static_cast<int>(
                                                         __si2drObjectGetLineNo(
                                                             group, &err)),
                                                     __si2drGroupGetGroupType(
                                                         group, &err),
                                                     strx);
                                            (this->*si2ErrMsg_)(
                                                kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                                            snprintf(
                                                EB, sizeof(EB),
                                                "                  which says "
                                                "there should be 1 string, "
                                                "with %d numbers in it in the "
                                                "values table,",
                                                tpl1nums);
                                            (this->*si2ErrMsg_)(
                                                kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                                            snprintf(
                                                EB, sizeof(EB),
                                                "                  but in "
                                                "the group's values "
                                                "attribute (ending at line "
                                                "%d) has %d numbers.",
                                                static_cast<int>(
                                                    __si2drObjectGetLineNo(
                                                        vs, &err)),
                                                numc);
                                            (this->*si2ErrMsg_)(
                                                kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                                            errcount_++;
                                        }
                                    } else {
                                        char EB[8000];
                                        snprintf(
                                            EB, sizeof(EB),
                                            "%s:%d, %s references template %s,",
                                            __si2drObjectGetFileName(group,
                                                                     &err),
                                            static_cast<int>(
                                                __si2drObjectGetLineNo(group,
                                                                       &err)),
                                            __si2drGroupGetGroupType(group,
                                                                     &err),
                                            strx);
                                        (this->*si2ErrMsg_)(
                                            kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                                        snprintf(EB, sizeof(EB),
                                                 "                  which says "
                                                 "there should be 1 string, "
                                                 "with %d numbers in it in the "
                                                 "values table,",
                                                 tpl1nums);
                                        (this->*si2ErrMsg_)(
                                            kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                                        snprintf(
                                            EB, sizeof(EB),
                                            "                  but in the "
                                            "group's values attribute (ending "
                                            "at line %d) is not a string.",
                                            static_cast<int>(
                                                __si2drObjectGetLineNo(vs,
                                                                       &err)));
                                        (this->*si2ErrMsg_)(
                                            kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                                        errcount_++;
                                    }
                                }
                                __si2drIterQuit(vals, &err);
                            } else {
                                char EB[8000];
                                snprintf(
                                    EB, sizeof(EB),
                                    "%s:%d, %s references template %s, but",
                                    __si2drObjectGetFileName(group, &err),
                                    static_cast<int>(
                                        __si2drObjectGetLineNo(group, &err)),
                                    __si2drGroupGetGroupType(group, &err),
                                    strx);
                                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                    kSI2DR_SYNTAX_ERROR, EB,
                                                    &err);
                                snprintf(
                                    EB, sizeof(EB),
                                    "                  the template's "
                                    "index_1 attribute (line %d) does not "
                                    "have a string as a value.",
                                    static_cast<int>(
                                        __si2drObjectGetLineNo(ind1, &err)));
                                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                    kSI2DR_SYNTAX_ERROR, EB,
                                                    &err);
                                errcount_++;
                            }
                        } else {
                            char EB[8000];
                            snprintf(EB, sizeof(EB),
                                     "%s:%d, %s references template %s, and "
                                     "only index_1 is defined, but",
                                     __si2drObjectGetFileName(group, &err),
                                     static_cast<int>(
                                         __si2drObjectGetLineNo(group, &err)),
                                     __si2drGroupGetGroupType(group, &err),
                                     strx);
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                            snprintf(
                                EB, sizeof(EB),
                                "                  the group has more than "
                                "one value item....");
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                            errcount_++;
                        }
                    } else {
                        char EB[8000];
                        snprintf(EB, sizeof(EB),
                                 "%s:%d, %s references template %s, but",
                                 __si2drObjectGetFileName(group, &err),
                                 static_cast<int>(
                                     __si2drObjectGetLineNo(group, &err)),
                                 __si2drGroupGetGroupType(group, &err), strx);
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                        snprintf(EB, sizeof(EB),
                                 "                  the group does not have a "
                                 "values attribute.");
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                        errcount_++;
                    }
                } else if (__si2drObjectIsNull(ind1, &err) &&
                           !__si2drObjectIsNull(ind2, &err)) {
                    char EB[8000];
                    snprintf(
                        EB, sizeof(EB),
                        "%s:%d, %s references template %s, and index_2 was "
                        "defined, but index_1 could not be found",
                        __si2drObjectGetFileName(group, &err),
                        static_cast<int>(__si2drObjectGetLineNo(group, &err)),
                        __si2drGroupGetGroupType(group, &err), strx);
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                    snprintf(
                        EB, sizeof(EB),
                        "                 in the group or the template "
                        "(line %d).",
                        static_cast<int>(__si2drObjectGetLineNo(rg2, &err)));
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                    errcount_++;
                }
                /* else ? */
            }
        } else {
            /* it could be a good 'ol polynomial */
            rg2 = __si2drGroupFindGroupByName(
                rg, strx, si2drStringT("power_poly_template"), &err);
            if (__si2drObjectIsNull(rg2, &err)) {
                char EB[8000];
                snprintf(EB, sizeof(EB),
                         "%s:%d, %s's reference to template %s: the referenced "
                         "group could not be found",
                         __si2drObjectGetFileName(group, &err),
                         static_cast<int>(__si2drObjectGetLineNo(group, &err)),
                         __si2drGroupGetGroupType(group, &err), strx);
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR,
                                    EB, &err);
                errcount_++;
            }
        }
    }
}
void LibAnalysis::__checkCellNProp(si2drGroupIdT group) {
    si2drErrorT err;
    si2drGroupsIdT gs = __si2drGroupGetGroups(group, &err);
    si2drGroupIdT g2;
    group_enum gt;
    int gcell_rise = 0, gcell_fall = 0, gprop_fall = 0, gprop_rise = 0;
    int gcell_rise_ln, gcell_fall_ln, gprop_fall_ln, gprop_rise_ln;
    char *gcell_rise_fi, *gcell_fall_fi, *gprop_fall_fi, *gprop_rise_fi;
    int gtran_rise = 0, gtran_fall = 0;
    // int gtran_rise_ln, gtran_fall_ln;
    // char *gtran_rise_fi, *gtran_fall_fi;
    while (!__si2drObjectIsNull((g2 = __si2drIterNextGroup(gs, &err)), &err)) {
        gt = __si2drGroupGetID(g2, &err);
        switch (gt) {
            case kTIMINGLIB_GROUPENUM_cell_rise:
                gcell_rise++;
                gcell_rise_ln = __si2drObjectGetLineNo(g2, &err);
                gcell_rise_fi = __si2drObjectGetFileName(g2, &err);
                break;
            case kTIMINGLIB_GROUPENUM_cell_fall:
                gcell_fall++;
                gcell_fall_ln = __si2drObjectGetLineNo(g2, &err);
                gcell_fall_fi = __si2drObjectGetFileName(g2, &err);
                break;
            case kTIMINGLIB_GROUPENUM_rise_propagation:
                gprop_rise++;
                gprop_rise_ln = __si2drObjectGetLineNo(g2, &err);
                gprop_rise_fi = __si2drObjectGetFileName(g2, &err);
                break;
            case kTIMINGLIB_GROUPENUM_fall_propagation:
                gprop_fall++;
                gprop_fall_ln = __si2drObjectGetLineNo(g2, &err);
                gprop_fall_fi = __si2drObjectGetFileName(g2, &err);
                break;
            case kTIMINGLIB_GROUPENUM_rise_transition:
                gtran_rise++;
                // gtran_rise_ln = __si2drObjectGetLineNo(g2,&err);
                // gtran_rise_fi = __si2drObjectGetFileName(g2,&err);
                break;
            case kTIMINGLIB_GROUPENUM_fall_transition:
                gtran_fall++;
                // gtran_fall_ln = __si2drObjectGetLineNo(g2,&err);
                // gtran_fall_fi = __si2drObjectGetFileName(g2,&err);
                break;
            default:
                break;
        }
    }
    __si2drIterQuit(gs, &err);
    if (gcell_rise && gprop_fall) {
        char EB[8000];
        snprintf(EB, sizeof(EB),
                 "%s:%d and %s:%d: It is illegal to specify both cell_rise and "
                 "fall_propagation in the same timing group.",
                 gcell_rise_fi, gcell_rise_ln, gprop_fall_fi, gprop_fall_ln);
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                            &err);
        errcount_++;
    }
    if (gcell_fall && gprop_rise) {
        char EB[8000];
        snprintf(EB, sizeof(EB),
                 "%s:%d and %s:%d: It is illegal to specify both cell_fall and "
                 "rise_propagation in the same timing group.",
                 gcell_fall_fi, gcell_fall_ln, gprop_rise_fi, gprop_rise_ln);
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                            &err);
        errcount_++;
    }
    if (gcell_rise && !gtran_rise) {
        char EB[8000];
        snprintf(EB, sizeof(EB),
                 "%s:%d: It is illegal to specify cell_rise without a "
                 "rise_transition in the same timing group.",
                 gcell_rise_fi, gcell_rise_ln);
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                            &err);
        errcount_++;
    }
    if (gcell_fall && !gtran_fall) {
        char EB[8000];
        snprintf(EB, sizeof(EB),
                 "%s:%d: It is illegal to specify cell_fall without a "
                 "fall_transition in the same timing group.",
                 gcell_fall_fi, gcell_fall_ln);
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                            &err);
        errcount_++;
    }
}
void LibAnalysis::__checkClearNPreset(si2drGroupIdT group) {
    si2drErrorT err;
    si2drAttrIdT clear, preset, var1, var2;
    clear = __si2drGroupFindAttrByName(group, si2drStringT("clear"), &err);
    preset = __si2drGroupFindAttrByName(group, si2drStringT("preset"), &err);
    if (!__si2drObjectIsNull(clear, &err) &&
        !__si2drObjectIsNull(preset, &err)) {
        var1 = __si2drGroupFindAttrByName(
            group, si2drStringT("clear_preset_var1"), &err);
        var2 = __si2drGroupFindAttrByName(
            group, si2drStringT("clear_preset_var2"), &err);
        if (__si2drObjectIsNull(var1, &err) &&
            __si2drObjectIsNull(var2, &err)) {
            char EB[8000];
            snprintf(EB, sizeof(EB),
                     "%s:%d, Group %s, with both 'clear', and 'preset' "
                     "attributes,\nrequires 'clear_preset_var1' and/or "
                     "'clear_preset_var2' attributes.",
                     __si2drObjectGetFileName(group, &err),
                     static_cast<int>(__si2drObjectGetLineNo(group, &err)),
                     __si2drGroupGetGroupType(group, &err));
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                                &err);
            errcount_++;
        }
    }
}
void LibAnalysis::__checkMembers(si2drGroupIdT group) {
    /* is members specified? Is it first? */
    si2drErrorT err;
    si2drAttrIdT mem =
        __si2drGroupFindAttrByName(group, si2drStringT("members"), &err);
    int faln, fgln, mln;
    if (!__si2drObjectIsNull(mem, &err)) {
        /* a member attr is present */
        si2drAttrsIdT attrs;
        si2drAttrIdT attr;
        si2drGroupsIdT groups;
        si2drGroupIdT g2;
        mln = __si2drObjectGetLineNo(mem, &err);
        attrs = __si2drGroupGetAttrs(group, &err);
        attr = __si2drIterNextAttr(attrs, &err);
        __si2drIterQuit(attrs, &err);
        if (!__si2drObjectIsNull(attr, &err)) {
            faln = __si2drObjectGetLineNo(attr, &err);
        } else {
            faln = -1;
        }
        groups = __si2drGroupGetGroups(group, &err);
        g2 = __si2drIterNextGroup(groups, &err);
        __si2drIterQuit(groups, &err);
        if (!__si2drObjectIsNull(g2, &err)) {
            fgln = __si2drObjectGetLineNo(g2, &err);
        } else {
            fgln = -1;
        }
        if (!__si2drObjectIsSame(mem, attr, &err) || (fgln > 0 && fgln < mln)) {
            si2drNamesIdT names;
            si2drStringT name, attrname;
            names = __si2drGroupGetNames(group, &err);
            name = __si2drIterNextName(names, &err);
            __si2drIterQuit(names, &err);
            /* [nanda:20060824] Fix incorrect check for NULL name */
            /* if( !name && !name[0] ) */
            if (!name || !name[0]) name = si2drStringT("<Unnamed>");
            if (fgln > 0 && fgln < mln) {
                si2drNamesIdT gnames;
                si2drStringT gname;
                gnames = __si2drGroupGetNames(g2, &err);
                gname = __si2drIterNextName(gnames, &err);
                __si2drIterQuit(gnames, &err);
                char EB[8000];
                snprintf(EB, sizeof(EB),
                         "%s:%d, The %s bundle needs to have its 'members' "
                         "specified first.\nThe group %s (line %d) is found "
                         "before 'members'.",
                         __si2drObjectGetFileName(mem, &err),
                         static_cast<int>(__si2drObjectGetLineNo(mem, &err),
                                          name, gname, fgln));
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR,
                                    EB, &err);
            }
            if (!__si2drObjectIsSame(mem, attr, &err)) {
                attrname = __si2drAttrGetName(attr, &err);
                char EB[8000];
                snprintf(EB, sizeof(EB),
                         "%s:%d, The %s bundle needs to have its 'members' "
                         "specified first.\nThe attr %s (line %d) is found "
                         "before 'members'.",
                         __si2drObjectGetFileName(mem, &err),
                         static_cast<int>(__si2drObjectGetLineNo(mem, &err),
                                          name, attrname, faln));
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR,
                                    EB, &err);
            }
            errcount_++;
        }
    }
}
void LibAnalysis::__checkInterfaceTiming(si2drGroupIdT group) {
    si2drErrorT err;
    si2drAttrIdT ift;
    si2drGroupsIdT groups;
    si2drGroupIdT g2;
    group_enum gt;
    si2drNamesIdT names;
    si2drStringT name;
    int iftn;
    si2drAttrIdT attr;
    names = __si2drGroupGetNames(group, &err);
    name = __si2drIterNextName(names, &err);
    __si2drIterQuit(names, &err);
    ift = __si2drGroupFindAttrByName(group, si2drStringT("interface_timing"),
                                     &err);
    if (!__si2drObjectIsNull(ift, &err)) {
        si2drExprT *e;
        si2drErrorT err2x;
        si2drBooleanT iftv = __si2drSimpleAttrGetBooleanValue(ift, &err2x);
        if (__si2drSimpleAttrGetValueType(ift, &err) == kSI2DR_EXPR) {
            e = __si2drSimpleAttrGetExprValue(ift, &err);
            if (e->type == kSI2DR_EXPR_VAL && e->valuetype == kSI2DR_BOOLEAN) {
                iftv = e->u.b;
            } else {
                err = err2x;
            }
        }
        if (err == kSI2DR_NO_ERROR && iftv == SI2DR_TRUE) {
            iftn = __si2drObjectGetLineNo(ift, &err);
            /* check to make sure the stuff is as it should be... */
            groups = __si2drGroupGetGroups(group, &err);
            while (!__si2drObjectIsNull(
                (g2 = __si2drIterNextGroup(groups, &err)), &err)) {
                gt = __si2drGroupGetID(g2, &err);
                switch (gt) {
                    case kTIMINGLIB_GROUPENUM_ff:
                        char EB[8000];
                        snprintf(
                            EB, sizeof(EB),
                            "%s:%d, The cell '%s' has the interface_timing "
                            "attribute (line %d), but is not a black box.",
                            __si2drObjectGetFileName(g2, &err),
                            static_cast<int>(__si2drObjectGetLineNo(g2, &err),
                                             name, iftn));
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                        snprintf(EB, sizeof(EB),
                                 "                  -- The ff group should not "
                                 "be specified when interface_timing is true.");
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                        errcount_++;
                        break;
                    case kTIMINGLIB_GROUPENUM_pin:
                        attr = __si2drGroupFindAttrByName(
                            g2, si2drStringT("function"), &err);
                        if (!__si2drObjectIsNull(attr, &err)) {
                            char EB[8000];
                            snprintf(
                                EB, sizeof(EB),
                                "%s:%d, The cell '%s' has the interface_timing "
                                "attribute (line %d), but is not a black box.",
                                __si2drObjectGetFileName(attr, &err),
                                static_cast<int>(
                                    __si2drObjectGetLineNo(attr, &err), name,
                                    iftn));
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                            snprintf(
                                EB, sizeof(EB),
                                "                  -- pin groups should "
                                "not have the function attribute specified "
                                "when interface_timing is true.");
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                            errcount_++;
                        }
                        break;
                    default:
                        break;
                }
            }
            __si2drIterQuit(groups, &err);
        }
    }
}
si2drStringT LibAnalysis::__getFirstGroupName(si2drGroupIdT group) {
    si2drErrorT err;
    si2drStringT str;
    si2drNamesIdT names;
    names = __si2drGroupGetNames(group, &err);
    str = __si2drIterNextName(names, &err);
    __si2drIterQuit(names, &err);
    return str;
}
void LibAnalysis::__checkBusPinDirections(
    si2drGroupIdT group) { /* group is bus or bundle */
    si2drErrorT err;
    si2drAttrIdT bdir, pdir;
    group_enum gt;
    char buf[100];
    si2drStringT dirstr;
    bdir = __si2drGroupFindAttrByName(group, si2drStringT("direction"), &err);
    if (!__si2drObjectIsNull(bdir, &err)) {
        si2drGroupIdT g2;
        si2drGroupsIdT gs = __si2drGroupGetGroups(group, &err);
        si2drValueTypeT vt = __si2drSimpleAttrGetValueType(bdir, &err);
        dirstr = __si2drSimpleAttrGetStringValue(bdir, &err);
        if (!dirstr || !dirstr[0]) {
            if (vt == kSI2DR_EXPR) {
                si2drExprT *e = __si2drSimpleAttrGetExprValue(bdir, &err);
                if (e->type != kSI2DR_EXPR_VAL)
                    return; /* if it is an expr, we may need to evaluate it? */
                if (e->valuetype == kSI2DR_STRING)
                    dirstr = e->u.s;
                else
                    return;
            } else {
                return;
            }
        }
        snprintf(buf, sizeof(buf), "%s", dirstr);
        while (
            !__si2drObjectIsNull((g2 = __si2drIterNextGroup(gs, &err)), &err)) {
            gt = __si2drGroupGetID(g2, &err);
            if (gt == kTIMINGLIB_GROUPENUM_pin) {
                pdir = __si2drGroupFindAttrByName(g2, si2drStringT("direction"),
                                                  &err);
                if (!__si2drObjectIsNull(pdir, &err)) {
                    vt = __si2drSimpleAttrGetValueType(pdir, &err);
                    dirstr = __si2drSimpleAttrGetStringValue(pdir, &err);
                    if (!dirstr || !dirstr[0]) {
                        if (vt == kSI2DR_EXPR) {
                            si2drExprT *e =
                                __si2drSimpleAttrGetExprValue(pdir, &err);
                            if (e->type != kSI2DR_EXPR_VAL) {
                                continue;  // if it is an expr, we may need to
                                           // evaluate it?
                            }
                            if (e->valuetype == kSI2DR_STRING)
                                dirstr = e->u.s;
                            else
                                continue;
                        } else {
                            continue;
                        }
                    }
                    if (!dirstr || !dirstr[0]) continue;
                    if (strcmp(buf, dirstr)) {
                        char EB[8000];
                        snprintf(EB, sizeof(EB),
                                 "%s:%d, The group '%s' has the direction "
                                 "attribute (line %d) '%s', but",
                                 __si2drObjectGetFileName(pdir, &err),
                                 static_cast<int>(
                                     __si2drObjectGetLineNo(pdir, &err)),
                                 __getFirstGroupName(group),
                                 static_cast<int>(
                                     __si2drObjectGetLineNo(bdir, &err)),
                                 buf);
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                        snprintf(EB, sizeof(EB),
                                 "                  the pin '%s' has the "
                                 "conflicting direction attribute '%s'.",
                                 __getFirstGroupName(g2), dirstr);
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                        errcount_++;
                    }
                }
            }
        }
        __si2drIterQuit(gs, &err);
    }
}
void LibAnalysis::__checkDriverTypes(
    si2drGroupIdT
        group) { /* group should be a pin, perhaps even bus or bundle  */
    int pullup = 0, pulldown = 0, bushold = 0, opensource = 0, opendrain = 0;
    si2drAttrIdT drivertype;
    si2drAttrIdT direct;
    si2drErrorT err;
    si2drStringT str;
    drivertype =
        __si2drGroupFindAttrByName(group, si2drStringT("driver_type"), &err);
    if (__si2drObjectIsNull(
            drivertype, &err)) /* if nothing's there, nothing could be wrong! */
        return;
    if (__si2drAttrGetAttrType(drivertype, &err) != kSI2DR_SIMPLE) return;
    if (__si2drSimpleAttrGetValueType(drivertype, &err) != kSI2DR_STRING)
        return;
    str = __si2drSimpleAttrGetStringValue(drivertype, &err);
    if (!str || !str[0]) return;
    /* we can do checks now on the drivertype combinations */
    if (strstr(str, "pull_up")) pullup = 1;
    if (strstr(str, "pull_down")) pulldown = 1;
    if (strstr(str, "open_source")) opensource = 1;
    if (strstr(str, "open_drain")) opendrain = 1;
    if (strstr(str, "bus_hold")) bushold = 1;
    if (pullup && pulldown) {
        char EB[8000];
        snprintf(EB, sizeof(EB),
                 "%s:%d, The driver_types 'pull_up' and 'pull_down' cannot be "
                 "specified on the same pin",
                 __si2drObjectGetFileName(drivertype, &err),
                 static_cast<int>(__si2drObjectGetLineNo(drivertype, &err)));
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                            &err);
        errcount_++;
    }
    if (pullup && bushold) {
        char EB[8000];
        snprintf(EB, sizeof(EB),
                 "%s:%d, The driver_types 'pull_up' and 'bus_hold' cannot be "
                 "specified on the same pin",
                 __si2drObjectGetFileName(drivertype, &err),
                 static_cast<int>(__si2drObjectGetLineNo(drivertype, &err)));
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                            &err);
        errcount_++;
    }
    if (pulldown && bushold) {
        char EB[8000];
        snprintf(EB, sizeof(EB),
                 "%s:%d, The driver_types 'pull_down' and 'bus_hold' cannot be "
                 "specified on the same pin",
                 __si2drObjectGetFileName(drivertype, &err),
                 static_cast<int>(__si2drObjectGetLineNo(drivertype, &err)));
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                            &err);
        errcount_++;
    }
    if (opendrain && opensource) {
        char EB[8000];
        snprintf(
            EB, sizeof(EB),
            "%s:%d, The driver_types 'open_source' and 'open_drain' cannot "
            "be specified on the same pin",
            __si2drObjectGetFileName(drivertype, &err),
            static_cast<int>(__si2drObjectGetLineNo(drivertype, &err)));
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                            &err);
        errcount_++;
    }
    if (opendrain && bushold) {
        char EB[8000];
        snprintf(
            EB, sizeof(EB),
            "%s:%d, The driver_types 'open_drain' and 'bus_hold' cannot be "
            "specified on the same pin",
            __si2drObjectGetFileName(drivertype, &err),
            static_cast<int>(__si2drObjectGetLineNo(drivertype, &err)));
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                            &err);
        errcount_++;
    }
    if (opensource && bushold) {
        char EB[8000];
        snprintf(EB, sizeof(EB),
                 "%s:%d, The driver_types 'open_source' and 'bus_hold' cannot "
                 "be specified on the same pin",
                 __si2drObjectGetFileName(drivertype, &err),
                 static_cast<int>(__si2drObjectGetLineNo(drivertype, &err)));
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                            &err);
        errcount_++;
    }
    if (opensource || opendrain) {
        direct =
            __si2drGroupFindAttrByName(group, si2drStringT("direction"), &err);
        if (__si2drObjectIsNull(
                direct, &err)) /* if nothing's there, nothing could be wrong! */
            return;
        if (__si2drAttrGetAttrType(direct, &err) != kSI2DR_SIMPLE) return;
        if (__si2drSimpleAttrGetValueType(direct, &err) == kSI2DR_EXPR) {
            si2drExprT *e = __si2drSimpleAttrGetExprValue(direct, &err);
            if (e->type != kSI2DR_EXPR_VAL)
                return; /* if it is an expr, we may need to evaluate it? */
            if (e->valuetype == kSI2DR_STRING) str = e->u.s;
        } else if (__si2drSimpleAttrGetValueType(direct, &err) ==
                   kSI2DR_STRING) {
            str = __si2drSimpleAttrGetStringValue(direct, &err);
        } else {
            return;
        }
        if (!str || !str[0]) return;
        if (!strcmp(str, "input")) {
            if (opensource) {
                char EB[8000];
                snprintf(
                    EB, sizeof(EB),
                    "%s:%d, The 'open_source' driver_type (line %d) cannot "
                    "be specified on an input pin",
                    __si2drObjectGetFileName(direct, &err),
                    static_cast<int>(__si2drObjectGetLineNo(direct, &err)),
                    static_cast<int>(__si2drObjectGetLineNo(drivertype, &err)));
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR,
                                    EB, &err);
                errcount_++;
            } else {
                char EB[8000];
                snprintf(
                    EB, sizeof(EB),
                    "%s:%d, The 'open_drain' driver_type (line %d) cannot "
                    "be specified on an input pin",
                    __si2drObjectGetFileName(direct, &err),
                    static_cast<int>(__si2drObjectGetLineNo(direct, &err)),
                    static_cast<int>(__si2drObjectGetLineNo(drivertype, &err)));
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR,
                                    EB, &err);
                errcount_++;
            }
        }
    }
}
int LibAnalysis::__getBusSize(si2drGroupIdT group, char *busname) {
    si2drErrorT err;
    si2drGroupIdT bus =
        __si2drGroupFindGroupByName(group, busname, si2drStringT("bus"), &err);
    if (!__si2drObjectIsNull(bus, &err)) {
        si2drAttrIdT bustype =
            __si2drGroupFindAttrByName(bus, si2drStringT("bus_type"), &err);
        si2drGroupIdT typeb, libr;
        si2drAttrIdT bitfrom, bitto, bitwid;
        si2drInt32T wid = -1;
        if (!__si2drObjectIsNull(bustype, &err)) {
            si2drStringT str = NULL;
            if (__si2drAttrGetAttrType(bustype, &err) != kSI2DR_SIMPLE) {
                return -1; /* this will already be reported */
            }
            if (__si2drSimpleAttrGetValueType(bustype, &err) != kSI2DR_STRING) {
                if (__si2drSimpleAttrGetValueType(bustype, &err) ==
                    kSI2DR_EXPR) {
                    si2drExprT *e = __si2drSimpleAttrGetExprValue(bitwid, &err);
                    if (e->type != kSI2DR_EXPR_VAL) {
                        return -1;  // if it is an expr, we may need to evaluate
                                    // it?
                    }
                    if (e->valuetype != kSI2DR_STRING) str = e->u.s;
                } else {
                    return -1;
                }
            } else {
                str = __si2drSimpleAttrGetStringValue(bustype, &err);
            }
            if (!str || !str[0]) {
                return -1;
            }
            /* find the type in the cell or lib */
            typeb = __si2drGroupFindGroupByName(group, str,
                                                si2drStringT("type"), &err);
            if (__si2drObjectIsNull(typeb, &err)) {
                libr = __getContainingGroup(group, si2drStringT("library"));
                if (!__si2drObjectIsNull(libr, &err)) {
                    typeb = __si2drGroupFindGroupByName(
                        libr, str, si2drStringT("type"), &err);
                }
            }
            if (__si2drObjectIsNull(typeb, &err)) {
                return -1;
            }
            bitwid = __si2drGroupFindAttrByName(
                typeb, si2drStringT("bit_width"), &err);
            if (!__si2drObjectIsNull(bitwid, &err)) {
                si2drValueTypeT vt;
                if (__si2drAttrGetAttrType(bitwid, &err) != kSI2DR_SIMPLE) {
                    return -1; /* this will already be reported */
                }
                vt = __si2drSimpleAttrGetValueType(bitwid, &err);
                if (vt == kSI2DR_STRING) {
                    return -1;
                }
                if (vt == kSI2DR_INT32) {
                    wid = __si2drSimpleAttrGetInt32Value(bitwid, &err);
                } else if (vt == kSI2DR_FLOAT64) {
                    wid = __si2drSimpleAttrGetFloat64Value(bitwid, &err);
                } else if (vt == kSI2DR_EXPR) {
                    si2drExprT *e = __si2drSimpleAttrGetExprValue(bitwid, &err);
                    if (e->type != kSI2DR_EXPR_VAL) {
                        return -1;  // if it is an expr, we may need to evaluate
                                    // it?
                    }
                    if (e->valuetype == kSI2DR_INT32)
                        wid = e->u.i;
                    else if (e->valuetype == kSI2DR_FLOAT64)
                        wid = e->u.d;
                    else
                        return -1;
                }
            } else {
                bitfrom = __si2drGroupFindAttrByName(
                    typeb, si2drStringT("bit_from"), &err);
                bitto = __si2drGroupFindAttrByName(
                    typeb, si2drStringT("bit_to"), &err);
                if (!__si2drObjectIsNull(bitfrom, &err) &&
                    !__si2drObjectIsNull(bitto, &err)) {
                    si2drValueTypeT vt;
                    int from = 0, to = 0, big, lit;
                    if (__si2drAttrGetAttrType(bitfrom, &err) !=
                        kSI2DR_SIMPLE) {
                        return -1; /* this will already be reported */
                    }
                    vt = __si2drSimpleAttrGetValueType(bitfrom, &err);
                    if (vt == kSI2DR_STRING) {
                        return -1;
                    }
                    if (vt == kSI2DR_INT32) {
                        from = __si2drSimpleAttrGetInt32Value(bitfrom, &err);
                    } else if (vt == kSI2DR_FLOAT64) {
                        from = __si2drSimpleAttrGetFloat64Value(bitfrom, &err);
                    } else if (vt == kSI2DR_EXPR) {
                        si2drExprT *e =
                            __si2drSimpleAttrGetExprValue(bitfrom, &err);
                        if (e->type != kSI2DR_EXPR_VAL) {
                            return -1;  // if it is an expr, we may need to
                                        // evaluate it?
                        }
                        if (e->valuetype == kSI2DR_INT32)
                            from = e->u.i;
                        else if (e->valuetype == kSI2DR_FLOAT64)
                            from = e->u.d;
                        else
                            return -1;
                    }
                    if (__si2drAttrGetAttrType(bitto, &err) != kSI2DR_SIMPLE) {
                        return -1; /* this will already be reported */
                    }
                    vt = __si2drSimpleAttrGetValueType(bitto, &err);
                    if (vt == kSI2DR_STRING) {
                        return -1;
                    }
                    if (vt == kSI2DR_INT32) {
                        to = __si2drSimpleAttrGetInt32Value(bitto, &err);
                    } else if (vt == kSI2DR_FLOAT64) {
                        to = __si2drSimpleAttrGetFloat64Value(bitto, &err);
                    } else if (vt == kSI2DR_EXPR) {
                        si2drExprT *e =
                            __si2drSimpleAttrGetExprValue(bitto, &err);
                        if (e->type != kSI2DR_EXPR_VAL) {
                            return -1;  // if it is an expr, we may need to
                                        // evaluate it?
                        }
                        if (e->valuetype == kSI2DR_INT32)
                            to = e->u.i;
                        else if (e->valuetype == kSI2DR_FLOAT64)
                            to = e->u.d;
                        else
                            return -1;
                    }
                    big = (from > to ? from : to);
                    lit = (from > to ? to : from);
                    wid = big - lit + 1;
                } else {
                    return -1;
                }
            }
        }
        return wid;
    }
    return -1;
}
int LibAnalysis::__getBunSize(si2drGroupIdT group, char *bun) {
    si2drErrorT err;
    si2drGroupIdT bus =
        __si2drGroupFindGroupByName(group, bun, si2drStringT("bundle"), &err);
    if (!__si2drObjectIsNull(bus, &err)) {
        si2drAttrIdT bunmem =
            __si2drGroupFindAttrByName(bus, si2drStringT("members"), &err);
        if (!__si2drObjectIsNull(bunmem, &err)) {
            return __countStringsInList(bunmem);
        }
    }
    return -1;
}
void LibAnalysis::__genVarList(char *formula) {
    /* a very crude boolean algebra formula parser */
    char *p, *q;
    char varname[100];
    p = formula;
    q = varname;
    while (*p) {
        /* skip over any non-name chars */
        while (*p && (*p == '(' || *p == ')' || *p == ' ' || *p == '\t' ||
                      *p == '&' || *p == '|' || *p == '*' || *p == '!' ||
                      *p == '^' || *p == '+' || *p == '\''))
            p++;
        while (*p && (*p != '(' && *p != ')' && *p != ' ' && *p != '+' &&
                      *p != '\'' && *p != '\t' && *p != '&' && *p != '|' &&
                      *p != '*' && *p != '^' && *p != '!'))
            *q++ = *p++;
        *q = 0;
        if (varname[0]) {
            __addVarlist(varname);
        }
        q = varname;
    }
}
si2drGroupIdT LibAnalysis::__findBuByFunction(si2drGroupIdT cellequiv,
                                              char *name) {
    si2drErrorT err;
    si2drGroupsIdT gs = __si2drGroupGetGroups(cellequiv, &err);
    si2drGroupIdT g2;
    si2drAttrIdT attr;
    si2drValueTypeT vtype;
    si2drStringT string = 0;
    si2drAttrTypeT atype;
    group_enum gt;
    while (!__si2drObjectIsNull((g2 = __si2drIterNextGroup(gs, &err)), &err)) {
        gt = __si2drGroupGetID(g2, &err);
        switch (gt) {
            case kTIMINGLIB_GROUPENUM_bus:
            case kTIMINGLIB_GROUPENUM_bundle:
                attr = __si2drGroupFindAttrByName(g2, si2drStringT("function"),
                                                  &err);
                if (!__si2drObjectIsNull(attr, &err)) {
                    atype = __si2drAttrGetAttrType(attr, &err);
                    if (atype != kSI2DR_COMPLEX) {
                        vtype = __si2drSimpleAttrGetValueType(attr, &err);
                        switch (vtype) {
                            case kSI2DR_STRING:
                                string =
                                    __si2drSimpleAttrGetStringValue(attr, &err);
                                if (!strcasecmp(name, string)) {
                                    __si2drIterQuit(gs, &err);
                                    return g2;
                                }
                                break;
                            case kSI2DR_EXPR: {
                                si2drExprT *e =
                                    __si2drSimpleAttrGetExprValue(attr, &err);
                                if (e->type == kSI2DR_EXPR_VAL) {
                                    if (e->valuetype == kSI2DR_STRING) {
                                        if (!strcasecmp(name, e->u.s)) {
                                            __si2drIterQuit(gs, &err);
                                            return g2;
                                        }
                                    }
                                }
                            } break;
                            default:
                                break;
                        }
                    }
                }
                break;
            default:
                break;
        }
    }
    __si2drIterQuit(gs, &err);
    return __si2drPIGetNullId(&err);
}
si2drGroupIdT LibAnalysis::__findPinInCellequiv(si2drGroupIdT cellequiv,
                                                char *name) {
    si2drErrorT err;
    si2drGroupsIdT gs = __si2drGroupGetGroups(cellequiv, &err);
    si2drGroupIdT g2;
    int found = 0;
    si2drNamesIdT names;
    si2drStringT str;
    group_enum gt;
    while (!__si2drObjectIsNull((g2 = __si2drIterNextGroup(gs, &err)), &err)) {
        gt = __si2drGroupGetID(g2, &err);
        switch (gt) {
            case kTIMINGLIB_GROUPENUM_pin:
                names = __si2drGroupGetNames(g2, &err);
                while ((str = __si2drIterNextName(names, &err))) {
                    if (!strcmp(name, str)) {
                        found++;
                        break;
                    }
                }
                __si2drIterQuit(names, &err);
                if (found) {
                    __si2drIterQuit(gs, &err);
                    return g2;
                }
                break;
            default:
                break;
        }
    }
    __si2drIterQuit(gs, &err);
    return __si2drPIGetNullId(&err);
}
void LibAnalysis::__checkNextStateFormula(si2drGroupIdT cell, char *formula,
                                          int ffbankwidth, si2drAttrIdT attr) {
    si2drErrorT err;
    var_list *v;
    si2drGroupIdT bus;
    __destroyVarlist();
    __genVarList(formula);
    for (v = master_var_list_; v; v = v->next) {
        /* assume: every variable is either a bus or bundle;
               a variable may match the function of a bus or bundle;
               the width of the bus must match the width of the ff_bank */
        /* first, try to match the bus or bun */
        bus = __si2drGroupFindGroupByName(cell, v->name, si2drStringT("bus"),
                                          &err);
        if (__si2drObjectIsNull(bus, &err)) {
            bus = __si2drGroupFindGroupByName(cell, v->name,
                                              si2drStringT("bundle"), &err);
            if (!__si2drObjectIsNull(bus, &err)) {
                /* si2drAttrIdT bunmem = __si2drGroupFindAttrByName(bus,
                 * si2drStringT("members"), &err); */
                si2drInt32T wid;
                wid = __getBunSize(cell, v->name);
                if (wid == -1) {
                    char EB[8000];
                    snprintf(
                        EB, sizeof(EB),
                        "%s:%d, The 'members' attribute was not found in the "
                        "bundle. Cannot make bus width correspondence checks.",
                        __si2drObjectGetFileName(bus, &err),
                        static_cast<int>(__si2drObjectGetLineNo(bus, &err)));
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                    errcount_++;
                    continue;
                } else {
                    if (wid != ffbankwidth) {
                        char EB[8000];
                        snprintf(
                            EB, sizeof(EB),
                            "%s:%d, This ff_bank, with width %d, does not "
                            "match the referenced bundle %s (line %d), "
                            "whose width is %d",
                            __si2drObjectGetFileName(attr, &err),
                            static_cast<int>(
                                __si2drObjectGetLineNo(attr, &err)),
                            ffbankwidth, v->name,
                            static_cast<int>(__si2drObjectGetLineNo(bus, &err)),
                            static_cast<int>(wid));
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                        errcount_++;
                        continue;
                    }
                }
                continue;
            }
        }
        if (!__si2drObjectIsNull(bus, &err)) {
            si2drInt32T wid = __getBusSize(cell, v->name);
            if (wid != ffbankwidth) {
                char EB[8000];
                snprintf(EB, sizeof(EB),
                         "%s:%d, This ff_bank, with width %d, does not match "
                         "the referenced bus %s (line %d), whose width is %d",
                         __si2drObjectGetFileName(attr, &err),
                         static_cast<int>(__si2drObjectGetLineNo(attr, &err)),
                         ffbankwidth, v->name,
                         static_cast<int>(__si2drObjectGetLineNo(bus, &err)),
                         static_cast<int>(wid));
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR,
                                    EB, &err);
                errcount_++;
                continue;
            }
        } else {
            si2drNamesIdT names;
            group_enum gt;
            si2drStringT str;
            si2drInt32T wid;
            si2drGroupIdT pinn;
            bus = __findBuByFunction(cell, v->name);
            if (__si2drObjectIsNull(bus, &err)) {
                pinn = __findPinInCellequiv(cell, v->name);
                if (__si2drObjectIsNull(pinn, &err)) {
                    char EB[8000];
                    snprintf(
                        EB, sizeof(EB),
                        "%s:%d, This ff_bank, with width %d, references "
                        "the object %s, which I cannot find.",
                        __si2drObjectGetFileName(attr, &err),
                        static_cast<int>(__si2drObjectGetLineNo(attr, &err)),
                        ffbankwidth, v->name);
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                    errcount_++;
                    continue;
                } else {
                    continue; /* not an error */
                }
            }
            names = __si2drGroupGetNames(bus, &err);
            str = __si2drIterNextName(names, &err);
            __si2drIterQuit(names, &err);
            if (str) {
                gt = __si2drGroupGetID(bus, &err);
                switch (gt) {
                    case kTIMINGLIB_GROUPENUM_bus:
                        wid = __getBusSize(cell, str);
                        break;
                    case kTIMINGLIB_GROUPENUM_bundle:
                        wid = __getBunSize(cell, str);
                        break;
                    default:
                        break;
                }
                if (wid != ffbankwidth) {
                    char EB[8000];
                    snprintf(
                        EB, sizeof(EB),
                        "%s:%d, This ff_bank, with width %d, does not "
                        "match the referenced bus %s with function %s "
                        "(line %d), whose width is %d",
                        __si2drObjectGetFileName(attr, &err),
                        static_cast<int>(__si2drObjectGetLineNo(attr, &err)),
                        ffbankwidth, str, v->name,
                        static_cast<int>(__si2drObjectGetLineNo(bus, &err)),
                        static_cast<int>(wid));
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                    errcount_++;
                    continue;
                }
            }
        }
    }
    __destroyVarlist();
}
void LibAnalysis::__checkFfBankWidths(
    si2drGroupIdT group) { /* called for ff_banks */
    si2drErrorT err;
    si2drStringT str;
    si2drNamesIdT names;
    int ffbankwidth = 0;
    si2drAttrIdT attr;
    si2drGroupIdT cell;
    char busnam[1000];
    names = __si2drGroupGetNames(group, &err);
    str = __si2drIterNextName(names, &err);
    if (str) {
        str = __si2drIterNextName(names, &err);
        if (str) {
            str = __si2drIterNextName(names, &err);
            if (str) {
                ffbankwidth = atoi(str);
            }
        }
    }
    __si2drIterQuit(names, &err);
    if (ffbankwidth == 0) {
        char EB[8000];
        snprintf(EB, sizeof(EB),
                 "%s:%d, Could not retrieve ff_bank width from file, or it has "
                 "value 0. Cannot make bus width correspondence checks.",
                 __si2drObjectGetFileName(group, &err),
                 static_cast<int>(__si2drObjectGetLineNo(group, &err)));
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                            &err);
        errcount_++;
        return;
    }
    attr = __si2drGroupFindAttrByName(group, si2drStringT("next_state"), &err);
    if (!__si2drObjectIsNull(attr, &err)) {
        si2drExprT *e;
        if (__si2drAttrGetAttrType(attr, &err) != kSI2DR_SIMPLE) {
            return; /* this will already be reported */
        }
        if (__si2drSimpleAttrGetValueType(attr, &err) == kSI2DR_EXPR)
            e = __si2drSimpleAttrGetExprValue(attr, &err);
        if ((__si2drSimpleAttrGetValueType(attr, &err) != kSI2DR_STRING &&
             __si2drSimpleAttrGetValueType(attr, &err) != kSI2DR_EXPR) ||
            (__si2drSimpleAttrGetValueType(attr, &err) == kSI2DR_EXPR &&
             e->valuetype != kSI2DR_STRING)) {
            char EB[8000];
            snprintf(EB, sizeof(EB),
                     "%s:%d, The 'next_state' attribute is not of string type. "
                     "Cannot make bus width correspondence checks.",
                     __si2drObjectGetFileName(attr, &err),
                     static_cast<int>(__si2drObjectGetLineNo(attr, &err)));
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                                &err);
            errcount_++;
            return;
        }
        str = __si2drSimpleAttrGetStringValue(attr, &err);
        if (!str || !str[0]) {
            char EB[8000];
            snprintf(EB, sizeof(EB),
                     "%s:%d, The 'next_state' attribute is an empty string. "
                     "Cannot make bus width correspondence checks.",
                     __si2drObjectGetFileName(attr, &err),
                     static_cast<int>(__si2drObjectGetLineNo(attr, &err)));
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                                &err);
            errcount_++;
            return;
        }
        /* go find this referenced bus */
        snprintf(busnam, sizeof(busnam), "%s", str);
        cell = __getContainingGroup(group, si2drStringT("test_cell"));
        if (__si2drObjectIsNull(cell, &err))
            cell = __getContainingGroup(group, si2drStringT("model"));
        if (__si2drObjectIsNull(cell, &err))
            cell = __getContainingGroup(group, si2drStringT("scaled_cell"));
        if (__si2drObjectIsNull(cell, &err))
            cell = __getContainingGroup(group, si2drStringT("cell"));
        if (!__si2drObjectIsNull(cell, &err)) {
            __checkNextStateFormula(cell, busnam, ffbankwidth, attr);
        }
    }
    return;
}
void LibAnalysis::__checkBusType(si2drGroupIdT bus) {
    si2drErrorT err;
    si2drStringT str;
    si2drGroupIdT cell;
    si2drAttrIdT bustype =
        __si2drGroupFindAttrByName(bus, si2drStringT("bus_type"), &err);
    si2drGroupIdT typeb, libr;
    if (!__si2drObjectIsNull(bustype, &err)) {
        if (__si2drAttrGetAttrType(bustype, &err) != kSI2DR_SIMPLE) {
            return; /* this will already be reported */
        }
        if (__si2drSimpleAttrGetValueType(bustype, &err) != kSI2DR_STRING) {
            if (__si2drSimpleAttrGetValueType(bustype, &err) == kSI2DR_EXPR) {
                si2drExprT *e;
                e = __si2drSimpleAttrGetExprValue(bustype, &err);
                if (e->valuetype != kSI2DR_STRING) return;
            }
            return;
        }
        str = __si2drSimpleAttrGetStringValue(bustype, &err);
        if (!str || !str[0]) {
            char EB[8000];
            snprintf(EB, sizeof(EB),
                     "%s:%d, The 'bus_type' attribute is an empty string.",
                     __si2drObjectGetFileName(bustype, &err),
                     static_cast<int>(__si2drObjectGetLineNo(bustype, &err)));
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR, EB,
                                &err);
            errcount_++;
            return;
        }
        /* find the type in the cell or lib */
        cell = __getContainingGroup(bus, si2drStringT("test_cell"));
        if (__si2drObjectIsNull(cell, &err))
            cell = __getContainingGroup(bus, si2drStringT("model"));
        if (__si2drObjectIsNull(cell, &err))
            cell = __getContainingGroup(bus, si2drStringT("scaled_cell"));
        if (__si2drObjectIsNull(cell, &err))
            cell = __getContainingGroup(bus, si2drStringT("cell"));
        typeb =
            __si2drGroupFindGroupByName(cell, str, si2drStringT("type"), &err);
        if (__si2drObjectIsNull(typeb, &err)) {
            libr = __getContainingGroup(bus, si2drStringT("library"));
            if (!__si2drObjectIsNull(libr, &err)) {
                typeb = __si2drGroupFindGroupByName(libr, str,
                                                    si2drStringT("type"), &err);
            }
        }
        if (__si2drObjectIsNull(typeb, &err)) {
            char EB[8000];
            snprintf(EB, sizeof(EB),
                     "%s:%d, The 'bus_type' references the type '%s', which "
                     "cannot be found in the containing cell or library.",
                     __si2drObjectGetFileName(bustype, &err),
                     static_cast<int>(__si2drObjectGetLineNo(bustype, &err)),
                     str);
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                                &err);
            errcount_++;
            return;
        }
    } else {
        char EB[8000];
        snprintf(EB, sizeof(EB),
                 "%s:%d, The 'bus_type' attribute is missing in bus '%s'.",
                 __si2drObjectGetFileName(bus, &err),
                 static_cast<int>(__si2drObjectGetLineNo(bus, &err)),
                 __getFirstGroupName(bus));
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                            &err);
        errcount_++;
        return;
    }
}
void LibAnalysis::__findAllRefsToTemplate(si2drGroupIdT group, char *name,
                                          int num) {
    si2drErrorT err;
    si2drGroupsIdT gs = __si2drGroupGetGroups(group, &err);
    si2drGroupIdT g2;
    si2drNamesIdT names;
    si2drStringT str;
    while (!__si2drObjectIsNull((g2 = __si2drIterNextGroup(gs, &err)), &err)) {
        if (!strcmp(__si2drGroupGetGroupType(g2, &err), "lu_table_template"))
            continue;
        names = __si2drGroupGetNames(g2, &err);
        while ((str = __si2drIterNextName(names, &err))) {
            if (!strcmp(name, str)) {
                if (!num) {
                    char EB[8000];
                    snprintf(EB, sizeof(EB),
                             "         Here is a list of references to this "
                             "invalid template:");
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_NOTE,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                }
                num++;
                char EB[8000];
                snprintf(EB, sizeof(EB), "           %s:%d, group '%s'",
                         __si2drObjectGetFileName(g2, &err),
                         static_cast<int>(__si2drObjectGetLineNo(g2, &err)),
                         __si2drGroupGetGroupType(g2, &err));
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_NOTE, kSI2DR_SEMANTIC_ERROR,
                                    EB, &err);
                break;
            }
        }
        __si2drIterQuit(names, &err);
        __findAllRefsToTemplate(g2, name, num); /* recursively descend */
    }
    __si2drIterQuit(gs, &err);
}
void LibAnalysis::__checkIndexX(si2drStringT indnam, si2drGroupIdT group) {
    si2drErrorT err;
    si2drGroupIdT lib;
    si2drAttrIdT index_x = __si2drGroupFindAttrByName(group, indnam, &err);
    si2drValuesIdT vals;
    si2drValueTypeT vtype;
    si2drInt32T intgr;
    si2drFloat64T float64;
    si2drStringT string;
    si2drBooleanT boolean;
    si2drExprT *expr;
    char *p, *t;
    double x;
    if (!__si2drObjectIsNull(index_x, &err)) {
        int inval;
        /* should be a string with a list of comma/space sep'd numbers */
        if (__si2drAttrGetAttrType(index_x, &err) != kSI2DR_COMPLEX) {
            return; /* this will already be reported */
        }
        vals = __si2drComplexAttrGetValues(index_x, &err);
        __si2drIterNextComplexValue(vals, &vtype, &intgr, &float64, &string,
                                    &boolean, &expr, &err);
        __si2drIterQuit(vals, &err);
        if (vtype == kSI2DR_UNDEFINED_VALUETYPE) {
            char EB[8000];
            snprintf(EB, sizeof(EB), "%s:%d, The '%s' attribute has no values.",
                     __si2drObjectGetFileName(index_x, &err),
                     static_cast<int>(__si2drObjectGetLineNo(index_x, &err)),
                     indnam);
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR, EB,
                                &err);
            errcount_++;
            return;
        }
        if (vtype != kSI2DR_STRING) {
            char EB[8000];
            snprintf(EB, sizeof(EB),
                     "%s:%d, The '%s' attribute should be a string.",
                     __si2drObjectGetFileName(index_x, &err),
                     static_cast<int>(__si2drObjectGetLineNo(index_x, &err)),
                     indnam);
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR, EB,
                                &err);
            errcount_++;
            return;
        }
        if (!string || !string[0]) {
            char EB[8000];
            snprintf(EB, sizeof(EB),
                     "%s:%d, The '%s' attribute is an empty string.",
                     __si2drObjectGetFileName(index_x, &err),
                     static_cast<int>(__si2drObjectGetLineNo(index_x, &err)),
                     indnam);
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR, EB,
                                &err);
            errcount_++;
            return;
        }
        p = string;
        inval = 0;
        while ((x = strtod(p, &t)), t != p) {
            if (x < 0.0) {
                char EB[8000];
                snprintf(
                    EB, sizeof(EB),
                    "%s:%d, The '%s' attribute contains the negative value "
                    "'%g'. The template '%s' is therefore invalid.",
                    __si2drObjectGetFileName(index_x, &err),
                    static_cast<int>(__si2drObjectGetLineNo(index_x, &err)),
                    indnam, x, __getFirstGroupName(group));
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR,
                                    EB, &err);
                errcount_++;
                inval = 1;
            }
            if (t && *t == 0) break;
            p = t + 1;
        }
        if (inval) {
            char buf[1000];
            snprintf(
                buf, sizeof(buf), "%s",
                __getFirstGroupName(
                    group)); /* do this because si2dr strings are volatile.  */
            lib = __getContainingGroup(group, si2drStringT("library"));
            __findAllRefsToTemplate(lib, buf, 0);
        }
    }
}
void LibAnalysis::__checkLutTemplate(si2drGroupIdT group) {
    __checkIndexX(si2drStringT("index_1"), group);
    __checkIndexX(si2drStringT("index_2"), group);
}
void LibAnalysis::__getIndexInfo(si2drAttrIdT index_x,
                                 struct timinglib_value_data *vd, int dimno) {
    si2drErrorT err;
    si2drValuesIdT vals;
    si2drValueTypeT vtype;
    si2drInt32T intgr;
    si2drFloat64T float64;
    si2drStringT string;
    si2drBooleanT boolean;
    si2drExprT *expr;
    char *p, *t = NULL;
    int numcount, i2;
    LONG_DOUBLE x;
    if (!__si2drObjectIsNull(index_x, &err)) {
        // int inval;
        /* should be a string with a list of comma/space sep'd numbers */
        if (__si2drAttrGetAttrType(index_x, &err) != kSI2DR_COMPLEX) {
            return; /* this will already be reported */
        }
        vals = __si2drComplexAttrGetValues(index_x, &err);
        __si2drIterNextComplexValue(vals, &vtype, &intgr, &float64, &string,
                                    &boolean, &expr, &err);
        __si2drIterQuit(vals, &err);
        if (vtype == kSI2DR_UNDEFINED_VALUETYPE) {
            char EB[8000];
            snprintf(EB, sizeof(EB),
                     "%s:%d, The 'index_%d' attribute has no values.",
                     __si2drObjectGetFileName(index_x, &err),
                     static_cast<int>(__si2drObjectGetLineNo(index_x, &err),
                                      dimno + 1));
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR, EB,
                                &err);
            errcount_++;
            return;
        }
        if (vtype != kSI2DR_STRING) {
            char EB[8000];
            snprintf(EB, sizeof(EB),
                     "%s:%d, The 'index_%d' attribute should be a string.",
                     __si2drObjectGetFileName(index_x, &err),
                     static_cast<int>(__si2drObjectGetLineNo(index_x, &err),
                                      dimno + 1));
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR, EB,
                                &err);
            errcount_++;
            return;
        }
        if (!string || !string[0]) {
            char EB[8000];
            snprintf(EB, sizeof(EB),
                     "%s:%d, The 'index_%d' attribute is an empty string.",
                     __si2drObjectGetFileName(index_x, &err),
                     static_cast<int>(__si2drObjectGetLineNo(index_x, &err),
                                      dimno + 1));
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR, EB,
                                &err);
            errcount_++;
            return;
        }
        p = string;
        numcount = 0;
        while ((x = strtod(p, &t)), t != p) {
            if (x < 0.0) {
#ifdef NO_LONG_DOUBLE
                char EB[8000];
                snprintf(
                    EB, sizeof(EB),
                    "%s:%d, The 'index_%d' attribute contains the negative "
                    "value '%g'.",
                    __si2drObjectGetFileName(index_x, &err),
                    static_cast<int>(__si2drObjectGetLineNo(index_x, &err),
                                     dimno + 1, x));
#else
                char EB[8000];
                snprintf(
                    EB, sizeof(EB),
                    "%s:%d, The 'index_%d' attribute contains the negative "
                    "value '%Lg'.",
                    __si2drObjectGetFileName(index_x, &err),
                    static_cast<int>(__si2drObjectGetLineNo(index_x, &err),
                                     dimno + 1, x));
#endif
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR,
                                    EB, &err);
                errcount_++;
                // inval = 1;
            }
            numcount++;
            if (t && *t == 0) break;
            /* skip over intervening stuff to the next number */
            while (t && (*t == ',' || *t == ' ' || *t == '\t' || *t == '\n' ||
                         *t == '\r' || *t == '\\')) {
                if (*t == '\\' && *(t + 1) == '\n') /*skip over line escapes */
                    t++;
                t++;
            }
            p = t;
        }
        if (numcount) {
            vd->index_info[dimno] = static_cast<LONG_DOUBLE *>(
                calloc(sizeof(LONG_DOUBLE), numcount));
            vd->dim_sizes[dimno] = numcount;
            p = string;
            i2 = 0;
            while ((x = strtod(p, &t)), t != p) {
                LONG_DOUBLE *pg = vd->index_info[dimno];
                pg[i2] = x;
                if (t && *t == 0) break;
                i2++;
                p = t + 1;
            }
        }
    }
}
struct timinglib_value_data *LibAnalysis::__getVectorData(
    si2drGroupIdT vector) {
    si2drErrorT err;
    si2drGroupIdT dad = __si2drObjectGetOwner(vector, &err);
    group_enum dadtype = __si2drGroupGetID(dad, &err);
    si2drAttrIdT index_1 =
        __si2drGroupFindAttrByName(vector, si2drStringT("index_1"), &err);
    si2drAttrIdT index_2 =
        __si2drGroupFindAttrByName(vector, si2drStringT("index_2"), &err);
    si2drAttrIdT index_3 =
        __si2drGroupFindAttrByName(vector, si2drStringT("index_3"), &err);
    si2drAttrIdT index_4 =
        __si2drGroupFindAttrByName(vector, si2drStringT("index_4"), &err);
    si2drAttrIdT index_5 =
        __si2drGroupFindAttrByName(vector, si2drStringT("index_5"), &err);
    si2drAttrIdT index_6 =
        __si2drGroupFindAttrByName(vector, si2drStringT("index_6"), &err);
    si2drAttrIdT index_7 =
        __si2drGroupFindAttrByName(vector, si2drStringT("index_7"), &err);
    si2drAttrIdT valuesd =
        __si2drGroupFindAttrByName(vector, si2drStringT("values"), &err);
    si2drValuesIdT vals;
    si2drValueTypeT vtype;
    si2drInt32T intgr;
    si2drFloat64T float64;
    si2drStringT string;
    si2drBooleanT boolean;
    si2drExprT *expr;
    char *p, *t;
    LONG_DOUBLE x;
    struct timinglib_value_data *vd =
        static_cast<struct timinglib_value_data *>(
            calloc(sizeof(struct timinglib_value_data), 1));
    int numels;
    /* find the number of dims */
    if (!__si2drObjectIsNull(index_1, &err)) {
        vd->dimensions = 1;
        if (!__si2drObjectIsNull(index_2, &err)) {
            vd->dimensions = 2;
            if (!__si2drObjectIsNull(index_3, &err)) {
                vd->dimensions = 3;
                if (!__si2drObjectIsNull(index_4, &err)) {
                    vd->dimensions = 4;
                    if (!__si2drObjectIsNull(index_5, &err)) {
                        vd->dimensions = 5;
                        if (!__si2drObjectIsNull(index_6, &err)) {
                            vd->dimensions = 6;
                            if (!__si2drObjectIsNull(index_7, &err)) {
                                vd->dimensions = 7;
                            }
                        }
                    }
                }
            }
        }
    }
    vd->dim_sizes = static_cast<int *>(calloc(sizeof(int), vd->dimensions));
    vd->index_info = static_cast<LONG_DOUBLE **>(
        calloc(sizeof(LONG_DOUBLE *), vd->dimensions));
    /* now, find and set the sizes of each dim */
    if (!__si2drObjectIsNull(index_1, &err)) {
        __getIndexInfo(index_1, vd, 0);
        if (!__si2drObjectIsNull(index_2, &err)) {
            __getIndexInfo(index_2, vd, 1);
            if (!__si2drObjectIsNull(index_3, &err)) {
                __getIndexInfo(index_3, vd, 2);
                if (!__si2drObjectIsNull(index_4, &err)) {
                    __getIndexInfo(index_4, vd, 3);
                    if (!__si2drObjectIsNull(index_5, &err)) {
                        __getIndexInfo(index_5, vd, 4);
                        if (!__si2drObjectIsNull(index_6, &err)) {
                            __getIndexInfo(index_6, vd, 5);
                            if (!__si2drObjectIsNull(index_7, &err)) {
                                __getIndexInfo(index_7, vd, 6);
                            }
                        }
                    }
                }
            }
        }
    }
    /* now, malloc and fill the values */
    numels = 1;
    if (!__si2drObjectIsNull(index_1, &err)) {
        numels *= vd->dim_sizes[0];
        if (!__si2drObjectIsNull(index_2, &err)) {
            numels *= vd->dim_sizes[1];
            if (!__si2drObjectIsNull(index_3, &err)) {
                numels *= vd->dim_sizes[2];
                if (!__si2drObjectIsNull(index_4, &err)) {
                    numels *= vd->dim_sizes[3];
                    if (!__si2drObjectIsNull(index_5, &err)) {
                        numels *= vd->dim_sizes[4];
                        if (!__si2drObjectIsNull(index_6, &err)) {
                            numels *= vd->dim_sizes[5];
                            if (!__si2drObjectIsNull(index_7, &err)) {
                                numels *= vd->dim_sizes[6];
                            }
                        }
                    }
                }
            }
        }
    }
    /* Now, fill in the values array from the values attr */
    vd->values =
        static_cast<LONG_DOUBLE *>(calloc(sizeof(LONG_DOUBLE), numels));
    if (!__si2drObjectIsNull(valuesd, &err)) {
        // int inval = 0;
        int totalcount = 0;
        int numcount;
        int first = 1;
        /* should be a string with a list of comma/space sep'd numbers */
        if (__si2drAttrGetAttrType(valuesd, &err) != kSI2DR_COMPLEX) {
            __timinglibDestroyValueData(vd);
            return 0; /* this will already be reported */
        }
        vals = __si2drComplexAttrGetValues(valuesd, &err);
        do {
            __si2drIterNextComplexValue(vals, &vtype, &intgr, &float64, &string,
                                        &boolean, &expr, &err);
            if (first && vtype == kSI2DR_UNDEFINED_VALUETYPE) {
                char EB[8000];
                snprintf(
                    EB, sizeof(EB),
                    "%s:%d, The 'values' attribute has no values.",
                    __si2drObjectGetFileName(valuesd, &err),
                    static_cast<int>(__si2drObjectGetLineNo(valuesd, &err)));
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR,
                                    EB, &err);
                errcount_++;
                break;
            }
            if (vtype == kSI2DR_UNDEFINED_VALUETYPE) break;
            first = 0;
            if (vtype != kSI2DR_STRING) {
                char EB[8000];
                snprintf(
                    EB, sizeof(EB),
                    "%s:%d, The 'values' attribute should be a string.",
                    __si2drObjectGetFileName(valuesd, &err),
                    static_cast<int>(__si2drObjectGetLineNo(valuesd, &err)));
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR,
                                    EB, &err);
                errcount_++;
                break;
            }
            if (!string || !string[0]) {
                char EB[8000];
                snprintf(
                    EB, sizeof(EB),
                    "%s:%d, The 'values' attribute is an empty string.",
                    __si2drObjectGetFileName(valuesd, &err),
                    static_cast<int>(__si2drObjectGetLineNo(valuesd, &err)));
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR,
                                    EB, &err);
                errcount_++;
                break;
            }
            p = string;
            numcount = 0;
            while ((x = strtod(p, &t)), t != p) {
                if (dadtype == kTIMINGLIB_GROUPENUM_output_current_rise &&
                    x < 0.0) {
#ifdef NO_LONG_DOUBLE
                    char EB[8000];
                    snprintf(EB, sizeof(EB),
                             "%s:%d, The 'values' attribute contains the "
                             "negative value '%g'.",
                             __si2drObjectGetFileName(valuesd, &err),
                             static_cast<int>(
                                 __si2drObjectGetLineNo(valuesd, &err), x));
#else
                    char EB[8000];
                    snprintf(EB, sizeof(EB),
                             "%s:%d, The 'values' attribute contains the "
                             "negative value '%Lg'.",
                             __si2drObjectGetFileName(valuesd, &err),
                             static_cast<int>(
                                 __si2drObjectGetLineNo(valuesd, &err), x));
#endif
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                    errcount_++;
                    // inval = 1;
                }
                if (dadtype == kTIMINGLIB_GROUPENUM_output_current_fall &&
                    x > 0.0) {
#ifdef NO_LONG_DOUBLE
                    char EB[8000];
                    snprintf(EB, sizeof(EB),
                             "%s:%d, The 'values' attribute contains the "
                             "positive value '%g'.",
                             __si2drObjectGetFileName(valuesd, &err),
                             static_cast<int>(
                                 __si2drObjectGetLineNo(valuesd, &err), x));
#else
                    char EB[8000];
                    snprintf(EB, sizeof(EB),
                             "%s:%d, The 'values' attribute contains the "
                             "positive value '%Lg'.",
                             __si2drObjectGetFileName(valuesd, &err),
                             static_cast<int>(
                                 __si2drObjectGetLineNo(valuesd, &err), x));
#endif
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                    errcount_++;
                    // inval = 1;
                }
                if (0) {  // liwei
                    // x > -0.00000000000000001 && x < 0.00000000000000001) {
                    char EB[8000];
                    snprintf(
                        EB, sizeof(EB),
                        "%s:%d, The vector 'values' attribute contains one "
                        "or more zero values.",
                        __si2drObjectGetFileName(valuesd, &err),
                        static_cast<int>(
                            __si2drObjectGetLineNo(valuesd, &err)));
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                }
                if (0) {  // liwei
                          // __numGetPrecision(p) < 4) {
#ifdef NO_LONG_DOUBLE
                    char EB[8000];
                    snprintf(
                        EB, sizeof(EB),
                        "%s:%d, The vector 'values' attribute contains %g, "
                        "which has less than 4 digits of precision.",
                        __si2drObjectGetFileName(valuesd, &err),
                        static_cast<int>(__si2drObjectGetLineNo(valuesd, &err),
                                         x));
#else
                    char EB[8000];
                    snprintf(EB, sizeof(EB),
                             "%s:%d, The vector 'values' attribute contains "
                             "%Lg, which has less than 4 digits of precision.",
                             __si2drObjectGetFileName(valuesd, &err),
                             static_cast<int>(
                                 __si2drObjectGetLineNo(valuesd, &err), x));
#endif
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                }
                vd->values[totalcount + numcount] = x;
                numcount++;
                if (t && *t == 0) break;
                /* skip over intervening stuff to the next number */
                while (*t == ',' || *t == ' ' || *t == '\t' || *t == '\n' ||
                       *t == '\r' || *t == '\\') {
                    if (*t == '\\' &&
                        *(t + 1) == '\n') /*skip over line escapes */
                        t++;
                    t++;
                }
                p = t;
            }
            totalcount += numcount;
        } while (vtype != kSI2DR_UNDEFINED_VALUETYPE);
        __si2drIterQuit(vals, &err);
        if (totalcount != numels) {
            char EB[8000];
            snprintf(EB, sizeof(EB),
                     "%s:%d, The vector 'values' attribute with %d vs. %d "
                     "expected values",
                     __si2drObjectGetFileName(vector, &err),
                     static_cast<int>(__si2drObjectGetLineNo(vector, &err)),
                     totalcount, numels);
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                                &err);
            errcount_++;
        }
    } else {
        char EB[8000];
        snprintf(EB, sizeof(EB),
                 "%s:%d, The vector contains no 'values' attribute.",
                 __si2drObjectGetFileName(vector, &err),
                 static_cast<int>(__si2drObjectGetLineNo(vector, &err)));
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR, EB, &err);
        errcount_++;
    }
    return vd;
}
void LibAnalysis::__checkVector(si2drGroupIdT group) {
    si2drErrorT err;
    struct timinglib_value_data *vd = __getVectorData(group);
    si2drGroupIdT dad = __si2drObjectGetOwner(group, &err);
    group_enum gt = __si2drGroupGetID(dad, &err);
    int i;
    int neg = 0;
    /* checks? */
    if (gt == kTIMINGLIB_GROUPENUM_output_current_fall) {
        neg = 1;
    }
    /* negative and zero values are already checked */
    /* peak at first element */
    if (0) {  // liwei
        // vd->dimensions > 2) {
        int found = 0;
        for (i = 1; i < vd->dim_sizes[2]; i++) {
            if ((!neg && vd->values[i] > vd->values[0]) ||
                (neg && vd->values[i] < vd->values[0])) {
                found = 1;
                break;
            }
        }
        if (!found) {
#ifdef NO_LONG_DOUBLE
            char EB[8000];
            snprintf(
                EB, sizeof(EB),
                "%s:%d, The vector has its peak value (%g) at the beginning",
                __si2drObjectGetFileName(group, &err)
                    ? __si2drObjectGetFileName(group, &err)
                    : "",
                static_cast<int>(__si2drObjectGetLineNo(group, &err)),
                vd->values[0]);
#else
            char EB[8000];
            snprintf(
                EB, sizeof(EB),
                "%s:%d, The vector has its peak value (%Lg) at the beginning",
                __si2drObjectGetFileName(group, &err),
                static_cast<int>(__si2drObjectGetLineNo(group, &err)),
                vd->values[0]);
#endif
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                                &err);
            errcount_++;
        }
    }
    __timinglibDestroyValueData(vd);
}
void LibAnalysis::__checkOutputCurrentTemplate(si2drGroupIdT group) {
    si2drErrorT err;
    si2drAttrIdT var_1 =
        __si2drGroupFindAttrByName(group, si2drStringT("variable_1"), &err);
    si2drAttrIdT var_2 =
        __si2drGroupFindAttrByName(group, si2drStringT("variable_2"), &err);
    si2drAttrIdT var_3 =
        __si2drGroupFindAttrByName(group, si2drStringT("variable_3"), &err);
    si2drValueTypeT attrvtype;
    si2drStringT string;
    /* IF THIS CONSTRUCT IS PRESENT, THIS IS A CCS MODEL, SO WE
       NEED TO CHECK THE NOMINAL VOLTAGES */
    if (!operating_conds_checked_) {
        si2drGroupIdT libr =
            __getContainingGroup(group, si2drStringT("library"));
        si2drAttrIdT defopcond = __si2drGroupFindAttrByName(
            libr, si2drStringT("default_operating_conditions"), &err);
        si2drAttrIdT nomvolt =
            __si2drGroupFindAttrByName(libr, si2drStringT("nom_voltage"), &err);
        if (!__si2drObjectIsNull(defopcond, &err) &&
            __si2drAttrGetAttrType(defopcond, &err) == kSI2DR_SIMPLE &&
            __si2drSimpleAttrGetValueType(defopcond, &err) == kSI2DR_STRING) {
            si2drStringT opcondname =
                __si2drSimpleAttrGetStringValue(defopcond, &err);
            if (opcondname && *opcondname) {
                si2drGroupIdT opcondg = __si2drGroupFindGroupByName(
                    libr, opcondname, si2drStringT("operating_conditions"),
                    &err);
                if (!__si2drObjectIsNull(opcondg, &err)) {
                    si2drAttrIdT opcondvoltage = __si2drGroupFindAttrByName(
                        opcondg, si2drStringT("voltage"), &err);
                    if (!__si2drObjectIsNull(opcondvoltage, &err) &&
                        __si2drAttrGetAttrType(opcondvoltage, &err) ==
                            kSI2DR_SIMPLE &&
                        __si2drSimpleAttrGetValueType(opcondvoltage, &err) ==
                            kSI2DR_FLOAT64) {
                        si2drFloat64T opcondvoltval =
                            __si2drSimpleAttrGetFloat64Value(opcondvoltage,
                                                             &err);
                        /* we now have one value, let's get the other */
                        if (!__si2drObjectIsNull(nomvolt, &err) &&
                            __si2drAttrGetAttrType(nomvolt, &err) ==
                                kSI2DR_SIMPLE &&
                            __si2drSimpleAttrGetValueType(nomvolt, &err) ==
                                kSI2DR_FLOAT64) {
                            si2drFloat64T nomvoltval =
                                __si2drSimpleAttrGetFloat64Value(nomvolt, &err);
                            /* OK, Here's the test: */
                            if (nomvoltval != opcondvoltval) {
                                char EB[8000];
                                snprintf(
                                    EB, sizeof(EB),
                                    "%s:%d, The library nom_voltage (%g) "
                                    "doesn't match the voltage (%g) in the "
                                    "operatingConditions(%s).",
                                    __si2drObjectGetFileName(nomvolt, &err),
                                    static_cast<int>(
                                        __si2drObjectGetLineNo(nomvolt, &err)),
                                    nomvoltval, opcondvoltval, opcondname);
                                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                    kSI2DR_SEMANTIC_ERROR, EB,
                                                    &err);
                                errcount_++;
                                return;
                            }
                        }
                    }
                }
            }
        }
        operating_conds_checked_ = 1;
    }
    /* check out variable_3 first */
    if (__si2drAttrGetAttrType(var_3, &err) != kSI2DR_SIMPLE) {
        char EB[8000];
        snprintf(EB, sizeof(EB),
                 "%s:%d, The variable_3 attribute is not a simple attribute.",
                 __si2drObjectGetFileName(group, &err),
                 static_cast<int>(__si2drObjectGetLineNo(group, &err)));
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR, EB, &err);
        errcount_++;
        return;
    }
    attrvtype = __si2drSimpleAttrGetValueType(var_3, &err);
    if (attrvtype != kSI2DR_STRING) {
        char EB[8000];
        snprintf(EB, sizeof(EB),
                 "%s:%d, The variable_3 attribute is not a string.",
                 __si2drObjectGetFileName(group, &err),
                 static_cast<int>(__si2drObjectGetLineNo(group, &err)));
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR, EB, &err);
        errcount_++;
        return;
    }
    string = __si2drSimpleAttrGetStringValue(var_3, &err);
    if (strcasecmp(string, "time") != 0) {
        char EB[8000];
        snprintf(
            EB, sizeof(EB),
            "%s:%d, The variable_3 attribute is '%s', but should be 'time'.",
            __si2drObjectGetFileName(group, &err),
            static_cast<int>(__si2drObjectGetLineNo(group, &err)), string);
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                            &err);
        errcount_++;
        return;
    }
    /* check out variable_1 */
    if (__si2drAttrGetAttrType(var_1, &err) != kSI2DR_SIMPLE) {
        char EB[8000];
        snprintf(EB, sizeof(EB),
                 "%s:%d, The variable_1 attribute is not a simple attribute.",
                 __si2drObjectGetFileName(group, &err),
                 static_cast<int>(__si2drObjectGetLineNo(group, &err)));
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR, EB, &err);
        errcount_++;
        return;
    }
    attrvtype = __si2drSimpleAttrGetValueType(var_1, &err);
    if (attrvtype != kSI2DR_STRING) {
        char EB[8000];
        snprintf(EB, sizeof(EB),
                 "%s:%d, The variable_1 attribute is not a string.",
                 __si2drObjectGetFileName(group, &err),
                 static_cast<int>(__si2drObjectGetLineNo(group, &err)));
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR, EB, &err);
        errcount_++;
        return;
    }
    string = __si2drSimpleAttrGetStringValue(var_1, &err);
    if (strcasecmp(string, "input_net_transition") != 0 &&
        strcasecmp(string, "total_output_net_capacitance") != 0) {
        char EB[8000];
        snprintf(
            EB, sizeof(EB),
            "%s:%d, The variable_1 attribute is '%s', but should be either "
            "'input_net_transition' or 'total_output_net_capacitance'.",
            __si2drObjectGetFileName(group, &err),
            static_cast<int>(__si2drObjectGetLineNo(group, &err)), string);
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                            &err);
        errcount_++;
        return;
    }
    /* check out variable_2 */
    if (__si2drAttrGetAttrType(var_2, &err) != kSI2DR_SIMPLE) {
        char EB[8000];
        snprintf(EB, sizeof(EB),
                 "%s:%d, The variable_2 attribute is not a simple attribute.",
                 __si2drObjectGetFileName(group, &err),
                 static_cast<int>(__si2drObjectGetLineNo(group, &err)));
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR, EB, &err);
        errcount_++;
        return;
    }
    attrvtype = __si2drSimpleAttrGetValueType(var_2, &err);
    if (attrvtype != kSI2DR_STRING) {
        char EB[8000];
        snprintf(EB, sizeof(EB),
                 "%s:%d, The variable_2 attribute is not a string.",
                 __si2drObjectGetFileName(group, &err),
                 static_cast<int>(__si2drObjectGetLineNo(group, &err)));
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR, EB, &err);
        errcount_++;
        return;
    }
    string = __si2drSimpleAttrGetStringValue(var_2, &err);
    if (strcasecmp(string, "input_net_transition") != 0 &&
        strcasecmp(string, "total_output_net_capacitance") != 0) {
        char EB[8000];
        snprintf(
            EB, sizeof(EB),
            "%s:%d, The variable_1 attribute is '%s', but should be either "
            "'input_net_transition' or 'total_output_net_capacitance'.",
            __si2drObjectGetFileName(group, &err),
            static_cast<int>(__si2drObjectGetLineNo(group, &err)), string);
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                            &err);
        errcount_++;
        return;
    }
}
void LibAnalysis::__checkRetentionCell(si2drGroupIdT group) {
    si2drErrorT err;
    si2drAttrIdT retention, power_gating;
    power_gating = __si2drGroupFindAttrByName(
        group, si2drStringT("power_gating_cell"), &err);
    retention =
        __si2drGroupFindAttrByName(group, si2drStringT("retention_cell"), &err);
    if (!__si2drObjectIsNull(retention, &err) &&
        !__si2drObjectIsNull(power_gating, &err)) {
        si2drStringT rf = __si2drObjectGetFileName(retention, &err);
        int rl = __si2drObjectGetLineNo(retention, &err);
        si2drStringT pgf = __si2drObjectGetFileName(power_gating, &err);
        int pgl = __si2drObjectGetLineNo(power_gating, &err);
        char EB[8000];
        snprintf(EB, sizeof(EB),
                 "%s:%d, and %s:%d -- retention_cell and power_gating_cell "
                 "attributes are not allowed in the same cell.",
                 rf, rl, pgf, pgl);
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                            &err);
        errcount_++;
    }
}
void LibAnalysis::__checkRetentionPin(si2drGroupIdT group) {
    si2drErrorT err;
    si2drAttrIdT retention, power_gating, mtl, dir;
    power_gating = __si2drGroupFindAttrByName(
        group, si2drStringT("power_gating_pin"), &err);
    retention =
        __si2drGroupFindAttrByName(group, si2drStringT("retention_pin"), &err);
    mtl = __si2drGroupFindAttrByName(group, si2drStringT("map_to_logic"), &err);
    dir = __si2drGroupFindAttrByName(group, si2drStringT("direction"), &err);
    if (!__si2drObjectIsNull(retention, &err) &&
        !__si2drObjectIsNull(power_gating, &err)) {
        si2drStringT rf = __si2drObjectGetFileName(retention, &err);
        int rl = __si2drObjectGetLineNo(retention, &err);
        si2drStringT pgf = __si2drObjectGetFileName(power_gating, &err);
        int pgl = __si2drObjectGetLineNo(power_gating, &err);
        char EB[8000];
        snprintf(EB, sizeof(EB),
                 "%s:%d, and %s:%d -- retention_pin and power_gating_pin "
                 "attributes are not allowed in the same pin.",
                 rf, rl, pgf, pgl);
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                            &err);
        errcount_++;
    }
    if (!__si2drObjectIsNull(retention, &err) &&
        !__si2drObjectIsNull(mtl, &err)) {
        si2drStringT rf = __si2drObjectGetFileName(retention, &err);
        int rl = __si2drObjectGetLineNo(retention, &err);
        si2drStringT pgf = __si2drObjectGetFileName(mtl, &err);
        int pgl = __si2drObjectGetLineNo(mtl, &err);
        char EB[8000];
        snprintf(EB, sizeof(EB),
                 "%s:%d, and %s:%d -- when retention_pin and map_to_logic "
                 "attributes are spec'd on the same pin, the retention_pin's "
                 "definition will have precedence.",
                 rf, rl, pgf, pgl);
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_WARN, kSI2DR_NO_ERROR, EB, &err);
        errcount_++;
    }
    if (!__si2drObjectIsNull(retention, &err) &&
        !__si2drObjectIsNull(dir, &err)) {
        if (__si2drAttrGetAttrType(dir, &err) == kSI2DR_SIMPLE) {
            if (__si2drSimpleAttrGetValueType(dir, &err) == kSI2DR_STRING) {
                si2drStringT x = __si2drSimpleAttrGetStringValue(dir, &err);
                if (x && strlen(x) && !strcasecmp(x, "output")) {
                    si2drStringT rf = __si2drObjectGetFileName(retention, &err);
                    int rl = __si2drObjectGetLineNo(retention, &err);
                    si2drStringT df = __si2drObjectGetFileName(dir, &err);
                    int dl = __si2drObjectGetLineNo(dir, &err);
                    char EB[8000];
                    snprintf(EB, sizeof(EB),
                             "%s:%d, and %s:%d -- retention_pin cannot be "
                             "defined on an output pin.",
                             rf, rl, df, dl);
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                    errcount_++;
                }
                /* else we are OK */
            } else {
                si2drStringT rf = __si2drObjectGetFileName(dir, &err);
                int rl = __si2drObjectGetLineNo(dir, &err);
                char EB[8000];
                snprintf(EB, sizeof(EB),
                         "%s:%d, The pin direction attribute is NOT a string?",
                         rf, rl);
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR,
                                    EB, &err);
                errcount_++;
            }
        } else {
            si2drStringT rf = __si2drObjectGetFileName(dir, &err);
            int rl = __si2drObjectGetLineNo(dir, &err);
            char EB[8000];
            snprintf(
                EB, sizeof(EB),
                "%s:%d, The pin direction attribute is NOT a simple attribute?",
                rf, rl);
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                                &err);
            errcount_++;
        }
    }
}
void LibAnalysis::__checkLevelShift(si2drGroupIdT group) {
    si2drErrorT err;
    si2drAttrIdT ils, lst, ivr, ovr;
    ils = __si2drGroupFindAttrByName(group, si2drStringT("is_level_shifter"),
                                     &err);
    lst = __si2drGroupFindAttrByName(group, si2drStringT("level_shifter_type"),
                                     &err);
    ivr = __si2drGroupFindAttrByName(group, si2drStringT("input_voltage_range"),
                                     &err);
    ovr = __si2drGroupFindAttrByName(
        group, si2drStringT("output_voltage_range"), &err);
    if (!__si2drObjectIsNull(ovr, &err) && __si2drObjectIsNull(ils, &err)) {
        si2drStringT df = __si2drObjectGetFileName(ovr, &err);
        int dl = __si2drObjectGetLineNo(ovr, &err);
        char EB[8000];
        snprintf(
            EB, sizeof(EB),
            "%s:%d, output_voltage_range is defined, but not is_level_shifter.",
            df, dl);
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                            &err);
        errcount_++;
    }
    if (!__si2drObjectIsNull(ivr, &err) && __si2drObjectIsNull(ils, &err)) {
        si2drStringT df = __si2drObjectGetFileName(ivr, &err);
        int dl = __si2drObjectGetLineNo(ivr, &err);
        char EB[8000];
        snprintf(
            EB, sizeof(EB),
            "%s:%d, input_voltage_range is defined, but not is_level_shifter.",
            df, dl);
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                            &err);
        errcount_++;
    }
    if (!__si2drObjectIsNull(lst, &err) && __si2drObjectIsNull(ils, &err)) {
        si2drStringT df = __si2drObjectGetFileName(lst, &err);
        int dl = __si2drObjectGetLineNo(lst, &err);
        char EB[8000];
        snprintf(
            EB, sizeof(EB),
            "%s:%d, level_shifter_type is defined, but not is_level_shifter.",
            df, dl);
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                            &err);
        errcount_++;
    }
    if ((!__si2drObjectIsNull(ivr, &err) && __si2drObjectIsNull(ovr, &err)) ||
        (__si2drObjectIsNull(ivr, &err) && !__si2drObjectIsNull(ovr, &err))) {
        si2drStringT df = __si2drObjectGetFileName(ivr, &err);
        int dl = __si2drObjectGetLineNo(ivr, &err);
        char EB[8000];
        snprintf(
            EB, sizeof(EB),
            "%s:%d, output_voltage_range and input_voltage_range must both "
            "be defined in a cell.",
            df, dl);
        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                            &err);
        errcount_++;
    }
    /* not all the rules are checked here (yet) */
}
void LibAnalysis::__checkRefTimes(si2drGroupIdT group) {
    si2drErrorT err;
    si2drGroupsIdT subs = __si2drGroupGetGroups(group, &err);
    si2drGroupIdT subg, rg, rg2;
    group_enum gt;
    si2drAttrIdT refattr;
    int i = 0, j = 0, transvar;
    si2drFloat64T translewval = 0.0;
    refvals_.items = 0;
    while (
        !__si2drObjectIsNull((subg = __si2drIterNextGroup(subs, &err)), &err)) {
        gt = __si2drGroupGetID(subg, &err);
        if (gt == kTIMINGLIB_GROUPENUM_vector) {
            refattr = __si2drGroupFindAttrByName(
                subg, si2drStringT("reference_time"), &err);
            if (!__si2drObjectIsNull(refattr, &err)) {
                if (__si2drAttrGetAttrType(refattr, &err) == kSI2DR_SIMPLE &&
                    __si2drSimpleAttrGetValueType(refattr, &err) ==
                        kSI2DR_FLOAT64) {
                    si2drFloat64T val =
                        __si2drSimpleAttrGetFloat64Value(refattr, &err);
                    si2drNamesIdT nams = __si2drGroupGetNames(subg, &err);
                    si2drStringT nam = __si2drIterNextName(nams, &err);
                    int found = 0;
                    __si2drIterQuit(nams, &err);
                    /* any negative ref vals? */
                    if (val < 0.0) {
                        char EB[8000];
                        snprintf(EB, sizeof(EB),
                                 "%s:%d, The reference_time attribute should "
                                 "not be negative.",
                                 __si2drObjectGetFileName(refattr, &err),
                                 static_cast<int>(
                                     __si2drObjectGetLineNo(refattr, &err)));
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                        errcount_++;
                    }
                    /* find the template, and decide whether index_1 or index_2
                   is the transition/slew  val */
                    transvar = 0;
                    rg = __getContainingGroup(group, si2drStringT("library"));
                    rg2 = __si2drGroupFindGroupByName(
                        rg, nam, si2drStringT("output_current_template"), &err);
                    if (!__si2drObjectIsNull(rg2, &err)) {
                        si2drAttrIdT var1, var2;
                        var1 = __si2drGroupFindAttrByName(
                            rg2, si2drStringT("variable_1"), &err);
                        if (__si2drAttrGetAttrType(var1, &err) ==
                                kSI2DR_SIMPLE &&
                            __si2drSimpleAttrGetValueType(var1, &err) ==
                                kSI2DR_STRING) {
                            si2drStringT varval =
                                __si2drSimpleAttrGetStringValue(var1, &err);
                            if (strstr(varval, "transition") ||
                                strstr(varval, "slew")) {
                                transvar = 1;
                            }
                        } else {
                            char EB[8000];
                            snprintf(EB, sizeof(EB),
                                     "%s:%d, The output_current_template has a "
                                     "problem with the variable_1 attribute.",
                                     __si2drObjectGetFileName(rg2, &err),
                                     static_cast<int>(
                                         __si2drObjectGetLineNo(rg2, &err)));
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                            errcount_++;
                            refvals_.reported[j] = 1;
                        }
                        var2 = __si2drGroupFindAttrByName(
                            rg2, si2drStringT("variable_2"), &err);
                        if (__si2drAttrGetAttrType(var2, &err) ==
                                kSI2DR_SIMPLE &&
                            __si2drSimpleAttrGetValueType(var2, &err) ==
                                kSI2DR_STRING) {
                            si2drStringT varval =
                                __si2drSimpleAttrGetStringValue(var2, &err);
                            if (strstr(varval, "transition") ||
                                strstr(varval, "slew")) {
                                transvar = 2;
                            }
                        } else {
                            char EB[8000];
                            snprintf(EB, sizeof(EB),
                                     "%s:%d, The output_current_template has a "
                                     "problem with the variable_2 attribute.",
                                     __si2drObjectGetFileName(rg2, &err),
                                     static_cast<int>(
                                         __si2drObjectGetLineNo(rg2, &err)));
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                            errcount_++;
                            refvals_.reported[j] = 1;
                        }
                    }
                    if (transvar) {
                        /* get the transition/slew value */
                        si2drAttrIdT translew_index;
                        translew_index = __si2drGroupFindAttrByName(
                            subg,
                            (si2drStringT)(
                                (transvar == 1 ? "index_1" : "index_2")),
                            &err);
                        if (!__si2drObjectIsNull(translew_index, &err)) {
                            if (__si2drAttrGetAttrType(translew_index, &err) ==
                                kSI2DR_COMPLEX) {
                                si2drValuesIdT v1 = __si2drComplexAttrGetValues(
                                    translew_index, &err);
                                si2drValueTypeT v1_type;
                                si2drInt32T v1_int;
                                si2drFloat64T v1_float;
                                si2drStringT v1_str;
                                si2drBooleanT v1_bool;
                                si2drExprT *v1_expr;
                                char *eon;
                                __si2drIterNextComplexValue(
                                    v1, &v1_type, &v1_int, &v1_float, &v1_str,
                                    &v1_bool, &v1_expr, &err);
                                __si2drIterQuit(v1, &err);
                                if (v1_type == kSI2DR_STRING) {
                                    translewval = strtod(v1_str, &eon);
                                } else {
                                    char EB[8000];
                                    snprintf(
                                        EB, sizeof(EB),
                                        "%s:%d, This index value should be "
                                        "a string, but is not.",
                                        __si2drObjectGetFileName(translew_index,
                                                                 &err),
                                        static_cast<int>(__si2drObjectGetLineNo(
                                            translew_index, &err)));
                                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                        kSI2DR_SEMANTIC_ERROR,
                                                        EB, &err);
                                    errcount_++;
                                }
                            }
                        } else {
                            char EB[8000];
                            snprintf(EB, sizeof(EB),
                                     "%s:%d, Could not find the index_1 or "
                                     "index_2 value.",
                                     __si2drObjectGetFileName(subg, &err),
                                     static_cast<int>(
                                         __si2drObjectGetLineNo(subg, &err)));
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SEMANTIC_ERROR, EB,
                                                &err);
                            errcount_++;
                        }
                    } else {
                        char EB[8000];
                        snprintf(
                            EB, sizeof(EB),
                            "%s:%d, Could not determine whether index_1 or "
                            "index_2 holds the transition/slew value",
                            __si2drObjectGetFileName(group, &err),
                            static_cast<int>(
                                __si2drObjectGetLineNo(group, &err)));
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                        errcount_++;
                    }
                    for (i = 0; i < refvals_.items; i++) {
                        if (!strncasecmp(refvals_.templateStr[i], nam, 200) &&
                            refvals_.val[i] == val &&
                            (refvals_.translew[i] == translewval)) {
                            found = 1;
                            break;
                        }
                    }
                    if (found) {
                        refvals_.count[i]++;
                    } else if (refvals_.items < NUMENTRIES) {
                        strncpy(refvals_.templateStr[refvals_.items], nam, 200);
                        refvals_.val[refvals_.items] = val;
                        refvals_.count[refvals_.items] = 1;
                        refvals_.reported[refvals_.items] = 0;
                        refvals_.oid[refvals_.items] = subg;
                        refvals_.translew[refvals_.items] = translewval;
                        refvals_.items++;
                    }
                }
            }
        }
    }
    __si2drIterQuit(subs, &err);
    for (i = 0; i < refvals_.items; i++) {
        if (refvals_.reported[i]) continue;
        for (j = i + 1; j < refvals_.items; j++) {
            if (refvals_.reported[j]) continue;
            if (strcasecmp(refvals_.templateStr[i], refvals_.templateStr[j]) ==
                    0 &&
                refvals_.translew[i] == refvals_.translew[j]) {
                /* the same template, same trans/slew, but different
                 * reference_time values */
                char EB[8000];
                snprintf(
                    EB, sizeof(EB),
                    "%s:%d, The reference_time attribute value (%g) "
                    "differs from that used in the vector (%g) at line %d.",
                    __si2drObjectGetFileName(refvals_.oid[j], &err),
                    static_cast<int>(
                        __si2drObjectGetLineNo(refvals_.oid[j], &err)),
                    refvals_.val[j], refvals_.val[i],
                    static_cast<int>(
                        __si2drObjectGetLineNo(refvals_.oid[i], &err)));
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR,
                                    EB, &err);
                errcount_++;
                refvals_.reported[j] = 1;
            }
        }
        refvals_.reported[i] = 1;
    }
}
void LibAnalysis::__checkCcsDensity(si2drGroupIdT group) {
    si2drErrorT err;
    si2drGroupIdT rg4;
    struct vector_list *vl_oc;
    int numvecs = 0;
    // int num_vl_oc = 0; /* actual size of the vl_ocr */
    si2drGroupsIdT vectors;
    int i, j;
    int found = 0;
    int num_x;
    int num_y;
    LONG_DOUBLE *x_vals;
    LONG_DOUBLE *y_vals;
    char *scoreboard;
    vectors = __si2drGroupGetGroups(group, &err);
    while (!(__si2drObjectIsNull((rg4 = __si2drIterNextGroup(vectors, &err)),
                                 &err))) {
        numvecs++;
    }
    __si2drIterQuit(vectors, &err);
    /* allocate a set of vector_list */
    vl_oc = static_cast<struct vector_list *>(
        calloc(sizeof(struct vector_list), numvecs));
    i = 0;
    vectors = __si2drGroupGetGroups(group, &err);
    while (!(__si2drObjectIsNull((rg4 = __si2drIterNextGroup(vectors, &err)),
                                 &err))) {
        struct timinglib_value_data *vdv = __timinglibGetValuesData(rg4);
        /* does this value already exist? */
        for (j = 0; j < i; j++) {
            if (vl_oc[j].x == vdv->index_info[0][0] &&
                vl_oc[j].y == vdv->index_info[1][0]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            /* the only really useful info in the vector is the index_1 and
             * index_2 values */
            vl_oc[i].x = vdv->index_info[0][0];
            vl_oc[i].y = vdv->index_info[1][0];
            vl_oc[i].group = rg4;
            i++;
        } else {
#ifdef NO_LONG_DOUBLE
            char EB[8000];
            snprintf(
                EB, sizeof(EB),
                "%s:%d, The index_1 (%g) and index_2 (%g) values in this "
                "vector already appear in a previous vector group (line %d).",
                __si2drObjectGetFileName(rg4, &err),
                static_cast<int>(__si2drObjectGetLineNo(rg4, &err)),
                vdv->index_info[0][0], vdv->index_info[1][0],
                static_cast<int>(__si2drObjectGetLineNo(vl_oc[j].group, &err)));
#else
            char EB[8000];
            snprintf(
                EB, sizeof(EB),
                "%s:%d, The index_1 (%Lg) and index_2 (%Lg) values in this "
                "vector already appear in a previous vector group (line %d).",
                __si2drObjectGetFileName(rg4, &err),
                static_cast<int>(__si2drObjectGetLineNo(rg4, &err)),
                vdv->index_info[0][0], vdv->index_info[1][0],
                static_cast<int>(__si2drObjectGetLineNo(vl_oc[j].group, &err)));
#endif
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR, EB,
                                &err);
            errcount_++;
        }
        __timinglibDestroyValueData(vdv);
    }
    __si2drIterQuit(vectors, &err);
    /* i should hold the actual number of unique vector index values */
    numvecs = i;
    x_vals = static_cast<LONG_DOUBLE *>(calloc(sizeof(LONG_DOUBLE), numvecs));
    num_x = 0;
    y_vals = static_cast<LONG_DOUBLE *>(calloc(sizeof(LONG_DOUBLE), numvecs));
    num_y = 0;
    for (i = 0; i < numvecs; i++) {
        found = 0;
        for (j = 0; j < num_x; j++) {
            if (x_vals[j] == vl_oc[i].x) {
                found = 1;
                break;
            }
        }
        if (!found) {
            x_vals[num_x++] = vl_oc[i].x;
        }
        found = 0;
        for (j = 0; j < num_y; j++) {
            if (y_vals[j] == vl_oc[i].y) {
                found = 1;
                break;
            }
        }
        if (!found) {
            y_vals[num_y++] = vl_oc[i].y;
        }
    }
    /* now, we can alloc and fill in the scoreboard */
#define INDX(x, y) (x * num_y + y)
    scoreboard = static_cast<char *>(calloc(sizeof(char), num_x * num_y));
    for (i = 0; i < numvecs; i++) {
        /* find the x and y vals in their respective x_vals and y_vals array,
         * remember the index to each */
        int x_ind, y_ind;
        found = 0;
        for (j = 0; j < num_x; j++) {
            if (x_vals[j] == vl_oc[i].x) {
                found = 1;
                break;
            }
        }
        if (!found) {
            fprintf(stderr, "STRANGE-- Shouldn't be printing this!\n");
        } else {
            x_ind = j;
            found = 0;
            for (j = 0; j < num_y; j++) {
                if (y_vals[j] == vl_oc[i].y) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                fprintf(stderr,
                        "STRANGE-- Really Shouldn't be printing this!\n");
            } else {
                y_ind = j;
                /* both found now-- and well they should be found! */
                scoreboard[INDX(x_ind, y_ind)] = 'X';
            }
        }
    }
    /* and now that the scoreboard is filled in, let's check for
       empty slots in the array */
    for (i = 0; i < num_x; i++) {
        for (j = 0; j < num_y; j++) {
            if (scoreboard[INDX(i, j)] != 'X') {
#ifdef NO_LONG_DOUBLE
                char EB[8000];
                snprintf(EB, sizeof(EB),
                         "%s:%d, There is no vector describing the current "
                         "values for index_1 = %g and index_2 = %g for this "
                         "output_current group.",
                         __si2drObjectGetFileName(group, &err),
                         static_cast<int>(__si2drObjectGetLineNo(group, &err)),
                         x_vals[i], y_vals[j]);
#else
                char EB[8000];
                snprintf(EB, sizeof(EB),
                         "%s:%d, There is no vector describing the current "
                         "values for index_1 = %Lg and index_2 = %Lg for this "
                         "output_current group.",
                         __si2drObjectGetFileName(group, &err),
                         static_cast<int>(__si2drObjectGetLineNo(group, &err)),
                         x_vals[i], y_vals[j]);
#endif
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR,
                                    EB, &err);
                errcount_++;
            }
        }
    }
    free(vl_oc);
    vl_oc = 0;
    // num_vl_oc =0;
    free(x_vals);
    free(y_vals);
    free(scoreboard);
}
void LibAnalysis::__groupSpecificChecks(si2drGroupIdT group) {
    si2drErrorT err;
    si2drStringT name = __si2drGroupGetGroupType(group, &err);

    const struct libGroupMap *x = lookup_group_name(name, strlen(name));
    group_enum grouptype = x->type;
    si2drAttrIdT attr;
    switch (grouptype) {
        case kTIMINGLIB_GROUPENUM_lut:
            break;
        case kTIMINGLIB_GROUPENUM_wire_load_table:
            break;
        case kTIMINGLIB_GROUPENUM_wire_load_selection:
            break;
        case kTIMINGLIB_GROUPENUM_interconnect_delay:
            break;
        case kTIMINGLIB_GROUPENUM_wire_load:
            break;
        case kTIMINGLIB_GROUPENUM_timing_range:
            break;
        case kTIMINGLIB_GROUPENUM_scaling_factors:
            break;
        case kTIMINGLIB_GROUPENUM_scaled_cell:
            break;
        case kTIMINGLIB_GROUPENUM_rise_transition_degradation:
            __checkLuTableTemplateArraysize(group);
            break;
        case kTIMINGLIB_GROUPENUM_rise_net_delay:
            break;
        case kTIMINGLIB_GROUPENUM_power_supply:
            break;
        case kTIMINGLIB_GROUPENUM_power_lut_template:
            break;
        case kTIMINGLIB_GROUPENUM_output_voltage:
            break;
        case kTIMINGLIB_GROUPENUM_operating_conditions:
            break;
        case kTIMINGLIB_GROUPENUM_model:
            break;
        case kTIMINGLIB_GROUPENUM_lu_table_template:
            __checkLutTemplate(group);
            break;
        case kTIMINGLIB_GROUPENUM_input_voltage:
            break;
        case kTIMINGLIB_GROUPENUM_fall_transition_degradation:
            __checkLuTableTemplateArraysize(group);
            break;
        case kTIMINGLIB_GROUPENUM_fall_net_delay:
            break;
        case kTIMINGLIB_GROUPENUM_em_lut_template:
            break;
        case kTIMINGLIB_GROUPENUM_type:
            break;
        case kTIMINGLIB_GROUPENUM_test_cell:
            break;
        case kTIMINGLIB_GROUPENUM_statetable:
            break;
        case kTIMINGLIB_GROUPENUM_state:
            break;
        case kTIMINGLIB_GROUPENUM_seq_bank:
            __checkClearNPreset(group);
            break;
        case kTIMINGLIB_GROUPENUM_seq:
            __checkClearNPreset(group);
            break;
        case kTIMINGLIB_GROUPENUM_routing_track:
            break;
        case kTIMINGLIB_GROUPENUM_mode_value:
            break;
        case kTIMINGLIB_GROUPENUM_mode_definition:
            break;
        case kTIMINGLIB_GROUPENUM_memory:
            break;
        case kTIMINGLIB_GROUPENUM_leakage_power:
            break;
        case kTIMINGLIB_GROUPENUM_latch_bank:
            __checkClearNPreset(group);
            break;
        case kTIMINGLIB_GROUPENUM_latch:
            __checkClearNPreset(group);
            break;
        case kTIMINGLIB_GROUPENUM_generated_clock:
            break;
        case kTIMINGLIB_GROUPENUM_ff_bank:
            __checkClearNPreset(group);
            __checkFfBankWidths(group);
            break;
        case kTIMINGLIB_GROUPENUM_ff:
            __checkClearNPreset(group);
            break;
        case kTIMINGLIB_GROUPENUM_bus:
            __checkBusPinDirections(group);
            __checkBusType(group);
            break;
        case kTIMINGLIB_GROUPENUM_tlatch:
            break;
        case kTIMINGLIB_GROUPENUM_rise_transition:
            __checkLuTableTemplateArraysize(group);
            break;
        case kTIMINGLIB_GROUPENUM_rise_propagation:
            __checkLuTableTemplateArraysize(group);
            break;
        case kTIMINGLIB_GROUPENUM_rise_constraint:
            break;
        case kTIMINGLIB_GROUPENUM_retaining_rise:
            break;
        case kTIMINGLIB_GROUPENUM_retaining_fall:
            break;
        case kTIMINGLIB_GROUPENUM_fall_transition:
            __checkLuTableTemplateArraysize(group);
            break;
        case kTIMINGLIB_GROUPENUM_fall_propagation:
            __checkLuTableTemplateArraysize(group);
            break;
        case kTIMINGLIB_GROUPENUM_fall_constraint:
            break;
        case kTIMINGLIB_GROUPENUM_cell_rise:
            __checkLuTableTemplateArraysize(group);
            break;
        case kTIMINGLIB_GROUPENUM_cell_fall:
            __checkLuTableTemplateArraysize(group);
            break;
        case kTIMINGLIB_GROUPENUM_cell_degradation:
            break;
        case kTIMINGLIB_GROUPENUM_timing:
            __checkCellNProp(group);
            break;
        case kTIMINGLIB_GROUPENUM_pin:
            __checkDriverTypes(group);
            __checkRetentionPin(group);
            break;
        case kTIMINGLIB_GROUPENUM_minimum_period:
            break;
        case kTIMINGLIB_GROUPENUM_min_pulse_width:
            break;
        case kTIMINGLIB_GROUPENUM_memory_write:
            break;
        case kTIMINGLIB_GROUPENUM_memory_read:
            break;
        case kTIMINGLIB_GROUPENUM_rise_power:
            __checkPowerLutTemplateArraysize(group);
            break;
        case kTIMINGLIB_GROUPENUM_power:
            __checkPowerLutTemplateArraysize(group);
            break;
        case kTIMINGLIB_GROUPENUM_fall_power:
            __checkPowerLutTemplateArraysize(group);
            break;
        case kTIMINGLIB_GROUPENUM_internal_power:
            /* if there is a value here, then check it; otherwise the
               power/rise_power/fall_power groups will be there */
            attr =
                __si2drGroupFindAttrByName(group, si2drStringT("values"), &err);
            if (!__si2drObjectIsNull(attr, &err)) {
                __checkPowerLutTemplateArraysize(group);
            }
            break;
        case kTIMINGLIB_GROUPENUM_em_max_toggle_rate:
            break;
        case kTIMINGLIB_GROUPENUM_electromigration:
            break;
        case kTIMINGLIB_GROUPENUM_bundle:
            __checkMembers(group);
            __checkBusPinDirections(group);
            break;
        case kTIMINGLIB_GROUPENUM_cell:
            __checkInterfaceTiming(group);
            __checkRetentionCell(group);
            __checkLevelShift(group);
            break;
        case kTIMINGLIB_GROUPENUM_library:
            break;
        case kTIMINGLIB_GROUPENUM_vector:
            __checkVector(group);
            break;
        case kTIMINGLIB_GROUPENUM_output_current_template:
            __checkOutputCurrentTemplate(group);
            break;
        case kTIMINGLIB_GROUPENUM_output_current_rise:
            __checkRefTimes(group);
            __checkCcsDensity(group);
            break;
        case kTIMINGLIB_GROUPENUM_output_current_fall:
            __checkRefTimes(group);
            __checkCcsDensity(group);
            break;
        default:
            break;
    }
}
void LibAnalysis::__checkGroupCorrespondence(si2drGroupIdT g,
                                             libsynt_group_info *gi) {
    int nc;
    si2drNamesIdT names;
    si2drErrorT err;
    si2drStringT str;
    si2drAttrIdT attr;
    si2drAttrsIdT attrs;
    si2drGroupIdT group;
    si2drGroupsIdT groups;
    int gl = __si2drObjectGetLineNo(g, &err);
    char *gf = __si2drObjectGetFileName(g, &err);
    char gtype[100];
    /* check all group attributes, including names, etc. to constraints */
    if (gi && gi->ref) gi = gi->ref;
    snprintf(gtype, sizeof(gtype), "%s", __si2drGroupGetGroupType(g, &err));
    /* names */
    nc = 0;
    names = __si2drGroupGetNames(g, &err);
    while ((str = __si2drIterNextName(names, &err))) {
        nc++;
    }
    __si2drIterQuit(names, &err);
    if (gi) {
        switch (gi->name_constraints) {
            case kSYNTAX_GNAME_NONE:
                if (nc > 0) {
                    char EB[8000];
                    snprintf(
                        EB, sizeof(EB),
                        "%s:%d, The %s group in this context is not supposed "
                        "to have any names, but %d name(s) is(are) there.",
                        gf, gl, gtype, nc);
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SYNTAX_ERROR, EB, &err);
                    errcount_++;
                }
                break;
            case kSYNTAX_GNAME_ONE:
                if (nc != 1) {
                    char EB[8000];
                    snprintf(
                        EB, sizeof(EB),
                        "%s:%d, The %s group in this context should have 1 "
                        "name, but %d names are there.",
                        gf, gl, gtype, nc);
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SYNTAX_ERROR, EB, &err);
                    errcount_++;
                }
                break;
            case kSYNTAX_GNAME_TWO:
                if (nc != 2) {
                    char EB[8000];
                    snprintf(
                        EB, sizeof(EB),
                        "%s:%d, The %s group in this context should have 2 "
                        "nams, but %d name(s) is(are) there.",
                        gf, gl, gtype, nc);
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SYNTAX_ERROR, EB, &err);
                    errcount_++;
                }
                break;
            case kSYNTAX_GNAME_THREE:
                if (nc != 3) {
                    char EB[8000];
                    snprintf(
                        EB, sizeof(EB),
                        "%s:%d, The %s group in this context should have 3 "
                        "nams, but %d name(s) is(are) there.",
                        gf, gl, gtype, nc);
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SYNTAX_ERROR, EB, &err);
                    errcount_++;
                }
                break;
            case kSYNTAX_GNAME_NONE_OR_ONE:
                if (nc != 1 && nc != 0) {
                    char EB[8000];
                    snprintf(
                        EB, sizeof(EB),
                        "%s:%d, The %s group in this context should have 0 "
                        "or 1 names, but %d names are there.",
                        gf, gl, gtype, nc);
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SYNTAX_ERROR, EB, &err);
                    errcount_++;
                }
                break;
            case kSYNTAX_GNAME_ONE_OR_MORE:
                if (nc == 0) {
                    char EB[8000];
                    snprintf(
                        EB, sizeof(EB),
                        "%s:%d, The %s group in this context should have 1 "
                        "or more names, but no names are there.",
                        gf, gl, gtype);
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SYNTAX_ERROR, EB, &err);
                    errcount_++;
                }
                break;
        }
    }
    /* attrs */
    attrs = __si2drGroupGetAttrs(g, &err);
    while (!(
        __si2drObjectIsNull((attr = __si2drIterNextAttr(attrs, &err)), &err))) {
        si2drStringT anam = __si2drAttrGetName(attr, &err);
        si2drAttrTypeT at = __si2drAttrGetAttrType(attr, &err);
        si2drObjectIdT toid;
        int al = __si2drObjectGetLineNo(attr, &err);
        char *af = __si2drObjectGetFileName(attr, &err);
        toid.v1 = NULL;
        if (at == kSI2DR_SIMPLE) {
            int isvar = __si2drSimpleAttrGetIsVar(attr, &err);
            if (isvar) /* don't check a variable definition */
                continue;
        }
        if (gi) LibHash::timinglibHashLookup(gi->attr_hash, anam, &toid);
        if (toid.v1 == NULL) {
            si2drDefineIdT def;
            int found;
            /* defines could explain the presence of a simple attr that
                       is not found in the group attrs */
            def = __si2drPIFindDefineByName(anam, &err);
            if (err != kSI2DR_NO_ERROR) {
                char EB[8000];
                snprintf(EB, sizeof(EB),
                         "%s:%d, The %s attribute is not allowed in the %s "
                         "group in this context.",
                         af ? af : "", al, anam, gtype);
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR,
                                    EB, &err);
                errcount_++;
            } else {
                /* check to make sure the define's constraints are being obeyed
                 */
                si2drValueTypeT valtype;
                si2drStringT allowed_group, name;
                si2drAttrTypeT t = __si2drAttrGetAttrType(attr, &err);
                char buf[1000], *p, *q;
                int dl = __si2drObjectGetLineNo(def, &err);
                char *df = __si2drObjectGetFileName(def, &err);
                si2drDefineGetInfo(def, &name, &allowed_group, &valtype, &err);
                snprintf(buf, sizeof(buf), "%s", allowed_group);
                q = buf;
                found = 0;
                snprintf(buf, sizeof(buf), "%s|", buf);
                if (strcmp(gtype, allowed_group)) {
                    while (*q && (p = strchr(q, '|'))) {
                        *p = 0;
                        if (!strcmp(q, gtype)) {
                            found = 1;
                            break;
                        }
                        q = p + 1;
                    }
                    if (!found) {
                        char EB[8000];
                        snprintf(EB, sizeof(EB),
                                 "%s:%d, The %s attribute was defined to be "
                                 "used in the %s group, not the %s group.",
                                 af ? af : "", al, anam, allowed_group, gtype);
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SYNTAX_ERROR, EB, &err);
                        errcount_++;
                    }
                }
                if (t != kSI2DR_SIMPLE) {
                    char EB[8000];
                    snprintf(EB, sizeof(EB),
                             "%s:%d, The %s attribute was defined and needs to "
                             "used as a simple attribute.",
                             af ? af : "", al, anam);
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SYNTAX_ERROR, EB, &err);
                    errcount_++;
                } else { /* #ifdef DONT_VERIFY_DEFINE_ATTRIBUTE_TYPE */
                    si2drValueTypeT vt;
                    vt = __si2drSimpleAttrGetValueType(attr, &err);
                    if (valtype == kSI2DR_STRING && vt != kSI2DR_STRING) {
                        if (vt == kSI2DR_EXPR) {
                            si2drExprT *e;
                            e = __si2drSimpleAttrGetExprValue(attr, &err);
                            if (e->valuetype != kSI2DR_STRING) {
                                char EB[8000];
                                snprintf(EB, sizeof(EB),
                                         "%s:%d, The %s attribute should be of "
                                         "type String.(See the define at line "
                                         "%d of file %s.)",
                                         af ? af : "", al, anam, dl, df);
                                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                    kSI2DR_SYNTAX_ERROR, EB,
                                                    &err);
                                errcount_++;
                            }
                        } else {
                            char EB[8000];
                            snprintf(
                                EB, sizeof(EB),
                                "%s:%d, The %s attribute should be of type "
                                "String.(See the define at line %d of file "
                                "%s.)",
                                af ? af : "", al, anam, dl, df);
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SYNTAX_ERROR, EB, &err);
                            errcount_++;
                        }
                    }
                    if (valtype != kSI2DR_STRING && vt == kSI2DR_STRING) {
                        char EB[8000];
                        snprintf(
                            EB, sizeof(EB),
                            "%s:%d, The %s attribute should not be of type "
                            "String.(See the define at line %d of file %s.)",
                            af ? af : "", al, anam, dl, df);
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SYNTAX_ERROR, EB, &err);
                        errcount_++;
                    }
                    if (valtype == kSI2DR_INT32 && vt != kSI2DR_INT32) {
                        char EB[8000];
                        snprintf(
                            EB, sizeof(EB),
                            "%s:%d, The %s attribute should be of type "
                            "'integer'.(See the define at line %d of file %s.)",
                            af ? af : "", al, anam, dl, df);
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SYNTAX_ERROR, EB, &err);
                        errcount_++;
                    }
                    if (valtype == kSI2DR_FLOAT64 && vt != kSI2DR_FLOAT64 &&
                        vt != kSI2DR_INT32) {
                        char EB[8000];
                        snprintf(
                            EB, sizeof(EB),
                            "%s:%d, The %s attribute should be of type "
                            "'float'.(See the define at line %d of file %s.)",
                            af ? af : "", al, anam, dl, df);
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SYNTAX_ERROR, EB, &err);
                        errcount_++;
                    }
                }
                /* #endif */
            }
        } else {
            __checkAttrCorrespondence(
                attr, static_cast<libsynt_attribute_info *>(toid.v1));
        }
    }
    __si2drIterQuit(attrs, &err);
    /* subgroups */
    groups = __si2drGroupGetGroups(g, &err);
    while (!(__si2drObjectIsNull((group = __si2drIterNextGroup(groups, &err)),
                                 &err))) {
        si2drStringT gt = __si2drGroupGetGroupType(group, &err);
        si2drObjectIdT toid;
        int al = __si2drObjectGetLineNo(group, &err);
        char *af = __si2drObjectGetFileName(group, &err);
        toid.v1 = NULL;
        if (gi) LibHash::timinglibHashLookup(gi->group_hash, gt, &toid);
        if (toid.v1 == NULL) {
            si2drDefineIdT def;
            /* check to see if a define_group is defined for this situation */
            def = __si2drPIFindDefineByName(gt, &err);
            if (err != kSI2DR_NO_ERROR) {
                char EB[8000];
                snprintf(
                    EB, sizeof(EB),
                    "%s:%d, The %s group is not allowed in the %s group in "
                    "this context.",
                    af ? af : "", al, gt, gtype);
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SEMANTIC_ERROR,
                                    EB, &err);
                errcount_++;
            } else {
                si2drStringT grname, grall;
                si2drValueTypeT grval;
                char *allowedParents;
                char *nextParent;
                int groupInListOfParents = 0;
                si2drDefineGetInfo(def, &grname, &grall, &grval, &err);
                /* the following code is a fix provided by David Mallis of SI2
                 */
                /* many thanks, David! */
                /* If group allowed in > 1 parents, grall comes in as
               "grp1|grp2|..."
                            // The original check "strcmp(grall,gtype)" only
               works if there is only 1.
                            // Instead, it is necessary to loop through the
               grall entries checking
                            // each one against the current parent group.
                            //
                            // See if this group is allowed in its parent by
               its define_group statement.
                            // - allowedParents must be turned to NULL for
               subsequent calls to strtok.
                            // - strdup the grall so strtok's NULL planting
               on delims doesn't hose it.
                            */
                allowedParents = strdup(grall);
                nextParent = allowedParents;
                do {
                    char *sep = strchr(nextParent, '|');
                    if (sep) *sep = 0;
                    if (!strcmp(nextParent, gtype)) {
                        groupInListOfParents = 1;
                        break;
                    }
                    if (sep)
                        nextParent = sep + 1;
                    else
                        nextParent = NULL;
                } while (nextParent);
                free(allowedParents);
                /* Synopsys infopop user Roman reports that the original version
               of the above code would leak allowedParents. He offered a
               variation that would eliminate the problem. I decided to
               modify the above to eliminate the use of strtok, whose linux
               man page advises "Never use these functions.", giving some
                               reasons, and this was a perfect example of
               why not.
                            */
                if (grval == kSI2DR_UNDEFINED_VALUETYPE) {
                    /* if this is a group, I hear it MUST have at least one name
                     * associated with it! */
                    si2drErrorT err;
                    si2drStringT str;
                    si2drNamesIdT names;
                    names = __si2drGroupGetNames(group, &err);
                    str = __si2drIterNextName(names, &err);
                    if (!str || !str[0]) {
                        char EB[8000];
                        snprintf(EB, sizeof(EB),
                                 "%s:%d, The %s group (a user defined group) "
                                 "should have a name associated with it .",
                                 af ? af : "", al, gt);
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SEMANTIC_ERROR, EB, &err);
                        errcount_++;
                    }
                    __si2drIterQuit(names, &err);
                }
                if (!groupInListOfParents &&
                    grval == kSI2DR_UNDEFINED_VALUETYPE) {
                    char EB[8000];
                    snprintf(EB, sizeof(EB),
                             "%s:%d, The %s group is allowed in the %s group, "
                             "but not allowed in the %s group in this case.",
                             af ? af : "", al, gt, grall, gtype);
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SEMANTIC_ERROR, EB, &err);
                    errcount_++;
                } else {
                    __checkGroupCorrespondence(
                        group, static_cast<libsynt_group_info *>(toid.v1));
                }
            }
        } else {
            __checkGroupCorrespondence(
                group, static_cast<libsynt_group_info *>(toid.v1));
        }
    }
    __si2drIterQuit(groups, &err);
    if (gi) __groupSpecificChecks(g);
}
void LibAnalysis::__checkAttrCorrespondence(si2drAttrIdT attr,
                                            libsynt_attribute_info *ai) {
    si2drErrorT err;
    si2drAttrTypeT t = __si2drAttrGetAttrType(attr, &err);
    int al = __si2drObjectGetLineNo(attr, &err);
    char *af = __si2drObjectGetFileName(attr, &err);
    si2drStringT aname = __si2drAttrGetName(attr, &err);
    si2drValueTypeT vt;
    si2drInt32T intgr;
    si2drFloat64T float64;
    si2drStringT string;
    // si2drBooleanT   boolean;
    int found;
    int i;
    if (t == kSI2DR_SIMPLE) {
        if (ai->type == kSYNTAX_ATTRTYPE_COMPLEX ||
            ai->type == kSYNTAX_ATTRTYPE_COMPLEX_UNKNOWN ||
            ai->type == kSYNTAX_ATTRTYPE_COMPLEX_LIST) {
            char EB[8000];
            snprintf(
                EB, sizeof(EB),
                "%s:%d, Attribute %s should be complex, but is simple here.",
                af ? af : "", al, aname);
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR, EB,
                                &err);
            errcount_++;
        } else {
            char *x;
            si2drExprT *expr;
            vt = __si2drSimpleAttrGetValueType(attr, &err);
            if (vt == kSI2DR_EXPR) {
                expr = __si2drSimpleAttrGetExprValue(attr, &err);
                vt = expr->valuetype;
            } else {
                expr = 0;
            }
            switch (vt) {
                case kSI2DR_INT32:
                    if (expr)
                        intgr = expr->u.i;
                    else
                        intgr = __si2drSimpleAttrGetInt32Value(attr, &err);
                    switch (ai->type) {
                        case kSYNTAX_ATTRTYPE_STRING:
                            char EB[8000];
                            snprintf(EB, sizeof(EB),
                                     "%s:%d, Attribute %s should be a string "
                                     "Attribute, but is integer instead.",
                                     af ? af : "", al, aname);
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SYNTAX_ERROR, EB, &err);
                            errcount_++;
                            break;
                        case kSYNTAX_ATTRTYPE_INT:
                            if (ai->u.intcon) {
                                if (ai->u.intcon->greater_specd) {
                                    if (intgr < ai->u.intcon->greater_than) {
                                        char EB[8000];
                                        snprintf(
                                            EB, sizeof(EB),
                                            "%s:%d, Attribute %s has value %d, "
                                            "but it should be greater than %d.",
                                            af ? af : "", al, aname,
                                            static_cast<int>(intgr),
                                            ai->u.intcon->greater_than);
                                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                            kSI2DR_SYNTAX_ERROR,
                                                            EB, &err);
                                        errcount_++;
                                    }
                                }
                                if (ai->u.intcon->less_specd) {
                                    if (intgr > ai->u.intcon->less_than) {
                                        char EB[8000];
                                        snprintf(
                                            EB, sizeof(EB),
                                            "%s:%d, Attribute %s has value %d, "
                                            "but it should be less than %d.",
                                            af ? af : "", al, aname,
                                            static_cast<int>(intgr),
                                            ai->u.intcon->less_than);
                                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                            kSI2DR_SYNTAX_ERROR,
                                                            EB, &err);
                                        errcount_++;
                                    }
                                }
                            }
                            break;
                        case kSYNTAX_ATTRTYPE_FLOAT:
                            if (ai->u.floatcon) {
                                if (ai->u.floatcon->greater_specd) {
                                    if (static_cast<double>(intgr) <
                                        ai->u.floatcon->greater_than) {
                                        char EB[8000];
                                        snprintf(
                                            EB, sizeof(EB),
                                            "%s:%d, Attribute %s has value %d, "
                                            "but it should be greater than %g.",
                                            af ? af : "", al, aname,
                                            static_cast<int>(intgr),
                                            ai->u.floatcon->greater_than);
                                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                            kSI2DR_SYNTAX_ERROR,
                                                            EB, &err);
                                        errcount_++;
                                    }
                                }
                                if (ai->u.floatcon->less_specd) {
                                    if (static_cast<double>(intgr) >
                                        ai->u.floatcon->less_than) {
                                        char EB[8000];
                                        snprintf(
                                            EB, sizeof(EB),
                                            "%s:%d, Attribute %s has value %d, "
                                            "but it should be less than %g.",
                                            af ? af : "", al, aname,
                                            static_cast<int>(intgr),
                                            ai->u.floatcon->less_than);
                                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                            kSI2DR_SYNTAX_ERROR,
                                                            EB, &err);
                                        errcount_++;
                                    }
                                }
                            }
                            break;
                        case kSYNTAX_ATTRTYPE_VIRTUAL:
                            break;
                        case kSYNTAX_ATTRTYPE_BOOLEAN: {
                            char EB[8000];
                            snprintf(EB, sizeof(EB),
                                     "%s:%d, Attribute %s should be a boolean "
                                     "Attribute, but is integer instead.",
                                     af ? af : "", al, aname);
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SYNTAX_ERROR, EB, &err);
                            errcount_++;
                        } break;
                        case kSYNTAX_ATTRTYPE_ENUM: {
                            char EB[8000];
                            snprintf(EB, sizeof(EB),
                                     "%s:%d, Attribute %s should be a string "
                                     "Attribute, but is integer instead.",
                                     af ? af : "", al, aname);
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SYNTAX_ERROR, EB, &err);
                            errcount_++;
                        } break;
                        default:
                            break;
                    }
                    break;
                case kSI2DR_STRING:
                    if (expr)
                        string = expr->u.s;
                    else
                        string = __si2drSimpleAttrGetStringValue(attr, &err);
                    switch (ai->type) {
                        case kSYNTAX_ATTRTYPE_STRING:
                        case kSYNTAX_ATTRTYPE_VIRTUAL:
                            break;
                        case kSYNTAX_ATTRTYPE_ENUM:
                            found = 0;
                            for (i = 0; i < ai->u.stringenum->size; i++) {
                                if (!strcasecmp(ai->u.stringenum->array[i],
                                                string)) {
                                    found = 1;
                                    break;
                                }
                            }
                            if (!found) {
                                char EB[8000];
                                snprintf(EB, sizeof(EB),
                                         "%s:%d, Attribute %s has value %s, "
                                         "which does not match any of the "
                                         "following values:\n     ",
                                         af ? af : "", al, aname, string);
                                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                    kSI2DR_SYNTAX_ERROR, EB,
                                                    &err);
                                for (i = 0; i < ai->u.stringenum->size; i++) {
                                    printf("%s  ", ai->u.stringenum->array[i]);
                                }
                                EB[0] = 0;
                            }
                            break;
                        case kSYNTAX_ATTRTYPE_BOOLEAN:
                            if (strcasecmp(string, "false") &&
                                strcasecmp(string, "true")) {
                                char EB[8000];
                                snprintf(
                                    EB, sizeof(EB),
                                    "%s:%d, Attribute %s should be a boolean "
                                    "Attribute, but is a string (%s) instead.",
                                    af ? af : "", al, aname, string);
                                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                    kSI2DR_SYNTAX_ERROR, EB,
                                                    &err);
                            }
                            break;
                        case kSYNTAX_ATTRTYPE_INT:
                        case kSYNTAX_ATTRTYPE_FLOAT:
                            strtod(string, &x);
                            if (*x != 0) {
                                if (!__isaFormula(string)) {
                                    char EB[8000];
                                    snprintf(EB, sizeof(EB),
                                             "%s:%d, Attribute %s should be an "
                                             "integer or float Attribute, but "
                                             "is a string (%s) instead.",
                                             af ? af : "", al, aname, string);
                                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                        kSI2DR_SYNTAX_ERROR, EB,
                                                        &err);
                                    errcount_++;
                                }
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                case kSI2DR_FLOAT64:
                    if (expr)
                        float64 = expr->u.d;
                    else
                        float64 = __si2drSimpleAttrGetFloat64Value(attr, &err);
                    switch (ai->type) {
                        case kSYNTAX_ATTRTYPE_STRING: {
                            char EB[8000];
                            snprintf(EB, sizeof(EB),
                                     "%s:%d, Attribute %s should be a string "
                                     "Attribute, but is float instead.",
                                     af ? af : "", al, aname);
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SYNTAX_ERROR, EB, &err);
                            errcount_++;
                        } break;
                        case kSYNTAX_ATTRTYPE_BOOLEAN: {
                            char EB[8000];
                            snprintf(EB, sizeof(EB),
                                     "%s:%d, Attribute %s should be a boolean "
                                     "Attribute, but is float instead.",
                                     af ? af : "", al, aname);
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SYNTAX_ERROR, EB, &err);
                            errcount_++;
                        } break;
                        case kSYNTAX_ATTRTYPE_INT:
                            if (ai->u.intcon) {
                                if (ai->u.intcon->greater_specd) {
                                    if (float64 < ai->u.intcon->greater_than) {
                                        char EB[8000];
                                        snprintf(
                                            EB, sizeof(EB),
                                            "%s:%d, Attribute %s has value %g, "
                                            "but it should be greater than %d.",
                                            af ? af : "", al, aname, float64,
                                            ai->u.intcon->greater_than);
                                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                            kSI2DR_SYNTAX_ERROR,
                                                            EB, &err);
                                        errcount_++;
                                    }
                                }
                                if (ai->u.intcon->less_specd) {
                                    if (float64 > ai->u.intcon->less_than) {
                                        char EB[8000];
                                        snprintf(
                                            EB, sizeof(EB),
                                            "%s:%d, Attribute %s has value %g, "
                                            "but it should be less than %d.",
                                            af ? af : "", al, aname, float64,
                                            ai->u.intcon->less_than);
                                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                            kSI2DR_SYNTAX_ERROR,
                                                            EB, &err);
                                        errcount_++;
                                    }
                                }
                            }
                            break;
                        case kSYNTAX_ATTRTYPE_FLOAT:
                            if (ai->u.floatcon) {
                                if (ai->u.floatcon->greater_specd) {
                                    if (static_cast<double>(float64) <
                                        ai->u.floatcon->greater_than) {
                                        char EB[8000];
                                        snprintf(
                                            EB, sizeof(EB),
                                            "%s:%d, Attribute %s has value %g, "
                                            "but it should be greater than %g.",
                                            af ? af : "", al, aname, float64,
                                            ai->u.floatcon->greater_than);
                                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                            kSI2DR_SYNTAX_ERROR,
                                                            EB, &err);
                                        errcount_++;
                                    }
                                }
                                if (ai->u.floatcon->less_specd) {
                                    if (static_cast<double>(float64) >
                                        ai->u.floatcon->less_than) {
                                        char EB[8000];
                                        snprintf(
                                            EB, sizeof(EB),
                                            "%s:%d, Attribute %s has value %g, "
                                            "but it should be less than %g.",
                                            af ? af : "", al, aname, float64,
                                            ai->u.floatcon->less_than);
                                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                            kSI2DR_SYNTAX_ERROR,
                                                            EB, &err);
                                        errcount_++;
                                    }
                                }
                            }
                            break;
                        case kSYNTAX_ATTRTYPE_VIRTUAL:
                            break;
                        case kSYNTAX_ATTRTYPE_ENUM: {
                            char EB[8000];
                            snprintf(
                                EB, sizeof(EB),
                                "%s:%d, Attribute %s should be an enumerated "
                                "string Attribute, but is float instead.",
                                af ? af : "", al, aname);
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SYNTAX_ERROR, EB, &err);
                            errcount_++;
                        } break;
                        default:
                            break;
                    }
                    break;
                case kSI2DR_BOOLEAN:
                    switch (ai->type) {
                        case kSYNTAX_ATTRTYPE_STRING: {
                            char EB[8000];
                            snprintf(EB, sizeof(EB),
                                     "%s:%d, Attribute %s should be a string "
                                     "Attribute, but is boolean instead.",
                                     af ? af : "", al, aname);
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SYNTAX_ERROR, EB, &err);
                            errcount_++;
                        } break;
                        case kSYNTAX_ATTRTYPE_BOOLEAN:
                        case kSYNTAX_ATTRTYPE_VIRTUAL:
                            break;
                        case kSYNTAX_ATTRTYPE_INT:
                        case kSYNTAX_ATTRTYPE_FLOAT: {
                            char EB[8000];
                            snprintf(EB, sizeof(EB),
                                     "%s:%d, Attribute %s should be an "
                                     "integer/float Attribute, but is boolean "
                                     "instead.",
                                     af ? af : "", al, aname);
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SYNTAX_ERROR, EB, &err);
                            errcount_++;
                        } break;
                        case kSYNTAX_ATTRTYPE_ENUM: {
                            char EB[8000];
                            snprintf(
                                EB, sizeof(EB),
                                "%s:%d, Attribute %s should be an enumerated "
                                "string Attribute, but is boolean instead.",
                                af ? af : "", al, aname);
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SYNTAX_ERROR, EB, &err);
                            errcount_++;
                        } break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
    } else if (t == kSI2DR_COMPLEX) {
        if (ai->type != kSYNTAX_ATTRTYPE_COMPLEX &&
            ai->type != kSYNTAX_ATTRTYPE_COMPLEX_UNKNOWN &&
            ai->type != kSYNTAX_ATTRTYPE_COMPLEX_LIST) {
            char EB[8000];
            snprintf(
                EB, sizeof(EB),
                "%s:%d, Attribute %s should be simple, but is complex here.",
                af ? af : "", al, aname);
            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR, EB,
                                &err);
            errcount_++;
        } else {
            si2drValuesIdT vals = __si2drComplexAttrGetValues(attr, &err);
            si2drValueTypeT type;
            si2drInt32T intgr;
            si2drFloat64T float64;
            si2drStringT string;
            si2drBooleanT boolean;
            si2drExprT *expr;
            libsynt_argument *arg = ai->u.args;
            int argnum;
            argnum = 1;
            while (__si2drIterNextComplexValue(vals, &type, &intgr, &float64,
                                               &string, &boolean, &expr, &err),
                   type != kSI2DR_UNDEFINED_VALUETYPE) {
                if (!arg) {
                    char EB[8000];
                    snprintf(EB, sizeof(EB),
                             "%s:%d, %s has too many arguments.", af ? af : "",
                             al, aname);
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SYNTAX_ERROR, EB, &err);
                    errcount_++;
                }
                /* [nanda:20060824] Check if arg is NULL before dereferencing */
                if (arg && (arg->type == kSYNTAX_ATTRTYPE_COMPLEX_UNKNOWN ||
                            arg->type == kSYNTAX_ATTRTYPE_COMPLEX_LIST)) {
                    arg = arg->next;
                    break; /* that's all folks */
                }
                if (arg && (arg->type == kSYNTAX_ATTRTYPE_STRING &&
                            type != kSI2DR_STRING)) {
                    char EB[8000];
                    snprintf(
                        EB, sizeof(EB),
                        "%s:%d, Argument #%d of %s should be a string, but "
                        "it is not.",
                        af ? af : "", al, argnum, aname);
                    (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                        kSI2DR_SYNTAX_ERROR, EB, &err);
                    errcount_++;
                } else if (arg && (arg->type == kSYNTAX_ATTRTYPE_FLOAT &&
                                   (type == kSI2DR_STRING ||
                                    type == kSI2DR_BOOLEAN))) {
                    if (type == kSI2DR_STRING) {
                        /* it's ok, if what's in the string is an int! */
                        if (strspn(string, "0123456789.Ee-+") !=
                            strlen(string)) {
                            char EB[8000];
                            snprintf(
                                EB, sizeof(EB),
                                "%s:%d, Argument #%d of %s should be an int, "
                                "but it has non-integer characters in it (%s).",
                                af ? af : "", al, argnum, aname, string);
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SYNTAX_ERROR, EB, &err);
                            errcount_++;
                        }
                    } else {
                        char EB[8000];
                        snprintf(EB, sizeof(EB),
                                 "%s:%d, Argument #%d of %s should be a float, "
                                 "but it is not.",
                                 af ? af : "", al, argnum, aname);
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SYNTAX_ERROR, EB, &err);
                        errcount_++;
                    }
                } else if (arg && (arg->type == kSYNTAX_ATTRTYPE_INT &&
                                   (type == kSI2DR_STRING ||
                                    type == kSI2DR_BOOLEAN))) {
                    if (type == kSI2DR_STRING) {
                        /* it's ok, if what's in the string is an int! */
                        if (strspn(string, "0123456789+-") != strlen(string)) {
                            char EB[8000];
                            snprintf(
                                EB, sizeof(EB),
                                "%s:%d, Argument #%d of %s should be an int, "
                                "but it has non-integer characters in it (%s).",
                                af ? af : "", al, argnum, aname, string);
                            (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                                kSI2DR_SYNTAX_ERROR, EB, &err);
                            errcount_++;
                        }
                    } else {
                        char EB[8000];
                        snprintf(EB, sizeof(EB),
                                 "%s:%d, Argument #%d of %s should be an int, "
                                 "but it is not.",
                                 af ? af : "", al, argnum, aname);
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SYNTAX_ERROR, EB, &err);
                        errcount_++;
                    }
                } else if (arg && (arg->type == kSYNTAX_ATTRTYPE_BOOLEAN &&
                                   (type != kSI2DR_BOOLEAN))) {
                    if (type != kSI2DR_STRING ||
                        (strcasecmp(string, "true") &&
                         strcasecmp(string, "false"))) {
                        char EB[8000];
                        snprintf(
                            EB, sizeof(EB),
                            "%s:%d, Argument #%d of %s should be a boolean "
                            "(true or false), but it is not.",
                            af ? af : "", al, argnum, aname);
                        (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR,
                                            kSI2DR_SYNTAX_ERROR, EB, &err);
                        errcount_++;
                    }
                }
                argnum++;
                if (arg) arg = arg->next;
            }
            __si2drIterQuit(vals, &err);
            if (arg) {
                char EB[8000];
                snprintf(EB, sizeof(EB), "%s:%d, %s has too few arguments.",
                         af ? af : "", al, aname);
                (this->*si2ErrMsg_)(kSI2DR_SEVERITY_ERR, kSI2DR_SYNTAX_ERROR,
                                    EB, &err);
                errcount_++;
            }
        }
    }
    __attributeSpecificChecks(attr);
}
void LibAnalysis::__dumpAttr(libsynt_attribute_info *a, FILE *outC,
                             FILE *outH) {
    int i;
    libsynt_argument *arg;
    if (!a) return;
    switch (a->type) {
        case kSYNTAX_ATTRTYPE_STRING:
            fprintf(outH, "extern libsynt_attribute_info attr_%llx;\n",
                    reinterpret_cast<uint64_t>(a));
            fprintf(outC,
                    "libsynt_attribute_info attr_%llx = { "
                    "static_cast<char*>(\"%s\"), k%s, "
                    "%d, {0}, %s};\n",
                    reinterpret_cast<uint64_t>(a), a->name, toString(a->type),
                    a->lineno, LibHash::makeRep("attr", a->next).c_str());
            break;
        case kSYNTAX_ATTRTYPE_VIRTUAL:
            fprintf(outH, "extern libsynt_attribute_info attr_%llx;\n",
                    reinterpret_cast<uint64_t>(a));
            fprintf(outC,
                    "libsynt_attribute_info attr_%llx = { "
                    "static_cast<char*>(\"%s\"), k%s, "
                    "%d, {0}, %s};\n",
                    reinterpret_cast<uint64_t>(a), a->name, toString(a->type),
                    a->lineno, LibHash::makeRep("attr", a->next).c_str());
            break;
        case kSYNTAX_ATTRTYPE_ENUM:
            fprintf(outC, "char *enum_%llx[%d] = {",
                    reinterpret_cast<uint64_t>(a->u.stringenum->array),
                    a->u.stringenum->size);
            for (i = 0; i < a->u.stringenum->size; i++) {
                if (a->u.stringenum->array[i] == nullptr) {
                    int a = 10;
                }
                fprintf(outC, "(char*)(\"%s\"),", a->u.stringenum->array[i]);
            }
            fprintf(outC, "};\n");
            fprintf(outC,
                    "libsynt_string_enum strenum_%llx = { enum_%llx, %d };\n",
                    reinterpret_cast<uint64_t>(a->u.stringenum),
                    reinterpret_cast<uint64_t>(a->u.stringenum->array),
                    a->u.stringenum->size);
            fprintf(outH, "extern libsynt_attribute_info attr_%llx;\n",
                    reinterpret_cast<uint64_t>(a));
            fprintf(outC,
                    "libsynt_attribute_info attr_%llx = { "
                    "static_cast<char*>(\"%s\"), k%s, "
                    "%d, {.stringenum = (libsynt_string_enum*)%s}, %s};\n",
                    reinterpret_cast<uint64_t>(a), a->name, toString(a->type),
                    a->lineno,
                    LibHash::makeRep("strenum", a->u.stringenum).c_str(),
                    LibHash::makeRep("attr", a->next).c_str());
            break;
        case kSYNTAX_ATTRTYPE_FLOAT:
            if (a->u.floatcon && a->u.floatcon->refd_default)
                fprintf(
                    outC,
                    "libsynt_float_constraint floatcons_%llx = {%g, %g, %g, "
                    "static_cast<char*>(\"%s\"), %d, %d, %d, %d};\n",
                    reinterpret_cast<uint64_t>(a->u.floatcon),
                    a->u.floatcon->greater_than, a->u.floatcon->less_than,
                    a->u.floatcon->default_value, a->u.floatcon->refd_default,
                    a->u.floatcon->greater_specd, a->u.floatcon->less_specd,
                    a->u.floatcon->default_specd,
                    a->u.floatcon->ref_default_specd);
            else if (a->u.floatcon && !a->u.floatcon->refd_default)
                fprintf(
                    outC,
                    "libsynt_float_constraint floatcons_%llx = {%g, %g, %g, "
                    "0, %d, %d, %d, %d};\n",
                    reinterpret_cast<uint64_t>(a->u.floatcon),
                    a->u.floatcon->greater_than, a->u.floatcon->less_than,
                    a->u.floatcon->default_value, a->u.floatcon->greater_specd,
                    a->u.floatcon->less_specd, a->u.floatcon->default_specd,
                    a->u.floatcon->ref_default_specd);
            fprintf(outH, "extern libsynt_attribute_info attr_%llx;\n",
                    reinterpret_cast<uint64_t>(a));
            fprintf(outC,
                    "libsynt_attribute_info attr_%llx = { "
                    "static_cast<char*>(\"%s\"), k%s, "
                    "%d, {.floatcon=(libsynt_float_constraint*)%s}, %s};\n",
                    reinterpret_cast<uint64_t>(a), a->name, toString(a->type),
                    a->lineno,
                    LibHash::makeRep("floatcons", a->u.stringenum).c_str(),
                    LibHash::makeRep("attr", a->next).c_str());
            break;
        case kSYNTAX_ATTRTYPE_INT:
            if (a->u.intcon && a->u.intcon->refd_default)
                fprintf(outC,
                        "libsynt_int_constraint intcons_%llx = {%d, %d, %d, "
                        "static_cast<char*>(\"%s\"), %d, %d, %d, %d};\n",
                        reinterpret_cast<uint64_t>(a->u.intcon),
                        a->u.intcon->greater_than, a->u.intcon->less_than,
                        a->u.intcon->default_value, a->u.intcon->refd_default,
                        a->u.intcon->greater_specd, a->u.intcon->less_specd,
                        a->u.intcon->default_specd,
                        a->u.intcon->ref_default_specd);
            else if (a->u.intcon && !a->u.intcon->refd_default)
                fprintf(outC,
                        "libsynt_int_constraint intcons_%llx = {%d, %d, %d, 0, "
                        "%d, %d, %d, %d};\n",
                        reinterpret_cast<uint64_t>(a->u.intcon),
                        a->u.intcon->greater_than, a->u.intcon->less_than,
                        a->u.intcon->default_value, a->u.intcon->greater_specd,
                        a->u.intcon->less_specd, a->u.intcon->default_specd,
                        a->u.intcon->ref_default_specd);
            fprintf(outH, "extern libsynt_attribute_info attr_%llx;\n",
                    reinterpret_cast<uint64_t>(a));
            fprintf(outC,
                    "libsynt_attribute_info attr_%llx = { "
                    "static_cast<char*>(\"%s\"), k%s, "
                    "%d, {.intcon=(libsynt_int_constraint*)%s}, %s};\n",
                    reinterpret_cast<uint64_t>(a), a->name, toString(a->type),
                    a->lineno,
                    LibHash::makeRep("intcons", a->u.stringenum).c_str(),
                    LibHash::makeRep("attr", a->next).c_str());
            break;
        case kSYNTAX_ATTRTYPE_BOOLEAN:
            fprintf(outH, "extern libsynt_attribute_info attr_%llx;\n",
                    reinterpret_cast<uint64_t>(a));
            fprintf(outC,
                    "libsynt_attribute_info attr_%llx = { "
                    "static_cast<char*>(\"%s\"), k%s, "
                    "%d, {0}, %s};\n",
                    reinterpret_cast<uint64_t>(a), a->name, toString(a->type),
                    a->lineno, LibHash::makeRep("attr", a->next).c_str());
            break;
        case kSYNTAX_ATTRTYPE_COMPLEX:
        case kSYNTAX_ATTRTYPE_COMPLEX_UNKNOWN:
        case kSYNTAX_ATTRTYPE_COMPLEX_LIST:
            for (arg = a->u.args; arg; arg = arg->next) {
                fprintf(outH, "extern libsynt_argument arg_%llx;\n",
                        reinterpret_cast<uint64_t>(arg));
                fprintf(outC, "libsynt_argument arg_%llx = {k%s, %s};\n",
                        reinterpret_cast<uint64_t>(arg), toString(arg->type),
                        LibHash::makeRep("arg", arg->next).c_str());
            }
            fprintf(outH, "extern libsynt_attribute_info attr_%llx;\n",
                    reinterpret_cast<uint64_t>(a));
            fprintf(outC,
                    "libsynt_attribute_info attr_%llx = { "
                    "static_cast<char*>(\"%s\"), k%s, "
                    "%d, {.args=(libsynt_argument*)%s}, %s};\n",
                    reinterpret_cast<uint64_t>(a), a->name, toString(a->type),
                    a->lineno, LibHash::makeRep("arg", a->u.stringenum).c_str(),
                    LibHash::makeRep("attr", a->next).c_str());
            break;
    }
}
void LibAnalysis::__dumpGroup(libsynt_group_info *g, FILE *outC, FILE *outH) {
    libsynt_group_info *ng;
    libsynt_attribute_info *at;
    g->mark = 1;
    for (ng = g->group_list; ng; ng = ng->next) {
        if (!ng->mark) {
            __dumpGroup(ng, outC, outH);
            ng->mark++;
        }
    }
    for (at = g->attr_list; at; at = at->next) {
        __dumpAttr(at, outC, outH);
    }
    LibHash::dumpGroupHash(g->group_hash, outC, outH);
    LibHash::dumpAttrHash(g->attr_hash, outC, outH);
    fprintf(
        outC,
        "libsynt_group_info group_%llx = {%d, k%s, (char*)(\"%s\"), %d, %d, "
        "%s, %s, %s, %s, %s, %s};\n",
        reinterpret_cast<uint64_t>(g), g->mark, toString(g->name_constraints),
        g->type, g->ID, g->lineno,
        LibHash::makeRep("attr", g->attr_list).c_str(),
        LibHash::makeRep("group", g->group_list).c_str(),
        LibHash::makeRep("ht", g->attr_hash).c_str(),
        LibHash::makeRep("ht", g->group_hash).c_str(),
        LibHash::makeRep("group", g->next).c_str(),
        LibHash::makeRep("group", g->ref).c_str());
    fprintf(outH, "extern libsynt_group_info group_%llx;\n",
            reinterpret_cast<uint64_t>(g));
}
}  // namespace Timinglib
