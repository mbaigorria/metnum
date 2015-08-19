#include <algorithm>
#include <iostream>
#include <math.h>
#include "matrix.h"
#include "eqsys.h"

using namespace std;

// La matriz respeta la notacion convencional, es decir, el primer subindice 
// es la fila y el segundo es la columna

int main() {
    Matrix<double> a(3,3);
    Matrix<double> b(3);
    
    a(0,0) = 1;
    a(0,1) = 3;
    a(0,2) = 5;
    a(2,0) = 1;
    a(2,1) = 3;
    a(2,2) = 7;
    a(1,0) = 2;
    a(1,1) = 5;
    a(1,2) = 6;

    b(0,0) = 1;
    b(1,0) = 2;
    b(2,0) = 3;
    
    a.printMatrix();
    b.printMatrix();

    EquationSystemLU<double> e(a);
    
    Matrix<double> c(e.solve(b));
    c.printMatrix();

    a *= c;

    a.printMatrix();

    return 0;
}