#include "bytecode.hpp"
#include <iostream>
#include "stack.hpp"
#include <stdexcept>

void executeByteCode(uint8_t* bytecode, size_t length, size_t& ip, Stack<int32_t>& stack, bool& halted) {
    uint8_t* end = bytecode + length;

    while (ip < length && !halted) {
        OpCode op = static_cast<OpCode>(bytecode[ip++]);

        switch (op) {
            case OpCode::OP_PUSH: {
                if (ip + sizeof(int32_t) > length)
                    throw std::runtime_error("Not enough bytes for PUSH argument");

                int32_t val = *reinterpret_cast<int32_t*>(&bytecode[ip]);
                stack.push(val);
                ip += sizeof(int32_t);
                break;
            }
            case OpCode::OP_ADD: {
                if (stack.size() < 2)
                    throw std::runtime_error("Not enough values on stack for ADD");

                int32_t b = stack.top(); stack.pop();
                int32_t a = stack.top(); stack.pop();
                stack.push(a + b);
                break;
            }
            case OpCode::OP_SUB: {
                if (stack.size() < 2)
                    throw std::runtime_error("Not enough values on stack for SUB");

                int32_t b = stack.top(); stack.pop();
                int32_t a = stack.top(); stack.pop();
                stack.push(a - b);
                break;
            }
            case OpCode::OP_MUL: {
                if (stack.size() < 2)
                    throw std::runtime_error("Not enough values on stack for MUL");

                int32_t b = stack.top(); stack.pop();
                int32_t a = stack.top(); stack.pop();
                stack.push(a * b);
                break;
            }
            case OpCode::OP_DIV: {
                if (stack.size() < 2)
                    throw std::runtime_error("Not enough values on stack for DIV");

                int32_t b = stack.top(); stack.pop();
                if (b == 0)
                    throw std::runtime_error("Division by zero");

                int32_t a = stack.top(); stack.pop();
                stack.push(a / b);
                break;
            }
            case OpCode::OP_PRINT: {
                if (stack.empty())
                    throw std::runtime_error("Stack is empty for PRINT");
                std::cout << stack.top() << std::endl;
                break;
            }
            case OpCode::OP_HALT:
                halted = true;
                return;
            default:
                throw std::runtime_error("Unknown opcode");
        }
    }
}