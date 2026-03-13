#include <iostream>
#include <cstdint>

//Bit manipulation

bool is_pow2(uint32_t x){
    return x && !(x & (x - 1));
}

int main() {

    int n = 10;
    uint32_t x = 0b1010;
    uint32_t y = 0b1100;

    uint32_t a = x & y; // 1000
    uint32_t b = x | y; // 1110
    uint32_t c = x ^ y; // 0110
    //set bit
    x |= (1 << n);

    //clear bit
    x &= ~(1 << n);

    //toggle bit
    x ^= (1 << n);

    //get bit
    bool bit = (x >> n) & 1;
    return 0;
}