#include "temperature.h"
#include <cmath>
#include <iostream>


Temperature::Temperature()
    : kelvin_(zero_) {
}


Temperature::Temperature(double value, TempUnit unit)
    : kelvin_(toKelvin(value, unit)) {}

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

std::ostream& operator<<(std::ostream& os, const Temperature& t) {
    return os << t.kelvin_ << " K";
}