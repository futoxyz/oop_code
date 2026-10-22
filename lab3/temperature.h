#include <cstddef>
#pragma once
#include <iosfwd>

// Единицы измерения температуры.
enum class TempUnit {
    Celsius,
    Fahrenheit,
    Kelvin
};

// Температура с канонизацией: внутри всегда хранится значение в кельвинах.
// Канонизация выполняется в конструкторе, поэтому она идемпотентна:
// Temperature(t.in(Kelvin), Kelvin) == t.
class Temperature {
public:
    Temperature();                           // 0 °C (273.15 K)
    Temperature(double value, TempUnit unit);

    // Копирование/перемещение/деструктор тривиальны (нет владения памятью),
    // поэтому компилятор генерирует их корректно.
    Temperature(const Temperature&) = default;
    Temperature(Temperature&&) noexcept = default;
    Temperature& operator=(const Temperature&) = default;
    Temperature& operator=(Temperature&&) noexcept = default;
    ~Temperature() = default;

    // Значение в канонической единице (кельвины).
    double kelvin() const;

    // Значение, пересчитанное в заданную единицу.
    double in(TempUnit unit) const;

    // Сравнение выполняется по каноническому значению с допуском kEpsilon.
    bool operator==(const Temperature& other) const;
    bool operator!=(const Temperature& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Temperature& t);

    // Допуск при сравнении вещественных значений (в кельвинах).
    static constexpr double eps_ = 1e-9;

private:
    static double toKelvin(double value, TempUnit unit);
    static double fromKelvin(double kelvin, TempUnit unit);

    double kelvin_;  // каноническое значение
};
