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

/* define a PI for manipulating the liberty data */

/* basic data types */

#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_SI2DR_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_SI2DR_H_

/* ***********************************************************************
 File contents : LIBERTY-DR type definitions.
*********************************************************************** */

#include <memory>
#include <stdexcept>
#include <string>

namespace Timinglib {
/*
 * These primitive types are compatible with the ANSI C Standard (ANSI/
 * X3.159-1990 Programming Language C Standard.)
 */
typedef enum si2BooleanEnum { SI2_FALSE = 0, SI2_TRUE = 1 } si2BooleanT;
typedef int64_t si2LongT;
typedef float si2FloatT;
typedef double si2DoubleT;
typedef char *si2StringT;
typedef void si2VoidT;

typedef enum si2TypeEnum {
    kSI2_UNDEFINED_VALUETYPE = 0,
    kSI2_BOOLEAN = 1,
    kSI2_LONG = 2,
    kSI2_FLOAT = 3,
    kSI2_DOUBLE = 4,
    kSI2_STRING = 5,
    kSI2_VOID = 6,
    kSI2_OID = 7,
    kSI2_ITER = 8,
    kSI2_EXPR = 9,
    kSI2_MAX_VALUETYPE = 10
} si2TypeT;

/*
 * Size limits
 */

#define kSI2_LONG_MAX 2147483647
#define kSI2_LONG_MIN -2147483647
#define SI2_ULONG_MAX 4294967295UL

typedef struct si2OID {
    void *v1, *v2;
} si2ObjectIdT;
/*
 * Iterators are blind handles the size of a single pointer.
 */
typedef void *si2IteratorIdT;

#define SI2DR_MIN_FLOAT32 -1E+37
#define SI2DR_MAX_FLOAT32 1E+37
#define SI2DR_MIN_FLOAT64 -1.797693e+308
#define SI2DR_MAX_FLOAT64 1.797693e+308
#define SI2DR_MIN_INT32 -2147483647
#define SI2DR_MAX_INT32 2147483647

#define SI2DR_TRUE SI2_TRUE
#define SI2DR_FALSE SI2_FALSE

typedef si2BooleanT si2drBooleanT;

typedef si2LongT si2drInt32T;
typedef si2FloatT si2drFloat32T;
typedef si2DoubleT si2drFloat64T;
typedef si2StringT si2drStringT;
typedef si2VoidT si2drVoidT;

typedef si2IteratorIdT si2drIterIdT;
typedef si2ObjectIdT si2drObjectIdT;

typedef si2drObjectIdT si2drGroupIdT;
typedef si2drObjectIdT si2drAttrIdT;
typedef si2drObjectIdT si2drAttrComplexValIdT;
typedef si2drObjectIdT si2drDefineIdT;

typedef si2drIterIdT si2drObjectsIdT;
typedef si2drIterIdT si2drGroupsIdT;
typedef si2drIterIdT si2drAttrsIdT;
typedef si2drIterIdT si2drDefinesIdT;
typedef si2drIterIdT si2drNamesIdT;
typedef si2drIterIdT si2drValuesIdT;

typedef enum si2drSeverityT {
    kSI2DR_SEVERITY_NOTE = 0,
    kSI2DR_SEVERITY_WARN = 1,
    kSI2DR_SEVERITY_ERR = 2
} si2drSeverityT;

typedef enum si2drObjectTypeT {
    kSI2DR_UNDEFINED_OBJECTTYPE = 0,
    kSI2DR_GROUP = 1,
    kSI2DR_ATTR = 2,
    kSI2DR_DEFINE = 3,
    kkSI2DR_COMPLEXVAL = 4,
    kSI2DR_MAX_OBJECTTYPE = 5
} si2drObjectTypeT;
inline const char *toString(si2drObjectTypeT ot) {
    switch (ot) {
        case kSI2DR_UNDEFINED_OBJECTTYPE:
            return "SI2DR_UNDEFINED_OBJECTTYPE";
        case kSI2DR_GROUP:
            return "SI2DR_GROUP";
        case kSI2DR_ATTR:
            return "SI2DR_ATTR";
        case kSI2DR_DEFINE:
            return "SI2DR_DEFINE";
        case kSI2DR_MAX_OBJECTTYPE:
            return "SI2DR_MAX_OBJECTTYPE";
        default:
            break;
    }
    return "UNKNOWN";
}

typedef enum si2drAttrTypeT { kSI2DR_SIMPLE, kSI2DR_COMPLEX } si2drAttrTypeT;
inline const char *toString(si2drAttrTypeT vt) {
    switch (vt) {
        case kSI2DR_SIMPLE:
            return "SI2DR_SIMPLE";
        case kSI2DR_COMPLEX:
            return "SI2DR_COMPLEX";
        default:
            break;
    }
    return "SI2DR_SIMPLE";
}

typedef enum si2drValueTypeT {
    kSI2DR_UNDEFINED_VALUETYPE = kSI2_UNDEFINED_VALUETYPE,
    kSI2DR_INT32 = kSI2_LONG,
    kSI2DR_STRING = kSI2_STRING,
    kSI2DR_FLOAT64 = kSI2_DOUBLE,
    kSI2DR_BOOLEAN = kSI2_BOOLEAN,
    kSI2DR_EXPR = kSI2_EXPR,
    kSI2DR_MAX_VALUETYPE = kSI2_MAX_VALUETYPE
} si2drValueTypeT;
inline const char *toString(si2drValueTypeT vt) {
    switch (vt) {
        case kSI2DR_UNDEFINED_VALUETYPE:
            return "SI2DR_UNDEFINED_VALUETYPE";
        case kSI2DR_INT32:
            return "SI2DR_INT32";
        case kSI2DR_STRING:
            return "SI2DR_STRING";
        case kSI2DR_FLOAT64:
            return "SI2DR_FLOAT32";
        case kSI2DR_BOOLEAN:
            return "SI2DR_BOOLEAN";
        case kSI2DR_MAX_VALUETYPE:
            return "SI2DR_MAX_VALUETYPE";
        default:
            break;
    }
    return "SI2DR_UNDEFINED_VALUETYPE";
}

typedef enum si2drExprTypeT {
    kSI2DR_EXPR_VAL =
        0, /* unary ops just have left arg set: add, sub, logs, paren */
    kSI2DR_EXPR_OP_ADD = 1,
    kSI2DR_EXPR_OP_SUB = 2,
    kSI2DR_EXPR_OP_MUL = 3,
    kSI2DR_EXPR_OP_DIV = 4,
    kSI2DR_EXPR_OP_PAREN = 5,
    kSI2DR_EXPR_OP_LOG2 = 6, /* pretty much flight of fancy from here on? */
    kSI2DR_EXPR_OP_LOG10 = 7,
    kSI2DR_EXPR_OP_EXP = 8, /* exponent? */
} si2drExprTypeT;

typedef struct si2drExprT {
    si2drExprTypeT type;
    union {
        si2drInt32T i;
        si2drFloat64T d;
        si2drStringT s; /* most likely an identifier */
        si2drBooleanT b;
    } u;

    si2drValueTypeT valuetype; /* if the type is a fixed value */

    struct si2drExprT *left;  // the exprs form a classic binary tree rep of an
                              // arithmetic expression
    struct si2drExprT *right;
} si2drExprT;

typedef enum si2drErrorT {
    kSI2DR_NO_ERROR = 0,
    kSI2DR_INTERNAL_SYSTEM_ERROR = 1,
    kSI2DR_INVALID_VALUE = 4,
    kSI2DR_INVALID_NAME = 5,
    kSI2DR_INVALID_OBJECTTYPE = 6,
    kSI2DR_INVALID_ATTRTYPE = 10,
    kSI2DR_UNUSABLE_OID = 7,
    kSI2DR_OBJECT_ALREADY_EXISTS = 8,
    kSI2DR_OBJECT_NOT_FOUND = 9,
    kSI2DR_SYNTAX_ERROR = 2,
    kSI2DR_TRACE_FILES_CANNOT_BE_OPENED = 3,
    kSI2DR_PIINIT_NOT_CALLED = 11,
    kSI2DR_SEMANTIC_ERROR = 12,
    kSI2DR_REFERENCE_ERROR = 13,
    kSI2DR_MAX_ERROR = 14
} si2drErrorT;

/* HELPER FUNCTIONS/STRUCTURES */

#ifdef NO_LONG_DOUBLE
#define LONG_DOUBLE double
#define strtold strtod
#else
#define LONG_DOUBLE long double
#endif

struct timinglib_value_data {
    int dimensions;
    int *dim_sizes;  // a malloc'd array of <dimensions> numbers,
                     // each number is the size of the array in that dim.*/
    LONG_DOUBLE *
        *index_info;  // a Ptr to a malloc'd array of size <dimensions>, ptrs to
                      // malloc'd arrays of long double index values.
                      // Each array of long double is of length set by
                      // the corresponding dim_sizes array values
    LONG_DOUBLE *values;  // a ptr to a malloc'd array of long doubles,
                          // starting with [0,0,...,0,0], and ending with
                          // [z-1,y-1,....b-1,a-1], where a-z are the max
                          // number of elements in each dimension.
};

template <typename... Args>
std::string stringFormat(const std::string &format, Args... args) {
    size_t size = snprintf(nullptr, 0, format.c_str(), args...) +
                  1;  // Extra space for '\0'
    if (size <= 0) {
        printf("Error during formatting.\n");
    }
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(),
                       buf.get() + size - 1);  // We don't want the '\0' inside
}

}  // namespace Timinglib
#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_SI2DR_H_
