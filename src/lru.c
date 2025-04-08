#include "lru.h"
#include <stdio.h>
#include <stdlib.h>
#include "cache.h"

void lru_init_queue(Set *set) {
  LRUNode *s = NULL;
  LRUNode **pp = &s;  // place to chain in the next node
  for (int i = 0; i < set->line_count; i++) {
    Line *line = &set->lines[i];
    LRUNode *node = (LRUNode *)(malloc(sizeof(LRUNode)));
    node->line = line;
    node->next = NULL;
    (*pp) = node;
    pp = &((*pp)->next);
  }
  set->lru_queue = s;
}

void lru_init(Cache *cache) {
  Set *sets = cache->sets;
  for (int i = 0; i < cache->set_count; i++) {
    lru_init_queue(&sets[i]);
  }
}

void lru_destroy(Cache *cache) {
  Set *sets = cache->sets;
  for (int i = 0; i < cache->set_count; i++) {
    LRUNode *p = sets[i].lru_queue;
    LRUNode *n = p;
    while (p != NULL) {
      p = p->next;
      free(n);
      n = p;
    }
    sets[i].lru_queue = NULL;
  }
}

void lru_fetch(Set *set, unsigned int tag, LRUResult *result) {

  LRUNode *p = set->lru_queue;   // head 
  LRUNode *prev = NULL;
  // LRUNode *prev_prev = prev;

  int counter = 0;
  while(p){
    counter++;    
    // check for if the p is in a valid line and tag is mached. If that happen then that is hit
    // I want to more that node in to update the head. i MAY WANT TO CREATE THAT  METHOD
      if((p->line->valid) && (p->line->tag == tag )){
        result->line = p->line;
        result->access = HIT;
        if(set->lru_queue != p){
          prev->next = p->next;  
          p->next = set->lru_queue; 
          set->lru_queue = p;  // move it to the front
        } break;
      }
      //  prev_prev = prev; 
       prev = p;
       p = p->next;

    
  }
     if(result->access != HIT){
        set->line_count;

        if(set->line_count <= counter){  // full
          result->access = CONFLICT_MISS;  // kick out the last guy // p is null or dead // prev
            result->line = prev->line;
            LRUNode *d=( LRUNode *) malloc(sizeof(LRUNode));
            d->line = (Line *) malloc(sizeof(Line)); 
            d->line->valid = 0;
            d->line->tag = tag;

            // d->line->block_size 
            prev = NULL; 
              // delete last node 
            d->next = set->lru_queue;
            set->lru_queue = d; // front of the list

            

        }
        else if(set->line_count > counter){
          result->access = COLD_MISS;     // put in the front 
          // result->access = CONFLICT_MISS;  // kick out the last guy // p is null or dead // prev
            result->line = prev->line; // idk
            LRUNode *d=( LRUNode *) malloc(sizeof(LRUNode));
            d->line = (Line *) malloc(sizeof(Line));
            d->line->valid = 0;
            d->line->tag = tag;
            d->next = set->lru_queue;
            set->lru_queue = d; // front of the list
        
     }  

    



  // TODO:
  // Implement the LRU algorithm to determine which line in
  // the cache should be accessed.
  //
}
}