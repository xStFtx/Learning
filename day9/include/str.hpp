#ifndef STR_HPP
#define STR_HPP
#include <ostream>
namespace str {

    class Str {
        public:
            Str();
            Str(const char* cstr); 
            Str(const Str& other);
            Str& operator=(const Str& other);
            Str(Str&& other) noexcept; //move constructor
            Str& operator=(Str&& other) noexcept; // move assignment
            ~Str(); 

            char& operator[] (int index);
            Str substr(int begin , int len) const;
            void push_back(char c);

            int length() const;
            int capacity() const;

            const char* c_str() const { return data; }
        private:
            int size_;
            int cap_;
            char* data;

            void reserve(int new_cap);
    };

    inline std::ostream& operator<<(std::ostream& os, const Str& s) {
        return os << s.c_str();
    }

} 

#include "str.tpp"

#endif