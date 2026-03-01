#include "vm.hpp"
#include <utility>
#include "bytecode.hpp"
#include <stdexcept>
#include <cstddef>
#include <algorithm>

VM::VM() 
    : stack(), ip_(0), len_(0), halted_(false), bytecode_(nullptr) {}

VM::VM(VM&& other) noexcept 
    : stack(std::move(other.stack)),
      ip_(other.ip_),
      len_(other.len_),
      halted_(other.halted_),
      bytecode_(other.bytecode_) 
{
    other.ip_ = 0;
    other.len_ = 0;
    other.halted_ = false;
    other.bytecode_ = nullptr;
}

VM& VM::operator=(VM&& other) noexcept {
    if (this != &other) {
        delete[] bytecode_;
        stack = std::move(other.stack);
        bytecode_ = other.bytecode_;
        ip_ = other.ip_;
        len_ = other.len_;
        halted_ = other.halted_;

        other.bytecode_ = nullptr;
        other.ip_ = 0;
        other.len_ = 0;
        other.halted_ = false;
    }
    return *this;
}

VM::~VM() {
    delete[] bytecode_;
    bytecode_ = nullptr;
}

void VM::loadProgram(const uint8_t* program, size_t len) {
    delete[] bytecode_;
    bytecode_ = new uint8_t[len];
    std::copy(program, program + len, bytecode_);
    len_ = len;
    ip_ = 0;
    halted_ = false;
}

void VM::run() {
    if (!bytecode_ || len_ == 0)
        throw std::runtime_error("Nothing Loaded");

    halted_ = false;
    while (!halted_ && ip_ < len_) {
        executeByteCode(bytecode_, len_, ip_, stack , halted_);
    }
    halted_ = true; // implicit halt if no OP_HALT
}

bool VM::isHalted() const {
    return halted_;
}

void VM::reset() {
    ip_ = 0;
    halted_ = false;
}