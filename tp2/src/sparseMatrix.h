/*
 * File:   SparseMatrix.h
 * Author: Rodrigo Kapobel
 *
 * Created on August 21, 2015, 23:05 PM
 */

#ifndef SparseMatrix_H
#define	SparseMatrix_H

#include <algorithm>
#include <math.h>
#include <vector>
#include <stdio.h>
#include "matrix.h"

using namespace std;

//CSR implementation for sparse matrix. For more information check the next link: https://en.wikipedia.org/wiki/Sparse_matrix#Compressed_row_Storage_.28CRS_or_CSR.29

template<class T>
class SparseMatrix {
public:
    SparseMatrix();
    SparseMatrix(int rows); // column vector with 0´s
    SparseMatrix(vector<int>& values, vector<int>& iValues, vector<int>& jValues, int columns); // PRE: jValues::size == values::size & values of iValues[0..iValues::size-2] are indices of values & iValue[iValues::size-1] == values::size
    SparseMatrix(const SparseMatrix<T>& other); // compress matrix
    SparseMatrix(const Matrix<T>& other);
    ~SparseMatrix();
    
    SparseMatrix<T>& operator=(const SparseMatrix<T>& other);
    SparseMatrix<T> operator*(const SparseMatrix<T>& other); // performs a matrix-vector multiplication
    SparseMatrix<T>& operator*=(const SparseMatrix<T>& other); // performs a matrix-vector multiplication
    SparseMatrix<T> operator+(const SparseMatrix<T>& other); // performs a vector-vector sum
    SparseMatrix<T>& operator+=(const SparseMatrix<T>& other); // performs a vector-vector sum
    SparseMatrix<T> operator-(const SparseMatrix<T>& other); // performs a vector-vector subs
    SparseMatrix<T>& operator-=(const SparseMatrix<T>& other); // performs a vector-vector subs
    
    SparseMatrix<T> operator*(const T& scalar);
    SparseMatrix<T> operator/(const T& scalar);
    
    T& operator()(int a, int b);
    const T& operator()(const int a, const int b) const;
    T& operator()(int a);
    const T& operator()(const int a) const;
    
    Matrix<T> descompress();
    
    int rows();
    int columns();
    void printSparseMatrix();
    
private:
    vector<T> _values;
    vector<int> _iValues;
    vector<int> _jValues;
    
    int _columns;
};

template<class T>
SparseMatrix<T>::SparseMatrix()
: _values(1), _iValues(1), _jValues(1), _columns(1)
{}

template<class T>
SparseMatrix<T>::SparseMatrix(int rows)
: _values(rows), _iValues(1), _jValues(rows), _columns(1)
{
    for(int i = 0; i <= rows; i++){
        _iValues.resize(i+1, i);
    }
}

template<class T>
SparseMatrix<T>::SparseMatrix(vector<int>& values, vector<int>& iValues, vector<int>& jValues, int columns)
: _values(values), _iValues(iValues), _jValues(jValues), _columns(columns)
{}

template<class T>
SparseMatrix<T>::SparseMatrix(const SparseMatrix<T>& other)
: _values(other._values), _iValues(other._iValues), _jValues(other._jValues), _columns(other._columns)
{}

template<class T>
SparseMatrix<T>::SparseMatrix(const Matrix<T>& other)
: _values(0), _iValues(0), _jValues(0), _columns(other.columns())
{
    int new_rows = other.rows();
    int new_columns = other.columns();
    
    for(int i = 0; i < new_rows; i++) {
        _iValues.resize(_iValues.size()+1, _values.size());
        for(int j = 0; j < new_columns; j++) {
            if (other(i, j) != 0) {
                _values.resize(_values.size()+1, other(i, j));
                _jValues.resize(_jValues.size()+1, j);
            }
        }
    }
    _iValues.resize(_iValues.size()+1, _values.size());
}

template<class T>
SparseMatrix<T>::~SparseMatrix() {}

template<class T>
SparseMatrix<T>& SparseMatrix<T>::operator=(const SparseMatrix<T>& other) {
    if (&other == this)
        return *this;
    
    _values = other._values;
    _iValues = other._iValues;
    _jValues = other._jValues;
    _columns = other.columns();
    
    return *this;
}

template<class T>
SparseMatrix<T> SparseMatrix<T>::operator*(const SparseMatrix<T>& other) {
    // ASUME QUE LAS DIMENSIONES DAN
    SparseMatrix<T> result(rows());
    
    for (int i = 0; i < rows(); i++) {
        for (int j = _iValues[i]; j < _iValues[i+1]; j++) {
            result(i) += _values[j]*other(_jValues[j]);
        }
    }
    
    return result;
}

template<class T>
SparseMatrix<T>& SparseMatrix<T>::operator*=(const SparseMatrix<T>& other) {
    SparseMatrix<T> result = (*this) * other;
    (*this) = result;
    return (*this);
}

template<class T>
SparseMatrix<T> SparseMatrix<T>::operator+(const SparseMatrix<T>& other) {
    SparseMatrix<T> result(rows());
    
    for(int i = 0; i < rows(); i++) {
        result(i) = _values[i] + other(i);
    }
    
    return result;
}

template<class T>
SparseMatrix<T>& SparseMatrix<T>::operator+=(const SparseMatrix<T>& other) {
    SparseMatrix<T> result = (*this) + other;
    (*this) = result;
    return (*this);
}

template<class T>
SparseMatrix<T> SparseMatrix<T>::operator-(const SparseMatrix<T>& other) {
    SparseMatrix<T> result(rows());
    
    for(int i = 0; i < rows(); i++) {
        result(i) = _values[i] - other(i);
    }
    
    return result;
}

template<class T>
SparseMatrix<T>& SparseMatrix<T>::operator-=(const SparseMatrix<T>& other) {
    SparseMatrix<T> result = (*this) - other;
    (*this) = result;
    return (*this);
}

template<class T>
SparseMatrix<T> SparseMatrix<T>::operator*(const T& scalar) {
    
    vector<T> resValues(_values.size());
    
    for(int i = 0; i < _values.size(); i++) {
       resValues(i) = _values[i] * scalar;
    }
    
    SparseMatrix<T> result(_values, _iValues, _jValues, _columns);
    
    return result;
}

template<class T>
SparseMatrix<T> SparseMatrix<T>::operator/(const T& scalar) {
    vector<T> resValues(_values.size());
    
    for(int i = 0; i < _values.size(); i++) {
       resValues(i) = _values[i] / scalar;
    }
    
    SparseMatrix<T> result(_values, _iValues, _jValues, _columns);
    
    return result;
}

template<class T>
T& SparseMatrix<T>::operator ()(int a) {
    return _values[a];
}

template<class T>
const T& SparseMatrix<T>::operator ()(const int a) const {
    return _values[a];
}

template<class T>
Matrix<T> SparseMatrix<T>::descompress() {
    Matrix<T> result(rows(), _columns, 0);
    for (int i = 0; i < rows(); i++) {
        for (int j = _iValues[i]; j < _iValues[i+1]; j++) {
            result(i, _jValues[j]) = _values[j];
        }
    }
    
    return result;
}

template<class T>
int SparseMatrix<T>::rows() {
    return _iValues.size()-1;
}

template<class T>
int SparseMatrix<T>::columns() {
    return _columns;
}

template<class T>
void SparseMatrix<T>::printSparseMatrix() {
    Matrix<T> descompressedMatrix = descompress();
    descompressedMatrix.printMatrix();
}

#endif	/* SparseMatrix_H */

