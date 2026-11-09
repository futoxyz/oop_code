#pragma once
#include <cstddef>
#include <iosfwd>
#include "shape.h"


class ShapeSet {
public:
    ShapeSet();
    ShapeSet(const ShapeSet& other);
    ShapeSet(ShapeSet&& other) noexcept;
    ShapeSet& operator=(const ShapeSet& other);
    ShapeSet& operator=(ShapeSet&& other) noexcept;
    virtual ~ShapeSet() noexcept;

    bool add(Shape* shape);
    bool remove(std::size_t index);
    void clear() noexcept;
    std::size_t size() const;
    bool empty() const;
    Shape& at(std::size_t index);
    const Shape& at(std::size_t index) const;
    double total_area() const;
    friend std::ostream& operator<<(std::ostream& os, const ShapeSet& s);

private:
    std::size_t indexOf(const Shape* shape) const;
    void increase();
    Shape** data_;
    std::size_t size_;
    std::size_t capacity_;
};