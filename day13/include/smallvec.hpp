#pragma once
#include <cstdint>
#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include <utility>
template <typename T, size_t N>
class smallvec {
    public:
        smallvec() :  size_(0), cap_(N), data(reinterpret_cast<T*>(stack_buffer_)) {};
        
        ~smallvec() {
            for (size_t i =0 ; i < size_; ++i) 
                data[i].~T();
            if (!using_stack())
                ::operator delete(data);
        };


        void push_back(const T& value) {
            if (size_ >= cap_) 
                grow();
            new (&data[size_]) T(value);
            size_++;
        }

        void push_back(T&& value) {
            if (size_ >= cap_)
                grow();
            new (&data[size_]) T(std::move(value));
            size_++;
        }

        void grow() {
            size_t new_cap = cap_ ? cap_ * 2 : 1;

            T* new_data = static_cast<T*>(
                operator new(sizeof(T) * new_cap)
            );

            for (size_t i = 0; i < size_; ++i) {
                new (&new_data[i]) T(std::move(data[i]));
                data[i].~T();
            }

            if (!using_stack())
                operator delete(data);
            
            data = new_data;
            cap_ = new_cap;
        }

        T& at(size_t i) {
            if (i >= size_)
                throw std::out_of_range("smallvec index");
            return data[i];
        }

        size_t size() const { return size_; }
        size_t capacity() const { return cap_; }

        T& operator[](size_t i) { return data[i]; }
        const T& operator[](size_t i) const {return data[i]; }


    private:
        size_t cap_;
        size_t size_;
        T* data;

        alignas(T) unsigned char stack_buffer_[sizeof(T) * N];

        bool using_stack() const {
            return data == reinterpret_cast<const T*>(stack_buffer_);
        }
};