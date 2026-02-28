# Memory Hierarchy Simulator

A comprehensive architectural simulator developed in **C++** to model and analyze the complete data flow within a computer's memory system. This project simulates the complex interactions between the **CPU, Translation Lookaside Buffer (TLB), Page Tables, and Physical Memory**.

## 🚀 Key Features
* **Full Address Translation:** Simulates the transition from Virtual Addresses to Physical Addresses using a multi-level hierarchy.
* **Integrated TLB & Cache:** Models a Translation Lookaside Buffer to demonstrate speed improvements in address translation.
* **Page Table Management:** Implements Page Table logic, including Page Fault handling and Disk/Memory swap simulations.
* **Configurable Parameters:** Supports customizable page sizes, TLB entries, and physical frame counts.
* **Detailed Performance Reporting:** Tracks and outputs critical system metrics:
    * TLB Hit/Miss Rates
    * Page Table Hit/Miss Rates
    * Average Memory Access Time (AMAT)

## 🛠 Technical Stack
* **Language:** C++ (Standard C++11/14)
* **Environment:** Linux / Unix
* **Core Concepts:** Virtual Memory, Page Replacement, Bit Manipulation, and Hardware Interfacing.

## 📐 System Architecture
The simulator follows a strict data-request path to mirror real hardware:
1. **CPU** generates a Virtual Address.
2. **TLB** is checked first for the Physical Frame.
3. If a TLB Miss occurs, the **Page Table** is consulted.
4. If a Page Fault occurs, the system simulates fetching data from **Disk** to **Physical Memory**.
5. The **Physical Address** is then constructed and data is "returned" to the CPU.

## 📋 Getting Started

### Prerequisites
* G++ (GNU C++ Compiler)
* Make

### Installation & Compilation
```bash
git clone [https://github.com/elsamird003/project-Memory-Hierachy.git](https://github.com/elsamird003/project-Memory-Hierachy.git)
cd project-Memory-Hierachy
make
