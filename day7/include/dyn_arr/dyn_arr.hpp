#ifndef DYN_ARR_HPP
#define DYN_ARR_HPP

namespace dyn_arr {

template <typename T>
class DynArr {
    public:
        DynArr();
        ~DynArr();
        DynArr(const DynArr<T>& other);
        DynArr& operator=(const DynArr<T>& other);
        //void set(size_t index, const T& value);
        T& operator[] (size_t index);
        void push_back(const T& value);
        void resize(size_t new_size);
        void reserve(size_t new_cap);
        size_t size() const;
        size_t capacity() const;

    private:
        size_t size_; //Current number of elements
        size_t cap_; // Current allocated space
        T* data; 
};

}
#include "dyn_arr.tpp"

#endif