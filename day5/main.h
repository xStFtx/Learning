#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <vector>

long fib(int n , long a = 0, long  b= 1) {
    if (n == 0)
        return a;
    return fib(n-1,b,a+b);
}

#endif