#include <iostream>
#include <math.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <new>
#include <regex>
#include <iterator>
#include "sparseMatrix.h"

using namespace std;

struct dataNode {
    int node;
    int edgesCount;
};

Matrix<double> pageRank(Matrix<double>& M, double c, double d, vector<int>& nodesCount);
Matrix<double> enhancementPageRank(Matrix<double>& M, double c, double d, vector<int>& nodesCount);
void in_deg(vector<dataNode>& nodesCount);
void saveResultPageRank(FILE * pFile, SparseMatrix<double> res);
void saveResultInDeg(FILE * pFile, vector<dataNode> data);
double uniform_rand(double a, double b);

const int number_line = 3;

int main(int argc, char** argv) {

	if (argc < 6) {
		printf("Usage: %s algorithm: 0 (pageRank) 1 (alternative) / teletransportation probability / instance type: 0 (web pages) 1 (deportes) / input file / tolerance / result (optional)\n", argv[0]);
		return 0;
	}
	
	ifstream inputFile(argv[4]);

	if (!inputFile.good()) {
		printf("Non-existant input file.\n");
		return 0;
	}

    FILE * pFile = NULL;
    
    if (argc == 7) {
        pFile = fopen(argv[6],"w");
    }
    
    cout << "alg: " << (*argv[1] - '0') << endl;
	int alg = (int) (*argv[1] - '0');
	if (alg != 0 && alg != 1) {
		printf("Error: Invalid algorithm.\n");
		return 0;
	}
	
	cout << "c: " << atof(argv[2]) << endl;
	double c = (double) atof(argv[2]);
	if (c <= 0 || c > 1) {
		printf("Error: Invalid teletransportation probability.\n");
		return 0;
	}
	
	cout << "inst: " << (*argv[3] - '0') << endl;
	int inst = (int) (*argv[3] - '0');
	if (inst != 0 && inst != 1) {
		printf("Error: Invalid instance type.\n");
		return 0;
	}

	cout << "tolerance: " << atof(argv[5]) << endl;
	double e = (double) atof(argv[5]);
	if (e <= 0 || e > 1) {
		printf("Error: Invalid tolerance.\n");
		return 0;
	}
		
    int nodes = 0;
    int edges = 0;
  
  	string line;
	getline(inputFile, line);
	sscanf(line.c_str(),"%d %d", &nodes, &edges);
 
    cout << "nodes: " << nodes << " edges: " << edges << endl;

    if (inst == 0){
        if (alg == 0){
           Matrix<double> M(nodes, nodes);
        
           vector<int> nodesCount(nodes);
           
           for(int i = 0; i < nodes; i++) {
                nodesCount[i] = 0;
           }
        
           int i = 0;           
           while(i < edges){
                int node_from = 0;
                int node_to = 0;
                
	            getline(inputFile, line);
	            sscanf(line.c_str(),"%d %d", &node_from, &node_to);
	            //cout << "node_from: " << node_from << " node_to: " << node_to << endl;
	            
	            nodesCount[node_from-1] += 1;
	            M(node_to-1, node_from-1) = 1;
                i++;
           }
                                
           Matrix<double> res = pageRank(M, c, e, nodesCount);
//           Matrix<double> res = enhancementPageRank(M, c, e, nodesCount); 
 
           cout <<  "page rank result: \n" << endl;           
           res.printMatrix();
             
            if (argc == 7) {
                saveResultPageRank(pFile, res);
            }
            
        }else{
            // group algorithm webs
           vector<dataNode> nodesCount(nodes);
           
           for(int i = 0; i < nodes; i++){
                dataNode nod;
                nod.node = i;
                nod.edgesCount = 0;
                nodesCount[i] = nod;
           }
        
           int i = 0;           
           while(i < edges){
                int node_from = 0;
                int node_to = 0;
                
	            getline(inputFile, line);
	            sscanf(line.c_str(),"%d %d", &node_from, &node_to);
	            //cout << "node_from: " << node_from << " node_to: " << node_to << endl;
	            
	            dataNode nod = nodesCount[node_from-1];
	            
	            nod.edgesCount += 1;
	            
	            nodesCount[node_from-1] = nod;
	            
                i++;
           }
           
           in_deg(nodesCount);
           
           if (argc == 7) {
                saveResultInDeg(pFile, nodesCount);
           }
        }
    }else{
        if (alg == 0){
            // page rank sports
        }else{
            // group algorithm sports
        }
    }
    
    inputFile.close();

	if (pFile != NULL) fclose(pFile);
    
    //M.printMatrix();
    
    //Depending on input data, create a matrix with the input file and call rank with matrix a values

	return 0;
}

Matrix<double> pageRank(Matrix<double>& M, double c, double d, vector<int>& nodesCount) {
    srand(45);

    int n = M.rows();
    double dbl_n = M.rows();

    int j = 0;
    while(j < n){
        int i = 0;
        while(i < n){
            if(M(i, j) != 0){
                M(i, j) = 1/ (float)nodesCount[j];
            }else if(nodesCount[j] == 0){
                M(i, j) = 1/dbl_n; // dangling node
            }
            i++;
        }
        j++;
    }
    
    SparseMatrix<double> v(n, 1/dbl_n);

    Matrix<double> E(n, n, (1 - c)*1/dbl_n); // PRE: rows == columns

    Matrix<double> M_hat = M*c + E;

    SparseMatrix<double> A(M_hat);
     
    SparseMatrix<double> x(n, 1/dbl_n);
    
    //for (int i = 0; i < M.rows(); i++) {
    //    x(i) = uniform_rand(0, 1);
    //}    
    
    SparseMatrix<double> last_x(n);

    double delta = 0;
   
    do {
        last_x = x;
        x = A*x;
        delta = x.L1(last_x);
    }while (delta > d);
    
    printf("delta is %f\r\n", delta); //Deberia devolverse.
    
    return x.descompress();
}

Matrix<double> enhancementPageRank(Matrix<double>& M, double c, double d, vector<int>& nodesCount) {
    srand(45);

    int n = M.rows();
    double dbl_n = M.rows();
    
    int j = 0;
    while(j < n){
        int i = 0;
        while(i < n){
            if(M(i, j) != 0){
                M(i, j) = 1/ (float)nodesCount[j];
            }
            i++;
        }
        j++;
    }
    
    SparseMatrix<double> A(M);
     
    SparseMatrix<double> x(n, 1/dbl_n);
    
    //for (int i = 0; i < M.rows(); i++) {
    //    x(i) = uniform_rand(0, 1);
    //}    
    
    SparseMatrix<double> last_x(n);

    SparseMatrix<double> v(n, 1/dbl_n);
    
    double delta = 0;
   
    do {
        last_x = x;
       
        x = A*x;
        x = x*c;
 
        double w = last_x.norm1() - x.norm1();    

        x = x +  v*w;
        
        delta = x.L1(last_x);
    }while (delta > d);
    
    printf("delta is %f\r\n", delta); //Deberia devolverse.
    
    return x.descompress();
}

void in_deg(vector<dataNode>& nodesCount) {
    sort(nodesCount.begin(), nodesCount.end(), [](dataNode a, dataNode b) {
        return b.edgesCount < a.edgesCount;   
    });

    cout << "IN-DEG result \n" << endl;
    for (dataNode a : nodesCount) {        
        cout << "node: " << a.node << " points: " << a.edgesCount << "\n";
    }   
}

void saveResultPageRank(FILE * pFile, SparseMatrix<double> res) {
    int n = res.rows();
    int i = 0;
    
    while(i < n){
        fprintf(pFile, "%f\r\n", res(i));
        i++;    
    }
}

void saveResultInDeg(FILE * pFile, vector<dataNode> data) {  
    for (dataNode a : data){
        fprintf(pFile, "%d %d\r\n", a.node, a.edgesCount);   
    }
}

double uniform_rand(double a, double b) {
    return ((b-a)*((double)rand()/RAND_MAX))+a;
}
