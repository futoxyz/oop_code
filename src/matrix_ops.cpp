#include <iostream>
#include "matrix_ops.h"


int **matrix_create(std::size_t rows, std::size_t cols) {
    if (rows == 0 || cols == 0) return nullptr;
    int **m{new int*[rows]{}};
    if (!m) return nullptr;
    for (int i = 0; i < rows; i++) {
        m[i] = new int[cols]{};
        if (!m[i]) {
            for (int j = 0; j < i; j++) {
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
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m[i][j] = value;
        }
    }
    return;
}


void matrix_print(const int *const *m, std::size_t rows, std::size_t cols) {
    if (!m || rows == 0 || cols == 0) return;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << m[i][j] << " ";
        }
        std::cout << "\n";
    }
}


void matrix_delete(int **m, std::size_t rows) {
    if (!m || rows == 0) return;
    for (int i = 0; i < rows; i++) {
        delete[] m[i];
    }
    delete[] m;
    return;
}