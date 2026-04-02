#include "tlb.h"
#include <stdlib.h>

TLB *make_tlb(int entry_count, int page_bits) {
  TLB *tlb = (TLB *)malloc(sizeof(TLB));
  tlb->entry_count = entry_count;
  tlb->page_bits = page_bits;
  tlb->access_counter = 0;
  tlb->hits = 0;
  tlb->misses = 0;
  tlb->entries = (TLBEntry *)malloc(entry_count * sizeof(TLBEntry));
  for (int i = 0; i < entry_count; i++) {
    tlb->entries[i].valid = 0;
    tlb->entries[i].vpn = 0;
    tlb->entries[i].ppn = 0;
    tlb->entries[i].last_used = 0;
  }
  return tlb;
}

void delete_tlb(TLB *tlb) {
  free(tlb->entries);
  free(tlb);
}

unsigned int get_vpn(address_type address, int page_bits) {
  return address >> page_bits;
}

unsigned int get_page_offset(address_type address, int page_bits) {
  return address & ((1U << page_bits) - 1);
}

int tlb_search(TLB *tlb, unsigned int vpn) {
  for (int i = 0; i < tlb->entry_count; i++) {
    if (tlb->entries[i].valid && tlb->entries[i].vpn == vpn) {
      return i;
    }
  }
  return -1;
}

void tlb_insert(TLB *tlb, unsigned int vpn, unsigned int ppn) {
  int slot = -1;

  for (int i = 0; i < tlb->entry_count; i++) {
    if (!tlb->entries[i].valid) {
      slot = i;
      break;
    }
  }

  if (slot == -1) {
    /* Evict LRU entry: find the entry with the smallest last_used */
    unsigned int min_used = tlb->entries[0].last_used;
    slot = 0;
    for (int i = 1; i < tlb->entry_count; i++) {
      if (tlb->entries[i].last_used < min_used) {
        min_used = tlb->entries[i].last_used;
        slot = i;
      }
    }
  }

  tlb->entries[slot].valid = 1;
  tlb->entries[slot].vpn = vpn;
  tlb->entries[slot].ppn = ppn;
  tlb->entries[slot].last_used = tlb->access_counter;
}

address_type tlb_lookup(TLB *tlb, address_type virtual_address, unsigned int ppn) {
  unsigned int vpn = get_vpn(virtual_address, tlb->page_bits);
  unsigned int offset = get_page_offset(virtual_address, tlb->page_bits);

  tlb->access_counter++;

  int idx = tlb_search(tlb, vpn);
  if (idx >= 0) {
    tlb->hits++;
    tlb->entries[idx].last_used = tlb->access_counter;
    return (tlb->entries[idx].ppn << tlb->page_bits) | offset;
  }

  /* TLB miss: caller already resolved the PPN via page table */
  tlb->misses++;
  tlb_insert(tlb, vpn, ppn);
  return (ppn << tlb->page_bits) | offset;
}
