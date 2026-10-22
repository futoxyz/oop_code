#pragma once
#include <cstddef>
#include <iosfwd>

#include "Temperature.h"

// Множество температур (вариант 13): unsorted, линейный поиск,
// дубликаты определяются после канонизации (через Temperature::operator==,
// то есть через эпсилон), remove — сдвигом с сохранением порядка,
// рост capacity — удвоением (0 -> 1 -> 2 -> 4 -> ...).
class TemperatureSet {
public:
    TemperatureSet();
    TemperatureSet(const TemperatureSet& other);
    TemperatureSet(TemperatureSet&& other) noexcept;
    TemperatureSet& operator=(const TemperatureSet& other);
    TemperatureSet& operator=(TemperatureSet&& other) noexcept;
    ~TemperatureSet();

    bool add(const Temperature& t);            // true — множество изменилось
    bool remove(const Temperature& t);         // true — элемент был и удалён
    bool contains(const Temperature& t) const;
    bool operator()(const Temperature& t) const;  // принадлежность

    std::size_t size() const;
    bool empty() const;

    TemperatureSet& operator+=(const Temperature& t);
    TemperatureSet& operator-=(const Temperature& t);

    TemperatureSet operator|(const TemperatureSet& other) const;  // объединение
    TemperatureSet operator&(const TemperatureSet& other) const;  // пересечение
    TemperatureSet operator-(const TemperatureSet& other) const;  // разность

    bool operator==(const TemperatureSet& other) const;
    bool operator!=(const TemperatureSet& other) const;

    void swap(TemperatureSet& other) noexcept;

    friend std::ostream& operator<<(std::ostream& os, const TemperatureSet& s);

private:
    // Индекс элемента или size_, если не найден. Линейный поиск через
    // Temperature::operator== (сравнение с эпсилон).
    std::size_t indexOf(const Temperature& t) const;

    // Гарантирует место под ещё один элемент (рост удвоением).
    void growIfFull();

    Temperature* data_;
    std::size_t size_;
    std::size_t capacity_;
};