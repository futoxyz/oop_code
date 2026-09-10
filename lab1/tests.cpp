#include <gtest/gtest.h>
#include "matrix_ops.h"

using namespace matrix_ops;

TEST(MatrixLifecycleTest, CreateAndDeleteStandard) {
    std::size_t rows = 3;
    std::size_t cols = 4;
    int** m = matrix_create(rows, cols);

    ASSERT_NE(m, nullptr);
    for (std::size_t i = 0; i < rows; ++i) {
        EXPECT_NE(m[i], nullptr);
    }

    matrix_delete(m, rows);
}

TEST(MatrixLifecycleTest, CreateZeroDimensions) {
    EXPECT_EQ(matrix_create(0, 5), nullptr);
    EXPECT_EQ(matrix_create(5, 0), nullptr);
    EXPECT_EQ(matrix_create(0, 0), nullptr);
}

TEST(MatrixOperationsTest, FillMatrix) {
    std::size_t rows = 2;
    std::size_t cols = 3;
    int** m = matrix_create(rows, cols);

    matrix_fill(m, rows, cols, 7);

    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            EXPECT_EQ(m[i][j], 7);
        }
    }

    matrix_delete(m, rows);
}


TEST(MatrixLifecycleTest, DeleteNullptrSafety) {
    EXPECT_NO_THROW(matrix_delete(nullptr, 5));
    EXPECT_NO_THROW(matrix_delete(nullptr, 0));
}


TEST(MatrixVariantTest, RowMaxStandard) {
    std::size_t rows = 2;
    std::size_t cols = 3;
    int** m = matrix_create(rows, cols);

    m[0][0] = 3; m[0][1] = 1; m[0][2] = 4; // Максимум строки 0 -> 4
    m[1][0] = 5; m[1][1] = 2; m[1][2] = 0; // Максимум строки 1 -> 5

    int* maxes = matrix_row_max(m, rows, cols);
    ASSERT_NE(maxes, nullptr);

    EXPECT_EQ(maxes[0], 4);
    EXPECT_EQ(maxes[1], 5);

    delete[] maxes;
    matrix_delete(m, rows);
}


TEST(MatrixVariantTest, ColMinStandard) {
    std::size_t rows = 2;
    std::size_t cols = 3;
    int** m = matrix_create(rows, cols);

    m[0][0] = 3; m[0][1] = 1; m[0][2] = 4;
    m[1][0] = 5; m[1][1] = 2; m[1][2] = 0;

    int* mins = matrix_col_min(m, rows, cols);
    ASSERT_NE(mins, nullptr);

    EXPECT_EQ(mins[0], 3);
    EXPECT_EQ(mins[1], 1);
    EXPECT_EQ(mins[2], 0);

    delete[] mins;
    matrix_delete(m, rows);
}


TEST(MatrixVariantTest, NegativeNumbers) {
    std::size_t rows = 2;
    std::size_t cols = 2;
    int** m = matrix_create(rows, cols);

    m[0][0] = -10; m[0][1] = -5;
    m[1][0] = -2;  m[1][1] = -8;

    int* maxes = matrix_row_max(m, rows, cols);
    int* mins = matrix_col_min(m, rows, cols);

    ASSERT_NE(maxes, nullptr);
    ASSERT_NE(mins, nullptr);

    EXPECT_EQ(maxes[0], -5);
    EXPECT_EQ(maxes[1], -2);

    EXPECT_EQ(mins[0], -10);
    EXPECT_EQ(mins[1], -8);

    delete[] maxes;
    delete[] mins;
    matrix_delete(m, rows);
}

TEST(MatrixVariantTest, SingleElementMatrix) {
    std::size_t rows = 1;
    std::size_t cols = 1;
    int** m = matrix_create(rows, cols);

    m[0][0] = 42;

    int* maxes = matrix_row_max(m, rows, cols);
    int* mins = matrix_col_min(m, rows, cols);

    ASSERT_NE(maxes, nullptr);
    ASSERT_NE(mins, nullptr);

    EXPECT_EQ(maxes[0], 42);
    EXPECT_EQ(mins[0], 42);

    delete[] maxes;
    delete[] mins;
    matrix_delete(m, rows);
}


TEST(MatrixVariantTest, RowMaxInvalidInput) {
    EXPECT_EQ(matrix_row_max(nullptr, 2, 2), nullptr);

    int** m = matrix_create(2, 2);
    EXPECT_EQ(matrix_row_max(m, 0, 2), nullptr);
    EXPECT_EQ(matrix_row_max(m, 2, 0), nullptr);

    matrix_delete(m, 2);
}

TEST(MatrixVariantTest, ColMinInvalidInput) {
    EXPECT_EQ(matrix_col_min(nullptr, 2, 2), nullptr);

    int** m = matrix_create(2, 2);
    EXPECT_EQ(matrix_col_min(m, 0, 2), nullptr);
    EXPECT_EQ(matrix_col_min(m, 2, 0), nullptr);

    matrix_delete(m, 2);
}
