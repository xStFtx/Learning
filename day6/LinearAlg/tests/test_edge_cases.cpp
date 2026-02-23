#include "lalib/matrix.hpp"
#include <cassert>
#include <iostream>
#include <stdexcept>

void test_edge_cases() {
    lalib::Matrix<int> single(1,1);
    single.set(0,0,42);
    assert(single.get(0,0) == 42);
    lalib::Matrix<int> singleT = single.transpose();
    assert(singleT.get(0,0) == 42);
    lalib::Matrix<int> A(2,3);
    lalib::Matrix<int> B(3,2);
    bool caught = false;
    try {
        lalib::Matrix<int> C = A + B;
    } catch (std::runtime_error&) {
        caught = true;
    }
    assert(caught);

    caught = false;
    try {
        A += B;
    } catch (std::runtime_error&) {
        caught = true;
    }
    assert(caught);
}