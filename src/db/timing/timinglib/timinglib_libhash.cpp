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
#include "db/timing/timinglib/timinglib_libhash.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>

namespace Timinglib {

si2drObjectIdT LibHash::nulloid_ = {0, 0};

timinglib_hash_table *LibHash::timinglibHashCreateHashTable(
    int numels, int auto_resize, int case_insensitive) {
    timinglib_hash_table *ht;

    /* create a new hash table, with numels initial size */

    if (numels == 0) /* lets set an "auto" initial size of 53 els */
        numels = 53;

    ht = static_cast<timinglib_hash_table *>(
        calloc(sizeof(timinglib_hash_table), 1));
    ht->auto_resize = auto_resize;
    ht->size = numels;
    ht->table = static_cast<timinglib_hash_bucket **>(
        calloc(sizeof(timinglib_hash_bucket), numels));
    ht->counts = static_cast<int *>(calloc(sizeof(int), numels));
    if (ht->auto_resize) ht->threshold = 8;
    ht->case_insensitive = case_insensitive;
    return ht;
}

int LibHash::timinglibHashGetNextHigherPrime(int prime) {
    if (prime < 20) return 53;
    if (prime < 60) return 409;
    if (prime < 400) return 1019;
    if (prime < 1020) return 5009;
    if (prime < 5010) return 20011;
    return 99529;  // I hope we don't have to worry about tables any bigger than
                   //  this!
}

void LibHash::timinglibHashDestroyHashTable(timinglib_hash_table *ht) {
    if (ht == nullptr) return;
    /* traverse the all chain, and destroy all the buckets. */
    timinglib_hash_bucket *hbptr, *hb_next;

    for (hbptr = ht->all_list; hbptr; hbptr = hb_next) {
        hb_next = hbptr->all_next;
        hbptr->next = reinterpret_cast<timinglib_hash_bucket *>(NULL);
        hbptr->all_next = reinterpret_cast<timinglib_hash_bucket *>(NULL);
        hbptr->all_prev = reinterpret_cast<timinglib_hash_bucket *>(NULL);
        hbptr->name = reinterpret_cast<char *>(NULL);
        hbptr->item = nulloid_;
        free(hbptr);
    }

    /* free the table */
    free(ht->table);

    /* zero out fields for the sake of memory debug */

    ht->table = 0;
    ht->all_list = 0;
    ht->all_last = 0;
    ht->size = 0;
    ht->longest_bucket_string = 0;
    ht->entry_count = 0;
    if (ht->counts) {
        free(ht->counts);
        ht->counts = 0;
    }
    free(ht);
}

void LibHash::timinglibHashResizeHashTable(timinglib_hash_table *ht,
                                           int new_size) {
    timinglib_hash_bucket *hb;
    int hash_num;

    /* safety valve: if the new size is the same as the old, just return! */

    if (new_size == ht->size) return; /* that was easy! */

    /* free the old table, calloc a new one */

    free(ht->table);
    free(ht->counts);

    ht->table = static_cast<timinglib_hash_bucket **>(
        calloc(sizeof(timinglib_hash_bucket), new_size));
    ht->counts = static_cast<int *>(calloc(sizeof(int), new_size));
    ht->size = new_size;
    ht->longest_bucket_string = 0;

    /* the all list is still valid, reform the next links around the new table
     * size */
    for (hb = ht->all_list; hb; hb = hb->all_next) {
        if (ht->case_insensitive)
            hash_num = timinglibHashNameHashNocase(hb->name, ht->size);
        else
            hash_num = timinglibHashNameHash(hb->name, ht->size);

        hb->next = ht->table[hash_num];
        ht->table[hash_num] = hb;

        ht->counts[hash_num]++;

        if (ht->longest_bucket_string < ht->counts[hash_num])
            ht->longest_bucket_string = ht->counts[hash_num];
    }
}

unsigned int LibHash::timinglibHashNameHash(char *name, int size) {
    unsigned int h = 0;
    for (; *name; name++) h = 13 * h + static_cast<int>(*name);
    h = (h % size);
    return h;
}

unsigned int LibHash::timinglibHashNameHashNocase(char *name, int size) {
    unsigned int h = 0;
    for (; *name; name++) h = 13 * h + static_cast<int>(toupper(*name));
    h = (h % size);
    return h;
}

int LibHash::timinglibHashEnterOid(timinglib_hash_table *ht, char *name,
                                   si2drObjectIdT oid) {
    timinglib_hash_bucket *hb;
    int hash_num;

    if (name == NULL || *name == 0) {
        return -1;
    }
    if (ht->case_insensitive)
        hash_num = LibHash::timinglibHashNameHashNocase(name, ht->size);
    else
        hash_num = LibHash::timinglibHashNameHash(name, ht->size);

    for (hb = ht->table[hash_num]; hb; hb = hb->next) {
        if (ht->case_insensitive) {
            if (strcasecmp(hb->name, name) == 0) {
                return -2;
            }
        } else {
            if (strcmp(hb->name, name) == 0) {
                return -2;
            }
        }
    }

    hb = static_cast<timinglib_hash_bucket *>(
        calloc(sizeof(timinglib_hash_bucket), 1));
    hb->name = name;
    hb->item = oid;

    hb->next = ht->table[hash_num];
    ht->table[hash_num] = hb;

    if (ht->all_last == reinterpret_cast<timinglib_hash_bucket *>(NULL)) {
        ht->all_last = hb;
        ht->all_list = hb;
    } else {
        ht->all_list->all_prev = hb;
        hb->all_next = ht->all_list;
        ht->all_list = hb;
    }

    ht->entry_count++;
    ht->counts[hash_num]++;

    if (ht->longest_bucket_string < ht->counts[hash_num])
        ht->longest_bucket_string = ht->counts[hash_num];

    if (ht->auto_resize && ht->longest_bucket_string > ht->threshold) {
        int biggest_size =
            (ht->size > ht->entry_count ? ht->size : ht->entry_count);
        int next_size = timinglibHashGetNextHigherPrime(biggest_size);
        if (next_size > ht->size) {
            timinglibHashResizeHashTable(ht, next_size);
            if (ht->longest_bucket_string > ht->threshold) {
                ht->threshold = ht->longest_bucket_string +
                                3; /* we don't want to get too radical! */
            }
        }
    }

    return 0;
}

void LibHash::timinglibHashDeleteElem(timinglib_hash_table *ht, char *name) {
    timinglib_hash_bucket *hb, *hb_last;
    int hash_num;
    // int count;

    if (name == NULL || *name == 0) {
        return;
    }
    if (ht->case_insensitive)
        hash_num = timinglibHashNameHashNocase(name, ht->size);
    else
        hash_num = timinglibHashNameHash(name, ht->size);

    hb_last = 0;
    for (hb = ht->table[hash_num]; hb; hb = hb->next) {
        int res;

        if (ht->case_insensitive)
            res = strcasecmp(hb->name, name);
        else
            res = strcmp(hb->name, name);

        if (res == 0) {
            /* unlink it, by stuffing the "next" into the last's next */
            if (hb_last == reinterpret_cast<timinglib_hash_bucket *>(NULL)) {
                ht->table[hash_num] = hb->next;
            } else {
                hb_last->next = hb->next;
            }

            /* relink the all's */

            if (ht->all_last == hb && ht->all_list == hb) {
                ht->all_last = 0;
                ht->all_list = 0;
            } else if (ht->all_last == hb) {
                hb->all_prev->all_next = 0;
                ht->all_last = hb->all_prev;
            } else if (hb->all_prev ==
                       reinterpret_cast<timinglib_hash_bucket *>(NULL)) {
                /* first guy in the list */

                ht->all_list = hb->all_next;
                hb->all_next->all_prev =
                    reinterpret_cast<timinglib_hash_bucket *>(NULL);
            } else {
                hb->all_next->all_prev = hb->all_prev;
                hb->all_prev->all_next = hb->all_next;
            }

            /* zero out fields and free the bucket */
            hb->next = reinterpret_cast<timinglib_hash_bucket *>(0);
            hb->all_next = reinterpret_cast<timinglib_hash_bucket *>(0);
            hb->all_prev = reinterpret_cast<timinglib_hash_bucket *>(0);
            hb->name = reinterpret_cast<char *>(0);
            hb->item = nulloid_;
            free(hb);

            ht->counts[hash_num]--;
            ht->entry_count--;

            return;
        }
        hb_last = hb;
    }
    return;
}

void LibHash::timinglibHashLookup(timinglib_hash_table *ht, char *name,
                                  si2drObjectIdT *oidptr) {
    timinglib_hash_bucket *hb;
    int hash_num;

    if (name == NULL || *name == 0) {
        *oidptr = nulloid_;
        return;
    }
    if (ht->case_insensitive)
        hash_num = timinglibHashNameHashNocase(name, ht->size);
    else
        hash_num = timinglibHashNameHash(name, ht->size);

    for (hb = ht->table[hash_num]; hb; hb = hb->next) {
        if (ht->case_insensitive) {
            if (strcasecmp(hb->name, name) == 0) {
                *oidptr = hb->item;
                return;
            }
        } else {
            if (strcmp(hb->name, name) == 0) {
                *oidptr = hb->item;
                return;
            }
        }
    }
    *oidptr = nulloid_;
    return;
}

std::string LibHash::makeRep(const char *prefix, void *x) {
    char buf[50];
    if (x == NULL) {
        snprintf(buf, sizeof(buf), "%s", "0");
    } else {
        snprintf(buf, sizeof(buf), "&%s_%llx", prefix,
                 reinterpret_cast<uint64_t>(x));
    }
    return std::string(buf);
}

void LibHash::dumpAttrHash(timinglib_hash_table *ht, FILE *outc, FILE *outh) {
    /* go thru all the hash buckets and print out forward references to the h
     * file */
    timinglib_hash_bucket *b;
    int i;

    for (b = ht->all_list; b; b = b->all_next) {
        fprintf(outh, "extern timinglib_hash_bucket hbuck_%llx;\n",
                reinterpret_cast<uint64_t>(b));
        fprintf(outc,
                "timinglib_hash_bucket hbuck_%llx = { %s, %s, %s, "
                "(char*)(\"%s\"), {%s,(void*)0x%llx}  };\n",
                reinterpret_cast<uint64_t>(b),
                makeRep("hbuck", b->next).c_str(),
                makeRep("hbuck", b->all_next).c_str(),
                makeRep("hbuck", b->all_prev).c_str(), b->name,
                makeRep("attr", b->item.v1).c_str(),
                reinterpret_cast<uint64_t>(b->item.v2));
    }

    /* print the table */
    fprintf(outc, "timinglib_hash_bucket *ht_table_%llx[%d] = {",
            reinterpret_cast<uint64_t>(ht->table), ht->size);
    for (i = 0; i < ht->size; i++) {
        if (ht->table[i])
            fprintf(outc, "&hbuck_%llx,",
                    reinterpret_cast<uint64_t>(ht->table[i]));
        else
            fprintf(outc, "0,");
    }
    fprintf(outc, "};\n");

    /* print the counts */
    fprintf(outc, "int ht_counts_%llx[%d] = {",
            reinterpret_cast<uint64_t>(ht->counts), ht->size);
    for (i = 0; i < ht->size; i++) {
        fprintf(outc, "%d,", ht->counts[i]);
    }
    fprintf(outc, "};\n");

    /* print the struct */
    fprintf(
        outc,
        "timinglib_hash_table ht_%llx = {ht_table_%llx, ht_counts_%llx, %s, "
        "%s, %d, %d, %d, %d, %d, %d};\n",
        reinterpret_cast<uint64_t>(ht), reinterpret_cast<uint64_t>(ht->table),
        reinterpret_cast<uint64_t>(ht->counts),
        makeRep("hbuck", ht->all_list).c_str(),
        makeRep("hbuck", ht->all_last).c_str(), ht->auto_resize, ht->threshold,
        ht->size, ht->longest_bucket_string, ht->entry_count,
        ht->case_insensitive);
}

void LibHash::dumpGroupHash(timinglib_hash_table *ht, FILE *outc, FILE *outh) {
    /* go thru all the hash buckets and print out forward references to the h
     * file */
    timinglib_hash_bucket *b;
    int i;

    for (b = ht->all_list; b; b = b->all_next) {
        fprintf(outh, "extern timinglib_hash_bucket hbuck_%llx;\n",
                reinterpret_cast<uint64_t>(b));
        fprintf(outc,
                "timinglib_hash_bucket hbuck_%llx = { %s, %s, %s, "
                "(char*)(\"%s\"), {%s,(void*)0x%llx}  };\n",
                reinterpret_cast<uint64_t>(b),
                makeRep("hbuck", b->next).c_str(),
                makeRep("hbuck", b->all_next).c_str(),
                makeRep("hbuck", b->all_prev).c_str(), b->name,
                makeRep("group", b->item.v1).c_str(),
                reinterpret_cast<uint64_t>(b->item.v2));
    }

    /* print the table */
    fprintf(outc, "timinglib_hash_bucket *ht_table_%llx[%d] = {",
            reinterpret_cast<uint64_t>(ht->table), ht->size);
    for (i = 0; i < ht->size; i++) {
        if (ht->table[i])
            fprintf(outc, "&hbuck_%llx,",
                    reinterpret_cast<uint64_t>(ht->table[i]));
        else
            fprintf(outc, "0,");
    }
    fprintf(outc, "};\n");

    /* print the counts */
    fprintf(outc, "int ht_counts_%llx[%d] = {",
            reinterpret_cast<uint64_t>(ht->counts), ht->size);
    for (i = 0; i < ht->size; i++) {
        fprintf(outc, "%d,", ht->counts[i]);
    }
    fprintf(outc, "};\n");

    /* print the struct */
    fprintf(
        outc,
        "timinglib_hash_table ht_%llx = {ht_table_%llx, ht_counts_%llx, %s, "
        "%s, %d, %d, %d, %d, %d, %d};\n",
        reinterpret_cast<uint64_t>(ht), reinterpret_cast<uint64_t>(ht->table),
        reinterpret_cast<uint64_t>(ht->counts),
        makeRep("hbuck", ht->all_list).c_str(),
        makeRep("hbuck", ht->all_last).c_str(), ht->auto_resize, ht->threshold,
        ht->size, ht->longest_bucket_string, ht->entry_count,
        ht->case_insensitive);
}

}  // namespace Timinglib
