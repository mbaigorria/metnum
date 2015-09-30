#include <iostream>
#include <math.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <new>
#include "sparseMatrix.h"

using namespace std;

int main(int argc, char** argv) {

    SparseMatrix<double> x(3);
    cout << "vector x " << endl;
    x(0) = 2;
    x(1) = 3;
    x(2) = 4;
    x.printSparseMatrix();
    
    cout << "matrix " << endl;
    Matrix<double> matrizNormal(3,3,0);
    matrizNormal(0,0) = 10;
    matrizNormal(1,2) = 5;
    matrizNormal(0,2) = 3;
    matrizNormal(2,2) = 8;
    matrizNormal.printMatrix();
    
    cout << "sparse matrix " << endl;
    SparseMatrix<double> matrizSparsa(matrizNormal);
    matrizSparsa.printSparseMatrix();
    
    cout << "b = sparse matrix * x " << endl;
    SparseMatrix<double> b = matrizSparsa*x;
    
    b.printSparseMatrix();
    
    cout << "norma manhattan b - x = " << b.L1(x) << endl;
    
    return 0;
}
