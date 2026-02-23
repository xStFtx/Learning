#include "lalib/matrix.hpp"
#include <cassert>

void test_transpose() {
    lalib::Matrix<int> A(2,3);
    int val=1;
    for(unsigned int r=0;r<2;++r)
        for(unsigned int c=0;c<3;++c)
            A.set(r,c,val++);

    lalib::Matrix<int> T = A.transpose();
    assert(T.get(0,0) == 1);
    assert(T.get(2,1) == 6);
}