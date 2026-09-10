#include "matrix_ops.h"
#include <iostream>
#include <new>


namespace matrix_ops {
int **matrix_create(std::size_t rows, std::size_t cols) {
    if (rows == 0 || cols == 0) return nullptr;
    int **m = new (std::nothrow) int*[rows]{};
    if (!m) return nullptr;
    for (std::size_t i = 0; i < rows; i++) {
        m[i] = new (std::nothrow) int[cols]{};
        if (!m[i]) {
            for (std::size_t j = 0; j < i; j++) {
                delete[] m[j];
            }
            delete[] m;
            return nullptr;
        }
    }
    return m;
}


void matrix_fill(int **m, std::size_t rows, std::size_t cols, int value) {
    if (!m || rows == 0 || cols == 0) return;
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < cols; j++) {
            m[i][j] = value;
        }
    }
}


void matrix_print(const int *const *m, std::size_t rows, std::size_t cols) {
    if (!m || rows == 0 || cols == 0) return;
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < cols; j++) {
            std::cout << m[i][j] << " ";
        }
        std::cout << "\n";
    }
}


void matrix_delete(int **m, std::size_t rows) {
    if (!m || rows == 0) return;
    for (std::size_t i = 0; i < rows; i++) {
        delete[] m[i];
    }
    delete[] m;
}


int *matrix_row_max(const int* const* m, std::size_t rows, std::size_t cols) {
    if (!m || rows == 0 || cols == 0) return nullptr;
    int *row_max = new (std::nothrow) int[rows];
    if (!row_max) return nullptr;
    for (std::size_t i = 0; i < rows; i++) {
        row_max[i] = m[i][0];
        if (cols > 1) {
            for (std::size_t j = 1; j < cols; j++) {
                if (m[i][j] > row_max[i]) {
                    row_max[i] = m[i][j];
                }
            }
        }
    }
    return row_max;
}


int *matrix_col_min(const int* const* m, size_t rows, size_t cols) {
    if (!m || rows == 0 || cols == 0) return nullptr;
    int *col_min = new (std::nothrow) int[cols];
    if (!col_min) return nullptr;
    for (std::size_t i = 0; i < cols; i++) {
        col_min[i] = m[0][i];
        if (rows > 1) {
            for (std::size_t j = 1; j < rows; j++) {
                if (m[j][i] < col_min[i]) {
                    col_min[i] = m[j][i];
                }
            }
        }
    }
    return col_min;
}
}
