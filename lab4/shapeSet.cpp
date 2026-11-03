#include "ShapeSet.h"
#include <ostream>
#include <stdexcept>
#include <utility>


ShapeSet::ShapeSet()
    : data_(nullptr), size_(0), capacity_(0) {
}

ShapeSet::ShapeSet(const ShapeSet& other)
    : data_(nullptr), size_(0), capacity_(0) {
    if (other.size_ > 0) {
        data_ = new Shape*[other.size_];
        capacity_ = other.size_;
        for (std::size_t i = 0; i < other.size_; ++i) {
            data_[i] = other.data_[i];
        }
        size_ = other.size_;
    }
}


ShapeSet::ShapeSet(ShapeSet&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

ShapeSet& ShapeSet::operator=(const ShapeSet& other) {
    if (this != &other) {
        ShapeSet tmp(other);
        std::swap(data_, tmp.data_);
        std::swap(size_, tmp.size_);
        std::swap(capacity_, tmp.capacity_);
    }
    return *this;
}

ShapeSet& ShapeSet::operator=(ShapeSet&& other) noexcept {
    if (this != &other) {
        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

ShapeSet::~ShapeSet() noexcept {
    clear();
    delete[] data_;
    data_ = nullptr;
}


bool ShapeSet::add(Shape* shape) {
    if (shape == nullptr) {
        return false;
    }
    if (indexOf(shape) != size_) {
        return false;
    }
    if (size_ == capacity_) {
        increase();
    }
    data_[size_] = shape;
    ++size_;
    return true;
}

bool ShapeSet::remove(std::size_t index) {
    if (index >= size_) {
        return false;
    }
    delete data_[index ];
    for (std::size_t i = index; i + 1 < size_; ++i) {
        data_[i] = data_[i + 1];
    }
    --size_;
    data_[size_] = nullptr;
    return true;
}

void ShapeSet::clear() noexcept {
    for (std::size_t i = 0; i < size_; ++i) {
        delete data_[i];
        data_[i] = nullptr;
    }
    size_ = 0;
}

std::size_t ShapeSet::size() const {
    return size_;
}

bool ShapeSet::empty() const {
    return size_ == 0;
}

Shape& ShapeSet::at(std::size_t index) {
    if (index >= size_) {
        throw std::out_of_range("ShapeSet::at: index out of range");
    }
    return *data_[index];
}

const Shape& ShapeSet::at(std::size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("ShapeSet::at: index out of range");
    }
    return *data_[index];
}

double ShapeSet::totalMetric() const {
    double total = 0.0;
    for (std::size_t i = 0; i < size_; ++i) {
        total += data_[i]->metric();
    }
    return total;
}

std::size_t ShapeSet::indexOf(const Shape* shape) const {
    for (std::size_t i = 0; i < size_; ++i) {
        if (data_[i] == shape) {
            return i;
        }
    }
    return size_;
}

void ShapeSet::increase() {
    std::size_t newCapacity = (capacity_ == 0) ? 1 : capacity_ * 2;
    Shape** newData = new Shape*[newCapacity];
    for (std::size_t i = 0; i < size_; ++i) {
        newData[i] = data_[i];
    }

    delete[] data_;
    data_ = newData;
    capacity_ = newCapacity;
}

std::ostream& operator<<(std::ostream& os, const ShapeSet& s) {
    os << "{ ";
    for (std::size_t i = 0; i < s.size_; ++i) {
        if (i > 0) {
            os << ", ";
        }
        os << *s.data_[i];
    }
    os << " }";
    return os;
}