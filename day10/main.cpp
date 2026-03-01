#include "include/vm.hpp"
#include "include/bytecode.hpp"
#include <cstdint>
#include <iostream>
#include <vector>

// Helper to encode int32_t into 4 bytes (little-endian)
void pushInt(std::vector<uint8_t>& program, int32_t val) {
    program.push_back(static_cast<uint8_t>(OpCode::OP_PUSH));
    program.push_back(static_cast<uint8_t>(val & 0xFF));
    program.push_back(static_cast<uint8_t>((val >> 8) & 0xFF));
    program.push_back(static_cast<uint8_t>((val >> 16) & 0xFF));
    program.push_back(static_cast<uint8_t>((val >> 24) & 0xFF));
}

int main() {
    VM vm;
    std::vector<uint8_t> program;

    // Program:
    // push 10
    // push 5
    // add
    // print (should be 15)
    // push 3
    // mul
    // print (should be 45)
    // push 9
    // sub
    // print (should be 36)
    // push 6
    // div
    // print (should be 6)
    // halt

    pushInt(program, 10);
    pushInt(program, 5);
    program.push_back(static_cast<uint8_t>(OpCode::OP_ADD));
    program.push_back(static_cast<uint8_t>(OpCode::OP_PRINT));

    pushInt(program, 3);
    program.push_back(static_cast<uint8_t>(OpCode::OP_MUL));
    program.push_back(static_cast<uint8_t>(OpCode::OP_PRINT));

    pushInt(program, 9);
    program.push_back(static_cast<uint8_t>(OpCode::OP_SUB));
    program.push_back(static_cast<uint8_t>(OpCode::OP_PRINT));

    pushInt(program, 6);
    program.push_back(static_cast<uint8_t>(OpCode::OP_DIV));
    program.push_back(static_cast<uint8_t>(OpCode::OP_PRINT));

    program.push_back(static_cast<uint8_t>(OpCode::OP_HALT));

    // Load program and run
    vm.loadProgram(program.data(), program.size());
    vm.run();

    // Reset and test empty stack error for PRINT
    vm.reset();
    try {
        std::vector<uint8_t> errorProgram = { static_cast<uint8_t>(OpCode::OP_PRINT), static_cast<uint8_t>(OpCode::OP_HALT) };
        vm.loadProgram(errorProgram.data(), errorProgram.size());
        vm.run();
    } catch (const std::runtime_error& e) {
        std::cout << "Caught error as expected: " << e.what() << std::endl;
    }

    // Test division by zero
    vm.reset();
    try {
        std::vector<uint8_t> divZeroProgram;
        pushInt(divZeroProgram, 10);
        pushInt(divZeroProgram, 0);
        divZeroProgram.push_back(static_cast<uint8_t>(OpCode::OP_DIV));
        divZeroProgram.push_back(static_cast<uint8_t>(OpCode::OP_HALT));
        vm.loadProgram(divZeroProgram.data(), divZeroProgram.size());
        vm.run();
    } catch (const std::runtime_error& e) {
        std::cout << "Caught division by zero error as expected: " << e.what() << std::endl;
    }

    return 0;
}