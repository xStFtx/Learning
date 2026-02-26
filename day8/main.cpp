#include <iostream>

int main() {

    int n = 10;
    int* ptr =  &n;

    std::cout << ptr << std::endl;
    //move 4 bytes ; sizeof(int)
    ptr++;
    std::cout << ptr << std::endl;
    ptr--;
    std::cout << ptr << std::endl;      

    int arr[3] = {1,2,3};

    int * ptr1 = &arr[0];
    int * ptr2 = &arr[1];

    std::cout << ptr2 - ptr1 << std::endl;

    return 0;
}