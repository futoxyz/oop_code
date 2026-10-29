#pragma once
#include <cstddef>
#include <iosfwd>

#include "Temperature.h"


class TemperatureSet {
public:
    TemperatureSet();
    TemperatureSet(const TemperatureSet& other);
    TemperatureSet(TemperatureSet&& other) noexcept;
    TemperatureSet& operator=(const TemperatureSet& other);
    TemperatureSet& operator=(TemperatureSet&& other) noexcept;
    virtual ~TemperatureSet() noexcept;

    bool add(const Temperature& t);
    bool remove(const Temperature& t);
    bool contains(const Temperature& t) const;

    std::size_t size() const;
    bool empty() const;

    TemperatureSet& operator+=(const Temperature& t);
    TemperatureSet& operator-=(const Temperature& t);

    TemperatureSet operator|(const TemperatureSet& other) const;
    TemperatureSet operator&(const TemperatureSet& other) const;
    TemperatureSet operator-(const TemperatureSet& other) const;

    bool operator==(const TemperatureSet& other) const;
    bool operator!=(const TemperatureSet& other) const;

    void swap(TemperatureSet& other) noexcept;

    friend std::ostream& operator<<(std::ostream& os, const TemperatureSet& s);

private:
    std::size_t indexOf(const Temperature& t) const;
    void increase();
    Temperature* data_;
    std::size_t size_;
    std::size_t capacity_;
};