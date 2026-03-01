//This is int by default in C++ so it has a size of 4 bytes instead of the 1 byte that I want.
//uint8_t* bytecode; I can do this to convert to 1 byte
//or I can do enum class OpCode : uint8_t. This guarantees each opcode to be 1 byte.
#pragma once
#include <cstdint>
#include <stdexcept>
#include "stack.hpp"
enum class OpCode : uint8_t {
    OP_PUSH,   // 0
    OP_ADD,    // 1
    OP_SUB,    // 2
    OP_MUL,    // 3
    OP_DIV,    // 4
    OP_PRINT,  // 5
    OP_HALT    // 6
};

void executeByteCode(uint8_t* bytecode, size_t len, size_t& ip, Stack<int32_t>& stack, bool& halted);