#include "temperature.h"

#include <cmath>
#include <iostream>

namespace {
constexpr double absoluteZero = 273.15;
}

Temperature::Temperature()
    : kelvin_(absoluteZero) {}

Temperature::Temperature(double value, TempUnit unit)
    : kelvin_(toKelvin(value, unit)) {}

double Temperature::toKelvin(double value, TempUnit unit) {
    switch (unit) {
        case TempUnit::Celsius:
            return value + absoluteZero;
        case TempUnit::Fahrenheit:
            return (value - 32.0) * 5.0 / 9.0 + absoluteZero;
        case TempUnit::Kelvin:
            return value;
    }
    return value;  // недостижимо; защита от предупреждений компилятора
}

double Temperature::fromKelvin(double kelvin, TempUnit unit) {
    switch (unit) {
        case TempUnit::Celsius:
            return kelvin - absoluteZero;
        case TempUnit::Fahrenheit:
            return (kelvin - absoluteZero) * 9.0 / 5.0 + 32.0;
        case TempUnit::Kelvin:
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

// Печатаем в канонической единице: "273.15 K".
std::ostream& operator<<(std::ostream& os, const Temperature& t) {
    return os << t.kelvin_ << " K";
}