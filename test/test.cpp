#include <gtest/gtest.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "bits.h"
#include "cache.h"
#include "cpu.h"
#include "tlb.h"
#include "page_table.h"

// Include these definitions to test against solution:
int soln_get_set(Cache *cache, address_type address);
int soln_get_line(Cache *cache, address_type address);
int soln_get_byte(Cache *cache, address_type address);

class ProjectTests : public ::testing::Test {
 protected:
  ProjectTests() {}           // constructor runs before each test
  virtual ~ProjectTests() {}  // destructor cleans up after tests
  virtual void SetUp() {}     // sets up before each test (after constructor)
  virtual void TearDown() {}  // clean up after each test, (before destructor)
};

TEST(ProjectTests, test_cache_make) {
  int sets = 4;
  int lines = 4;
  int bytes = 10;
  Cache *cache = make_cache(sets, lines, bytes);

  ASSERT_NE(cache, (Cache *)NULL) << "cache is NULL";
  
}

TEST(ProjectTests, test_cache_initialization_set_bits) {
  int sets = 4;
  int lines = 4;
  int bytes = 10;
  Cache *cache = make_cache(sets, lines, bytes);

  ASSERT_NE(cache, (Cache *)NULL) << "cache is NULL";

  // Test the basic initialization of the cache
  ASSERT_EQ(sets, cache->set_bits)
      << "cache->set_bits improperly initialized: expected " << sets;
}

TEST(ProjectTests, test_cache_initialization_block_bits) {
  int sets = 4;
  int lines = 4;
  int bytes = 10;
  Cache *cache = make_cache(sets, lines, bytes);

  ASSERT_NE(cache, (Cache *)NULL) << "cache is NULL";

  // Test the basic initialization of the cache
  ASSERT_EQ(bytes, cache->block_bits)
      << "cache->block_bits improperly initialized: expected " << bytes;
}

TEST(ProjectTests, test_cache_initialization_line_count) {
  int sets = 4;
  int lines = 4;
  int bytes = 10;
  Cache *cache = make_cache(sets, lines, bytes);

  ASSERT_NE(cache, (Cache *)NULL) << "cache is NULL";

  // Test the basic initialization of the cache
  ASSERT_EQ(lines, cache->line_count)
      << "cache->line_count improperly initialized: expected " << lines;
}

TEST(ProjectTests, test_cache_set_and_block_counts) {
  int sets = 4;
  int lines = 4;
  int bytes = 10;
  Cache *cache = make_cache(sets, lines, bytes);

  ASSERT_NE(cache, (Cache *)NULL) << "cache is NULL";

  // Test the basic initialization of the cache
  ASSERT_EQ(sets, cache->set_bits)
      << "cache->set_bits improperly initialized: expected " << sets;

  ASSERT_EQ(bytes, cache->block_bits)
      << "cache->block_bits improperly initialized: expected " << bytes;

  ASSERT_EQ(lines, cache->line_count)
      << "cache->line_count improperly initialized: expected " << lines;

  // Test the proper initialization of the set and block count.
  int set_count = exp2(sets);
  int block_size = exp2(bytes);

  ASSERT_EQ(set_count, cache->set_count)
      << "cache->set_count improperly initialized: expected " << set_count;

  ASSERT_EQ(block_size, cache->block_size)
      << "cache->block_size improperly initialized: expected " << block_size;
}

TEST(ProjectTests, test_cache_set_lists) {
  int sets = 4;
  int lines = 4;
  int bytes = 10;
  Cache *cache = make_cache(sets, lines, bytes);

  ASSERT_NE(cache, (Cache *)NULL) << "cache is NULL";

  // Test the basic initialization of the cache
  ASSERT_EQ(sets, cache->set_bits)
      << "cache->set_bits improperly initialized: expected " << sets;

  ASSERT_EQ(bytes, cache->block_bits)
      << "cache->block_bits improperly initialized: expected " << bytes;

  ASSERT_EQ(lines, cache->line_count)
      << "cache->line_count improperly initialized: expected " << lines;

  // Test the proper initialization of the set and block count.
  int set_count = exp2(sets);
  int block_size = exp2(bytes);

  ASSERT_EQ(set_count, cache->set_count)
      << "cache->set_count improperly initialized: expected " << set_count;

  ASSERT_EQ(block_size, cache->block_size)
      << "cache->block_size improperly initialized: expected " << block_size;

  // Test cache set lists
  Set *set_list = cache->sets;
  ASSERT_NE(set_list, (Set *)NULL) << "cache->sets should not be NULL";
}

TEST(ProjectTests, test_cache_set_lists_line_count) {
  int sets = 4;
  int lines = 4;
  int bytes = 10;
  Cache *cache = make_cache(sets, lines, bytes);

  ASSERT_NE(cache, (Cache *)NULL) << "cache is NULL";

  // Test the basic initialization of the cache
  ASSERT_EQ(sets, cache->set_bits)
      << "cache->set_bits improperly initialized: expected " << sets;

  ASSERT_EQ(bytes, cache->block_bits)
      << "cache->block_bits improperly initialized: expected " << bytes;

  ASSERT_EQ(lines, cache->line_count)
      << "cache->line_count improperly initialized: expected " << lines;

  // Test the proper initialization of the set and block count.
  int set_count = exp2(sets);
  int block_size = exp2(bytes);

  ASSERT_EQ(set_count, cache->set_count)
      << "cache->set_count improperly initialized: expected " << set_count;

  ASSERT_EQ(block_size, cache->block_size)
      << "cache->block_size improperly initialized: expected " << block_size;

  // Test cache set lists
  Set *set_list = cache->sets;
  ASSERT_NE(set_list, (Set *)NULL) << "cache->sets should not be NULL";

  // Test line counts.
  int testval = 0;
  for (int i = 0; i < cache->set_count; i++) {
    testval |= (set_list[i].line_count != lines);
  }

  ASSERT_TRUE(!testval)
      << "line_count in Set is properly initialized; expected " << lines;
}


TEST(ProjectTests, test_wc_trace) {
  int sets = 2;
  int lines = 4;
  int bytes = 8;
  Cache *cache = make_cache(sets, lines, bytes);
  ASSERT_NE(cache, (Cache *)NULL) << "cache is NULL";
  TLB *tlb = make_tlb(16, 12);
  PageTable *page_table = make_page_table(12);
  CPU *cpu = make_cpu(cache, tlb, page_table, "test/wc.trace");
  
  ASSERT_NE(cpu, (CPU *)NULL) << "cpu is NULL";
  
  run_cpu(cpu);
    printf("%d", cpu->cold ,"HERE!!!!");

  int diff = abs(cpu->cold + cpu->conflict - 22353);

  ASSERT_TRUE(cpu->cold + cpu->conflict > 22303 &&
              cpu->cold + cpu->conflict < 22403)
      << "the number of misses (cpu->cold+cpu->conflict) was not "
         "within the range of the expected result of 22353"
      << ". You were off by " << diff << "." << cpu->cold <<"." << cpu->conflict << "." << cpu->hits ;

  diff = abs(cpu->hits - 787417);

  ASSERT_TRUE(cpu->hits > 787367 && cpu->hits < 787467)
      << "the number of hits (cpu->hits) was not "
         "within the range of the expected result of 787417"
      << ". You were off by " << diff << ".";

  diff = abs(cpu->conflict - 22337);

  ASSERT_TRUE(cpu->conflict > 22287 && cpu->conflict < 22387)
      << "the number of conflicts (cpu->conflict) was not "
         "within the range of the expected result of 22337"
      << ". You were off by " << diff << ".";
}
