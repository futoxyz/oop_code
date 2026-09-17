#pragma once
#include <cstddef>


namespace matrix_ops {
int **matrix_create(std::size_t rows, std::size_t cols);
void matrix_delete(int **&m, std::size_t rows);
void matrix_fill(int **m, std::size_t rows, std::size_t cols, int value);
void matrix_print(const int *const *m, std::size_t rows, std::size_t cols);
int* matrix_row_max(const int* const* m, std::size_t rows, std::size_t cols);
int *matrix_col_min(const int* const* m, size_t rows, size_t cols);
}
