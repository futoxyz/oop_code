#include <iostream>
#include <cctype>

#include "Temperature.h"
#include "TemperatureSet.h"


bool parseUnit(char ch, TempUnit& unit) {
    switch (std::toupper(static_cast<unsigned char>(ch))) {
        case 'C': unit = TempUnit::C; return true;
        case 'F': unit = TempUnit::F; return true;
        case 'K': unit = TempUnit::K; return true;
        default: return false;
    }
}

bool readTemperature(Temperature& out) {
    double value = 0.0;
    char unitChar = 0;
    std::cout << "Enter temperature with a unit: ";
    if (!(std::cin >> value >> unitChar)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Bad input\n";
        return false;
    }
    TempUnit unit;
    if (!parseUnit(unitChar, unit)) {
        std::cout << "Bad unit";
            return false;
    }
    out = Temperature(value, unit);
    return true;
}

void printMenu() {
    std::cout << "\nTemperature Set\n"
              << "1. Add temperature\n"
              << "2. Delete temperature\n"
              << "3. Contains check\n"
              << "4. Print\n"
              << "5. Clear\n"
              << "0. Exit\n"
              << ": ";
}


int main() {
    TemperatureSet set;
    int choice = -1;

    while (true) {
        printMenu();
        if (!(std::cin >> choice)) {
            if (std::cin.eof()) {
                break;
            }
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Bad input\n";
            continue;
        }

        Temperature t;
        switch (choice) {
            case 1:
                if (readTemperature(t)) {
                    if (set.add(t)) {
                        std::cout << "Added: " << t << "\n";
                    } else {
                        std::cout << "Temperature is already in set (" << t << ")\n";
                    }
                }
                break;
            case 2:
                if (readTemperature(t)) {
                    if (set.remove(t)) {
                        std::cout << "Deleted: " << t << "\n";
                    } else {
                        std::cout << "Temperature is not in set\n";
                    }
                }
                break;
            case 3:
                if (readTemperature(t) && set.contains(t)) {
                    std::cout << "Temperature is in set\n";
                }
                break;
            case 4:
                std::cout << "Size: " << set.size() << "\n"
                          << set << "\n";
                break;
            case 5:
                set = TemperatureSet();
                std::cout << "Set was cleared\n";
                break;
            case 0:
                return 0;
            default:
                std::cout << "Bad input\n";
        }
    }
    return 0;
}