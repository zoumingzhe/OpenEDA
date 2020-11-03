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
    static timinglib_hash_table *timinglib_hash_create_hash_table(
        int numels, int auto_resize, int case_insensitive);

    static int timinglib_hash_get_next_higher_prime(int prime);

    static void timinglib_hash_destroy_hash_table(timinglib_hash_table *ht);

    static void timinglib_hash_resize_hash_table(timinglib_hash_table *ht,
                                                 int new_size);

    static unsigned int timinglib_hash_name_hash(char *name, int size);

    static unsigned int timinglib_hash_name_hash_nocase(char *name, int size);

    static int timinglib_hash_enter_oid(timinglib_hash_table *ht, char *name,
                                        si2drObjectIdT oid);

    static void timinglib_hash_delete_elem(timinglib_hash_table *ht,
                                           char *name);

    static void timinglib_hash_lookup(timinglib_hash_table *ht, char *name,
                                      si2drObjectIdT *oid);

    static void dump_attr_hash(timinglib_hash_table *ht, FILE *outc,
                               FILE *outh);

    static void dump_group_hash(timinglib_hash_table *ht, FILE *outc,
                                FILE *outh);

    static std::string make_rep(const char *prefix, void *x);

  private:
    static si2drObjectIdT nulloid_;
};

}  // namespace Timinglib

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIBHASH_H_
