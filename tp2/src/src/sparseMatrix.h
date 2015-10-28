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
#include <cmath>
#include "matrix.h"
#include <map>

using namespace std;

//CSR implementation for sparse matrix. For more information check the next link: https://en.wikipedia.org/wiki/Sparse_matrix#Compressed_row_Storage_.28CRS_or_CSR.29

typedef pair<int, int> rowCol;

struct comparator {
    bool operator() (const rowCol p1, const rowCol p2) const
    {
        if (p1.first == p2.first) {
            return p1.second < p2.second;
        }
        
        return p1.first < p2.first;
    }
};

typedef map< rowCol, double, comparator > dok;

template<class T>
class SparseMatrix {
public:
    SparseMatrix();
    SparseMatrix(int rows); // column vector with 0's
    SparseMatrix(int rows, T value);
    SparseMatrix(vector<T>& values, vector<int>& iValues, vector<int>& jValues, int columns); // PRE: jValues::size == values::size & values of iValues[0..iValues::size-2] are indices of values & iValue[iValues::size-1] == values::size
    SparseMatrix(const SparseMatrix<T>& other); // compress matrix
    SparseMatrix(const Matrix<T>& other);
    SparseMatrix(const dok, int num_rows, int num_cols); //create sparse matrix from a dok map
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
    //PRE are vectors
    double L1(const SparseMatrix<T>& other);
    double norm1();
    
    int rows();
    int columns();
    int rows() const;
    int columns() const;
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
SparseMatrix<T>::SparseMatrix(int rows, T value)
: _values(rows), _iValues(1), _jValues(rows), _columns(1)
{
    for(int i = 0; i <= rows; i++){
        _iValues.resize(i+1, i);
        if(i < rows){
            _values[i] = value;
        }
    }
}

template<class T>
SparseMatrix<T>::SparseMatrix(vector<T>& values, vector<int>& iValues, vector<int>& jValues, int columns)
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
SparseMatrix<T>::SparseMatrix(const dok dicc, int num_rows, int num_cols)
: _values(0), _iValues(0), _jValues(0), _columns(num_cols)
{
    //int new_rows = num_rows;
    //int new_columns = num_cols;
    
    int actual_row = -1;
    
    //Rows in dicc must be sequentially, because a not existant row means row of zeros. zeros are not represented.
    for (dok::const_iterator it= dicc.begin(); it!=dicc.end(); ++it) {
        //cout << "(" << it->first.first << ", " << it->first.second << ")" << " => " << it->second << endl;
        
        if (actual_row != it->first.first) {
            _iValues.resize(_iValues.size()+1, _values.size());
            actual_row = it->first.first;
        }
        
        if (it->second != 0) {
            _values.resize(_values.size()+1, it->second);
            _jValues.resize(_jValues.size()+1, it->first.second);
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
       resValues[i] = _values[i] * scalar;
    }
    
    SparseMatrix<T> result(resValues, _iValues, _jValues, _columns);

    return result;
}

template<class T>
SparseMatrix<T> SparseMatrix<T>::operator/(const T& scalar) {
    vector<T> resValues(_values.size());
    
    for(int i = 0; i < _values.size(); i++) {
       resValues[i] = _values[i] / scalar;
    }
    
    SparseMatrix<T> result(resValues, _iValues, _jValues, _columns);
    
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
double SparseMatrix<T>::L1(const SparseMatrix<T>& other) {
    SparseMatrix<T> vectorSubs = *this - other;
    double res = 0;
    for(int i = 0; i < rows(); i++){
        res = res + abs(vectorSubs(i));
    }
    
    return res;
}

template<class T>
double SparseMatrix<T>::norm1() {
    double res = 0;
    for(int i = 0; i < rows(); i++){
        res = res + abs(_values[i]);
    }
    
    return res;
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
int SparseMatrix<T>::rows() const{
    return _iValues.size()-1;
}

template<class T>
int SparseMatrix<T>::columns() const{
    return _columns;
}

template<class T>
void SparseMatrix<T>::printSparseMatrix() {
    Matrix<T> descompressedMatrix = descompress();
    descompressedMatrix.printMatrix();
}

#endif	/* SparseMatrix_H */

