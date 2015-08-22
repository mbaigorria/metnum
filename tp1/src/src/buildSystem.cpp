#include <iostream>
#include <math.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <new>
#include "eqsys.h"

using namespace std;

void insertValue(Matrix<double>& A, Matrix<double>& b, int j, int k, double r_i, double r_e, int n, int m, double* t_i, double* t_e);

int main(int argc, char** argv) {

	if (argc != 4) {
		printf("Usage: %s inputFile outputFile method (0: EG, 1: LU)\n", argv[0]);
		return 0;
	}

	ifstream inputFile(argv[1]);

	if (inputFile.bad()) {
		printf("Non-existant input file.\n");
		return 0;
	}

	// printf("Let's get started!\n");

	// granularity
	int n; // 0 = O0 < 0_k < ... < 0_n = 2PI
	int m; // ri = r0 < r_j < ... < r_m = re 

	// system parameters
	double r_i, r_e;

	double* t_i = new double[n];
	double* t_e = new double[n]; 

	double iso;
	int ninst; // instances of the problem to solve

	// load system parameters
	string line;
	getline(inputFile, line);
	sscanf(line.c_str(),"%lf %lf %d %d %lf %d",&r_i,&r_e,&m,&n,&iso,&ninst);

	cout << "r_i: " << r_i << " r_e: " << r_e << " m+1: " << m << " n: " << n << " iso: " << iso << " ninst: " << ninst << endl;

	// load temperatures (one instance for now)
	getline(inputFile, line);

 	char* buffer = strtok(strdup(line.c_str()), " ");

 	for (int i = 0; i < n; ++i) {
 		sscanf(buffer, "%lf", &t_i[i]);
 		buffer = strtok(NULL, " ");
 	}

 	for (int i = 0; i < n; ++i) {
 		sscanf(buffer, "%lf", &t_e[i]);
 		buffer = strtok(NULL, " ");
 	}

	inputFile.close();

	// build system: Ax = b
	Matrix<double> A(n*m,n*m,0);
	Matrix<double> b(n*m,1,0);

	// each temperature has 1 laplacian, and depends on 4 temperatures.
	// i'm looking for t_j,k in the valid range.
	
	// printf("Loading system!\n");
	for (int j = 0; j < m; j++) { // radius
		for (int k = 0; k < n; k++) { // angle
			insertValue(A,b,j,k,r_i,r_e,n,m,t_i,t_e);
		}
	}
	// printf("Finished loading system!\n");

	// A.printMatrix();

	EquationSystemLU<double> e(A);

	// // printf("Looking for solution!\n");
	Matrix<double> result(e.solve(b));
	// // printf("Solution found!\n");

	result.printMatrix();

	// A *= result;
	// A.printMatrix();

	delete[] t_i;
	delete[] t_e;

	return 0;
}

/* t_j,k
 * r0 < r_j < ... < r_m 
 * O0 < 0_k < ... < 0_n
 * b = | t1,0   | rows with fixed angle first.
 *     | ...... |
 *     | tm-1,0 |
 *     | ...... |
 *     | t1,n   |
 *     | ...... |
 *     | tm-1,n |
 */
void insertValue(Matrix<double>& A, Matrix<double>& b, int j, int k, double r_i, double r_e, int n, int m, double* t_i, double* t_e) {

	double dO = 2*M_PI / n;
	double dR = (r_e - r_i) / (m - 1);

	int r = j * n + k;
	double r_j = r_i + j*dR;

	// cout << "j: " << j << " k: " << k << " m: " << m << " n: " << n << " r: " << r << endl;

	if (j == 0) {
		A(r,r) = 1;
		b(r) = t_i[k];
		return;
	} else if (j == m - 1) {
		A(r,r) = 1;
		b(r) = t_e[k];
		return;
	}

	// t_j,k
	A(r,r) += - (2/pow(dR, 2)) + (1/(r_j*dR)) - (2/pow(r_j, 2)*pow(dO, 2));

	// t_j,k+1, border case! k > n, angle = 0
	A(r, j * n + (k+1 % n)) += 1/(pow(r_j, 2)*pow(dO, 2));

	// t_j,k-1, border case! k < 0 
	A(r, j * n + (k-1 % n)) += 1/(pow(r_j, 2)*pow(dO, 2));

	// t_j-1,k
	if (j == 1) { // inner circle
		b(r) -= t_i[k] * (1/pow(dR, 2) - 1/(r_j * dR));
	} else {
		A(r,(j-1) * n + k) += 1/pow(dR, 2) - 1/(r_j * dR);
	}
	
	// t_j+1,k
	if (j+1 == m-1) { // outer circle
		b(r) -= t_e[k] * (1/pow(dR, 2));
	} else {
		A(r, (j+1) * n + k) += (1/pow(dR, 2));
	}

}