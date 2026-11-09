#pragma once
#include <iosfwd>


class Shape {
public:
    virtual double area() const = 0;
    virtual void print(std::ostream& out) const = 0;
    virtual void read(std::istream& in) = 0;
    virtual Shape* clone() const = 0;
    explicit operator double() const;
    virtual ~Shape() = default;
    bool operator==(const Shape& other) const;
    bool operator!=(const Shape& other) const;

protected:
    Shape() = default;
    Shape(const Shape&) = default;
    Shape& operator=(const Shape&) = default;
};

std::ostream& operator<<(std::ostream& out, const Shape& shape);
std::istream& operator>>(std::istream& in, Shape& shape);


class Circle : public Shape {
public:
    Circle();
    explicit Circle(double r);
    double area() const override;
    void print(std::ostream& out) const override;
    void read(std::istream& in) override;
    Shape* clone() const override;
    double radius() const;

private:
    double r_;
};


class Ellipse : public Shape {
public:
    Ellipse();
    Ellipse(double a, double b);
    double area() const override;
    void print(std::ostream& out) const override;
    void read(std::istream& in) override;
    Shape* clone() const override;
    bool isCircle() const;

private:
    double a_;
    double b_;
};


class Sector : public Shape {
public:
    Sector();
    Sector(double r, double angle);
    double area() const override;
    void print(std::ostream& out) const override;
    void read(std::istream& in) override;
    Shape* clone() const override;
    bool isSemicircle() const;

private:
    double r_;
    double angle_;
};