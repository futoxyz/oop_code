#include "temperatureSet.h"
#include <iostream>
#include <utility>

TemperatureSet::TemperatureSet()
    : data_(nullptr), size_(0), capacity_(0) {}


TemperatureSet::TemperatureSet(const TemperatureSet& other)
    : data_(nullptr), size_(0), capacity_(0) {
    if (other.size_ > 0) {
        data_ = new Temperature[other.size_];
        capacity_ = other.size_;
        for (std::size_t i = 0; i < other.size_; ++i) {
            data_[i] = other.data_[i];
        }
        size_ = other.size_;
    }
}

TemperatureSet::TemperatureSet(TemperatureSet&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}


TemperatureSet& TemperatureSet::operator=(const TemperatureSet& other) {
    if (this != &other) {
        TemperatureSet tmp(other);
        swap(tmp);
    }
    return *this;
}

TemperatureSet& TemperatureSet::operator=(TemperatureSet&& other) noexcept {
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

TemperatureSet::~TemperatureSet() noexcept {
    delete[] data_;
    data_ = nullptr;
}

void TemperatureSet::swap(TemperatureSet& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

std::size_t TemperatureSet::indexOf(const Temperature& t) const {
    for (std::size_t i = 0; i < size_; ++i) {
        if (data_[i] == t) {
            return i;
        }
    }
    return size_;
}

void TemperatureSet::increase() {
    if (size_ < capacity_) {
        return;
    }
    const std::size_t newCapacity = (capacity_ == 0) ? 1 : capacity_ * 2;
    Temperature* newData = new Temperature[newCapacity];
    for (std::size_t i = 0; i < size_; ++i) {
        newData[i] = data_[i];
    }
    delete[] data_;
    data_ = newData;
    capacity_ = newCapacity;
}

bool TemperatureSet::add(const Temperature& t) {
    if (indexOf(t) != size_) {
        return false;
    }
    increase();
    data_[size_++] = t;
    return true;
}


bool TemperatureSet::remove(const Temperature& t) {
    const std::size_t idx = indexOf(t);
    if (idx == size_) {
        return false;
    }
    for (std::size_t i = idx; i + 1 < size_; ++i) {
        data_[i] = data_[i + 1];
    }
    --size_;
    return true;
}

bool TemperatureSet::contains(const Temperature& t) const {
    return indexOf(t) != size_;
}

bool TemperatureSet::operator()(const Temperature& t) const {
    return contains(t);
}

std::size_t TemperatureSet::size() const {
    return size_;
}

bool TemperatureSet::empty() const {
    return size_ == 0;
}

TemperatureSet& TemperatureSet::operator+=(const Temperature& t) {
    add(t);
    return *this;
}

TemperatureSet& TemperatureSet::operator-=(const Temperature& t) {
    remove(t);
    return *this;
}

TemperatureSet TemperatureSet::operator|(const TemperatureSet& other) const {
    TemperatureSet result(*this);
    for (std::size_t i = 0; i < other.size_; ++i) {
        result.add(other.data_[i]);
    }
    return result;
}

TemperatureSet TemperatureSet::operator&(const TemperatureSet& other) const {
    TemperatureSet result;
    for (std::size_t i = 0; i < size_; ++i) {
        if (other.contains(data_[i])) {
            result.add(data_[i]);
        }
    }
    return result;
}

TemperatureSet TemperatureSet::operator-(const TemperatureSet& other) const {
    TemperatureSet result;
    for (std::size_t i = 0; i < size_; ++i) {
        if (!other.contains(data_[i])) {
            result.add(data_[i]);
        }
    }
    return result;
}

bool TemperatureSet::operator==(const TemperatureSet& other) const {
    if (size_ != other.size_) {
        return false;
    }
    for (std::size_t i = 0; i < size_; ++i) {
        if (!other.contains(data_[i])) {
            return false;
        }
    }
    return true;
}

bool TemperatureSet::operator!=(const TemperatureSet& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const TemperatureSet& s) {
    os << "{ ";
    for (std::size_t i = 0; i < s.size_; ++i) {
        if (i > 0) {
            os << ", ";
        }
        os << s.data_[i];
    }
    if (s.size_ > 0) {
        os << " ";
    }
    os << "}";
    return os;
}