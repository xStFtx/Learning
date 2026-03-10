#include <stdexcept>
#include <cstddef>
template<typename T>
class span{
    public:
        span(T* ptr, size_t size) : data_(ptr) , size_(size) {}

        T& operator[](size_t i){
            if (i >= size_) 
                throw std::out_of_range("span index out of range");
            return data_[i];
        }
        const T& operator[](size_t i) const{
            if (i >= size_) 
                throw std::out_of_range("span index out of range");
            return data_[i];
        }

        size_t size() const {return size_;}

        T* begin() {return data_ ;}
        T* end() {return data_ + size_; }
        
        const T* begin() const {return data_ ;}
        const T* end() const {return data_ + size_; }

        T* data() { return data_ ;}
        const T* data() const{ return data_ ;}

        bool empty() const {return size_ == 0; }


    private:  
        T* data_;
        size_t size_;
};