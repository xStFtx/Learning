#ifndef LALIB_MATRIX_HPP
#define LALIB_MATRIX_HPP

namespace lalib {

template <typename T>
class Matrix {
public:
    //constructor
    Matrix(unsigned int rows, unsigned int cols);
    //destructor
    ~Matrix();
    //Copy Constructor
    Matrix(const Matrix<T>& other);
    // Assignment
    Matrix<T>& operator=(const Matrix<T>& other);
    //getter
    T get(unsigned int r, unsigned int c) const;
    //setter
    void set(unsigned int r, unsigned int c, T value);
    // overload the + operator
    Matrix<T> operator+(const Matrix<T>& other) const;
    //transpose matrix; swap rows with cols
    Matrix<T> transpose() const;
private:
    unsigned int rows_;
    unsigned int cols_;
    T* data_;
};

} // namespace lalib

#include "matrix.tpp"

#endif