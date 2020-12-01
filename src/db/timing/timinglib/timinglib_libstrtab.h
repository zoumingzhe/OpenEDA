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
#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIBSTRTAB_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIBSTRTAB_H_

#include "db/timing/timinglib/timinglib_libhash.h"
#include "db/timing/timinglib/timinglib_si2dr.h"

namespace Timinglib {

struct timinglib_strtable_chunk {
    char *strtab;
    struct timinglib_strtable_chunk *next;
    int strtab_allocated;
    int strtab_used;
};
typedef struct timinglib_strtable_chunk timinglib_strtable_chunk;

struct timinglib_strtable {
    Timinglib::timinglib_hash_table *hashtab; /* a ptr to an array of ptrs */
    timinglib_strtable_chunk *chunklist;
};
typedef struct timinglib_strtable timinglib_strtable;

class LibStrtab {
  public:
    timinglib_strtable *timinglibStrtableCreateStrtable(int numels, int strsize,
                                                        int case_insensitive);

    void timinglibStrtableDestroyStrtable(timinglib_strtable *ht);

    void timinglibStrtableResizeStrtable(timinglib_strtable *ht, int new_size);

    char *timinglibStrtableEnterString(timinglib_strtable *ht, char *str);

    /* there's no element delete here, because it wouldn't be very useful */

    void timinglibStrtableLookup(timinglib_strtable *ht, char *str,
                                 char **result);
    void printStrtabStats(void);

  private:
    int total_lookup_calls_ = 0;
    int total_enter_calls_ = 0;
    int total_strings_entered_ = 0;
    int total_bytes_entered_ = 0;
};

}  // end namespace Timinglib

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIBSTRTAB_H_
