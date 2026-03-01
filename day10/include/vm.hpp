#pragma once
#include <cstdint>
#include <cstddef>
#include "stack.hpp"
class VM {
    public:
        VM();
        VM(VM&& other) noexcept;
        VM& operator=(VM&& other) noexcept;
        //make the class movable-only
        VM(const VM&) = delete;
        VM& operator=(const VM&) = delete;
        ~VM();

        void loadProgram(const uint8_t* program, size_t len);
        void run();
        bool isHalted() const;
        void reset();

    private:
        Stack<int32_t> stack;
        uint8_t* bytecode_;
        size_t ip_;
        size_t len_;
        bool halted_;

};