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

  if(p->next == NULL){
      if (p->line->tag == tag && p->line->valid){
            result->access = HIT;
            result->line = p->line;

   }
      else if (p->line->tag!= tag && p->line->valid == 1){  // if the tag is not equal to the or the other tag 
        p->line->valid = 1;

         result->access = CONFLICT_MISS;
         p->line->tag = tag;
         result->line = p->line;
      
      }
      else {   
        result->access = COLD_MISS;
        p->line->valid = 1;  /// any time we have a miss we need to do this 
        p->line->tag = tag;      // IF I HAVE A MISS I NEED TO CHANGE THE TAG THE VALID BIT  bit to 1
        result->line = p->line;
      }
    
       return; 
    
  }

  // int counter = 0;
  while(p){
     
    // check for if the p is in a valid line and tag is mached. If that happen then that is hit
    // I want to more that node in to update the head. i MAY WANT TO CREATE THAT  METHOD
      if(p->line->valid && p->line->tag == tag ){
       
        result->access = HIT;
        
        if(set->lru_queue != p){
          prev->next = p->next;  
          p->next = set->lru_queue; 
          set->lru_queue = p;  // move it to the front
          
        
       
      }

      result->line = p->line;
      break;
      }

      //  prev_prev = prev; 
  
           
     if(p->line->valid == 0 ){ 
          // check if the current line == 0  
          p->line->tag = tag;
          p->line->valid = 1;
          result->access = COLD_MISS; 
          if (p != set->lru_queue){    
          prev->next = p->next;  
          p->next = set->lru_queue; // points to the front of the linked list
          set->lru_queue = p;  // move it to the front 
         
          }
          result->line = p->line;
          return;
    } 
    


    if(p->next ==NULL){
      p->line->tag = tag;
      p->line->valid = 1; 
      result->access = CONFLICT_MISS;
      if (p != set->lru_queue){ 
      prev->next = p->next;  
      p->next = set->lru_queue;  // point to the front of the list 
      set->lru_queue = p;
      //  printf(p);

    }
    break;


    result->line =  p->line;
  }

    prev = p;
    p = p->next;
  

  }




  // TODO:
  // Implement the LRU algorithm to determine which line in
  // the cache should be accessed.
  //
}
