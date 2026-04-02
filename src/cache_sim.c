#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "cache.h"
#include "cpu.h"
#include "tlb.h"
#include "page_table.h"

int main(int argc, char *argv[]) {
  if (argc != 7) {
    printf(
        "usage: cache-sim <set bits> <associativity> <block bits> "
        "<page bits> <tlb entries> <tracefile>\n");
    exit(1);
  }
  int sets = atoi(argv[1]);
  int lines = atoi(argv[2]);
  int bytes = atoi(argv[3]);
  int page_bits = atoi(argv[4]);
  int tlb_entries = atoi(argv[5]);
  char *file = argv[6];

  Cache *cache = make_cache(sets, lines, bytes);
  TLB *tlb = make_tlb(tlb_entries, page_bits);
  PageTable *page_table = make_page_table(page_bits);
  CPU *cpu = make_cpu(cache, tlb, page_table, file);

  run_cpu(cpu);

  delete_cpu(cpu);
  delete_tlb(tlb);
  delete_page_table(page_table);
  delete_cache(cache);
}
