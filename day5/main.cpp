#include "main.h"
/* 
x = 1
y = 2
z = x + y
x = y 
y = z
Recursive fib
Goofy ah code.
*/


//O(2^n) :(
/* long fib(int n){
    if (n <= 1) 
        return n;
    return fib(n-1) + fib(n-2);
} 
 */

// O(n)


// O(n) time but O(1) space; non recursive
/* long fib(int n ) {
    if (n <= 1) 
        return n;
    long a =0, b =1;
    for (int i = 2; i <= n; i++) {
        long temp = a +b;
        a = b;
        b = temp;
    }

    return b;
}
 */

int main() {
    int n = 10;
    std::cout << fib(n) << std::endl;
    return 0;
}