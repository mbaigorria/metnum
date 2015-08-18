#include <iostream>

using namespace std;

void insertValue(double* A, double* b, int j, int k, double r_i, double r_e, int m, int n);

int main() {

	int n = 3; // O0 < 0_k < ... < 0_n
	int m = 3; // r0 < r_j < ... < r_m 
	double t_e = 1000;

	// build system: Ax = b
	double A[(m-1)*(n+1)][(m-1)*(n+1)] = {0};
	double b[(m-1)*(n+1)] = {0};

	/* each temperature has 1 laplacian, and depends on at least 3 temperatures.
	 * i'm looking for t_j,k in the valid range.
	 */ 
	for(int k = 0; k <= n; k++) {
		for (int j = 1; j < m; j++) { // avoid borders
			insertValue(A,b,j,k,r_i,r_e,m,n, t_e);
		}
	}

}

/* t_j,k
 * j: radius
 * k: angle
 * b = | t1,0   | rows with fixed angle first.
 *     | ...... |
 *     | tm-1,0 |
 *     | ...... |
 *     | t1,n   |
 *     | ...... |
 *     | tm-1,n |
 */
void insertValue(double* A, double* b, int j, int k, double r_i, double r_e, int m, int n, double t_e) {

	double dO = 360 / n;
	double dR = (r_e - r_i) / k;
	int r = (j-1) * (n+1);
	double r_j = r_i + j*dR;

	A[r][k*m + (j-1)]     = - (2/pow(dR, 2)) + (1/(r_j*dR)) - (2/pow(r_j, 2)*pow(dO, 2)); // t_j,k

	A[r][k*m + (j-1) + 1] = (1/pow(dR, 2)); // t_j+1,k

	A[r][((k-1) % (n+1)) * m + (j-1)] = (1/(pow(r_j, 2)*pow(dO, 2))); // t_j,k-1, border case! k < 0

	if (k == 1) { // inner circle
		A[r][(j-1)*n + j - 1] = (1/pow(dR, 2)) + 1 / (r_j * dR); // t_j-1,k
		b[r] -= 1/pow(dR,2) * 1500;

	} else if (k == m) { // outer circle
		A[r][(j-1)*n + j - 1] = (1/pow(dR, 2)) + 1 / (r_j * dR); // t_j-1,k
		b[r] -= 1/pow(dR,2) * t_e;

	} else {
		A[r][(j-1)*n + j - 1] = 1 / (r_j * dR); // t_j-1,k
	}



}