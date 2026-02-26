#include <iostream>
#include <bits/stdc++.h>

int main() {
    int n = 10;

    //Pointer to n
    int* ptr1;
    //Pointer to ptr1; double pointer
    int** ptr2;

    ptr1 = &n;
    ptr2 = &ptr1;

    
    return 0;
}