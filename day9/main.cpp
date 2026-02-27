#include <iostream>
#include "include/str.hpp"

int main() {
    str::Str s = "hello";
    std::cout << s << std::endl;

    s.push_back('!');
    std::cout << s << std::endl;

    str::Str t = s.substr(0, 5);
    std::cout << t << std::endl;

    str::Str u = std::move(s);
    std::cout << u << std::endl;
}