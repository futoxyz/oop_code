#include <gtest/gtest.h>
#include "matrix.h"

using namespace matrix;

TEST(matrix_test, create_get) {
    Matrix m(3, 2, -3.75);
    ASSERT_EQ(m.rows(), 3);
    ASSERT_EQ(m.cols(), 2);
    for (std::size_t i = 1; i <= 3; i++) {
        for (std::size_t j = 1; j <= 2; j++) {
            ASSERT_EQ(m.get(i, j), -3.75);
        }
    }
}

TEST(matrix_test, create_null) {
    Matrix m(0,0,6666666);
    ASSERT_EQ(m.rows(), 0);
    ASSERT_EQ(m.cols(), 0);
}


TEST(matrix_test, set) {
    Matrix m(2, 2, 0.0);
    m.set(1, 2, -4.85);
    ASSERT_DOUBLE_EQ(m.get(1, 2), -4.85);
}


TEST(matrix_test, diagonal_func) {
    Matrix m = diagonal(3, -2.5);
    ASSERT_EQ(m.rows(), 3);
    ASSERT_EQ(m.cols(), 3);
    for (std::size_t i = 1; i <= 3; i++) {
        for (std::size_t j = 1; j <= 3; j++) {
            if (i == j) {
                ASSERT_DOUBLE_EQ(m.get(i, j), -2.5);
            } else {
                ASSERT_DOUBLE_EQ(m.get(i, j), 0.0);
            }
        }
    }
}


TEST(matrix_test, copy) {
    Matrix m1(2, 2, -1.25);
    Matrix m2(m1);
    ASSERT_EQ(m2.rows(), 2);
    ASSERT_EQ(m2.cols(), 2);
    ASSERT_DOUBLE_EQ(m2.get(1, 1), -1.25);
    m2.set(1, 1, 5.5);
    ASSERT_DOUBLE_EQ(m1.get(1, 1), -1.25);
    ASSERT_DOUBLE_EQ(m2.get(1, 1), 5.5);
}


TEST(matrix_test, move) {
    Matrix m1(2, 2, 3.33);
    Matrix m2(std::move(m1));
    ASSERT_EQ(m2.rows(), 2);
    ASSERT_EQ(m2.cols(), 2);
    ASSERT_DOUBLE_EQ(m2.get(1, 1), 3.33);
    ASSERT_EQ(m1.rows(), 0);
    ASSERT_EQ(m1.cols(), 0);
}


TEST(matrix_test, add) {
    Matrix m1(2, 2, -1.5);
    Matrix m2(2, 2, 3.25);
    Matrix res = m1.add(m2);
    ASSERT_EQ(res.rows(), 2);
    ASSERT_EQ(res.cols(), 2);
    ASSERT_DOUBLE_EQ(res.get(1, 1), 1.75);
    ASSERT_DOUBLE_EQ(res.get(2, 2), 1.75);
}


TEST(matrix_test, add_wrong_sizes) {
    Matrix m1(2, 2, -1.5);
    Matrix m2(2, 3, 3.25);
    Matrix res = m1.add(m2);
    ASSERT_EQ(res.rows(), 2);
    ASSERT_EQ(res.cols(), 2);
    for (std::size_t i = 1; i <= 2; i++) {
        for (std::size_t j = 1; j <= 2; j++) {
            ASSERT_DOUBLE_EQ(res.get(i,j), -1.5);
        }
    }
}


TEST(matrix_test, mul) {
    Matrix m1(2, 2, 0.0);
    m1.set(1, 1, -1.5); m1.set(1, 2, 2.0);
    m1.set(2, 1, 3.0);  m1.set(2, 2, -0.5);

    Matrix m2(2, 2, 0.0);
    m2.set(1, 1, 2.0);  m2.set(1, 2, -1.0);
    m2.set(2, 1, 4.0);  m2.set(2, 2, 3.0);

    Matrix res = m1.mul(m2);
    ASSERT_DOUBLE_EQ(res.get(1, 1), 5.0);
    ASSERT_DOUBLE_EQ(res.get(1, 2), 7.5);
    ASSERT_DOUBLE_EQ(res.get(2, 1), 4.0);
    ASSERT_DOUBLE_EQ(res.get(2, 2), -4.5);
}


TEST(matrix_test, mul_wrong_sizes) {
    Matrix m1(2, 2, 0.0);
    m1.set(1, 1, -1.5); m1.set(1, 2, 2.0);
    m1.set(2, 1, 3.0);  m1.set(2, 2, -0.5);

    Matrix m2(3, 3, 0.0);
    m2.set(1, 1, 2.0); m2.set(1, 2, -1.0);
    m2.set(2, 1, 4.0); m2.set(2, 2, 3.0);
    m2.set(2,3, -225); m2.set(3,3, -226);

    Matrix res = m1.mul(m2);
    ASSERT_EQ(res.rows(), 2);
    ASSERT_EQ(res.cols(), 2);
    ASSERT_EQ(m1.get(1, 1), -1.5); ASSERT_EQ(m1.get(1, 2), 2.0);
    ASSERT_EQ(m1.get(2, 1), 3.0); ASSERT_EQ(m1.get(2, 2), -0.5);
}


TEST(matrix_test, power) {
    Matrix m(2, 2, 0.0);
    m.set(1, 1, -1.5); m.set(1, 2, 0.0);
    m.set(2, 1, 0.0);  m.set(2, 2, 2.0);

    Matrix p0 = m.pow(0);
    ASSERT_DOUBLE_EQ(p0.get(1, 1), 1.0);
    ASSERT_DOUBLE_EQ(p0.get(2, 2), 1.0);
    ASSERT_DOUBLE_EQ(p0.get(1, 2), 0.0);

    Matrix p3 = m.pow(3);
    ASSERT_DOUBLE_EQ(p3.get(1, 1), -3.375);
    ASSERT_DOUBLE_EQ(p3.get(2, 2), 8.0);
    ASSERT_DOUBLE_EQ(p3.get(1, 2), 0.0);
}


TEST(matrix_test, diagonalizing_power) {
    Matrix m(2, 2, 0.0);
    m.set(1, 1, -2.0); m.set(1, 2, 0.0);
    m.set(2, 1, 0.0);  m.set(2, 2, 1.5);

    Matrix res = diag_pow(m, 3);
    ASSERT_DOUBLE_EQ(res.get(1, 1), -8.0);
    ASSERT_DOUBLE_EQ(res.get(2, 2), 3.375);
    ASSERT_DOUBLE_EQ(res.get(1, 2), 0.0);
    ASSERT_DOUBLE_EQ(res.get(2, 1), 0.0);
}
