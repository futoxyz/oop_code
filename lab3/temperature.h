#include <cstddef>
#pragma once
#include <iosfwd>


enum class TempUnit {
    C,
    F,
    K
};

class Temperature {
public:
    Temperature();
    Temperature(double value, TempUnit unit);

    double kelvin() const;
    double in(TempUnit unit) const;

    bool operator==(const Temperature& other) const;
    bool operator!=(const Temperature& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Temperature& t);

    static constexpr double eps_ = 1e-9;
    static constexpr double zero_ = 273.15;

private:
    static double toKelvin(double value, TempUnit unit);
    static double fromKelvin(double kelvin, TempUnit unit);
    double kelvin_;
};
