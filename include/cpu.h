#ifndef __CPU_H
#define __CPU_H
#include <stdio.h>
#include "cache.h"
#include "trace.h"
#include "tlb.h"
#include "page_table.h"

typedef struct {
  Cache *cache;
  TLB *tlb;
  PageTable *page_table;
  FILE *address_trace;
  int address_count;
  int hits;
  int cold;
  int conflict;
} CPU;

CPU *make_cpu(Cache *cache, TLB *tlb, PageTable *page_table,
              const char *address_trace_file);
void delete_cpu(CPU *cpu);
void run_cpu(CPU *cpu);

#endif
