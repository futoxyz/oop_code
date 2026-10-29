#include <gtest/gtest.h>
#include <sstream>
#include "Temperature.h"
#include "TemperatureSet.h"


TEST(temperature_test, default_init) {
    Temperature t;
    EXPECT_EQ(t, K(0));
    EXPECT_NEAR(t.kelvin(), 0, 1e-9);
}

TEST(temperature_test, unit_converts) {
    EXPECT_EQ(C(0), F(32));
    EXPECT_EQ(C(0), K(273.15));
    EXPECT_EQ(C(100), F(212));
    EXPECT_NE(C(0), C(1));
}

TEST(temperature_test, canonization) {
    Temperature t = F(98.6);
    Temperature again(t.in(TempUnit::K), TempUnit::K);
    EXPECT_EQ(t, again);
    EXPECT_NEAR(t.kelvin(), again.kelvin(), 1e-9);
}

TEST(temperature_set_test, init) {
    TemperatureSet s;
    EXPECT_EQ(s.size(), 0u);
    EXPECT_TRUE(s.empty());
    EXPECT_FALSE(s.contains(C(0)));
}

TEST(temperature_set_test, add_duplicates) {
    TemperatureSet s;
    EXPECT_TRUE(s.add(C(10)));
    EXPECT_FALSE(s.add(C(10)));
    EXPECT_EQ(s.size(), 1u);
}

TEST(temperature_set_test, duplicates_units) {
    TemperatureSet s;
    EXPECT_TRUE(s.add(C(0)));
    EXPECT_FALSE(s.add(F(32)));
    EXPECT_FALSE(s.add(K(273.15)));
    EXPECT_EQ(s.size(), 1u);
}

TEST(temperature_set_test, removing) {
    TemperatureSet s;
    s.add(C(1));
    s.add(C(2));
    EXPECT_TRUE(s.remove(C(1)));
    EXPECT_FALSE(s.remove(C(1)));
    EXPECT_FALSE(s.remove(C(99)));
    EXPECT_EQ(s.size(), 1u);
    EXPECT_TRUE(s.remove(F(35.6)));
    EXPECT_TRUE(s.empty());
}

std::string to_str(const TemperatureSet& s) {
    std::ostringstream os;
    os << s;
    return os.str();
}

TEST(temperature_set_test, empty_operations) {
    TemperatureSet a, b;
    EXPECT_TRUE((a | b).empty());
    EXPECT_TRUE((a & b).empty());
    EXPECT_TRUE((a - b).empty());
    EXPECT_EQ(a, b);
    EXPECT_FALSE(a.remove(C(5)));
    EXPECT_EQ(to_str(a), "{ }");
}

TEST(temperature_set_test, remove_ordering) {
    TemperatureSet s;
    s += K(1);
    s += K(2);
    s += K(3);
    s -= K(2);
    EXPECT_EQ(to_str(s), "{ 1 K, 3 K }");
}

TEST(temperature_set_test, contains) {
    TemperatureSet s;
    s.add(C(25));
    EXPECT_TRUE(s.contains(C(25)));
    EXPECT_FALSE(s.contains(C(-25)));
}

TEST(temperature_set_test, copy) {
    TemperatureSet a;
    a.add(C(1));
    a.add(C(2));
    TemperatureSet b(a);
    b.add(C(3));
    b.remove(C(1));
    EXPECT_EQ(a.size(), 2u);
    EXPECT_TRUE(a.contains(C(1)));
    EXPECT_FALSE(a.contains(C(3)));
    EXPECT_EQ(b.size(), 2u);
}

TEST(temperature_set_test, self_assign) {
    TemperatureSet a;
    a.add(C(1));
    TemperatureSet& ref = a;
    a = ref;
    EXPECT_EQ(a.size(), 1u);
    EXPECT_TRUE(a.contains(C(1)));
}

TEST(temperature_set_test, move) {
    TemperatureSet a;
    a.add(C(1));
    a.add(C(2));
    TemperatureSet b(std::move(a));
    EXPECT_EQ(b.size(), 2u);
    EXPECT_TRUE(a.empty());
    EXPECT_FALSE(a.contains(C(1)));
    EXPECT_TRUE(a.add(C(7)));
    EXPECT_EQ(a.size(), 1u);

    TemperatureSet c;
    c = std::move(b);
    EXPECT_EQ(c.size(), 2u);
    EXPECT_TRUE(b.empty());
}

TEST(temperature_set_test, move_empty) {
    TemperatureSet a;
    TemperatureSet b(std::move(a));
    EXPECT_TRUE(a.empty());
    EXPECT_TRUE(b.empty());
}

TEST(temperature_set_test, union_intersection) {
    TemperatureSet a, b;
    a.add(K(1)); a.add(K(2)); a.add(K(3));
    b.add(K(3)); b.add(K(4));

    TemperatureSet u = a | b;
    TemperatureSet i = a & b;
    TemperatureSet d = a - b;

    EXPECT_EQ(u.size(), 4u);
    for (double v : {1.0, 2.0, 3.0, 4.0}) EXPECT_TRUE(u.contains(K(v)));

    EXPECT_EQ(i.size(), 1u);
    EXPECT_TRUE(i.contains(K(3)));
    EXPECT_EQ(d.size(), 2u);
    EXPECT_TRUE(d.contains(K(1)));
    EXPECT_TRUE(d.contains(K(2)));
    EXPECT_EQ(a.size(), 3u);
    EXPECT_EQ(b.size(), 2u);
}

TEST(temperature_set_test, equality_order) {
    TemperatureSet a, b;
    a.add(C(0)); a.add(C(100));
    b.add(F(212)); b.add(K(273.15));
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    b.add(C(5));
    EXPECT_TRUE(a != b);
}

TEST(temperature_set_test, output) {
    TemperatureSet s;
    EXPECT_EQ(to_str(s), "{ }");
    s.add(C(1));
    EXPECT_EQ(to_str(s), "{ 1 C }");
    s.add(K(2.5));
    EXPECT_EQ(to_str(s), "{ 1 C, 2.5 K }");
}

TEST(temperature_set_test, stress_test) {
    TemperatureSet s;
    for (int i = 0; i < 1000; ++i) {
        EXPECT_TRUE(s.add(K(i)));
    }
    EXPECT_EQ(s.size(), 1000u);
    for (int i = 0; i < 1000; ++i) {
        EXPECT_FALSE(s.add(K(i)));
    }
    EXPECT_EQ(s.size(), 1000u);
    EXPECT_TRUE(s.contains(K(999)));
    EXPECT_FALSE(s.contains(K(1000)));
}