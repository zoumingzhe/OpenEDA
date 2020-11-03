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

/* declarations for structures to store the contents of liberty files */

#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_STRUCTS_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_STRUCTS_H_

#include "db/timing/timinglib/timinglib_libhash.h"

namespace Timinglib {
typedef struct timinglib_name_list {
    char *name;
    int prefix_len;
    struct timinglib_name_list *next;
} timinglib_name_list;

typedef struct timinglib_head {
    char *name;
    int lineno;
    char *filename;
    struct timinglib_attribute_value *list;
} timinglib_head;

struct timinglib_group {
    timinglib_name_list *names;
    timinglib_name_list *last_name;

    char *type;

    int EVAL;

    char *comment;

    struct timinglib_attribute *attr_list;
    struct timinglib_attribute *attr_last;
    timinglib_hash_table *attr_hash;

    struct timinglib_define *define_list;
    struct timinglib_define *define_last;
    timinglib_hash_table *define_hash;

    struct timinglib_group *group_list;
    struct timinglib_group *group_last;
    timinglib_hash_table *group_hash;

    struct timinglib_group *next;
    int lineno;
    char *filename;
    struct timinglib_group *owner;
};

typedef struct timinglib_group timinglib_group;

typedef enum timinglib_attribute_type {
    kTIMINGLIB__SIMPLE,
    kTIMINGLIB__COMPLEX
} timinglib_attribute_type;

typedef enum timinglib_attribute_value_type {
    kTIMINGLIB__VAL_STRING,
    kTIMINGLIB__VAL_DOUBLE,
    kTIMINGLIB__VAL_BOOLEAN,
    kTIMINGLIB__VAL_INT,
    kTIMINGLIB__VAL_EXPR,
    kTIMINGLIB__VAL_UNDEFINED
} timinglib_attribute_value_type;

struct timinglib_attribute_value {
    timinglib_attribute_value_type type;
    union {
        char *string_val;
        int int_val;
        double double_val;
        int bool_val;
        void *expr_val;
    } u;
    struct timinglib_attribute_value *next; /* for lists */
};

typedef struct timinglib_attribute_value timinglib_attribute_value;

struct timinglib_attribute {
    timinglib_attribute_type type;
    char *name;
    char *comment;
    timinglib_attribute_value *value;
    timinglib_attribute_value *last_value;

    struct timinglib_attribute *next;
    int EVAL;
    int is_var;
    int lineno;
    char *filename;
    timinglib_group *owner;
};

typedef struct timinglib_attribute timinglib_attribute;

struct timinglib_define {
    char *name;
    char *group_type;
    timinglib_attribute_value_type valtype;
    char *comment;

    struct timinglib_define *next;
    int lineno;
    char *filename;
    timinglib_group *owner;
};
typedef struct timinglib_define timinglib_define;

typedef enum libsynt_attr_type {
    kSYNTAX_ATTRTYPE_STRING,
    kSYNTAX_ATTRTYPE_VIRTUAL,
    kSYNTAX_ATTRTYPE_ENUM,
    kSYNTAX_ATTRTYPE_FLOAT,
    kSYNTAX_ATTRTYPE_INT,
    kSYNTAX_ATTRTYPE_BOOLEAN,
    kSYNTAX_ATTRTYPE_COMPLEX,
    kSYNTAX_ATTRTYPE_COMPLEX_UNKNOWN,
    kSYNTAX_ATTRTYPE_COMPLEX_LIST
} libsynt_attr_type;

inline const char *toString(libsynt_attr_type t) {
    switch (t) {
        case kSYNTAX_ATTRTYPE_STRING:
            return "SYNTAX_ATTRTYPE_STRING";
        case kSYNTAX_ATTRTYPE_VIRTUAL:
            return "SYNTAX_ATTRTYPE_VIRTUAL";
        case kSYNTAX_ATTRTYPE_ENUM:
            return "SYNTAX_ATTRTYPE_ENUM";
        case kSYNTAX_ATTRTYPE_FLOAT:
            return "SYNTAX_ATTRTYPE_FLOAT";
        case kSYNTAX_ATTRTYPE_INT:
            return "SYNTAX_ATTRTYPE_INT";
        case kSYNTAX_ATTRTYPE_BOOLEAN:
            return "SYNTAX_ATTRTYPE_BOOLEAN";
        case kSYNTAX_ATTRTYPE_COMPLEX:
            return "SYNTAX_ATTRTYPE_COMPLEX";
        case kSYNTAX_ATTRTYPE_COMPLEX_UNKNOWN:
            return "SYNTAX_ATTRTYPE_COMPLEX_UNKNOWN";
        case kSYNTAX_ATTRTYPE_COMPLEX_LIST:
            return "SYNTAX_ATTRTYPE_COMPLEX_LIST";
    }
    return "SYNTAX_ATTRTYPE_COMPLEX_UNKNOWN";
}

typedef struct libsynt_float_constraint {
    double greater_than;
    double less_than;
    double default_value;
    char *refd_default;

    int greater_specd, less_specd, default_specd, ref_default_specd;
} libsynt_float_constraint;

typedef struct libsynt_int_constraint {
    int greater_than;
    int less_than;
    int default_value;
    char *refd_default;
    int greater_specd, less_specd, default_specd, ref_default_specd;
} libsynt_int_constraint;

typedef struct libsynt_string_enum {
    char **array; /* sorted for binary search */
    int size;
} libsynt_string_enum;

typedef struct libsynt_argument {
    libsynt_attr_type type;
    struct libsynt_argument *next;
} libsynt_argument;

typedef struct libsynt_attribute_info {
    char *name;
    libsynt_attr_type type;
    int lineno;

    union {
        libsynt_float_constraint *floatcon;
        libsynt_int_constraint *intcon;
        libsynt_string_enum *stringenum;
        libsynt_argument *args;
    } u;

    struct libsynt_attribute_info *next;
} libsynt_attribute_info;

typedef enum libsynt_name_constraints {
    kSYNTAX_GNAME_NONE,
    kSYNTAX_GNAME_ONE,
    kSYNTAX_GNAME_TWO,
    kSYNTAX_GNAME_THREE,
    kSYNTAX_GNAME_NONE_OR_ONE,
    kSYNTAX_GNAME_ONE_OR_MORE
} libsynt_name_constraints;

inline const char *toString(libsynt_name_constraints x) {
    switch (x) {
        case kSYNTAX_GNAME_NONE:
            return "SYNTAX_GNAME_NONE";

        case kSYNTAX_GNAME_ONE:
            return "SYNTAX_GNAME_ONE";

        case kSYNTAX_GNAME_TWO:
            return "SYNTAX_GNAME_TWO";

        case kSYNTAX_GNAME_THREE:
            return "SYNTAX_GNAME_THREE";

        case kSYNTAX_GNAME_NONE_OR_ONE:
            return "SYNTAX_GNAME_NONE_OR_ONE";

        case kSYNTAX_GNAME_ONE_OR_MORE:
            return "SYNTAX_GNAME_ONE_OR_MORE";
    }
    return "";
}

typedef struct libsynt_head { /* used in the parser */
    libsynt_name_constraints namecons;
    libsynt_argument *arglist;
    char *ident;
    int lineno;
} libsynt_head;

typedef struct libsynt_group_info {
    int mark;
    libsynt_name_constraints name_constraints;

    char *type;
    int ID;
    int lineno;

    libsynt_attribute_info *attr_list;
    struct libsynt_group_info *group_list;

    timinglib_hash_table *attr_hash;
    timinglib_hash_table *group_hash;

    struct libsynt_group_info *next;
    struct libsynt_group_info
        *ref; /* if this group is really defined elsewheres */
} libsynt_group_info;

typedef struct libsynt_technology {
    char *name;
    libsynt_group_info *lib;
    timinglib_hash_table *group_hash;
} libsynt_technology;

}  // namespace Timinglib

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_STRUCTS_H_
