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
    //dataNode(int n) : node(n), edgesCount(0) {} // no lo pude compilar con listas de inicializacion...

    int node;
    int edgesCount;    
};

struct matchesStats {
    //matchesStats(int t) : team(t), matchesWin(0), matchesDefeat(0), pointsScored(0), pointsReceived(0) {}

    int team;
    int matchesWin;
    int matchesDefeat;
    int pointsScored;
    int pointsReceived;
};

//webs
Matrix<double> pageRank(Matrix<double>& M, double c, double d, vector<int>& nodesCount);
Matrix<double> enhancementPageRank(Matrix<double>& M, double c, double d, vector<int>& nodesCount);
void in_deg(vector<dataNode>& nodesCount);

//sports
Matrix<double> gem(Matrix<double>& M, double c, double d, vector<int>& totalAbs);
void basic_sort(vector<matchesStats>& stats);

//out data
void saveResultPageRank(FILE * pFile, Matrix<double>& data);
void saveResultInDeg(FILE * pFile, vector<dataNode>& data);
void saveResultBasicSort(FILE * pFile, vector<matchesStats>& data);

//utils
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
        if (pFile == NULL){
            printf("can´t open output file.\n");
            return 0;
        }
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
		  
  	string line;
	getline(inputFile, line);

    if (inst == 0){
        int nodes = 0;
        int edges = 0;
    
	    sscanf(line.c_str(),"%d %d", &nodes, &edges);
        cout << "nodes: " << nodes << " edges: " << edges << endl;

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
        int teams = 0;
        int matches = 0;
    
	    sscanf(line.c_str(),"%d %d", &teams, &matches);
        cout << "teams: " << teams << " matches: " << matches << endl;
    
        int day = 0;
        int local = 0;
        int visitor = 0;
        int local_score = 0;
        int visitor_score = 0;
    
        if (alg == 0){
            // page rank sports
           Matrix<double> M(teams, teams);
        
           vector<int> totalAbs(teams);
           
           for(int i = 0; i < teams; i++) {
                totalAbs[i] = 0;
           }
        
           int i = 0;           
           while(i < matches){
	            getline(inputFile, line);
	            sscanf(line.c_str(),"%d %d %d %d %d", &day, &local, &local_score, &visitor, &visitor_score);
	            
	            int abs_score = abs(local_score-visitor_score);
	            if (local_score > visitor_score) {
	                M(local-1, visitor-1) += abs_score;
	                totalAbs[visitor-1] += abs_score;       
	            }else if(visitor_score > local_score) {
	                M(visitor-1, local-1) += abs_score;
	                totalAbs[local-1] += abs_score;
	            }

                i++;
           }
           
           Matrix<double> res = gem(M, c, e, totalAbs); 
          
           cout <<  "gem result: \n" << endl; 
           res.printMatrix();
           
           if (argc == 7) {
               saveResultPageRank(pFile, res);
           }
        }else{
            // group algorithm sports
            vector<matchesStats> stats(teams);

            for(int i = 0; i < teams; i++){
                matchesStats teamStats;
                teamStats.team = i;
                teamStats.matchesWin = 0;
                teamStats.matchesDefeat = 0;
                teamStats.pointsScored = 0;
                teamStats.pointsReceived = 0;
                stats[i] = teamStats;
            }

            int i = 0;           
            while(i < matches){
                getline(inputFile, line);
                sscanf(line.c_str(),"%d %d %d %d %d", &day, &local, &local_score, &visitor, &visitor_score);

                matchesStats localStats = stats[local-1];
                matchesStats visitorStats = stats[visitor-1]; 

                if (local_score > visitor_score) {
                    localStats.matchesWin += 1;
                    visitorStats.matchesDefeat += 1;
                }else if(visitor_score > local_score) {
                    localStats.matchesDefeat += 1;
                    visitorStats.matchesWin += 1;
                }
                
                localStats.pointsScored += local_score;
                localStats.pointsReceived += visitor_score;
                visitorStats.pointsScored += visitor_score;
                visitorStats.pointsReceived += local_score;
                
                stats[local-1] = localStats;
                stats[visitor-1] = visitorStats;
                
                i++;
            }
            
            basic_sort(stats);
            
            if (argc == 7) {
                saveResultBasicSort(pFile, stats);
            }
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
                M(i, j) = 1/ (double)nodesCount[j];
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
                M(i, j) = 1/ (double)nodesCount[j];
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

Matrix<double> gem(Matrix<double>& M, double c, double d, vector<int>& totalAbs) {
    srand(45);

    int n = M.rows();
    double dbl_n = M.rows();

    int j = 0;
    while(j < n){
        int i = 0;
        while(i < n){
            if(M(i, j) != 0){
                M(i, j) = M(i, j) / (double)totalAbs[j];
            }else if(totalAbs[j] == 0){
                M(i, j) = 1/dbl_n; // undefeated team
            }
            i++;
        }
        j++;
    }
    
    Matrix<double> v(n, 1/dbl_n);

    Matrix<double> E(n, n, (1 - c)*1/dbl_n); // PRE: rows == columns

    Matrix<double> A = M*c + E;
     
    Matrix<double> x(n, 1, 1/dbl_n);

    //for (int i = 0; i < M.rows(); i++) {
    //    x(i) = uniform_rand(0, 1);
    //}    
    
    Matrix<double> last_x(n);

    double delta = 0;
   
    do {
        last_x = x;
        x = A*x;
        delta = x.L1(last_x);
    }while (delta > d);
    
    printf("delta is %f\r\n", delta); //Deberia devolverse.
    
    return x;
}

void basic_sort(vector<matchesStats>& stats) {
    sort(stats.begin(), stats.end(), [](matchesStats a, matchesStats b) {
        if (a.matchesWin - a.matchesDefeat != b.matchesWin - b.matchesDefeat) {
            return b.matchesWin - b.matchesDefeat < a.matchesWin - a.matchesDefeat;
        }else{
            return b.pointsScored - b.pointsReceived < a.pointsScored - a.pointsReceived;
        }   
    });

    cout << "basic sort result \n" << endl;
    for (matchesStats a : stats) {        
        cout << "team: " << a.team 
        << " matches win: " << a.matchesWin << " matches defeat: " << a.matchesDefeat 
        << " points scored: " << a.pointsScored << " points received: " << a.pointsReceived <<"\n";
    }   
}

void saveResultPageRank(FILE * pFile, Matrix<double>& data) {
    int n = data.rows();
    int i = 0;

    while(i < n){
        fprintf(pFile, "%f\r\n", data(i));
        i++;    
    }
}

void saveResultInDeg(FILE * pFile, vector<dataNode>& data) {  
    for (dataNode a : data){
        fprintf(pFile, "%d %d\r\n", a.node, a.edgesCount);   
    }
}

void saveResultBasicSort(FILE * pFile, vector<matchesStats>& data) {  
    for (matchesStats a : data){
        fprintf(pFile, "%d %d %d %d %d\r\n", a.team, a.matchesWin, a.matchesDefeat, a.pointsScored, a.pointsReceived);   
    }
}

double uniform_rand(double a, double b) {
    return ((b-a)*((double)rand()/RAND_MAX))+a;
}
