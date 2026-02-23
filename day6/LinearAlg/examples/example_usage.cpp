#include <iostream>
#include "lalib/matrix.hpp"

int main() {
    // Create a 3x3 matrix of ints
    lalib::Matrix<int> x(3, 3);
    lalib::Matrix<int> y(3,3);
    
    // Set values
    for (unsigned int r = 0; r < 3; ++r) {
        for (unsigned int c = 0; c < 3; ++c) {
            x.set(r, c, r * 3 + c);
            y.set(r, c, r * 3 + c);
        }
    }

    lalib::Matrix<int> z = x + y;
    lalib::Matrix<int> zt = z.transpose();

    std::cout << "A+B:" << std::endl;
    for (unsigned int r = 0; r < 3; ++r) {
        for (unsigned int c = 0; c < 3; ++c) {
            std::cout << z.get(r, c) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    
    std::cout << "Transpose of A+B:" << std::endl;
    for (unsigned int r = 0; r < 3; ++r) {
        for (unsigned int c = 0; c < 3; ++c) {
            std::cout << zt.get(r, c) << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}