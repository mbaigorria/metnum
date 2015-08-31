#include <iostream>
#include <math.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <new>
#include "eqsys.h"

using namespace std;

void load_a(Matrix<double>& A, double r_i, double r_e, int n, int m);
void load_b(Matrix<double>&b, double r_i, double r_e, int n, int m, double* t_i, double* t_e);
void insert_a(Matrix<double>& A, int j, int k, double r_i, double r_e, int n, int m);
void insert_b(Matrix<double>& b, int j, int k, double r_i, double r_e, int n, int m, double* t_i, double* t_e);
void load_temps(ifstream& inputFile, double* t_i, double* t_e, int n);
void save_result(Matrix<double>& m, FILE * pFile);
void generate_isotherm_lower(FILE * pFile, Matrix<double>& b, int m, int n, double r_i, double r_e, double iso);
void generate_isotherm_weighted(FILE * pFile, Matrix<double>& b, int m, int n, double r_i, double r_e, double iso);
int mod(int a, int b);

int main(int argc, char** argv) {

	if (argc < 4) {
		printf("Usage: %s inputFile outputFile method (0: EG, 1: LU) isoFile (optional)\n", argv[0]);
		return 0;
	}

	ifstream inputFile(argv[1]);

	if (!inputFile.good()) {
		printf("Non-existant input file.\n");
		return 0;
	}

	// granularity
	int n; // 0 = O0 < 0_k < ... < 0_n = 2PI
	int m; // ri = r0 < r_j < ... < r_m = re 

	double r_i, r_e;

	double iso;
	int ninst; // instances of the problem to solve

	string line;
	getline(inputFile, line);
	sscanf(line.c_str(),"%lf %lf %d %d %lf %d",&r_i,&r_e,&m,&n,&iso,&ninst);

	int solver = (int) (*argv[3] - '0');
	if (solver != 0 && solver != 1) {
		printf("Error: Invalid solver.\n");
		return 0;
	}

	cout << "r_i: " << r_i << " r_e: " << r_e << " m+1: " << m << " n: " << n << " iso: " << iso << " ninst: " << ninst << endl;
	cout << "inputFile: " << argv[1] << ", outputFile: " << argv[2] << ", method: " << argv[3] << endl;

	double t_i[n];
	double t_e[n];

 	FILE * pFile = fopen(argv[2],"w");

 	// build system: Ax = b
	Matrix<double> A(n*m,n*m,0);
	Matrix<double> b(n*m,1,0);
	FILE * pIsoFile = NULL;
	if (solver == 0) { // Gaussian Elimination
		load_a(A,r_i,r_e,n,m);
        clock_t before = clock();
		EquationSystemLU<double> e(A); //temp
		for (int j = 0; j < ninst; ++j) { // for every instance
			load_temps(inputFile, t_i, t_e, n);
 			load_b(b,r_i,r_e,n,m,t_i,t_e);

			Matrix<double> result(e.solve(b));
			save_result(result, pFile);
			if (argc == 5 && j == 0) {
				pIsoFile = fopen(argv[4],"w");
			}
			if (argc == 5) {
				// generate_isotherm_lower(pIsoFile, result, m, n, r_i, r_e, iso);
                generate_isotherm_weighted(pIsoFile, result, m, n, r_i, r_e, iso);
			}
		}
        clock_t result = clock() - before;
        cout << "method 0 takes: " << result/float(CLOCKS_PER_SEC) << " seconds" << endl;
	} else {
		load_a(A,r_i,r_e,n,m);
        clock_t before = clock();
		EquationSystemLU<double> e(A); //temp
		for (int j = 0; j < ninst; ++j) { // for every instance
			load_temps(inputFile, t_i, t_e, n);
 			load_b(b,r_i,r_e,n,m,t_i,t_e);
			Matrix<double> result(e.solve(b));
			save_result(result, pFile);
			if (argc == 5 && j == 0) {
				pIsoFile = fopen(argv[4],"w");
			}
			if (argc == 5) {
				generate_isotherm_lower(pIsoFile, result, m, n, r_i, r_e, iso);
				// generate_isotherm_weighted(pIsoFile, result, m, n, r_i, r_e, iso);
			}
		}
        clock_t result = clock() - before;
        cout << "method 1 takes: " << result/float(CLOCKS_PER_SEC) << " seconds" << endl;
	}

	inputFile.close();
	fclose(pFile);

	if (pIsoFile != NULL) fclose(pIsoFile);

	return 0;
}

void generate_isotherm_lower(FILE * pFile, Matrix<double>& b, int m, int n, double r_i, double r_e, double iso) {

	double dR = (r_e - r_i) / (m - 1);
	for (int k = 0; k < n; k++) {
		for (int j = 0; j < m; j++) {
			if ((b(j * n + k) <= iso || j == m-1) && j != 0) {
				fprintf(pFile, "%f\r\n", r_i + j*dR);
				break;
			} else if (b(j * n + k) <= iso && j == 0) {
				fprintf(pFile, "%f\r\n", r_i);
				break;
			}
		}
	}

}

void generate_isotherm_weighted(FILE * pFile, Matrix<double>& b, int m, int n, double r_i, double r_e, double iso) {

	double dR = (r_e - r_i) / (m - 1);

	for (int k = 0; k < n; k++) {
		for (int j = 0; j < m; j++) {
			if (b(j * n + k) < iso && j != m-1 && j != 0) {
				fprintf(pFile, "%f\r\n", r_i + (j-1)*dR + (b((j-1) * n + k) - b(j * n + k)) / iso * dR);
				break;
			} else if (b(j * n + k) < iso && j == 0) {
				fprintf(pFile, "%f\r\n", r_i);
				break;
			} else if (j == m-1) {
				fprintf(pFile, "%f\r\n", r_i + j*dR);
				break;
			}
		}
	}

}

void save_result(Matrix<double>& m, FILE * pFile) {
	
	if (pFile != NULL) {
		for (int i = 0; i < m.rows(); i++) {
			fprintf(pFile, "%1.6f\r\n", m(i));
		}
		// fprintf(pFile, "\r");
	} else {
		cout << "Failed to open file." << endl;
	}
}

void load_temps(ifstream& inputFile, double* t_i, double* t_e, int n) {
	string line;
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
}

void load_a(Matrix<double>& A, double r_i, double r_e, int n, int m) {
	for (int j = 0; j < m; j++) { // radius
		for (int k = 0; k < n; k++) { // angle
			insert_a(A,j,k,r_i,r_e,n,m);
		}
	}
}

void load_b(Matrix<double>&b, double r_i, double r_e, int n, int m, double* t_i, double* t_e) {
	for (int j = 0; j < m; j++) { // radius
		for (int k = 0; k < n; k++) { // angle
			insert_b(b,j,k,r_i,r_e,n,m,t_i,t_e);
		}
	}
}

/* Laplacian matrix helper function
 * r0 < r_j < ... < r_m 
 * O0 < 0_k < ... < 0_n
 */
void insert_a(Matrix<double>& A, int j, int k, double r_i, double r_e, int n, int m) {

	double dO = 2*M_PI / n;
	double dR = (r_e - r_i) / (m - 1);

	int r = j * n + k;
	double r_j = r_i + j*dR;

	if (j == 0) {
		A(r,r) = 1;
		return;
	} else if (j == m - 1) {
		A(r,r) = 1;
		return;
	}

	// t_j,k
	A(r,r) += (-2/pow(dR, 2)) + (1/(r_j*dR)) - 2/(pow(r_j, 2)*pow(dO, 2));

	// t_j,k+1, border case! k > n, angle = 0
	A(r, j * n + mod(k+1, n)) += 1/(pow(r_j, 2)*pow(dO, 2));

	// t_j,k-1, border case! k < 0 
	A(r, j * n + mod(k-1, n)) += 1/(pow(r_j, 2)*pow(dO, 2));

	// t_j-1,k
	if (j != 1) { // inner circle
		A(r,(j-1) * n + k) += 1/pow(dR, 2) - 1/(r_j * dR);
	}
	
	// t_j+1,k
	if (j+1 != m-1) { // outer circle
		A(r, (j+1) * n + k) += (1/pow(dR, 2));
	}

}

void insert_b(Matrix<double>& b, int j, int k, double r_i, double r_e, int n, int m, double* t_i, double* t_e) {

	double dR = (r_e - r_i) / (m - 1);

	int r = j * n + k;
	double r_j = r_i + j*dR;

	b(r) = 0;

	if (j == 0) {
		b(r) = t_i[k];
		return;
	} else if (j == m - 1) {
		b(r) = t_e[k];
		return;
	}

	if (j == 1) { // inner circle
		b(r) -= t_i[k] * (1/pow(dR, 2) - 1/(r_j * dR));
	}
	
	// t_j+1,k
	if (j+1 == m-1) { // outer circle
		b(r) -= t_e[k] * (1/pow(dR, 2));
	}

}

void insertValue(Matrix<double>& A, Matrix<double>& b, int j, int k, double r_i, double r_e, int n, int m, double* t_i, double* t_e) {

	double dO = 2*M_PI / n;
	double dR = (r_e - r_i) / (m - 1);

	int r = j * n + k;
	double r_j = r_i + j*dR;

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
	A(r,r) += (-2/pow(dR, 2)) + (1/(r_j*dR)) - 2/(pow(r_j, 2)*pow(dO, 2));

	// t_j,k+1, border case! k > n, angle = 0
	A(r, j * n + mod(k+1, n)) += 1/(pow(r_j, 2)*pow(dO, 2));

	// t_j,k-1, border case! k < 0 
	A(r, j * n + mod(k-1, n)) += 1/(pow(r_j, 2)*pow(dO, 2));

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

int mod(int a, int b) {
    int r = a % b;
    return r < 0 ? r + b : r;
}