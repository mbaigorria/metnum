/* 
 * File:   eqsys.h
 * Author: Federico
 *
 * Created on August 17, 2015, 5:57 PM
 */

#ifndef EQSYS_H
#define	EQSYS_H

#include <algorithm>
#include <math.h>
#include <vector>
#include "matrix.h"

template<class T>
class EquationSystemLU {
    public:
        EquationSystemLU(const Matrix<T>& inicial);
        
        //vector<T> solve(const vector<T>& values);
        
    private:
        Matrix<T> lower;
        Matrix<T> upper;
        bool isPermutated;
        Matrix<T> permutation;
};

template<class T>
EquationSystemLU<T>::EquationSystemLU(const Matrix<T>& inicial)
    : upper(inicial), isPermutated(false)
{
    int coef;
    int i, j, k, l, m;
    
    // Armar la matriz lower con la diagonal en uno
    lower = Matrix<T>(upper.rows(), upper.columns());
    for(i = 0; i < lower.rows(); i++){
        for(j = 0; j < lower.columns(); j++) {
            if(i == j) {
                lower(i,j) = 1;
            } else {
                lower(i,j) = 0;
            }
        }
    }
    
    for(i = 0; i < upper.columns(); i++) {
        for(j = i + 1; j < upper.rows(); j++) {
            if(upper(i, i) == 0) {
                // Hay que buscar la proxima fila sin cero
                for(k = i + 1; k < upper.rows(); k++) {
                    if(upper(k, i) != 0) {
                        break;
                    }
                }
                
                if(k == upper.rows()) { // No hay filas para permutar
                    abort();
                } else {
                    if(!isPermutated){
                        // Generamos la matriz de permutacion con uno en la diagonal
                        isPermutated = true;
                        permutation = Matrix<T>(upper.rows(), upper.columns());
                        
                        for(l = 0; l < permutation.rows(); l++) {
                            for(m = 0; m < permutation.columns(); m++) {
                                if(l == m) {
                                    permutation(l,m) = 1;
                                } else {
                                    permutation(l,m) = 0;
                                }
                            }
                        }
                    }
                    // Permutamos las filas
                    for(l = 0; l < permutation.columns(); l++) {
                        if(l == k) {
                            permutation(i, l) = 1;
                        } else {
                            permutation(i, l) = 0;
                        }
                        if(l == i) {
                            permutation(k, l) = 1;
                        } else {
                            permutation(k, l) = 0;
                        }                        
                    }
                    // Hacemos el producto para efectivamente permutar upper
                    upper = permutation * upper;
                }
            }
            
            // Calculamos y guardamos el coeficiente
            coef = upper(j, i)/upper(i, i);
            lower(j, i) = coef;

            // Colocamos cero en la columna bajo la diagonal
            for(k = i; k < upper.columns(); k++) {
                upper(j, k) = upper(j, k) - coef * upper(i, k);
            }
        }
    }
    // COSAS PARA TESTEAR NOMAS, IGNORAR
    
    upper.printMatrix();
    lower.printMatrix();

    lower *= upper;
    lower.printMatrix();
}

template<class T>
class EquationSystem{
    public:
        EquationSystem(const Matrix<T>& inicial);
        
        vector<T> solve(const vector<T>& values);
        
    private:
        Matrix<T> matriz;
};

#endif	/* EQSYS_H */

