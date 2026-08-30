# Memory Hierarchy Simulator

A set-associative **L1 cache simulator** written in **C** that models how a CPU accesses physical memory. The simulator reads memory access traces, decomposes addresses into tag/set/block bits, and applies an **LRU replacement policy** to classify hits, cold misses, and conflict misses.

## Features

* **Set-Associative Cache Simulation:** Configurable number of sets, associativity, and block size via command-line arguments.
* **Address Decomposition:** Extracts tag, set index, and block offset from 32-bit addresses using bitwise masking and shifts.
* **LRU Eviction:** Maintains a per-set linked list to track least-recently-used lines and handle evictions on conflict misses.
* **Performance Reporting:** Outputs hit rate, miss rate, total misses, and eviction counts after processing a trace file.
* **Automated Verification:** Google Test suite validates cache initialization, bit extraction, and end-to-end behavior against benchmark traces.

## Technical Stack

* **Language:** C (simulator) / C++ (unit tests via Google Test)
* **Build:** Make, GCC/G++
* **Environment:** Linux (Docker devcontainer supported)
* **Concepts:** Cache associativity, LRU replacement, bitwise address manipulation, trace-driven simulation

## How It Works

```
Trace file → CPU reads addresses → Tag/Set/Offset extraction → Cache lookup → Hit or Miss → LRU update → Stats
```

1. **CPU** reads memory operations (load/store + address) from a trace file.
2. Each address is split into **tag**, **set index**, and **block offset** bits.
3. The simulator checks the corresponding cache set for a matching tag.
4. On a **hit**, the accessed line is moved to the front of the LRU list.
5. On a **miss**, an empty line is filled (**cold miss**) or the LRU line is evicted (**conflict miss**).
6. Final statistics (hits, misses, evictions, rates) are printed to stdout.

## Project Structure

```
include/          Header files (cache.h, cpu.h, lru.h, bits.h, trace.h)
src/              C source files (cache, cpu, lru, bits, cache_sim)
test/             Google Test suite and trace files (cat.trace, ls.trace, wc.trace)
obj/              Compiled object files
Makefile          Build configuration
```

## Getting Started

### Prerequisites

* GCC/G++ compiler
* Make
* Google Test (for running the test suite)

### Build

```bash
git clone https://github.com/elsamird003/project-Memory-Hierachy.git
cd project-Memory-Hierachy
make
```

This produces two binaries:

* `cache_app` — the cache simulator
* `cache_test` — the Google Test suite

### Run the Simulator

```bash
./cache_app <set_bits> <associativity> <block_bits> <tracefile>
```

Example:

```bash
./cache_app 2 4 8 test/wc.trace
```

Arguments are specified in **bits**, not raw counts (e.g., `2` set bits = 4 sets, `8` block bits = 256-byte blocks).

Example output:

```
hits: 787666 misses: 22104 evictions: 22088 hrate: 0.972703 mrate: 0.027297
```

### Run Tests

```bash
./cache_test
```

Run a single test:

```bash
./cache_test --gtest_filter=ProjectTests.test_wc_trace
```

### Clean Rebuild

```bash
make clean
make
```
