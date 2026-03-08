#include <iostream>
#include "include/smallvec.hpp"

int main()
{
    smallvec<int,8> vec;

    for(int i=0;i<1337;i++)
        vec.push_back(i);

    for(size_t i=0;i<vec.size();i++)
        std::cout << vec[i] << "\n";
}