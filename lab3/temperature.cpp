#include "temperature.h"
#include <cmath>
#include <iostream>
#include <compare>


Temperature::Temperature(): kelvin_(0.0), unit_(TempUnit::K) {}
Temperature::Temperature(double value, TempUnit unit): kelvin_(toKelvin(value, unit)), unit_(unit) {}

double Temperature::toKelvin(double value, TempUnit unit) {
    switch (unit) {
        case TempUnit::C:
            return value + zero_;
        case TempUnit::F:
            return (value - 32.0) * 5.0 / 9.0 + zero_;
        case TempUnit::K:
            return value;
    }
    return value;
}

double Temperature::fromKelvin(double kelvin, TempUnit unit) {
    switch (unit) {
        case TempUnit::C:
            return kelvin - zero_;
        case TempUnit::F:
            return (kelvin - zero_) * 9.0 / 5.0 + 32.0;
        case TempUnit::K:
            return kelvin;
    }
    return kelvin;
}

double Temperature::kelvin() const {
    return kelvin_;
}

double Temperature::in(TempUnit unit) const {
    return fromKelvin(kelvin_, unit);
}

bool Temperature::operator==(const Temperature& other) const {
    return std::fabs(kelvin_ - other.kelvin_) <= eps_;
}

bool Temperature::operator!=(const Temperature& other) const {
    return !(*this == other);
}

bool Temperature::operator>(const Temperature& other) const {
    return kelvin_ - other.kelvin_ > eps_;
}

bool Temperature::operator<(const Temperature& other) const {
    return other > *this;
}
bool Temperature::operator>=(const Temperature& other) const {
    return (*this > other || *this == other);
}

bool Temperature::operator<=(const Temperature& other) const {
    return (*this < other || *this == other);
}


std::ostream& operator<<(std::ostream& os, const Temperature& t) {
    switch (t.unit_) {
        case TempUnit::K:
            return os << t.in(TempUnit::K) << " K";
        case TempUnit::C:
            return os << t.in(TempUnit::C) << " C";
        case TempUnit::F:
            return os << t.in(TempUnit::F) << " F";
    }
    return os << t.kelvin_ << " K";
}

Temperature C(double v) { return Temperature(v, TempUnit::C); }
Temperature F(double v) { return Temperature(v, TempUnit::F); }
Temperature K(double v) { return Temperature(v, TempUnit::K); }