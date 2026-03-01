#pragma once
#include <cstddef>
#include <stdexcept>

template <typename T>
class Stack {
public:
    Stack(size_t initialCap = 8)
        : cap_(initialCap), sp_(0), data(new T[initialCap]) {}

    Stack(const Stack& other)
        : cap_(other.cap_), sp_(other.sp_), data(new T[other.cap_]) {
        for (size_t i = 0; i < sp_; ++i)
            data[i] = other.data[i];
    }

    Stack(Stack&& other) noexcept
        : data(other.data), cap_(other.cap_), sp_(other.sp_) {
        other.data = nullptr;
        other.cap_ = 0;
        other.sp_ = 0;
    }
    
    Stack& operator=(const Stack& other) {
        if (this != &other) {
            T* new_data = new T[other.cap_];
            for (size_t i = 0; i < other.sp_; ++i)
                new_data[i] = other.data[i];
            delete[] data;
            data = new_data;
            cap_ = other.cap_;
            sp_ = other.sp_;
        }
        return *this;
    }

    Stack& operator=(Stack&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            sp_ = other.sp_;
            cap_ = other.cap_;
            other.data = nullptr;
            other.sp_ = 0;
            other.cap_ = 0;
        }
        return *this;
    }

    ~Stack() { delete[] data; }

    void push(T val) {
        if (sp_ == cap_)
            reserve(cap_ == 0 ? 1 : cap_ * 2);
        data[sp_++] = val;
    }

    void pop() {
        if (sp_ == 0)
            throw std::runtime_error("Stack is empty");
        --sp_;
    }

    T top() const {
        if (sp_ == 0)
            throw std::runtime_error("Stack is empty");
        return data[sp_ - 1];
    }

    bool empty() const { return sp_ == 0; }

    size_t size() const { return sp_; }
    
    void reserve(size_t new_cap) {
        if (new_cap <= cap_)
            return;
        T* new_data = new T[new_cap];
        for (size_t i = 0; i < sp_; ++i)
            new_data[i] = data[i];
        delete[] data;
        data = new_data;
        cap_ = new_cap;
    }

private:
    T* data = nullptr;
    size_t cap_ = 0;
    size_t sp_ = 0;
};