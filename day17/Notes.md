# Low-Level Optimization

- Bit manipulation
- SIMD basics
- Compiler optimizations
- Assembly inspection

## Compiler optimizations
```
-O0  no optimization
-O1  basic optimization
-O2  good default
-O3  aggressive optimization
-Ofast ignores strict IEEE rules
```

`-flto` - link-time optimization

`ffast-match` - faster floating math

`-march=native` - use CPU feutures

## Branch Prediction
Branches slow CPUs

Bad:
```
if(x > 0)
    sum += x;
```

Better:
```
sum += (x > 0) * x;
```

### Tools for optimization

```
perf
gprof
valgrind
```

Example:

`perf stat ./program`

