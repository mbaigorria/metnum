#include <iostream>
#include <math.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <new>
#include "sparseMatrix.h"

using namespace std;

SparseMatrix<double> rank(Matrix<double>& M, double c, double d);
float rand_FloatRange(float a, float b);

int main(int argc, char** argv) {

	if (argc < 5) {
		printf("Usage: %s algorithm: 0 (pageRank) 1 (alternative) / teletransportation probability / instance type: 0 (web pages) 1 (deportes) / input file / result (optional)\n", argv[0]);
		return 0;
	}

	ifstream inputFile(argv[3]);

	if (!inputFile.good()) {
		printf("Non-existant input file.\n");
		return 0;
	}

    //Depending on input data, create a matrix with the input file and call rank with matrix a values

	return 0;
}

SparseMatrix<double> rank(Matrix<double>& M, double c, double d) {
    //work work work (https://en.wikipedia.org/wiki/PageRank buen codigo en matlab, es simple, el laburo esta en parsear todo)
    srand(45);

    double n = M.rows();
    Matrix<double> E(M.rows(), M.rows(), (1 - c)*1/n); // PRE: rows == columns
    Matrix<double> M_hat = M*c - E;
    SparseMatrix<double> A(M_hat);
    SparseMatrix<double> x(M.rows());
    
    for (int i = 0; i < M.rows(); i++) {
        x(i) = uniform_rand(0, 1);
    }
    
    SparseMatrix<double> last_x(rows());
    
    double delta = 0;
    
    do {
        last_x = x;
        x = A*x;
        delta = x.L1(last_x);
    }while (delta > d);
    
    printf("delta is %3.3f ", delta); //Deberia devolverse.
    
    return x;
}

double uniform_rand(double a, double b) {
    return ((b-a)*((double)rand()/RAND_MAX))+a;
}

