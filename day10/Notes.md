# Day 10
## Building a Tiny Bytecode VM
1. [A simple instruction format](#instruction-format)

2. [A stack-based VM](#stack-based-vm)

3. Manual memory-managed heap

4. Function pointers or jump tables

5. Multi-file modular structure

6. Raw pointer dynamic array

## Instruction Format
Each instruction has to be stored in a byte buffer.

### I have to manually:
- Allocate Instruction Memory
- Parse Instructions
- Execute via Instruction Pointer (raw pointer)

## Stack Based VM
NOT Allowed to use `std::vector`

### Must:
- Reallocate manually
- Handle growth (amortize growth for O(1); where new_cap = cap_ *2)
- Avoid Leaks 
- Implement Destuctor
- Implement Copy Constructor + Copy Assignment
- Also break it and Observe UB and know how to fix the UB



### Update
This is insane. It is taking me hours.