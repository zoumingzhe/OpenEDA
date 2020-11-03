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
#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIBANALYSIS_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIBANALYSIS_H_

#include <alloca.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>

#include "db/timing/timinglib/attr_enum.h"
#include "db/timing/timinglib/group_enum.h"
#include "db/timing/timinglib/timinglib_libstrtab.h"
#include "db/timing/timinglib/timinglib_si2dr.h"
#include "db/timing/timinglib/timinglib_structs.h"

#define NUMENTRIES 200

#define MAX_INCLUDE_DEPTH 1

namespace Timinglib {

class LibBuilder;
class LibAnalysis;
typedef si2drVoidT (LibAnalysis::*si2drMessageHandlerT)(si2drSeverityT sev,
                                                        si2drErrorT errToPrint,
                                                        si2drStringT auxText,
                                                        si2drErrorT *err);
typedef enum {
    kITER_NAME,
    kITER_VAL,
    kITER_GROUP,
    kITER_ATTR,
    kITER_DEF,
    kITER_NOTHING
} timinglib_iter_type;

typedef struct iterat {
    void *next;
    timinglib_iter_type owner;
} iterat;

struct reference_time_vals {
    char templateStr[NUMENTRIES][200];
    si2drFloat64T val[NUMENTRIES];
    LONG_DOUBLE translew[NUMENTRIES];
    int count[NUMENTRIES];
    int items;
    int reported[NUMENTRIES];
    si2drGroupIdT oid[NUMENTRIES];
};

struct vector_list {
    LONG_DOUBLE x, y;
    int used;
    si2drGroupIdT group;
};

typedef struct var_list {
    char *name;
    struct var_list *next;
} var_list;

typedef struct token_q {
    int retval;      /* the token value to return */
    int set_num;     // set the yylval to contain the num; -1 is an int; +1 is a
                     // float
    double floatnum; /* number to put to yylval */
    int intnum;      /* number to put to yylval */
    char *str;       /* if non-zero, put to yylval */
} token_q;

typedef struct nl {
    char *fname;
    struct nl *next;
} struct_nl;

typedef struct scandata {
    timinglib_group *master_group_list = nullptr;
    timinglib_hash_table *master_group_hash = nullptr;
    timinglib_hash_table *master_define_hash = nullptr;
    timinglib_strtable *master_string_table = nullptr;
    int token_q_front = 9;
    int token_q_rear = 9;
    token_q token_q_buf[10];
    int tok_encountered = 0;

    unsigned lline = 0;
    unsigned lineno = 0;
    unsigned save_lline = 0;
    unsigned save_lineno = 0;
    int tight_colon_ok = 0;
    char filenamebuf[1024] = {'\0'};
    char string_buf[80960] = {'\0'};
    char token_comment_buf[102400] = {'\0'};
    char token_comment_buf2[102400] = {'\0'};
    char *string_buf_ptr = {'\0'};
    struct nl *file_name_list = nullptr;
    char *curr_file = {'\0'};
    char *curr_file_save = {'\0'};
    int include_stack_index = 0;
    void *include_stack[MAX_INCLUDE_DEPTH] = {nullptr};

    si2drGroupIdT gs[1000];
    int gsindex = 0;
    si2drErrorT err;
    si2drAttrTypeT atype;
    si2drAttrIdT curr_attr;
    si2drDefineIdT curr_def;
    int syntax_errors = 0;
    char PB[8000] = {'\0'};

    libsynt_group_info *gsinfo[100] = {nullptr};
    timinglib_hash_table *libsynt_groups = nullptr;
    timinglib_hash_table *libsynt_allgroups = nullptr;
    timinglib_hash_table *libsynt_attrs = nullptr;
    libsynt_technology libsynt_techs[20];
    int libsynt_tech_count = 0;
    char *enumlist[100];
    int enumcount = 0;
} scandata;

class LibStrtab;
class LibSyn;
class LibAnalysis {
  public:
    explicit LibAnalysis(LibBuilder *libbuilder);
    ~LibAnalysis();
    void setLibertyFileName(const char *filename);
    void setLibertyParseLogStr(std::string *parseLogStr);
    scandata *getScandata(void);
    LibStrtab *getStrtab(void);
    char *search_string_for_linefeeds(char *str);
    bool parse_desc_file();
    void generateAttrEnumHeader();
    void generateGroupEnumHeader();
    void generateSyntaxDeclsHAndCPP();
    void generateAttrAndGroupLookup();
    void set_tok(void);
    void add_token(int retval, int set_num, int intnum, double floatnum,
                   char *str);
    int token_q_empty(void);
    int injected_token(void *val);
    void push_group(timinglib_head *h);
    void pop_group(timinglib_head *h);
    void make_complex(timinglib_head *h);
    void make_simple(char *name, timinglib_attribute_value *v);
    void syntax_push_group(libsynt_head *h);
    void syntax_pop_group(void);
    libsynt_attribute_info *syntax_make_complex(libsynt_head *h);
    libsynt_attribute_info *syntax_make_simple(char *name,
                                               libsynt_attr_type type,
                                               void *constraint_ptr);
    bool isLibertySyntaxValid();
    void dumpLibFile(const char *const filename, bool clearFileContent = true);

    void clean_file_name(char *dirty, char *clean);
    si2drValueTypeT convert_vt(char *type);
    si2drMessageHandlerT si2drPIGetMessageHandler(si2drErrorT *err);
    si2drVoidT si2drSimpleAttrSetIsVar(si2drAttrIdT attr, si2drErrorT *err);
    /* helper functions for expr creation, building, and destruction */
    si2drVoidT si2drExprDestroy(
        si2drExprT *expr, /* recursively free the structures */
        si2drErrorT *err);
    si2drExprT *si2drCreateExpr(
        si2drExprTypeT type, /* malloc an Expr and return it */
        si2drErrorT *err);
    si2drStringT si2drExprToString(si2drExprT *expr, si2drErrorT *err);
    /* Define related funcs */
    si2drDefineIdT si2drGroupCreateDefine(si2drGroupIdT group,
                                          si2drStringT name,
                                          si2drStringT allowed_group_name,
                                          si2drValueTypeT valtype,
                                          si2drErrorT *err);
    si2drVoidT si2drDefineGetInfo(si2drDefineIdT def, si2drStringT *name,
                                  si2drStringT *allowed_group_name,
                                  si2drValueTypeT *valtype, si2drErrorT *err);
    si2drVoidT si2drPIInit(si2drErrorT *err);
    si2drVoidT si2drPIQuit(si2drErrorT *err);
    si2drVoidT si2drPIUnSetNocheckMode(si2drErrorT *err);
    si2drVoidT si2drPISetNocheckMode(si2drErrorT *err);
    si2drVoidT si2drDefineSetComment(si2drDefineIdT def, si2drStringT comment,
                                     si2drErrorT *err);
    si2drVoidT si2drObjectSetLineNo(si2drObjectIdT object, si2drInt32T lineno,
                                    si2drErrorT *err);
    si2drVoidT si2drObjectSetFileName(si2drObjectIdT object,
                                      si2drStringT filename, si2drErrorT *err);

  private:
    si2drObjectTypeT __si2drObjectGetObjectType(si2drObjectIdT object,
                                                si2drErrorT *err);
    si2drObjectIdT __si2drObjectGetOwner(si2drObjectIdT object,
                                         si2drErrorT *err);
    si2drBooleanT __si2drObjectIsNull(si2drObjectIdT object, si2drErrorT *err);
    si2drBooleanT __si2drObjectIsSame(si2drObjectIdT object1,
                                      si2drObjectIdT object2, si2drErrorT *err);
    si2drBooleanT __si2drObjectIsUsable(si2drObjectIdT object,
                                        si2drErrorT *err);
    si2drInt32T __si2drObjectGetLineNo(si2drObjectIdT object, si2drErrorT *err);
    si2drStringT __si2drObjectGetFileName(si2drObjectIdT object,
                                          si2drErrorT *err);
    si2drVoidT __si2drReadLibertyFile(char *filename, si2drErrorT *err);
    si2drVoidT __si2drWriteLibertyFile(FILE *of, si2drGroupIdT group,
                                       si2drErrorT *err);
    si2drVoidT __si2drCheckLibertyLibrary(si2drGroupIdT group,
                                          si2drErrorT *err);
    si2drBooleanT __si2drPIGetTraceMode(si2drErrorT *err);
    si2drVoidT __si2drPIUnSetTraceMode(si2drErrorT *err);
    si2drVoidT __si2drPISetTraceMode(si2drStringT fname, si2drErrorT *err);
    si2drVoidT __si2drPISetDebugMode(si2drErrorT *err);
    si2drVoidT __si2drPIUnSetDebugMode(si2drErrorT *err);
    si2drBooleanT __si2drPIGetDebugMode(si2drErrorT *err);
    si2drBooleanT __si2drPIGetNocheckMode(si2drErrorT *err);
    si2drVoidT __si2drGroupMoveAfter(si2drGroupIdT groupToMove,
                                     si2drGroupIdT targetGroup,
                                     si2drErrorT *err);
    si2drVoidT __si2drGroupMoveBefore(si2drGroupIdT groupToMove,
                                      si2drGroupIdT targetGroup,
                                      si2drErrorT *err);
    group_enum __si2drGroupGetID(si2drGroupIdT group, si2drErrorT *err);
    attr_enum __si2drAttrGetID(si2drAttrIdT attr, si2drErrorT *err);
    si2drVoidT __si2drIterQuit(si2drIterIdT iter, si2drErrorT *err);
    si2drVoidT __si2drObjectDelete(si2drObjectIdT object, si2drErrorT *err);
    static si2drStringT __si2drPIGetErrorText(si2drErrorT errorCode,
                                              si2drErrorT *err);
    si2drObjectIdT __si2drPIGetNullId(si2drErrorT *err);
    si2drStringT __si2drDefineGetName(si2drDefineIdT def, si2drErrorT *err);
    si2drStringT __si2drDefineGetAllowedGroupName(si2drDefineIdT def,
                                                  si2drErrorT *err);
    si2drValueTypeT __si2drDefineGetValueType(si2drDefineIdT def,
                                              si2drErrorT *err);
    si2drGroupIdT __si2drGroupCreateGroup(si2drGroupIdT group,
                                          si2drStringT name,
                                          si2drStringT group_type,
                                          si2drErrorT *err);
    si2drStringT __si2drGroupGetGroupType(si2drGroupIdT group,
                                          si2drErrorT *err);
    si2drStringT __si2drGroupGetComment(si2drGroupIdT group, si2drErrorT *err);
    si2drVoidT __si2drGroupSetComment(si2drGroupIdT group, si2drStringT comment,
                                      si2drErrorT *err);
    si2drStringT __si2drAttrGetComment(si2drAttrIdT attr, si2drErrorT *err);
    si2drVoidT __si2drAttrSetComment(si2drAttrIdT attr, si2drStringT comment,
                                     si2drErrorT *err);
    si2drStringT __si2drDefineGetComment(si2drDefineIdT def, si2drErrorT *err);
    si2drVoidT __si2drGroupAddName(si2drGroupIdT group, si2drStringT name,
                                   si2drErrorT *err);
    si2drVoidT __si2drGroupDeleteName(si2drGroupIdT group, si2drStringT name,
                                      si2drErrorT *err);
    si2drGroupIdT __si2drPIFindGroupByName(si2drStringT name, si2drStringT type,
                                           si2drErrorT *err);
    si2drGroupIdT __si2drGroupFindGroupByName(si2drGroupIdT group,
                                              si2drStringT name,
                                              si2drStringT type,
                                              si2drErrorT *err);
    si2drAttrIdT __si2drGroupFindAttrByName(si2drGroupIdT group,
                                            si2drStringT name,
                                            si2drErrorT *err);
    si2drDefineIdT __si2drGroupFindDefineByName(si2drGroupIdT group,
                                                si2drStringT name,
                                                si2drErrorT *err);
    si2drDefineIdT __si2drPIFindDefineByName(si2drStringT name,
                                             si2drErrorT *err);
    si2drGroupsIdT __si2drPIGetGroups(si2drErrorT *err);
    si2drGroupsIdT __si2drGroupGetGroups(si2drGroupIdT group, si2drErrorT *err);
    si2drNamesIdT __si2drGroupGetNames(si2drGroupIdT group, si2drErrorT *err);
    si2drAttrsIdT __si2drGroupGetAttrs(si2drGroupIdT group, si2drErrorT *err);
    si2drDefinesIdT __si2drGroupGetDefines(si2drGroupIdT group,
                                           si2drErrorT *err);
    si2drGroupIdT __si2drIterNextGroup(si2drGroupsIdT iter, si2drErrorT *err);
    si2drStringT __si2drIterNextName(si2drNamesIdT iter, si2drErrorT *err);
    si2drAttrIdT __si2drIterNextAttr(si2drAttrsIdT iter, si2drErrorT *err);
    si2drDefineIdT __si2drIterNextDefine(si2drDefinesIdT iter,
                                         si2drErrorT *err);
    si2drExprTypeT __si2drExprGetType(si2drExprT *expr, si2drErrorT *err);
    si2drValueTypeT __si2drValExprGetValueType(si2drExprT *expr,
                                               si2drErrorT *err);
    si2drInt32T __si2drIntValExprGetInt(si2drExprT *expr, si2drErrorT *err);
    si2drFloat64T __si2drDoubleValExprGetDouble(si2drExprT *expr,
                                                si2drErrorT *err);
    si2drBooleanT __si2drBooleanValExprGetBoolean(si2drExprT *expr,
                                                  si2drErrorT *err);
    si2drStringT __si2drStringValExprGetString(si2drExprT *expr,
                                               si2drErrorT *err);
    si2drExprT *__si2drOpExprGetLeftExpr(
        si2drExprT *expr, /* will apply to Unary & binary Ops */
        si2drErrorT *err);
    si2drExprT *__si2drOpExprGetRightExpr(si2drExprT *expr, si2drErrorT *err);
    si2drExprT *__si2drCreateBooleanValExpr(si2drBooleanT b, si2drErrorT *err);
    si2drExprT *__si2drCreateDoubleValExpr(si2drFloat64T d, si2drErrorT *err);
    si2drExprT *__si2drCreateStringValExpr(si2drStringT s, si2drErrorT *err);
    si2drExprT *__si2drCreateIntValExpr(si2drInt32T i, si2drErrorT *err);
    si2drExprT *__si2drCreateBinaryOpExpr(si2drExprT *left,
                                          si2drExprTypeT optype,
                                          si2drExprT *right, si2drErrorT *err);
    si2drExprT *__si2drCreateUnaryOpExpr(si2drExprTypeT optype,
                                         si2drExprT *expr, si2drErrorT *err);
    si2drVoidT __si2drDefaultMessageHandler(si2drSeverityT sev,
                                            si2drErrorT errToPrint,
                                            si2drStringT auxText,
                                            si2drErrorT *err);
    si2drVoidT __si2drPISetMessageHandler(si2drMessageHandlerT MsgHandFuncPtr,
                                          si2drErrorT *err);
    si2drGroupIdT __si2drPICreateGroup(si2drStringT name,
                                       si2drStringT group_type,
                                       si2drErrorT *err);
    si2drAttrIdT __si2drGroupCreateAttr(si2drGroupIdT group, si2drStringT name,
                                        si2drAttrTypeT type, si2drErrorT *err);
    si2drAttrTypeT __si2drAttrGetAttrType(si2drAttrIdT attr, si2drErrorT *err);
    si2drStringT __si2drAttrGetName(si2drAttrIdT attr, si2drErrorT *err);
    si2drVoidT __si2drComplexAttrAddInt32Value(si2drAttrIdT attr,
                                               si2drInt32T intgr,
                                               si2drErrorT *err);
    si2drVoidT __si2drComplexAttrAddStringValue(si2drAttrIdT attr,
                                                si2drStringT string,
                                                si2drErrorT *err);
    si2drVoidT __si2drComplexAttrAddBooleanValue(si2drAttrIdT attr,
                                                 si2drBooleanT boolval,
                                                 si2drErrorT *err);
    si2drVoidT __si2drComplexAttrAddFloat64Value(si2drAttrIdT attr,
                                                 si2drFloat64T float64,
                                                 si2drErrorT *err);
    si2drVoidT __si2drComplexAttrAddExprValue(si2drAttrIdT attr,
                                              si2drExprT *expr,
                                              si2drErrorT *err);
    si2drValuesIdT __si2drComplexAttrGetValues(si2drAttrIdT attr,
                                               si2drErrorT *err);
    si2drVoidT __si2drIterNextComplexValue(
        si2drValuesIdT iter, si2drValueTypeT *type, si2drInt32T *intgr,
        si2drFloat64T *float64, si2drStringT *string, si2drBooleanT *boolval,
        si2drExprT **expr, si2drErrorT *err);
    si2drAttrComplexValIdT __si2drIterNextComplex(si2drValuesIdT iter,
                                                  si2drErrorT *err);
    si2drValueTypeT __si2drComplexValGetValueType(si2drAttrComplexValIdT attr,
                                                  si2drErrorT *err);
    si2drInt32T __si2drComplexValGetInt32Value(si2drAttrComplexValIdT attr,
                                               si2drErrorT *err);
    si2drFloat64T __si2drComplexValGetFloat64Value(si2drAttrComplexValIdT attr,
                                                   si2drErrorT *err);
    si2drStringT __si2drComplexValGetStringValue(si2drAttrComplexValIdT attr,
                                                 si2drErrorT *err);
    si2drBooleanT __si2drComplexValGetBooleanValue(si2drAttrComplexValIdT attr,
                                                   si2drErrorT *err);
    si2drExprT *__si2drComplexValGetExprValue(si2drAttrComplexValIdT attr,
                                              si2drErrorT *err);
    si2drValueTypeT __si2drSimpleAttrGetValueType(si2drAttrIdT attr,
                                                  si2drErrorT *err);
    si2drInt32T __si2drSimpleAttrGetInt32Value(si2drAttrIdT attr,
                                               si2drErrorT *err);
    si2drFloat64T __si2drSimpleAttrGetFloat64Value(si2drAttrIdT attr,
                                                   si2drErrorT *err);
    si2drStringT __si2drSimpleAttrGetStringValue(si2drAttrIdT attr,
                                                 si2drErrorT *err);
    si2drBooleanT __si2drSimpleAttrGetBooleanValue(si2drAttrIdT attr,
                                                   si2drErrorT *err);
    si2drExprT *__si2drSimpleAttrGetExprValue(si2drAttrIdT attr,
                                              si2drErrorT *err);
    si2drVoidT __si2drSimpleAttrSetInt32Value(si2drAttrIdT attr,
                                              si2drInt32T intgr,
                                              si2drErrorT *err);
    si2drVoidT __si2drSimpleAttrSetBooleanValue(si2drAttrIdT attr,
                                                si2drBooleanT intgr,
                                                si2drErrorT *err);
    si2drVoidT __si2drSimpleAttrSetFloat64Value(si2drAttrIdT attr,
                                                si2drFloat64T float64,
                                                si2drErrorT *err);
    si2drVoidT __si2drSimpleAttrSetStringValue(si2drAttrIdT attr,
                                               si2drStringT string,
                                               si2drErrorT *err);
    si2drVoidT __si2drSimpleAttrSetExprValue(si2drAttrIdT attr,
                                             si2drExprT *expr,
                                             si2drErrorT *err);
    si2drBooleanT __si2drSimpleAttrGetIsVar(si2drAttrIdT attr,
                                            si2drErrorT *err);
    si2drErrorT __syntax_check(si2drGroupIdT);
    void __trace_check(si2drObjectIdT oid);
    void __outinit_oid(si2drObjectIdT oid);
    void __outinit_iter(si2drIterIdT iter);
    void __inc_tracecount(void);
    void __remove_token(int *retval, int *set_num, int *intnum,
                        double *floatnum, char **str);
    void __dump_attr(libsynt_attribute_info *a, FILE *outC, FILE *outH);
    void __dump_group(libsynt_group_info *g, FILE *outC, FILE *outH);
    void __lib__write_group(FILE *of, si2drGroupIdT group, char *indent);
    void __create_floating_define_for_cell_area(si2drStringT string);
    int __lib__name_needs_to_be_quoted(char *name);
    char *__expr_string(si2drExprT *e);
    LONG_DOUBLE __timinglib_get_element(struct timinglib_value_data *vd, ...);
    void __timinglib_destroy_value_data(struct timinglib_value_data *vd);
    void __timinglib___get_index_info(si2drAttrIdT index_x,
                                      struct timinglib_value_data *vd,
                                      int dimno);
    si2drGroupIdT __get_containing_group(si2drGroupIdT group,
                                         si2drStringT type);
    struct timinglib_value_data *__timinglib_get_values_data(
        si2drGroupIdT table_group);
    int __num_get_precision(char *numstr);
    si2drStringT __get_first_group_name(si2drGroupIdT group);
    void __check_ccs_density(si2drGroupIdT group);
    int __isa_formula(char *str);
    void __add_varlist(char *name);
    void __destroy_varlist(void);
    void __print_var_list(void);
    void __enter_define_cell_area(si2drAttrIdT a);
    void __attribute_specific_checks(si2drAttrIdT a);
    char *__mystrtod(char *str, char **end);
    int __count_floats_in_string(char *s);
    int __count_strings_in_list(si2drAttrIdT attr);
    void __check_lu_table_template_arraysize(si2drGroupIdT group);
    void __check_power_lut_template_arraysize(si2drGroupIdT group);
    void __check_cell_n_prop(si2drGroupIdT group);
    void __check_clear_n_preset(si2drGroupIdT group);
    void __check_members(si2drGroupIdT group);
    void __check_interface_timing(si2drGroupIdT group);
    void __check_bus_pin_directions(
        si2drGroupIdT group) /* group is bus or bundle */;
    void __check_driver_types(
        si2drGroupIdT
            group) /* group should be a pin, perhaps even bus or bundle  */;
    int __get_bus_size(si2drGroupIdT group, char *busname);
    int __get_bun_size(si2drGroupIdT group, char *bun);
    void __gen_var_list(char *formula);
    si2drGroupIdT __find_bu_by_function(si2drGroupIdT cellequiv, char *name);
    si2drGroupIdT __find_pin_in_cellequiv(si2drGroupIdT cellequiv, char *name);
    void __check_next_state_formula(si2drGroupIdT cell, char *formula,
                                    int ffbankwidth, si2drAttrIdT attr);
    void __check_ff_bank_widths(si2drGroupIdT group) /* called for ff_banks */;
    void __check_bus_type(si2drGroupIdT bus);
    void __find_all_refs_to_template(si2drGroupIdT group, char *name, int num);
    void __check_index_x(si2drStringT indnam, si2drGroupIdT group);
    void __check_lut_template(si2drGroupIdT group);
    void __get_index_info(si2drAttrIdT index_x, struct timinglib_value_data *vd,
                          int dimno);
    struct timinglib_value_data *__get_vector_data(si2drGroupIdT vector);
    void __check_vector(si2drGroupIdT group);
    void __check_output_current_template(si2drGroupIdT group);
    void __check_retention_cell(si2drGroupIdT group);
    void __check_retention_pin(si2drGroupIdT group);
    void __check_level_shift(si2drGroupIdT group);
    void __check_ref_times(si2drGroupIdT group);
    void __group_specific_checks(si2drGroupIdT group);
    void __check_group_correspondence(si2drGroupIdT g, libsynt_group_info *gi);
    void __check_attr_correspondence(si2drAttrIdT attr,
                                     libsynt_attribute_info *ai);
    std::string __oid_string(si2drObjectIdT oid);
    std::string __iter_string(si2drIterIdT iter);
    const char *__itype_string(si2drIterIdT iter);

    si2drMessageHandlerT si2ErrMsg_ = nullptr;
    static si2drObjectIdT nulloid_;

    /* idea-- add a debug mode (all errors printed out as well as returned */
    /* idea-- add trace mode; generates c file that can be run */
    /* idea-- add nocheck mode; (all/most type checking turned off (for speed)
     */

    int l__iter_group_count_ = 0;
    int l__iter_name_count_ = 0;
    int l__iter_val_count_ = 0;
    int l__iter_attr_count_ = 0;
    int l__iter_def_count_ = 0;
    int timinglib___debug_mode_ = 0;
    int timinglib___nocheck_mode_ = 0;
    FILE *timinglib___trace_mode_CFP_ = 0;
    /* ==========================================================================================================================
     */
    /*     TRACE ROUTINES */

    /* TRACE globals */
    int trace_ = 0;
    int tracecount_ = 0;
    int tracefilecount_ = 0;
    int tracefunc_ = 0;
    int trace_grand_total_ = 0;
    FILE *tracefile1_ = nullptr;
    FILE *tracefile2_ = nullptr;

    struct reference_time_vals refvals_;
    var_list *master_var_list_ = 0;
    int errcount_ = 0;
    int operating_conds_checked_ = 0;

    scandata *sd_ = nullptr;
    LibStrtab *strtab_ = nullptr;
    LibBuilder *libbuilder_ = nullptr;
    std::string *parseLogStr_ = nullptr;
};
}  // namespace Timinglib

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIBANALYSIS_H_
