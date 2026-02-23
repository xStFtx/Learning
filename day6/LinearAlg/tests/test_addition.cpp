#include "lalib/matrix.hpp"
#include <cassert>

void test_addition() {
    lalib::Matrix<int> A(2,2);
    lalib::Matrix<int> B(2,2);
    A.set(0,0,1); B.set(0,0,2);
    A.set(0,1,3); B.set(0,1,4);
    A.set(1,0,5); B.set(1,0,6);
    A.set(1,1,7); B.set(1,1,8);

    lalib::Matrix<int> C = A + B;
    assert(C.get(0,0) == 3);
    assert(C.get(1,1) == 15);

    A += B;
    assert(A.get(0,1) == 7);
}