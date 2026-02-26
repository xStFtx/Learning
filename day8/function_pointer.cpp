#include <iostream>

int multiply(int a, int b) {
    return a * b;
}

int main() {
    int (*func)(int, int);

    // func points to multiply
    func = multiply;

    int prod = func(15, 2);
    std::cout << "The value of the product is: " << prod << std::endl;

    return 0;
}