#include <gtest/gtest.h>
#include "matrix_ops.h"

using namespace matrix_ops;

TEST(matrix, create) {
    int **m = matrix_create(5, 3);
    ASSERT_NE(m, nullptr);
    for (std::size_t i = 0; i < 5; ++i) {
        EXPECT_NE(m[i], nullptr);
    }
    matrix_delete(m, 5);
}

TEST(matrix, deleting) {
    int **m = matrix_create(5, 3);
    ASSERT_NE(m, nullptr);
    matrix_delete(m, 5);
    EXPECT_EQ(m, nullptr);
}

TEST(matrix, deleting_empty) {
    int **m = nullptr;
    matrix_delete(m, 5);
    EXPECT_EQ(m, nullptr);
}

TEST(matrix, zero_size) {
    EXPECT_EQ(matrix_create(0, 2), nullptr);
    EXPECT_EQ(matrix_create(3, 0), nullptr);
    EXPECT_EQ(matrix_create(0, 0), nullptr);
}

TEST(matrix, fill) {
    int **m = matrix_create(5, 3);
    matrix_fill(m, 5, 3, 7);
    for (std::size_t i = 0; i < 5; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
            EXPECT_EQ(m[i][j], 7);
        }
    }
    matrix_delete(m, 5);
}

TEST(matrix, fill_zero_size) {
    int **m = matrix_create(5, 3);
    matrix_fill(m, 0, 0, 7);
    for (std::size_t i = 0; i < 5; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
            EXPECT_EQ(m[i][j], 0);
        }
    }
    matrix_delete(m, 5);
}

TEST(matrix, max_row) {
    int **m = matrix_create(2,3);
    m[0][0] = 3;
    m[0][1] = 1;
    m[0][2] = 4;
    m[1][0] = 5; 
    m[1][1] = 2;
    m[1][2] = 0;
    int *maxes = matrix_row_max(m, 2, 3);
    ASSERT_NE(maxes, nullptr);
    EXPECT_EQ(maxes[0], 4);
    EXPECT_EQ(maxes[1], 5);
    delete[] maxes;
    matrix_delete(m, 2);
}

TEST (matrix, min_col) {
    int **m = matrix_create(2, 3);
    m[0][0] = 3;
    m[0][1] = 1;
    m[0][2] = 4;
    m[1][0] = 5; 
    m[1][1] = 2;
    m[1][2] = 0;
    int *mins = matrix_col_min(m, 2, 3);
    ASSERT_NE(mins, nullptr);
    EXPECT_EQ(mins[0], 3);
    EXPECT_EQ(mins[1], 1);
    EXPECT_EQ(mins[2], 0);
    delete[] mins;
    matrix_delete(m, 2);
}

TEST(matrix, max_row_zero_size) {
    EXPECT_EQ(matrix_row_max(nullptr, 5, 3), nullptr);
    int **m = matrix_create(5, 3);
    EXPECT_EQ(matrix_row_max(m, 0, 3), nullptr);
    EXPECT_EQ(matrix_row_max(m, 5, 0), nullptr);
    matrix_delete(m, 5);
}

TEST(matrix, min_col_zero_size) {
    EXPECT_EQ(matrix_col_min(nullptr, 5, 3), nullptr);
    int **m = matrix_create(5, 3);
    EXPECT_EQ(matrix_col_min(m, 0, 3), nullptr);
    EXPECT_EQ(matrix_col_min(m, 5, 0), nullptr);
    matrix_delete(m, 5);
}
