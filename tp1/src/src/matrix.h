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
#include <stdio.h>
 
using namespace std;

// La matriz respeta la notacion de la catedra, es decir, el primer subindice 
// es la fila y el segundo es la columna

template<class T>
class Matrix {
    public:
        Matrix();
        Matrix(int rows); // Columnas impllicitas (col = 1)
        Matrix(int rows, int col);
        Matrix(int rows, int col, const T& init);
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
        T& operator()(int a);
        const T& operator()(const int a) const;
        
        int rows();
        int columns();
        void printMatrix();
        
    private:
        vector<vector<T> > _values;
        int _rows;
        int _columns;
        
};

template<class T>
Matrix<T>::Matrix()
    : _values(1), _rows(1), _columns(1)
{
    _values[0].resize(1);
}

template<class T>
Matrix<T>::Matrix(int rows)
    : _values(rows), _rows(rows), _columns(1)
{
    for(int i = 0; i < rows; i++) {
        _values[i].resize(1);
    }
}

template<class T>
Matrix<T>::Matrix(int rows, int col)
    : _values(rows), _rows(rows), _columns(col)
{
    for(int i = 0; i < rows; i++) {
        _values[i].resize(col);
    }
}

template<class T>
Matrix<T>::Matrix(int rows, int col, const T& init)
    : _values(rows), _rows(rows), _columns(col)
{
    for(int i = 0; i < rows; i++) {
        _values[i].resize(col, init);
    }
}

template<class T>
Matrix<T>::Matrix(const Matrix<T>& other)
    : _values(other._values), _rows(other._rows), _columns(other._columns)
{}

template<class T>
Matrix<T>::~Matrix() {}

template<class T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other) {
  if (&other == this)
    return *this;

  int new_rows = other._rows;
  int new_columns = other._columns;
  
  _rows = new_rows;
  _columns = new_columns;

  _values.resize(new_rows);
  for (int i = 0; i < new_columns; i++) {
      _values[i].resize(new_columns);
  }

  for(int i = 0; i < new_rows; i++) {
    for(int j = 0; j < new_columns; j++) {
      _values[i][j] = other(i, j);
    }
  }
  
  return *this;
}

template<class T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& other) {
    // ASUME QUE LAS DIMENSIONES DAN
    Matrix<T> result(_rows, other._columns);
    
    int innerDim = _columns; // Tambien podria ser other._rows
    
    for(int i = 0; i < result._rows; i++) {
        for(int j = 0; j < result._columns; j++) {
            result(i,j) = 0;
            for(int k = 0; k < innerDim; k++) {
                result(i,j) += _values[i][k] * other(k,j);
            }
        }
    }
    
    return result;
}

template<class T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& other) {
    Matrix<T> result = (*this) * other;
    (*this) = result;
    return (*this);
}

template<class T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& other) {
    // ASUME QUE LAS DIMENSIONES DAN
    Matrix<T> result(_rows, other._columns);
    
    for(int i = 0; i < result._rows; i++) {
        for(int j = 0; j < result._columns; j++) {
            result(i,j) = _values[i][j] + other(i,j);
        }
    }
    
    return result;
}

template<class T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& other) {
    Matrix<T> result = (*this) + other;
    (*this) = result;
    return (*this);
}

template<class T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& other) {
    // ASUME QUE LAS DIMENSIONES DAN
    Matrix<T> result(_rows, other._columns);
    
    for(int i = 0; i < result._rows; i++) {
        for(int j = 0; j < result._columns; j++) {
            result(i,j) = _values[i][j] - other(i,j);
        }
    }
    
    return result;
}

template<class T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& other) {
    Matrix<T> result = (*this) - other;
    (*this) = result;
    return (*this);
}

template<class T>
Matrix<T> Matrix<T>::operator*(const T& scalar) {
    Matrix<T> result(_rows, _columns);
    
    for(int i = 0; i < result._rows; i++) {
        for(int j = 0; j < result._columns; j++) {
            result(i,j) = _values[i][j] * scalar;
        }
    }
    
    return result;
}

template<class T>
Matrix<T> Matrix<T>::operator/(const T& scalar) {
    Matrix<T> result(_rows, _columns);
    
    for(int i = 0; i < result._rows; i++) {
        for(int j = 0; j < result._columns; j++) {
            result(i,j) = _values[i][j] / scalar;
        }
    }
    
    return result;
}

template<class T>
T& Matrix<T>::operator ()(int a, int b) {
    return _values[a][b];
}

template<class T>
const T& Matrix<T>::operator ()(const int a, const int b) const {
    return _values[a][b];
}

template<class T>
T& Matrix<T>::operator ()(int a) {
    return _values[a][0];
}

template<class T>
const T& Matrix<T>::operator ()(const int a) const {
    return _values[a][0];
}

template<class T>
int Matrix<T>::rows() {
    return _rows;
}

template<class T>
int Matrix<T>::columns() {
    return _columns;
}

template<class T>
void Matrix<T>::printMatrix() {
    for(int i = 0; i < _rows; i++) {;
        for(int j = 0; j < _columns; j++) {
        	printf("%3.3f ", _values[i][j]);
            // cout << _values[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

#endif	/* MATRIX_H */

