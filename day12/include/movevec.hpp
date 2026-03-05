#pragma once
#include <cstdint>
#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include <utility>
template <typename T>
class MoveVector {
    public:
        MoveVector() : size_(0), cap_(0), data(nullptr) {};
        MoveVector(const MoveVector& other) : cap_(other.cap_), size_(other.size_) {
            data = new T[cap_];
            for (size_t i = 0; i < size_; ++i)
                data[i] = other.data[i];
        };
        MoveVector& operator=(const MoveVector& other) {
            if (this != &other) {
                T* new_data = new T[other.cap_];
                for (size_t i = 0; i < other.size_; ++i)
                    new_data[i] = other.data[i];
                delete[] data;
                data = new_data;
                cap_ = other.cap_;
                size_ = other.size_;
            }
            return *this;
        };

        MoveVector(MoveVector&& other) noexcept : data(other.data) , size_(other.size_) , cap_(other.cap_) {
            other.data = nullptr;
            other.cap_ = 0;
            other.size_ = 0;
        };

        MoveVector& operator=(MoveVector&& other) noexcept {
            if (this != &other) {
                delete[] data;

                data = other.data;
                cap_ = other.cap_;
                size_ = other.size_;

                other.data = nullptr;
                other.cap_ = 0;
                other.size_ = 0;
            }
            return *this;
        };

        T& operator[](size_t index) { return data[index]; }
        const T& operator[](size_t index) const { return data[index]; }

        ~MoveVector() {
            for (size_t i = 0; i < size_; ++i)
                data[i].~T();
            operator delete(data);
        }

        size_t size() const { return size_; }
        size_t capacity() const { return cap_; }

        void resize(size_t new_size) {
            if (new_size < size_) {
                for (size_t i = new_size; i < size_; ++i)
                    data[i].~T();
                size_ = new_size;
            } else if (new_size <= cap_) {
                for (size_t i = size_ ; i < new_size; ++i) 
                    ::new (static_cast<void*>(&data[i])) T();
                size_ = new_size;
            } else {
                size_t new_cap = std::max(new_size, cap_ ? cap_ * 2 : size_t(1));
                T* new_data = static_cast<T*>(operator new(new_cap * sizeof(T)));
                //move
                for (size_t i = 0; i < size_; ++i) {
                        ::new (&new_data[i]) T(std::move(data[i]));
                        data[i].~T();
                }
                //construct
                for (size_t i = size_; i < new_size; ++i)
                    ::new (&new_data[i]) T();
                size_ = new_size;
                operator delete(data);
                data = new_data;
                cap_ = new_cap;
            }
        };

        void reserve(size_t new_cap) {
            if (new_cap <= cap_) return;

            T* new_data = static_cast<T*>(operator new(new_cap * sizeof(T)));

            //move-construct exisiting elements
            for (size_t i = 0; i < size_; ++i) {
                ::new (&new_data[i]) T(std::move(data[i]));
                data[i].~T();
            }

            operator delete(data);
            data = new_data;
            cap_ = new_cap;
        };

        void push_back(const T& value) {
            if (size_ == cap_) reserve(cap_ == 0 ? 1 : cap_ * 2);
            ::new (&data[size_]) T(value);
            ++size_;
        }

        void push_back(T&& value) {
            if (size_ == cap_) reserve(cap_ == 0 ? 1 : cap_ * 2);
            ::new (&data[size_]) T(std::move(value));
            ++size_;
        }

        void clear() {
            for (size_t i = 0; i < size_; ++i)
                data[i].~T();
            size_ = 0;
        }

        template<typename... Args>
        void emplace_back(Args&&... args) {
            if (size_ == cap_)
                reserve(cap_ == 0 ? 1 : cap_ * 2);

            new (&data[size_]) T(std::forward<Args>(args)...);
            ++size_;
        }

    private:
        size_t size_;
        size_t cap_;
        T* data;
};