#ifndef __TLB_H
#define __TLB_H
#include "trace.h"

typedef struct {
  int valid;
  unsigned int vpn;
  unsigned int ppn;
  unsigned int last_used;
} TLBEntry;

typedef struct {
  TLBEntry *entries;
  int entry_count;
  int page_bits;
  unsigned int access_counter;
  int hits;
  int misses;
} TLB;

TLB *make_tlb(int entry_count, int page_bits);
void delete_tlb(TLB *tlb);
unsigned int get_vpn(address_type address, int page_bits);
unsigned int get_page_offset(address_type address, int page_bits);
address_type tlb_lookup(TLB *tlb, address_type virtual_address, unsigned int ppn);
int tlb_search(TLB *tlb, unsigned int vpn);
void tlb_insert(TLB *tlb, unsigned int vpn, unsigned int ppn);

#endif
