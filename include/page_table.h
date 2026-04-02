#ifndef __PAGE_TABLE_H
#define __PAGE_TABLE_H
#include "trace.h"

typedef struct {
  int valid;
  unsigned int ppn;
} PageTableEntry;

typedef struct {
  PageTableEntry *entries;
  int entry_count;
  unsigned int next_ppn;
  int page_faults;
} PageTable;

PageTable *make_page_table(int page_bits);
void delete_page_table(PageTable *pt);
unsigned int page_table_lookup(PageTable *pt, unsigned int vpn);

#endif
