#include "matrix_ops.h"
#include <iostream>
#include <new>


namespace matrix_ops {
int **matrix_create(std::size_t rows, std::size_t cols) {
    if (rows == 0 || cols == 0) return nullptr;

    int **m = nullptr;
    try {
        m = new int*[rows]{};
        for (std::size_t i = 0; i < rows; i++) {
            m[i] = new int[cols]{};
        }
    } catch (const std::bad_alloc &) {
        if (m) {
            for (std::size_t j = 0; j < rows; j++) {
                delete[] m[j];
            }
            delete[] m;
            m = nullptr;
        }
        return nullptr;
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


void matrix_delete(int **&m, std::size_t rows) {
    if (!m) return;
    for (std::size_t i = 0; i < rows; i++) {
        delete[] m[i];
    }
    delete[] m;
    m = nullptr;
}

int *matrix_row_max(const int* const* m, std::size_t rows, std::size_t cols) {
    if (!m || rows == 0 || cols == 0) return nullptr;

    int *row_max = nullptr;
    try {
        row_max = new int[rows];
    } catch (const std::bad_alloc &) {
        return nullptr;
    }

    for (std::size_t i = 0; i < rows; i++) {
        row_max[i] = m[i][0];
        for (std::size_t j = 1; j < cols; j++) {
            if (m[i][j] > row_max[i]) {
                row_max[i] = m[i][j];
            }
        }
    }
    return row_max;
}

int *matrix_col_min(const int* const* m, std::size_t rows, std::size_t cols) {
    if (!m || rows == 0 || cols == 0) return nullptr;

    int *col_min = nullptr;
    try {
        col_min = new int[cols];
    } catch (const std::bad_alloc &) {
        return nullptr;
    }

    for (std::size_t i = 0; i < cols; i++) {
        col_min[i] = m[0][i];
        for (std::size_t j = 1; j < rows; j++) {
            if (m[j][i] < col_min[i]) {
                col_min[i] = m[j][i];
            }
        }
    }
    return col_min;
}
}
