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
        
        Matrix<T> solve(Matrix<T>& b_values);
        
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
    T coef;
    int i, j, k, l;
    
    // Armar la matriz lower
    lower = Matrix<T>(upper.rows(), upper.columns(), 0);
    
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
                        permutation = Matrix<T>(upper.rows(), upper.columns(), 0);
                        
                        for(l = 0; l < permutation.rows(); l++) {
                            permutation(l,l) = 1;
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
                    // Hacemos el producto para efectivamente permutar
                    upper = permutation * upper;
                    lower = permutation * lower;
                }
            }
            
            // Calculamos y guardamos el coeficiente
            // cout << upper(j,i) << " , " << upper(i,i) << endl;
            coef = upper(j, i) / upper(i, i);
            lower(j, i) = coef;
            // Colocamos cero en la columna bajo la diagonal
            //upper(j,i) = 0;
            for(k = i; k < upper.columns(); k++) {
                upper(j, k) = upper(j, k) - coef * upper(i, k);
            }
        }
    }
    // Agrego la diagonal de unos a lower
    for(i = 0; i < lower.rows(); i++){
        lower(i,i) = 1;
    }
}

template<class T>
Matrix<T> EquationSystemLU<T>::solve(Matrix<T>& b_values) {

    Matrix<T> temp_values = Matrix<T>(b_values);
    Matrix<T> y_values = Matrix<T>(b_values.rows());
    Matrix<T> x_values = Matrix<T>(b_values.rows());

    if(isPermutated) {
        temp_values = permutation * temp_values;
    }
    
    for(int i = 0; i < temp_values.rows(); i++) {
        for (int j = 0; j < i; j++) {
            temp_values(i) -= y_values(j) * lower(i,j);
        }
        if(i == 0) {
            y_values(0) = temp_values(0) / lower(0,0); // Calculo aparte el primer valor
        } else {
            y_values(i) = temp_values(i) / lower(i,i);
        }
    }

    temp_values = y_values;
    for(int i = temp_values.rows() - 1; i >= 0; i--) {
        for (int j = temp_values.rows() - 1; j > i; j--) {
            temp_values(i) -= x_values(j) * upper(i,j);
        }
        if(i == x_values.rows() - 1) {
            x_values(x_values.rows() - 1) = temp_values(temp_values.rows() - 1) / upper(upper.rows() - 1, upper.columns() - 1);
        } else {
            x_values(i) = temp_values(i) / upper(i,i);
        }
    }

    //if(isPermutated) {
    //    x_values = permutation * x_values;
    //}

    return x_values;

}



template<class T>
class EquationSystem{
    public:
        EquationSystem(const Matrix<T>& inicial);
        
        vector<T> solve(const vector<T>& values);
        
    private:
        Matrix<T> matrix;
};

#endif	/* EQSYS_H */

