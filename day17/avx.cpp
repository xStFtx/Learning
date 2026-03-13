#include <immintrin.h>

void add(float* a, float* b, float* c, size_t n){
    size_t i = 0;

    for(; i + 8 <= n; i += 8){
        __m256 va = _mm256_loadu_ps(a + i);
        __m256 vb = _mm256_loadu_ps(b + i);

        __m256 vc = _mm256_add_ps(va, vb);

        _mm256_storeu_ps(c + i, vc);
    }
}