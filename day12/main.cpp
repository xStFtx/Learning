#include <iostream>
#include "include/movevec.hpp"

int main() {
    MoveVector<std::string> v;
    v.emplace_back("Alice");
    v.emplace_back(10, 'x');   // constructs string of 10 'x's
    v.push_back(std::string("Bob")); // move or copy depending on temporary
    return 0;
}