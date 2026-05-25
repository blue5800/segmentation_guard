# Segmentation Guard

A low-level C library that attempts to keep your program alive by catching segmentation faults, decoding the offending instruction, and skipping over it. It forces a return when the program calls invalid memory, such as a null function pointer.

## Features

- **Automatic Fault Recovery:** Catches `SIGSEGV` signals and prevents the process from terminating immediately.
- **Instruction Skipping:** Uses [Intel XED](https://github.com/intelxed/xed) to decode the instruction at the faulting address, calculate its length, and advance the Instruction Pointer (`RIP`) to the next instruction.
- **Diagnostics:** Tracks the number of caught faults and can print the offending instruction.

## Prerequisites

- Linux (x86_64)
- GCC
- Python 3 (for building Intel XED)
- Git

## Building

This project uses Git submodules for Intel XED and mbuild.

1.  **Clone the repository (recursively):**
    ```bash
    git clone --recursive <repo_url>
    cd <repo_name>
    ```
    *If you already cloned without submodules, run: `git submodule update --init --recursive`*

2.  **Build everything:**
    ```bash
    make
    ```
    This command will:
    - Build Intel XED (statically).
    - compile `libsegmentation_guard.a` (Static).
    - compile `libsegmentation_guard.so` (Shared).
    - Compile all example programs in `out/examples/`.

3.  **Clean:**
    ```bash
    make clean
    ```

## Usage

1.  **Include the header:**
    ```c
    #include "segmentation_guard.h"
    ```

2.  **Enable the guard:**
    ```c
    int main() {
        enable_segmentation_guard();
        
        // Risky code here...
        
        return 0;
    }
    ```

3.  **Link against the library:**
    
    *Static:*
    ```bash
    gcc my_program.c -o my_program -L./out -lsegmentation_guard -Ixed_kit/include out/xed_kit/lib/libxed.a
    ```
    
    *Dynamic:*
    ```bash
    gcc my_program.c -o my_program -L./out -lsegmentation_guard -Ixed_kit/include -Wl,-rpath='$ORIGIN/out'
    ```

## API Reference

- `void enable_segmentation_guard()`: Registers the signal handler.
- `void disable_segmentation_guard()`: Resets the signal handler to default (crash).
- `int number_of_segfaults()`: Returns the total number of recovered faults.
- `void reset_segfault_counter()`: Resets the counter to 0.
- `void print_last_offending_instruction()`: Prints the assembly of the last skipped instruction.

## Limitations & "The Uncatchables"

This library cannot save you from everything. See the `examples/` directory for demonstrations of where it fails

## Safety Mechanisms

To prevent infinite loops (where the program keeps crashing on the same or subsequent instructions indefinitely), the library calls `exit(0)` if more than 128 segfaults occur 
