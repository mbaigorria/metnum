#include <iostream>
#include <math.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <new>
#include "SparseMatrix.h"

using namespace std;

SparseMatrix<double> rank(Matrix<double> M, double c, double d);

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

SparseMatrix<double> rank(Matrix<double> M, double c, double d) {
    //work work work (https://en.wikipedia.org/wiki/PageRank buen codigo en matlab, es simple, el laburo esta en parsear todo)
}

