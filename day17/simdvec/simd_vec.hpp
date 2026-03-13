#pragma once
#include <immintrin.h>
#include <cstddef>

namespace simd {

class vec8f {
private:
    __m256 data;

public:

    vec8f() : data(_mm256_setzero_ps()) {}

    vec8f(float v) {
        data = _mm256_set1_ps(v);
    }

    vec8f(__m256 v) : data(v) {}

    static vec8f load(const float* ptr) {
        return vec8f(_mm256_loadu_ps(ptr));
    }

    static vec8f load_aligned(const float* ptr) {
        return vec8f(_mm256_load_ps(ptr));
    }

    void store(float* ptr) const {
        _mm256_storeu_ps(ptr, data);
    }

    void store_aligned(float* ptr) const {
        _mm256_store_ps(ptr, data);
    }

    static vec8f broadcast(float v) {
        return vec8f(_mm256_set1_ps(v));
    }

    vec8f operator+(const vec8f& other) const {
        return vec8f(_mm256_add_ps(data, other.data));
    }

    vec8f operator-(const vec8f& other) const {
        return vec8f(_mm256_sub_ps(data, other.data));
    }

    vec8f operator*(const vec8f& other) const {
        return vec8f(_mm256_mul_ps(data, other.data));
    }

    vec8f operator/(const vec8f& other) const {
        return vec8f(_mm256_div_ps(data, other.data));
    }

    vec8f& operator+=(const vec8f& other) {
        data = _mm256_add_ps(data, other.data);
        return *this;
    }

    vec8f& operator-=(const vec8f& other) {
        data = _mm256_sub_ps(data, other.data);
        return *this;
    }

    vec8f& operator*=(const vec8f& other) {
        data = _mm256_mul_ps(data, other.data);
        return *this;
    }

    __m256 raw() const {
        return data;
    }

    float dot(const vec8f& other) const {

        __m256 mul = _mm256_mul_ps(data, other.data);

        __m128 low = _mm256_castps256_ps128(mul);
        __m128 high = _mm256_extractf128_ps(mul, 1);

        __m128 sum = _mm_add_ps(low, high);

        sum = _mm_hadd_ps(sum, sum);
        sum = _mm_hadd_ps(sum, sum);

        return _mm_cvtss_f32(sum);
    }
};

}