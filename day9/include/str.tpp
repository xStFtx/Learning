#include "str.hpp"
#include <stdexcept>
#include <algorithm>
#include <cstring>

namespace str {
    Str::Str() : cap_(0), size_(0), data(nullptr) {}

    Str::Str(const char* cstr){
        size_ = std::strlen(cstr);
        cap_ = size_;
        data = new char[cap_ +1];
        for (int i = 0 ; i < size_ ; ++i) 
            data[i] = cstr[i];
        data[size_] = '\0';
    }

    Str::Str(const Str& other) : cap_(other.cap_) , size_(other.size_) {
        data = new char[cap_ +1];
        for (int i = 0 ; i < size_; ++i) 
            data[i] = other.data[i];
        data[size_] = '\0';

    }

    Str& Str::operator=(const Str& other) {
        if (this != &other) {
            char* new_data = new char[other.cap_+1];
            for (int i = 0 ; i < other.size_; ++i) 
                new_data[i] = other.data[i];
            new_data[other.size_] = '\0';

            delete[] data;
            data = new_data;
            cap_ = other.cap_;
            size_ = other.size_;
        }

        return *this;
    }

    Str::Str(Str&& other) noexcept : data(other.data) , size_(other.size_) , cap_(other.cap_){
        other.data = nullptr;
        other.size_ = 0;
        other.cap_ = 0;
    }

    Str& Str::operator=(Str&& other) noexcept {
        if (this != &other) {
            delete[] data;
            
            data = other.data;
            size_ = other.size_;
            cap_ = other.cap_;

            other.data = nullptr;
            other.cap_ = 0;
            other.size_ = 0;
        }

        return *this;
    }

    Str::~Str() {
        delete[] data;
        data = nullptr;
    }

    char& Str::operator[](int index) {
        if (index >= size_ || index < 0)
            throw std::out_of_range("index out of range");
        return data[index];
    }

    Str Str::substr(int begin, int len) const {
        if (begin < 0 || begin + len > size_)
            throw std::out_of_range("invalid substring");

        Str result;
        result.size_ = len;
        result.cap_ = len;
        result.data = new char[len+1];

        for (int i = 0; i < len; ++i)
            result.data[i] = data[begin + i];
        result.data[len] = '\0';
        return result;
    }

    void Str::push_back(char c) {
        if (size_ == cap_)
            reserve(cap_ == 0 ? 1 : cap_*2);
        data[size_] = c;
        ++size_;
        data[size_] = '\0';
    }

    void Str::reserve(int new_cap) {
        if (new_cap <= cap_)
            return;
        char * new_data = new char[new_cap+1];
        for (int i = 0 ; i < size_; ++i) 
            new_data[i] = data[i];
        new_data[size_] = '\0';

        delete[] data;
        data = new_data;
        cap_ = new_cap;
    }

    int Str::length() const { return size_; }
    int Str::capacity() const { return cap_; }

}