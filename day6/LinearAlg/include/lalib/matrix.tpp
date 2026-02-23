#include "matrix.hpp"
#include <stdexcept>

namespace lalib {

template <typename T>
Matrix<T>::Matrix(unsigned int rows, unsigned int cols)
    : rows_(rows), cols_(cols)
{
    data_ = new T[rows * cols];
}

template <typename T>
Matrix<T>::~Matrix() {
    delete[] data_;
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& other)
    : rows_(other.rows_), cols_(other.cols_)
{
    data_ = new T[rows_ * cols_];
    for (unsigned int i = 0; i < rows_ * cols_; ++i)
        data_[i] = other.data_[i];
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other) {
    if (this != &other) {
        delete[] data_;
        rows_ = other.rows_;
        cols_ = other.cols_;
        data_ = new T[rows_ * cols_];
        for (unsigned int i = 0; i < rows_ * cols_; ++i)
            data_[i] = other.data_[i];
    }
    return *this;
}

template <typename T>
T Matrix<T>::get(unsigned int r, unsigned int c) const {
    return data_[r * cols_ + c];
}

template <typename T>
void Matrix<T>::set(unsigned int r, unsigned int c, T value) {
    data_[r * cols_ + c] = value;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::runtime_error("Dimensions should be the same");
    }

    Matrix<T> result(rows_, cols_);
    for (unsigned int i = 0; i < rows_ * cols_; ++i) {
            result.data_[i] = data_[i] + other.data_[i];
    }

    return result;
}

template <typename T>
Matrix<T> Matrix<T>::transpose() const {
    Matrix<T> result(cols_, rows_);

    for (unsigned int r = 0; r < rows_; ++r) {
        for (unsigned int c = 0; c < cols_; ++c) {
            result.data_[c * rows_ + r] = data_[r * cols_ + c];
        }
    }

    return result;
}

template <typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& other) {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::runtime_error("Dimensions should be the same");
    }

    for (unsigned int i = 0 ; i < cols_*rows_; ++i) {
        data_[i] += other.data_[i];
    }

    return *this;
}
} // namespace lalib