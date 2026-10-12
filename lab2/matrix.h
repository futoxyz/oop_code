#pragma once
#include <cstddef>
#include <algorithm>

namespace matrix {
class Matrix {
    private:
        std::size_t rows_;
        std::size_t cols_;
        double *arr = nullptr;
    public:
        Matrix(std::size_t r, std::size_t c, double fill_val) : rows_(r), cols_(c) {
            arr = new double[r*c]{};
            std::fill_n(arr, r*c, fill_val);
        }

        Matrix(const Matrix &other);
        Matrix& operator=(const Matrix &other);

        ~Matrix() {
            delete[] arr;
        }

        std::size_t rows() const {return rows_;};
        std::size_t cols() const {return cols_;};

        double get(std::size_t row, std::size_t col) const {return arr[(row-1)*cols_+(col-1)];};
        void print() const;
        void set(std::size_t row, std::size_t col, double value) {arr[(row-1)*cols_+(col-1)] = value;};
        Matrix mul(const Matrix &b);
        Matrix add(const Matrix &b);
        Matrix pow(std::size_t n);
};
Matrix diag_pow(const Matrix &a, std::size_t n);
Matrix diagonal(std::size_t n, double val);
}
