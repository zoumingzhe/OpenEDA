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

/* why doesn't libc have a nice hash table mechanism? */

#include "db/timing/timinglib/timinglib_libstrtab.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

namespace Timinglib {

void LibStrtab::printStrtabStats(void) {
    printf(
        "String Table Lookup called %d times\nString Table Enter called %d "
        "times\nNo. Strings entered into String Table: %d\n, Total Bytes "
        "entered: %d\n\n",
        total_lookup_calls_, total_enter_calls_, total_strings_entered_,
        total_bytes_entered_);
}

timinglib_strtable *LibStrtab::timinglibStrtableCreateStrtable(
    int numels, int strsize, int case_insensitive) {
    timinglib_strtable *ht;
    timinglib_strtable_chunk *htc;

    ht = static_cast<timinglib_strtable *>(
        calloc(sizeof(timinglib_strtable), 1));
    ht->hashtab = LibHash::timinglibHashCreateHashTable(
        numels, 1 /* make it resizeable */, case_insensitive);
    htc = static_cast<timinglib_strtable_chunk *>(
        calloc(sizeof(timinglib_strtable_chunk), 1));
    ht->chunklist = htc;
    htc->strtab = static_cast<char *>(calloc(1, strsize));
    htc->strtab_allocated = strsize;
    htc->strtab_used = 0;
    return ht;
}

void LibStrtab::timinglibStrtableDestroyStrtable(timinglib_strtable *ht) {
    if (ht == nullptr) return;

    timinglib_strtable_chunk *htc, *htcn;

    /* traverse the all chain, and destroy all the buckets. */
    if (ht->hashtab) LibHash::timinglibHashDestroyHashTable(ht->hashtab);
    ht->hashtab = 0;
    htc = ht->chunklist;
    while (htc) {
        htcn = htc->next;
        if (htc->strtab) free(htc->strtab);
        htc->strtab_allocated = 0;
        htc->strtab_used = 0;
        htc->strtab = 0;
        htc->next = 0;
        free(htc);
        htc = htcn;
    }
    ht->chunklist = 0;
    total_lookup_calls_ = 0;
    total_enter_calls_ = 0;
    total_strings_entered_ = 0;
    total_bytes_entered_ = 0;
    free(ht);
}

void LibStrtab::timinglibStrtableResizeStrtable(timinglib_strtable *ht,
                                                int new_size) {
    LibHash::timinglibHashResizeHashTable(ht->hashtab, new_size);

    /* there isn't anything to do about the string space */
}

char *LibStrtab::timinglibStrtableEnterString(timinglib_strtable *ht,
                                              char *str) {
    si2drObjectIdT t, ores;
    char *strptr;
    int slen = strlen(str);

    total_enter_calls_++;

    /* first, check the table for the string */

    LibHash::timinglibHashLookup(ht->hashtab, str, &ores);

    if (ores.v1 != 0) return static_cast<char *>(ores.v1);

    /* first, add the string to the table */

    if (ht->chunklist->strtab_used + slen + 1 >
        ht->chunklist->strtab_allocated) {
        /* this string won't fit in the current chunk. realloc isn't an option,
           all the string pointers would
           be off. So create a new chunk, and put it at the top of the list, and
           put the new string in there */
        timinglib_strtable_chunk *htc = static_cast<timinglib_strtable_chunk *>(
            calloc(sizeof(timinglib_strtable_chunk), 1));

        htc->strtab =
            static_cast<char *>(calloc(ht->chunklist->strtab_allocated, 1));
        htc->strtab_allocated = ht->chunklist->strtab_allocated;
        htc->strtab_used = 0;

        /* link in the new chunk to the head of the chunklist */
        htc->next = ht->chunklist;
        ht->chunklist = htc;
    }
    snprintf(ht->chunklist->strtab + ht->chunklist->strtab_used, slen + 1, "%s",
             str);
    strptr = ht->chunklist->strtab + ht->chunklist->strtab_used;
    total_strings_entered_++;
    total_bytes_entered_ += slen + 1;
    ht->chunklist->strtab_used += slen + 1;

    t.v1 = strptr;
    t.v2 = 0;

    LibHash::timinglibHashEnterOid(ht->hashtab, strptr, t);
    // ToDo: make a generic hashtab, then somehow use it as a
    // base for oid hashes, and then use it as a base for the
    // string hash

    return strptr;
}

void LibStrtab::timinglibStrtableLookup(timinglib_strtable *ht, char *str,
                                        char **result) {
    si2drObjectIdT ores;

    total_lookup_calls_++;

    if (str == NULL || *str == 0) {
        *result = reinterpret_cast<char *>(0);
        return;
    }

    /* first, check the table for the string */

    LibHash::timinglibHashLookup(ht->hashtab, str, &ores);

    if (ores.v1 != 0) {
        *result = static_cast<char *>(ores.v1);
        return;
    }

    *result = 0;
    return;
}

}  // namespace Timinglib
