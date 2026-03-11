#include <iostream>
#include "include/shared_ptr.hpp"

struct Test {
    Test() { std::cout << "Created\n"; }
    ~Test() { std::cout << "Destroyed\n"; }
};

int main() {

    SharedPtr<Test> a(new Test());

    {
        SharedPtr<Test> b = a;
        std::cout << a.use_count() << "\n"; // 2
    }

    std::cout << a.use_count() << "\n"; // 1

}