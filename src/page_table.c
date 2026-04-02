#include "page_table.h"
#include <stdlib.h>

PageTable *make_page_table(int page_bits) {
  int vpn_count = 1 << (32 - page_bits);

  PageTable *pt = (PageTable *)malloc(sizeof(PageTable));
  pt->entry_count = vpn_count;
  pt->next_ppn = 0;
  pt->page_faults = 0;
  pt->entries = (PageTableEntry *)calloc(vpn_count, sizeof(PageTableEntry));
  return pt;
}

void delete_page_table(PageTable *pt) {
  free(pt->entries);
  free(pt);
}

unsigned int page_table_lookup(PageTable *pt, unsigned int vpn) {
  if (pt->entries[vpn].valid) {
    return pt->entries[vpn].ppn;
  }

  /* Page fault: allocate a new physical frame */
  pt->page_faults++;
  pt->entries[vpn].valid = 1;
  pt->entries[vpn].ppn = pt->next_ppn;
  pt->next_ppn++;
  return pt->entries[vpn].ppn;
}
