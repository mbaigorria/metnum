#include <iostream>
#include <math.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include "eqsys.h"

#define INNER_TEMP 1500

using namespace std;

void insertValue(Matrix<double>& A, Matrix<double>& b, int j, int k, double r_i, double r_e, int n, int m, double t_e);

int main(int argc, char** argv) {

	if (argc != 4) {
		printf("Usage: %s inputFile outputFile method (0: EG, 1: LU)\n", argv[0]);
		return 0;
	}

	ifstream inputFile(argv[1]);

	if (!inputFile.is_open()) {
		printf("Non-existant input file.\n");
		return 0;
	}

	// granularity
	int n; // O0 < 0_k < ... < 0_n
	int m; // r0 < r_j < ... < r_m 

	// system parameters
	double r_i, r_e;

	double t_i[n];
	double t_e[n]; 

	double iso;
	int ninst; // instances of the problem to solve

	// load system parameters
	string line;
	getline(inputFile, line);
	istringstream ss(line);
	ss >> r_i >> r_e >> m >> n >> iso >> ninst;

	cout << "r_i: " << r_i << " r_e: " << r_e << " m: " << m << " n: " << n << " iso: " << iso << " ninst: " << ninst << endl;

	// load temperatures (one instance for now)
	getline(inputFile, line);

	ss.str(line);
	ss.clear();

	for (int i = 0; i < n; i++) {
		ss >> t_i[i];
	}

	for (int i = 0; i < n; i++) {
		ss >> t_e[i];
	}

	sleep(30);

	for (int i = 0; i < n; i++) {
		cout << t_i[i] << endl;
	}

	// // build system: Ax = b
	// Matrix<double> A((m-1)*(n+1),(m-1)*(n+1),0);
	// Matrix<double> b((m-1)*(n+1),1,0);

	// /* each temperature has 1 laplacian, and depends on 4 temperatures.
	//  * i'm looking for t_j,k in the valid range.
	//  */ 
	// for (int k = 0; k <= n; k++) {
	// 	for (int j = 1; j < m; j++) { // avoid borders
	// 		insertValue(A,b,j,k,r_i,r_e,n,m,t_e);
	// 	}
	// }

	// cout << endl;
	// cout << "Matrix A" << endl;
	// A.printMatrix();

	// cout << "Matrix b" << endl;
	// b.printMatrix();

	// EquationSystemLU<double> e(A);

	// Matrix<double> result(e.solve(b));

	// cout << "Matrix x" << endl;
	// result.printMatrix();

	// A *= result;
	// A.printMatrix();

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
void insertValue(Matrix<double>& A, Matrix<double>& b, int j, int k, double r_i, double r_e, int n, int m, double t_e) {

	// cout << "j: " << j << " k: " << k << " m: " << m << " n: " << n << endl;

	double dO = 2*M_PI / (n+1);
	double dR = (r_e - r_i) / m;

	int r = k * (m - 1) + (j - 1);
	double r_j = r_i + j*dR;

	// t_j,k
	A(r,r) += - (2/pow(dR, 2)) + (1/(r_j*dR)) - (2/pow(r_j, 2)*pow(dO, 2));

	// t_j,k+1, border case! k > n, angle = 0
	A(r, ((k+1) % (n+1)) * (m-1) + (j-1)) += 1/(pow(r_j, 2)*pow(dO, 2));

	// t_j,k-1, border case! k < 0 
	A(r, ((k+1) % (n+1)) * (m-1) + (j-1)) += 1/(pow(r_j, 2)*pow(dO, 2));

	// t_j-1,k
	if (j == 1) { // inner circle
		b(r) -= INNER_TEMP * (1/pow(dR, 2) - 1/(r_j * dR));
	} else {
		A(r,r - 1) += 1/pow(dR, 2) - 1/(r_j * dR);
	}
	
	// t_j+1,k
	if (j+1 == m) { // outer circle
		b(r) -= t_e * (1/pow(dR, 2));
	} else {
		A(r,r + 1) += (1/pow(dR, 2));
	}

}