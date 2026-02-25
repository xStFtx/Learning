#include "dyn_arr.hpp"
#include <stdexcept>
#include <algorithm>

namespace dyn_arr {

//Constructor
template <typename T>
DynArr<T>::DynArr() : cap_(0), size_(0) {
    data = new T[cap_];
}

//Destructor
template <typename T>
DynArr<T>::~DynArr() {
    delete[] data;
    data = nullptr;
}

//Copy Constructor
template <typename T>
DynArr<T>::DynArr(const DynArr<T>& other) : cap_(other.cap_), size_(other.size_) {
    data = new T[cap_];
    for (size_t i = 0; i < size_; ++i)
        data[i] = other.data[i];
}

// Copy Assignment
template <typename T>
DynArr<T>& DynArr<T>::operator=(const DynArr<T>& other) {
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
}

//operator overloading
template <typename T>
T& DynArr<T>::operator[](size_t index) {
    if (index >= size_) {
        throw std::out_of_range("index out of range");
    }
    return data[index];
}

//resize
template <typename T>
void DynArr<T>::resize(size_t new_size) {
    if (new_size < size_) {
        size_ = new_size;
    } else if (new_size <= cap_) {
        for (size_t i = size_; i < new_size; ++i)
            data[i] = T();
        size_ = new_size;
    } else {
        size_t new_cap = std::max(new_size, cap_ * 2);
        reserve(new_cap);
        for (size_t i = size_; i < new_size; ++i)
            data[i] = T();
        size_ = new_size;
    }
}

//reserve
template <typename T>
void DynArr<T>::reserve(size_t new_cap) {
    if (new_cap <= cap_) {
        return;
    } else {
        T* new_data = new T[new_cap];
        for (size_t i = 0; i < size_; ++i)
            new_data[i] = data[i];
        delete[] data;
        data = new_data;
        cap_ = new_cap;
    }
}

//push_back
template <typename T>
void DynArr<T>::push_back(const T& value) {
    if (size_ == cap_) {
        reserve(cap_ == 0 ? 1 : cap_ * 2);
    }
    data[size_] = value;
    size_++;
}

//size getter
template <typename T>
size_t DynArr<T>::size() const {
    return size_;
}

//capacity getter
template <typename T>
size_t DynArr<T>::capacity() const {
    return cap_;
}

} // namespace dyn_arr