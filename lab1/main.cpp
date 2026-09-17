#include "matrix_ops.h"
#include <iostream>


int main() {
    using namespace matrix_ops;
    int** m = nullptr;
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
        continue;
        }

        if (option == 0) {
            if (m) {
                matrix_delete(m, rows);
            }
            break;
        }

        if (option != 1 && !m) {
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

                m = matrix_create(rows, cols);
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
                matrix_fill(m, rows, cols, val);
                std::cout << "Successfully filled with " << val << ".\n";
                break;
            }
            case 3: {
                std::cout << "\n";
                matrix_print(m, rows, cols);
                break;
            }
            case 4: {
                int* maxes = matrix_row_max(m, rows, cols);
                int* mins = matrix_col_min(m, rows, cols);

                if (maxes) {
                    std::cout << "Max in rows: ";
                    for (std::size_t i = 0; i < rows; ++i) {
                        std::cout << maxes[i] << " ";
                    }
                    std::cout << "\n";
                    delete[] maxes;
                }

                if (mins) {
                    std::cout << "Min in cols: ";
                    for (std::size_t j = 0; j < cols; ++j) {
                        std::cout << mins[j] << " ";
                    }
                    std::cout << "\n";
                    delete[] mins;
                }
                break;
            }
            default:
                std::cout << "Wrong input\n";
                break;
        }
    }
    return 0;
}
