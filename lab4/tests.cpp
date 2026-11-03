#include <gtest/gtest.h>

#include "shape.h"

namespace {
constexpr double pi_ = 3.1415926535897932;
constexpr double eps_ = 1e-9;
}

TEST(metric, circle) {
    EXPECT_NEAR(Circle(2.0).metric(), pi_ * 4.0, eps_);
}

TEST(metric, ellipse) {
    EXPECT_NEAR(Ellipse(3.0, 2.0).metric(), pi_ * 6.0, eps_);
}

TEST(metric, sector) {
    EXPECT_NEAR(Sector(2.0, pi_ / 2.0).metric(), pi_, eps_);
}

TEST(type, circle) {
    EXPECT_DOUBLE_EQ(Circle(5.0).radius(), 5.0);
}

TEST(type, ellipse_circle) {
    EXPECT_TRUE(Ellipse(2.0, 2.0).isCircle());
    EXPECT_TRUE(Ellipse(2.0, 2.0 + 1e-12).isCircle());
    EXPECT_FALSE(Ellipse(2.0, 3.0).isCircle());
}

TEST(type, sector_semicircle) {
    EXPECT_TRUE(Sector(1.0, pi_).isSemicircle());
    EXPECT_FALSE(Sector(1.0, 1.0).isSemicircle());
}
