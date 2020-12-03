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
#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIBHASH_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIBHASH_H_

#include <string>

#include "db/timing/timinglib/timinglib_si2dr.h"

namespace Timinglib {

struct timinglib_hash_bucket {
    struct timinglib_hash_bucket *next;
    struct timinglib_hash_bucket *all_next;
    struct timinglib_hash_bucket *all_prev;
    char *name;
    si2drObjectIdT item;
};
typedef struct timinglib_hash_bucket timinglib_hash_bucket;

struct timinglib_hash_table {
    timinglib_hash_bucket **table; /* a ptr to an array of ptrs */
    int *counts;
    timinglib_hash_bucket *all_list;
    timinglib_hash_bucket *all_last;
    int auto_resize;
    int threshold;
    int size;
    int longest_bucket_string;
    int entry_count;
    int case_insensitive;
};
typedef struct timinglib_hash_table timinglib_hash_table;

class LibHash {
  public:
    static timinglib_hash_table *timinglibHashCreateHashTable(
        int numels, int auto_resize, int case_insensitive);

    static int timinglibHashGetNextHigherPrime(int prime);

    static void timinglibHashDestroyHashTable(timinglib_hash_table *ht);

    static void timinglibHashResizeHashTable(timinglib_hash_table *ht,
                                             int new_size);

    static unsigned int timinglibHashNameHash(char *name, int size);

    static unsigned int timinglibHashNameHashNocase(char *name, int size);

    static int timinglibHashEnterOid(timinglib_hash_table *ht, char *name,
                                     si2drObjectIdT oid);

    static void timinglibHashDeleteElem(timinglib_hash_table *ht, char *name);

    static void timinglibHashLookup(timinglib_hash_table *ht, char *name,
                                    si2drObjectIdT *oid);

    static void dumpAttrHash(timinglib_hash_table *ht, FILE *outc, FILE *outh);

    static void dumpGroupHash(timinglib_hash_table *ht, FILE *outc, FILE *outh);

    static std::string makeRep(const char *prefix, void *x);

  private:
    static si2drObjectIdT nulloid_;
};

}  // namespace Timinglib

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIBHASH_H_
