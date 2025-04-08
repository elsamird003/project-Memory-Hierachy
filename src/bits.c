#include "bits.h"
#include "cache.h"

int get_set(Cache *cache, address_type address) {  // getcount and set bits use // we are not gonna keep the tag.
  // TODO:
  //  Extract the set bits from a 32-bit address.
  // maybe setting the set to 
  // create a mask where set is 11
  // and then right shift
  // left shift can only shift when the list is full 
  // first left shift and then right shift

  return (address >> cache->block_bits) & ((1U << cache->set_bits) - 1);
}

int get_line(Cache *cache, address_type address) {  // getcount and set bits use // it 
    // line is just the first two/// but not always two or the number of tags bits
  // address is the original value
  // tag is the first couple of bits 
  // TODO:
  // we can use bit shift
  // Extract the tag bits from a 32-bit address.

  // when working with data think about number of bits, don't use count

  // need to shift it to the number of bits that are needed it.
  int shift_one =  address >> (cache->set_bits + cache->block_bits) ;
  // shift_one >> cache->set_count;
  //
  return shift_one;
}

int get_byte(Cache *cache, address_type address) {
  // TODO
  // Extract the block offset (byte index) bits from a 32-bit address.
  //

  return address&((1 << cache->block_bits)-1);

}
