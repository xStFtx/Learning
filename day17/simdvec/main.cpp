#include <iostream>
#include "simd_vec.hpp"

int main() {
    alignas(32) float a[8] = {1,2,3,4,5,6,7,8};
    alignas(32) float b[8] = {8,7,6,5,4,3,2,1};
    float result[8];

    auto va = simd::vec8f::load(a);
    auto vb = simd::vec8f::load(b);

    auto vc = va + vb;

    vc.store(result);

    for (int i = 0; i < 8; i++)
        std::cout << result[i] << " ";
    std::cout << "\nDot product: " << va.dot(vb) << std::endl; 
    return 0;
}