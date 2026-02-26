#include <iostream>

//Array Decay is the loss of an array's type and dimensions

void aDecay(int * p) {
    std::cout << "Modified size of array is by passing by value: ";
    std::cout << sizeof(p) << std::endl;
}

int main() {

    int a[4] = {2,4,6,8};
    std::cout << "size of the array: " << sizeof(a) << std::endl;

    aDecay(a);

    return 0;
}