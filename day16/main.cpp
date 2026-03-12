#include <iostream>
#include "include/thread_pool.hpp"
int main() {
    thread_pool pool(4);

    for (int i = 0; i < 20; i++)
    {
        pool.submit([i]{
            std::cout << "task " << i << "\n";
        });
    }
    return 0;
};