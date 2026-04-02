#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include "cache.h"
#include "tlb.h"
#include "page_table.h"

int read_address(CPU *cpu, TraceLine *trace_line) {
  return fscanf(cpu->address_trace, "%c %x,%c\n", &trace_line->operation,
                &trace_line->address, &trace_line->size);
}

CPU *make_cpu(Cache *cache, TLB *tlb, PageTable *page_table,
              const char *address_trace_file) {
  CPU *cpu = (CPU *)(malloc(sizeof(CPU)));
  cpu->cache = cache;
  cpu->tlb = tlb;
  cpu->page_table = page_table;
  cpu->address_count = 0;
  cpu->hits = 0;
  cpu->cold = 0;
  cpu->conflict = 0;
  cpu->address_trace = fopen(address_trace_file, "r");
  return cpu;
}

void delete_cpu(CPU *cpu) {
  if (cpu->address_trace) {
    fclose(cpu->address_trace);
  }
  free(cpu);
}

address_type translate_address(CPU *cpu, address_type virtual_address) {
  unsigned int vpn = get_vpn(virtual_address, cpu->tlb->page_bits);
  unsigned int offset = get_page_offset(virtual_address, cpu->tlb->page_bits);

  int idx = tlb_search(cpu->tlb, vpn);
  if (idx >= 0) {
    /* TLB hit */
    cpu->tlb->hits++;
    cpu->tlb->access_counter++;
    cpu->tlb->entries[idx].last_used = cpu->tlb->access_counter;
    return (cpu->tlb->entries[idx].ppn << cpu->tlb->page_bits) | offset;
  }

  /* TLB miss: walk the page table */
  cpu->tlb->misses++;
  cpu->tlb->access_counter++;
  unsigned int ppn = page_table_lookup(cpu->page_table, vpn);
  tlb_insert(cpu->tlb, vpn, ppn);
  return (ppn << cpu->tlb->page_bits) | offset;
}

void run_cpu(CPU *cpu) {
  TraceLine trace_line;
  while (read_address(cpu, &trace_line) != EOF) {
    cpu->address_count++;

    /* Step 1: Translate virtual address to physical address */
    trace_line.address = translate_address(cpu, trace_line.address);

    /* Step 2: Access cache with the physical address */
    AccessResult result = cache_access(cpu->cache, &trace_line);
    if (result == HIT) {
      cpu->hits++;
    } else if (result == COLD_MISS) {
      cpu->cold++;
    } else {
      cpu->conflict++;
    }
  }

  int miss = cpu->cold + cpu->conflict;
  float hit_rate = ((float)(cpu->hits)) / ((float)(cpu->hits + miss));
  float miss_rate = 1.0f - hit_rate;
  int tlb_total = cpu->tlb->hits + cpu->tlb->misses;
  float tlb_hit_rate = (tlb_total > 0)
                           ? ((float)cpu->tlb->hits) / ((float)tlb_total)
                           : 0.0f;
  float tlb_miss_rate = 1.0f - tlb_hit_rate;

  printf("=== Cache Stats ===\n");
  printf("hits: %d misses: %d evictions: %d hrate: %f mrate: %f\n",
         cpu->hits, miss, cpu->conflict, hit_rate, miss_rate);
  printf("=== TLB Stats ===\n");
  printf("hits: %d misses: %d hrate: %f mrate: %f\n",
         cpu->tlb->hits, cpu->tlb->misses, tlb_hit_rate, tlb_miss_rate);
  printf("=== Page Table Stats ===\n");
  printf("page faults: %d\n", cpu->page_table->page_faults);
}
