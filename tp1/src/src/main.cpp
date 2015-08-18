#include <algorithm>
#include <iostream>
#include <math.h>
#include "matrix.h"
#include "eqsys.h"

using namespace std;

// La matriz respeta la notacion de la catedra, es decir, el primer subindice 
// es la fila y el segundo es la columna

int main() {
    Matrix<int> a(3,3);
    
    a(0,0) = 1;
    a(0,1) = 3;
    a(0,2) = 5;
    a(1,0) = 2;
    a(1,1) = 6;
    a(1,2) = 10;
    a(2,0) = 1;
    a(2,1) = 4;
    a(2,2) = 7;
    
    for(int i = 0; i < a.filas(); i++) {
        for(int j = 0; j < a.columnas(); j++) {
            cout << a(i,j) << " ";
        }
        cout << endl;
    }
    cout << endl;

    EquationSystemLU<int> e(a);
    

}