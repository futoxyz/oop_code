#include "shape.h"
#include <cmath>
#include <istream>
#include <ostream>
#include <stdexcept>

namespace {
constexpr double pi_ = 3.1415926535897932;
constexpr double eps_ = 1e-9;

bool isValid(double x) { return std::isfinite(x) && x >= 0.0; }
}


Shape::operator double() const {
    return metric();
}

bool Shape::operator==(const Shape& other) const {
    return std::abs(metric() - other.metric()) <= eps_;
}

bool Shape::operator!=(const Shape& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& out, const Shape& shape) {
    shape.print(out);
    return out;
}

std::istream& operator>>(std::istream& in, Shape& shape) {
    shape.read(in);
    return in;
}

Circle::Circle()
    : r_(0.0) {
}

Circle::Circle(double r)
    : r_(r) {
    if (!isValid(r)) {
        throw std::invalid_argument("Circle: radius must be finite and non-negative");
    }
}

double Circle::metric() const {
    return pi_ * r_ * r_;
}

void Circle::print(std::ostream& out) const {
    out << "Circle: r=" << r_;
}

void Circle::read(std::istream& in) {
    double r;
    if (!(in >> r)) {
        return;
    }
    if (!isValid(r)) {
        return;
    }
    r_ = r;
}

Shape* Circle::clone() const {
    return new Circle(*this);
}

double Circle::radius() const {
    return r_;
}


Ellipse::Ellipse()
    : a_(0.0), b_(0.0) {
}

Ellipse::Ellipse(double a, double b)
    : a_(a), b_(b) {
    if (!isValid(a) || !isValid(b)) {
        throw std::invalid_argument("Ellipse: semi-axes must be finite and non-negative");
    }
}

double Ellipse::metric() const {
    return pi_ * a_ * b_;
}

void Ellipse::print(std::ostream& out) const {
    out << "Ellipse: a=" << a_ << " b=" << b_;
}

void Ellipse::read(std::istream& in) {
    double a;
    double b;
    if (!(in >> a >> b)) {
        return;
    }
    if (!isValid(a) || !isValid(b)) {
        return;
    }
    a_ = a;
    b_ = b;
}

Shape* Ellipse::clone() const {
    return new Ellipse(*this);
}

bool Ellipse::isCircle() const {
    return std::abs(a_ - b_) < eps_;
}


Sector::Sector()
    : r_(0.0), angle_(0.0) {
}

Sector::Sector(double r, double angle)
    : r_(r), angle_(angle) {
    if (!isValid(r) || (0.0 < angle < 2 * pi_)) {
        throw std::invalid_argument("Sector: radius must be finite and non-negative and angle must be between 0 and 2pi");
    }
}

double Sector::metric() const {
    return angle_ * r_ * r_ / 2.0;
}

void Sector::print(std::ostream& out) const {
    out << "Sector: r=" << r_ << " angle=" << angle_;
}

void Sector::read(std::istream& in) {
    double r;
    double angle;
    if (!(in >> r >> angle)) {
        return;
    }
    if (!isValid(r) || !isValid(angle)) {
        return;
    }
    r_ = r;
    angle_ = angle;
}

Shape* Sector::clone() const {
    return new Sector(*this);
}

bool Sector::isSemicircle() const {
    return std::abs(angle_ - pi_) < eps_;
}