#include <vector>
#include <iostream>
#include "include/span.hpp"
int main() {
    std::vector<int> v = {1,2,3,4};

    span<int> s(v.data(), v.size());

    for (auto& x : s)
        x *= 2;

    for (auto x : v)
        std::cout << x << " "; 
    std::cout << *s.data() << std::endl;
    std::cout << std::endl;
}