#pragma once
#include <cstddef>
#include <algorithm>
#include <cmath>



namespace matrix {
class Matrix {
    private:
        std::size_t rows_;
        std::size_t cols_;
        double *arr;
    public:
        Matrix() : rows_(0), cols_(0), arr(nullptr) {}
        Matrix(std::size_t r, std::size_t c, double fill_val) : rows_(r), cols_(c) {
            if (r == 0 || c == 0) {
                arr = nullptr;
                rows_ = 0;
                cols_ = 0;
                return;
            }
            try {
                arr = new double[r * c];
                std::fill_n(arr, r * c, fill_val);
            } catch (const std::bad_alloc&) {
                arr = nullptr;
                rows_ = 0;
                cols_ = 0;
            }
        }
        Matrix(const Matrix &other);
        Matrix(Matrix &&other) noexcept;
        void swap(Matrix &other) noexcept;
        Matrix& operator=(Matrix other);
        ~Matrix() {delete[] arr;}

        std::size_t rows() const { return rows_; }
        std::size_t cols() const { return cols_; }

        double get(std::size_t row, std::size_t col) const {
            if (rows_ == 0) return NAN;
            if (row > rows_ || col > cols_) return NAN;
            return arr[(row - 1) * cols_ + (col - 1)];
        }
        
        void set(std::size_t row, std::size_t col, double value) {
            if (rows_ == 0) return;
             if (row > rows_ || col > cols_) return;
            arr[(row - 1) * cols_ + (col - 1)] = value; 
        }

        void print() const;
        Matrix mul(const Matrix &b) const;
        Matrix add(const Matrix &b) const;
        Matrix pow(std::size_t n) const;
};

Matrix diag_pow(const Matrix &a, std::size_t n);
Matrix diagonal(std::size_t n, double val);
}