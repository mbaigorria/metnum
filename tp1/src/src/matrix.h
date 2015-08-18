/* 
 * File:   matrix.h
 * Author: Federico
 *
 * Created on August 16, 2015, 9:54 PM
 */

#ifndef MATRIX_H
#define	MATRIX_H

#include <algorithm>
#include <math.h>
#include <vector>

using namespace std;

// La matriz respeta la notacion de la catedra, es decir, el primer subindice 
// es la fila y el segundo es la columna

template<class T>
class Matrix {
    public:
        Matrix();
        Matrix(int filas, int col);
        Matrix(const Matrix<T>& other);
        ~Matrix();
        
        Matrix<T>& operator=(const Matrix<T>& other);
        Matrix<T> operator*(const Matrix<T>& other);
        Matrix<T>& operator*=(const Matrix<T>& other);
        Matrix<T> operator+(const Matrix<T>& other);
        Matrix<T>& operator+=(const Matrix<T>& other);
        Matrix<T> operator-(const Matrix<T>& other);
        Matrix<T>& operator-=(const Matrix<T>& other);
        
        Matrix<T> operator*(const T& scalar);
        Matrix<T> operator/(const T& scalar);
        
        T& operator()(int a, int b);
        const T& operator()(const int a, const int b) const;
        int filas();
        int columnas();
        
    private:
        vector<vector<T> > datos;
        int cantF;
        int cantC;
        
};

template<class T>
Matrix<T>::Matrix()
    : cantF(1), cantC(1)
{
    datos.resize(1);
    datos[0].resize(1);
}

template<class T>
Matrix<T>::Matrix(int filas, int col)
    : cantF(filas), cantC(col)
{
    datos.resize(filas);
    for(int i = 0; i < filas; i++) {
        datos[i].resize(col);
    }
}

template<class T>
Matrix<T>::Matrix(const Matrix<T>& other)
    : datos(other.datos), cantF(other.cantF), cantC(other.cantC)
{}

template<class T>
Matrix<T>::~Matrix() {}

template<class T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other) {
  if (&other == this)
    return *this;

  int nCantF = other.cantF;
  int nCantC = other.cantC;
  
  cantF = other.cantF;
  cantC = other.cantC;

  datos.resize(nCantF);
  for (int i = 0; i < nCantF; i++) {
      datos[i].resize(nCantC);
  }

  for(int i = 0; i < nCantF; i++) {
    for(int j = 0; j < nCantC; j++) {
      datos[i][j] = other(i, j);
    }
  }
  
  return *this;
}

template<class T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& other) {
    // ASUME QUE LAS DIMENSIONES DAN
    Matrix<T> resultado(cantF, other.cantC);
    
    int dimInterna = cantC; // Tambien podria ser other.cantF
    
    for(int i = 0; i < resultado.cantF; i++) {
        for(int j = 0; j < resultado.cantC; j++) {
            resultado(i,j) = 0;
            for(int k = 0; k < dimInterna; k++) {
                resultado(i,j) += datos[i][k] * other(k,j);
            }
        }
    }
    
    return resultado;
}

template<class T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& other) {
    Matrix<T> resultado = (*this) * other;
    (*this) = resultado;
    return (*this);
}

template<class T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& other) {
    // ASUME QUE LAS DIMENSIONES DAN
    Matrix<T> resultado(cantF, other.cantC);
    
    for(int i = 0; i < resultado.cantF; i++) {
        for(int j = 0; j < resultado.cantC; j++) {
            resultado(i,j) = datos[i][j] + other(i,j);
        }
    }
    
    return resultado;
}

template<class T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& other) {
    Matrix<T> resultado = (*this) + other;
    (*this) = resultado;
    return (*this);
}

template<class T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& other) {
    // ASUME QUE LAS DIMENSIONES DAN
    Matrix<T> resultado(cantF, other.cantC);
    
    for(int i = 0; i < resultado.cantF; i++) {
        for(int j = 0; j < resultado.cantC; j++) {
            resultado(i,j) = datos[i][j] - other(i,j);
        }
    }
    
    return resultado;
}

template<class T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& other) {
    Matrix<T> resultado = (*this) - other;
    (*this) = resultado;
    return (*this);
}

template<class T>
Matrix<T> Matrix<T>::operator*(const T& scalar) {
    Matrix<T> resultado(cantF, cantC);
    
    for(int i = 0; i < resultado.cantF; i++) {
        for(int j = 0; j < resultado.cantC; j++) {
            resultado(i,j) = datos[i][j] * scalar;
        }
    }
    
    return resultado;
}

template<class T>
Matrix<T> Matrix<T>::operator/(const T& scalar) {
    Matrix<T> resultado(cantF, cantC);
    
    for(int i = 0; i < resultado.cantF; i++) {
        for(int j = 0; j < resultado.cantC; j++) {
            resultado(i,j) = datos[i][j] / scalar;
        }
    }
    
    return resultado;
}

template<class T>
T& Matrix<T>::operator ()(int a, int b) {
    return datos[a][b];
}

template<class T>
const T& Matrix<T>::operator ()(const int a, const int b) const {
    return datos[a][b];
}

template<class T>
int Matrix<T>::filas() {
    return cantF;
}

template<class T>
int Matrix<T>::columnas() {
    return cantC;
}

#endif	/* MATRIX_H */

