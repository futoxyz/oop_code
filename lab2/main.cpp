#include "matrix.h"
#include <iostream>



int main() {
    using namespace matrix;
    Matrix m();
    std::size_t rows = 0;
    std::size_t cols = 0;

    int option = 0;
    while (true) {
        option = 0;
        std::cout << "\nCommands\n"
                  << "1. Create matrix\n"
                  << "2. Fill matrix\n"
                  << "3. Print matrix\n"
                  << "4. Max row array/min col array\n"
                  << "0. Exit\n"
                  << ": ";

        if (!(std::cin >> option)) {
            std::cout << "Invalid input\n";
            std::cin.clear();
            return 0;
        }

        if (option == 0) {
            std::cin.clear();
            return 0;
        }

        if (option != 1 && option < 5 && !m) {
            std::cout << "Matrix is not created\n";
            continue;
        }

        switch (option) {
            case 1: {
                if (m) {
                    std::cout << "Matrix is already created\n";
                    break;
                }
                std::cout << "Rows amount: ";
                if (!(std::cin >> rows) || rows == 0) {
                    std::cout << "Wrong input\n";
                    rows = 0;
                    break;
                }
                std::cout << "Cols amount: ";
                if (!(std::cin >> cols) || cols == 0) {
                    std::cout << "Wrong input\n";
                    rows = cols = 0;
                    break;
                }


                if (m) {
                    std::cout << "Matrix " << rows << "x" << cols << " was created\n";
                } else {
                    std::cout << "Memory allocation failed\n";
                    rows = cols = 0;
                }
                break;
            }
            case 2: {
                int val = 0;
                std::cout << "Enter value: ";
                if (!(std::cin >> val)) {
                    std::cout << "Wrong input\n";
                    break;
                }

                std::cout << "Successfully filled with " << val << ".\n";
                break;
            }
            default:
                std::cout << "Wrong input\n";
                break;
        }
    }
    return 0;
}