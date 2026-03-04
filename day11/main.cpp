#include <iostream>
#include "include/linarena.hpp"
int main() {
    LinArena arena(1024);

    int* a = arena.make<int>(42);
    std::string* s = arena.make<std::string>("Hello");

    arena.reset(); // safely destroys s, a is trivial

    return 0;
}