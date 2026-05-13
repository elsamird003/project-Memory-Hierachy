# Memory Hierarchy Simulator

A comprehensive architectural simulator developed in **C** to model and analyze the complete data flow within a computer's memory system. This project simulates the complex interactions between the **CPU, Page Tables, and Physical Memory**.

## Features
* **Full Address Translation:** Simulates the transition from Virtual Addresses to Physical Addresses using a multi-level hierarchy.
* **Integrated TLB & Cache:** Models a Translation Lookaside Buffer to demonstrate speed improvements in address translation.
* **Configurable Parameters:** Supports customizable cache dimensions, page sizes, and TLB entries via command-line arguments.
* **Detailed Performance Reporting:** Tracks and outputs critical system metrics including Hit/Miss Rates, Evictions, and Conflict Misses.

## 🛠 Technical Stack
* **Language:** C (Core Simulation) / C++ (Testing)
* **Environment:** VSCode- Windows
* **Core Concepts:** Memory Management, Cache Associativity, Least Recently Used (LRU) Eviction, Bitwise Manipulation, and Virtual-to-Physical Translation.

## System Architecture
 This project is split into two distinct architectural phases across branches:

* **`main` branch (Data Cache Layer):** Focuses strictly on simulating the physical L1 cache. It reads direct memory addresses from a trace file, extracts the Tag, Set, and Block bits using bitwise masking, and manages a set-associative cache using an LRU linked-list policy.
* **`tlb` branch (Translation Layer):** Expands the simulator into a full Memory Management Unit (MMU). It implements the Virtual-to-Physical address translation methodology.

**The Full Simulation Data Path:**
1. **CPU** generates a Virtual Address.
2. **TLB** is checked first for the Physical Frame Number (Fast Path).
3. On a TLB Miss, the **Page Table** is consulted (Slow Path).
4. If a Page Fault occurs, the system simulates fetching data from **Disk** to **Physical Memory**.
5. The **Physical Address** is constructed and passed to the Cache Access logic.

## Getting Started

### Prerequisites
* GCC/G++ Compiler
* Make

### Installation & Compilation
```bash
git clone [https://github.com/elsamird003/project-Memory-Hierachy.git](https://github.com/elsamird003/project-Memory-Hierachy.git)
cd project-Memory-Hierachy
make
