# I am learning a lot from objdump.

When I run `objdump -d main | grep fib`

I see `jne` , `jmp` , `jne` 

`jne` - jump if not equal 

`jmp` - unconditional jump

But because I ran `g++ -O2 main.cpp -o main`

There is no `call` instruction.

`gcc` optimized my tail recursion into a loop.

Tail recursion:
```cpp
long fib(int n , long a = 0, long  b= 1) {
    if (n == 0)
        return a;
    return fib(n-1,b,a+b);
}
```

# There is also the nm command I can use

`nm main | grep fib` :

```
0000000000001280 T _Z3fibill
```

This shows the function fib(int , long , long) still exists in the binary.
But the optimization removed the `call` instructions inside the function.
So it doesn't call itself anymore , it jumps internally instead.


# Linker vs Compiler
Compiler compiles the source code into object code. And the linker takes the object code and other libs to output a binary file. Quick high level description.

## g++
1. Preprocessing(`.cpp` -> `.i`)
    - Handles `#include` , `#define`, macros
    - Produces a single translation unit
2. Compilation(`.i` -> `.s`)
    - Converts C++ code to assembly
3. Assembly(`.s` -> `.o`)
    - Converts assembly into object files (machine code with symbols)
4. Linking(`.o` -> executable)
    - Combines multiple object files
    - Resolves references to functions/variables
    - Produces final binary (`a.out` or `main.exe`)


So when you call g++:
1. Preprocessing-> Compilation -> Assembly -> Linking
2. Automatically calls `cc1plus` (the actual c++ compiler)
3. Calls `as` (assembler)
4. Calls `ld` (linker) 