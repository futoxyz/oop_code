#include <gtest/gtest.h>
#include <cmath>

#include <stdexcept>
#include <string>
#include "ShapeSet.h"
#include "shape.h"

namespace {
constexpr double pi_ = 3.1415926535897932;
constexpr double eps_ = 1e-9;
}

TEST(area_, circle_init) {
    EXPECT_NEAR(Circle(2.0).area(), pi_ * 4.0, eps_);
}

TEST(area_, ellipse_init) {
    EXPECT_NEAR(Ellipse(3.0, 2.0).area(), pi_ * 6.0, eps_);
}

TEST(area_, sector_init) {
    EXPECT_NEAR(Sector(2.0, pi_ / 2.0).area(), pi_, eps_);
}

TEST(shape_type, circle_r) {
    EXPECT_DOUBLE_EQ(Circle(5.0).radius(), 5.0);
}

TEST(shape_type, ellipse_is_circle) {
    EXPECT_TRUE(Ellipse(2.0, 2.0).isCircle());
    EXPECT_TRUE(Ellipse(2.0, 2.0 + 1e-12).isCircle());
    EXPECT_FALSE(Ellipse(2.0, 3.0).isCircle());
}

TEST(shape_type, sector_is_semicircle) {
    EXPECT_TRUE(Sector(1.0, pi_).isSemicircle());
    EXPECT_FALSE(Sector(1.0, 1.0).isSemicircle());
}

TEST(shape_set, total_area) {
    ShapeSet set;
    set.add(new Circle(1.0));
    set.add(new Ellipse(2.0, 3.0));
    set.add(new Sector(2.0, pi_ / 2.0));
    EXPECT_EQ(set.size(), 3u);
    EXPECT_NEAR(set.total_area(), 8.0 * pi_, eps_);
}

TEST(shape_set, ConstAccess) {
    ShapeSet set;
    set.add(new Circle(1.0));
    const ShapeSet& cset = set;
    EXPECT_NEAR(cset.at(0).area(), pi_, eps_);
}

TEST(comparison, equal_area_) {
    Circle c(1.0);
    Ellipse e(1.0, 1.0);
    EXPECT_TRUE(c == e);
    EXPECT_TRUE(Circle(2.0) == Circle(2.0));
}

TEST(comparison, not_equal_area_) {
    EXPECT_FALSE(Circle(1.0) == Circle(2.0));
    EXPECT_TRUE(Circle(1.0) != Circle(2.0));
}

TEST(convert, to_double) {
    Circle c(2.0);
    Ellipse e(3.0, 2.0);
    Sector s(2.0, 1.0);
    EXPECT_DOUBLE_EQ(static_cast<double>(c), c.area());
    EXPECT_DOUBLE_EQ(static_cast<double>(e), e.area());
    EXPECT_DOUBLE_EQ(static_cast<double>(s), s.area());
}

TEST(area_, zero_size) {
    EXPECT_DOUBLE_EQ(Circle(0.0).area(), 0.0);
    EXPECT_DOUBLE_EQ(Ellipse(0.0, 5.0).area(), 0.0);
    EXPECT_DOUBLE_EQ(Sector(0.0, 1.0).area(), 0.0);
    EXPECT_DOUBLE_EQ(Circle().area(), 0.0);
}

TEST(area_, negative_size) {
    EXPECT_THROW(Circle(-1.0), std::invalid_argument);
    EXPECT_THROW(Ellipse(-1.0, 2.0), std::invalid_argument);
    EXPECT_THROW(Ellipse(1.0, -2.0), std::invalid_argument);
    EXPECT_THROW(Sector(-1.0, 1.0), std::invalid_argument);
    EXPECT_THROW(Sector(1.0, -1.0), std::invalid_argument);
}

TEST(shape_set, empty_set) {
    ShapeSet set;
    EXPECT_TRUE(set.empty());
    EXPECT_EQ(set.size(), 0u);
    EXPECT_DOUBLE_EQ(set.total_area(), 0.0);
    EXPECT_FALSE(set.remove(0));
    EXPECT_THROW(set.at(0), std::out_of_range);
}

TEST(shape_set, stress_test) {
    ShapeSet set;
    double expected = 0.0;
    for (int i = 0; i < 1000; ++i) {
        set.add(new Circle(static_cast<double>(i)));
        expected += pi_ * i * i;
    }
    EXPECT_EQ(set.size(), 1000u);
    EXPECT_NEAR(set.total_area(), expected, 1e-3);
}

namespace {
class Probe : public Shape {
public:
    static inline int alive = 0;

    Probe() { ++alive; }
    Probe(const Probe& other) : Shape(other) { ++alive; }
    ~Probe() override { --alive; }

    double area() const override { return 1.0; }
    void print(std::ostream& out) const override { out << "Probe"; }
    void read(std::istream&) override {}
    Shape* clone() const override { return new Probe(*this); }
};
}

TEST(memory, virtual_destructor) {
    ASSERT_EQ(Probe::alive, 0);
    {
        Shape* p = new Probe();
        EXPECT_EQ(Probe::alive, 1);
        delete p;
    }
    EXPECT_EQ(Probe::alive, 0);
}

TEST(memory, clear) {
    ASSERT_EQ(Probe::alive, 0);
    {
        ShapeSet set;
        set.add(new Probe());
        set.add(new Probe());
        set.add(new Probe());
        EXPECT_EQ(Probe::alive, 3);

        ShapeSet copy(set);
        EXPECT_EQ(Probe::alive, 6);

        set.remove(0);
        EXPECT_EQ(Probe::alive, 5);
    }
    EXPECT_EQ(Probe::alive, 0);
}

TEST(dynamic_cast_, correct_type) {
    Circle c(1.0);
    Ellipse e(1.0, 2.0);
    Sector s(1.0, 1.0);
    Shape& rc = c;
    Shape& re = e;
    Shape& rs = s;

    EXPECT_NE(dynamic_cast<Circle*>(&rc), nullptr);
    EXPECT_NE(dynamic_cast<Ellipse*>(&re), nullptr);
    EXPECT_NE(dynamic_cast<Sector*>(&rs), nullptr);
}

TEST(dynamis_cast_, wrong_type) {
    Circle c(1.0);
    Shape& rc = c;

    EXPECT_EQ(dynamic_cast<Ellipse*>(&rc), nullptr);
    EXPECT_EQ(dynamic_cast<Sector*>(&rc), nullptr);
}
