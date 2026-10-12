#include "matrix.h"
#include <iostream>
#include <new>
#include <algorithm>
#include <cmath>

namespace matrix {
Matrix::Matrix(const Matrix &other) : rows_(other.rows_), cols_(other.cols_) {
    arr = new double[rows_ * cols_];
    std::copy(other.arr, other.arr + (rows_ * cols_), arr);
}

Matrix& Matrix::operator=(const Matrix &other) {
    if (this != &other) {
        delete[] arr;
        rows_ = other.rows_;
        cols_ = other.cols_;
        arr = new double[rows_ * cols_];
        std::copy(other.arr, other.arr + (rows_ * cols_), arr);
    }
    return *this;
}

void Matrix::print() const {
    for (std::size_t i = 1; i <= rows_; i++) {
        for (std::size_t j = 1; j <= cols_; j++) {
            std::cout << get(i,j) << " ";
        }
        std::cout << "\n";
    }
}

Matrix Matrix::add(const Matrix &b) {
    if (this->cols_ != b.cols() || this->rows_ != b.rows()) return *this;
    Matrix result(this->rows_, this->cols_, 0);
    for (std::size_t i = 1; i <= this->rows_; i++) {
        for (std::size_t j = 1; j <= this->cols_; j++) {
            result.set(i, j, this->get(i,j) + b.get(i,j));
        }
    }    
    return result;
}

Matrix Matrix::mul(const Matrix &b) {
    if (this->cols_ != b.rows()) return *this;
    Matrix result(this->rows_, b.cols(), 0);
    for (std::size_t i = 1; i <= this->rows_; i++) {
        for (std::size_t j = 1; j <= b.cols(); j++) {
            double sum = 0.0;
            for (std::size_t k = 1; k <= this->cols_; k++) {
                sum += this->get(i, k) * b.get(k, j);
            }
            result.set(i, j, sum);
        }
    }
    return result;
}

Matrix diagonal(std::size_t n, double val) {
    Matrix diag(n, n, 0);
    for (std::size_t i = 1; i <= n; i++) {
        diag.set(i,i, val);
    }
    return diag;
}

Matrix Matrix::pow(std::size_t n) {
    if (cols_ != rows_ || cols_ == 0) return *this;
    if (n == 0) return diagonal(cols_, 1);
    Matrix result = *this;
    for (std::size_t h = 1; h < n; h++) {
        result = result.mul(*this);
    }
    return result;
}

Matrix diag_pow(const Matrix &a, std::size_t n) {
    if (a.cols() != a.rows() || a.cols() != 2) return a;

    double a11 = a.get(1, 1);
    double a12 = a.get(1, 2);
    double a21 = a.get(2, 1);
    double a22 = a.get(2, 2);

    if (a12 == 0 && a21 == 0) {
        Matrix res(2, 2, 0.0);
        res.set(1, 1, std::pow(a11, n));
        res.set(2, 2, std::pow(a22, n));
        return res;
    }

    double trace = a11 + a22;
    double det = a11 * a22 - a12 * a21;
    double d = trace * trace - 4 * det;
    if (d <= 0) return a; 
    double sqrt_d = std::sqrt(d);
    double lambda1 = (trace + sqrt_d) / 2.0;
    double lambda2 = (trace - sqrt_d) / 2.0;
    Matrix diag(2, 2, 0.0);
    diag.set(1, 1, std::pow(lambda1, n));
    diag.set(2, 2, std::pow(lambda2, n));
    Matrix S(2, 2, 0.0);
    if (a12 != 0) {
        S.set(1, 1, lambda1 - a22); S.set(1, 2, lambda2 - a22);
        S.set(2, 1, a21);           S.set(2, 2, a21);
    } else if (a21 != 0) {
        S.set(1, 1, a12);           S.set(1, 2, a12);
        S.set(2, 1, lambda1 - a11); S.set(2, 2, lambda2 - a11);
    }
    
    double det_S = S.get(1, 1) * S.get(2, 2) - S.get(1, 2) * S.get(2, 1);
    if (std::abs(det_S) < 1e-9) return a;
    Matrix S_inv(2, 2, 0.0);
    S_inv.set(1, 1,  S.get(2, 2) / det_S);
    S_inv.set(1, 2, -S.get(1, 2) / det_S);
    S_inv.set(2, 1, -S.get(2, 1) / det_S);
    S_inv.set(2, 2,  S.get(1, 1) / det_S);
    Matrix temp = S.mul(diag);
    Matrix result = temp.mul(S_inv);
    return result;
}
}
