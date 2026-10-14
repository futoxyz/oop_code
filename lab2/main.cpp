#include "matrix.h"
#include <iostream>

int main() {
    using namespace matrix;
    Matrix a(2,2,1);
    a.set(2,1,8.1131231231300054);
    a.set(2,2,3);
    a.pow(5).print();
    Matrix res = diag_pow(a, 5);
    res.print();
}
