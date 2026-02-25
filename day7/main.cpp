#include <iostream>
#include "include/dyn_arr/dyn_arr.hpp"

int main() {
    
    dyn_arr::DynArr<int> d;
    d.reserve(10);
    d.resize(5);
    for (int i = 0 ; i < d.size() ; i++) {
        d[i] = i;
    }
    for (int i = 0 ; i < d.size(); i++ ) {
        std::cout << d[i] << std::endl;
    }

    for (int i = 5; i < 20; ++i) {
        d.push_back(i);
    }

    std::cout << "Size: " << d.size() << std::endl;
    std::cout << "Cap: " << d.capacity() << std::endl;
    return 0;
}