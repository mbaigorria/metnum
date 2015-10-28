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

//webs / sports
Matrix<double> pageRank(Matrix<double>& M, double c, double d, vector<int>& nodesCount);
Matrix<double> enhancementPageRank(Matrix<double>& M, double c, double d, vector<int>& nodesCount);

//webs
void in_deg(vector<dataNode>& nodesCount);

//sports
void basic_sort(vector<matchesStats>& stats);

//out data
void saveResultPageRank(FILE * pFile, Matrix<double>& data);
void saveResultInDeg(FILE * pFile, vector<dataNode>& data);
void saveResultBasicSort(FILE * pFile, vector<matchesStats>& data);

//utils
double uniform_rand(double a, double b);

int main(int argc, char** argv) {

	if (argc < 3) {
		printf("Usage %s: parametros.in salida.out \n", argv[0]);
		return 0;
	}
	
	ifstream inputFile(argv[1]);

	if (!inputFile.good()) {
		printf("can't open input file.\n");
		return 0;
	}

    FILE * outputFile = NULL;
     
    outputFile = fopen(argv[2],"w");
    if (outputFile == NULL){
        printf("can't open output file.\n");
        return 0;
    }
	
	int alg = 0;
	double c = 0;
	int inst = 0;
	double e = 0;
	char testFileName[100];
	
	string line;
	getline(inputFile, line);

	sscanf(line.c_str(),"%d %lf %d %s %lf", &alg, &c, &inst, testFileName, &e);
			
	ifstream testFile(testFileName);
	getline(testFile, line);	  
  	
    if (inst == 0){
        int nodes = 0;
        int edges = 0;
    
	    sscanf(line.c_str(),"%d %d", &nodes, &edges);
        cout << "nodes: " << nodes << " edges: " << edges << endl;

      if (alg == 0){
          
          /*dok diccMatrix;
          
          rowCol p1(0,0);
          rowCol p2(0,1);
          rowCol p3(1,0);
          rowCol p4(1,1);
          
          diccMatrix.insert(dok::value_type(p2, 42));
          diccMatrix.insert(dok::value_type(p1, 55));
          diccMatrix.insert(dok::value_type(p4, 78));
          diccMatrix.insert(dok::value_type(p3, 100));
          
          SparseMatrix<double> B(diccMatrix, 2, 2);
          
          B.printSparseMatrix();*/
          
          
           Matrix<double> M(nodes, nodes);

           vector<int> nodesCount(nodes);
           
           for(int i = 0; i < nodes; i++) {
                nodesCount[i] = 0;
           }
           
           int i = 0;           
           while(i < 5){
                int node_from = 0;
                int node_to = 0;
                
	            getline(testFile, line);
                
                char * cstr = new char [line.length()+1];
                strcpy(cstr, line.c_str());

                char * p = strtok(cstr," ");
                  
                node_from = atoi(p);  
                p = strtok(NULL," ");
                node_to = atoi(p);

	            //cout << "node_from: " << node_from << " node_to: " << node_to << endl;
	            
	            nodesCount[node_from-1] += 1;
	            M(node_to-1, node_from-1) = 1;
                i++;
           }
                                
           Matrix<double> res = pageRank(M, c, e, nodesCount);
//           Matrix<double> res = enhancementPageRank(M, c, e, nodesCount); 
 
           cout <<  "page rank result: \n" << endl;           
           res.printMatrix();

           saveResultPageRank(outputFile, res);
        
        }else{
            // group algorithm webs
           vector<dataNode> nodesCount(nodes);
           
           for(int i = 0; i < nodes; i++){
                dataNode nod;
                nod.node = i+1;
                nod.edgesCount = 0;
                nodesCount[i] = nod;
           }
        
           int i = 0;           
           while(i < edges){
                int node_from = 0;
                int node_to = 0;
                
	            getline(testFile, line);
	            
	            char * cstr = new char [line.length()+1];
                strcpy(cstr, line.c_str());

                char * p = strtok(cstr," ");
                  
                node_from = atoi(p);  
                p = strtok(NULL," ");
                node_to = atoi(p);
	            
	            //cout << "node_from: " << node_from << " node_to: " << node_to << endl;
	            
	            dataNode nod = nodesCount[node_from-1];
	            
	            nod.edgesCount += 1;
	            
	            nodesCount[node_from-1] = nod;
	            
                i++;
           }
           
           in_deg(nodesCount);

           saveResultInDeg(outputFile, nodesCount);
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
	            getline(testFile, line);
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
           
           Matrix<double> res = pageRank(M, c, e, totalAbs); 
          
           cout <<  "gem result: \n" << endl; 
           res.printMatrix();
           
           saveResultPageRank(outputFile, res);
        }else{
            // group algorithm sports
            vector<matchesStats> stats(teams);

            for(int i = 0; i < teams; i++){
                matchesStats teamStats;
                teamStats.team = i+1;
                teamStats.matchesWin = 0;
                teamStats.matchesDefeat = 0;
                teamStats.pointsScored = 0;
                teamStats.pointsReceived = 0;
                stats[i] = teamStats;
            }

            int i = 0;           
            while(i < matches){
                getline(testFile, line);
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

            saveResultBasicSort(outputFile, stats);
        }
    }
    
    testFile.close();
    
    inputFile.close();

	if (outputFile != NULL) fclose(outputFile);
    
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
                M(i, j) = M(i, j) / (double)nodesCount[j];
            }else if(nodesCount[j] == 0){
                M(i, j) = 1/dbl_n; // dangling node / undefeated team
            }
            i++;
        }
        j++;
    }
    
    Matrix<double> v(n, 1/dbl_n);

    Matrix<double> E(n, n, (1 - c)*1/dbl_n); // PRE: rows == columns
    
    //Salvo que sea c = 1 no tiene sentido usar Sparse Matrix
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
    
    printf("delta is %f\r\n", delta);
    
    return x;
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
                M(i, j) = M(i, j) / (double)nodesCount[j];
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
