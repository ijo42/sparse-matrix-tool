#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "shared.h"

#define MIN_ZNACH 1e-30
void printMatrix(const SparseDoubleLinkedMatrix& matrix);
SparseDoubleLinkedMatrix deepCopy(const SparseDoubleLinkedMatrix& input);
void deepDelete(SparseDoubleLinkedMatrix& matrix);
SparseDoubleLinkedMatrixElement *initElement(double value);
SparseDoubleLinkedMatrix generateRnd(int n, int m);
SparseDoubleLinkedMatrix add(const SparseDoubleLinkedMatrix& matrix1, const SparseDoubleLinkedMatrix& matrix2);
SparseDoubleLinkedMatrix sub(const SparseDoubleLinkedMatrix& matrix1, const SparseDoubleLinkedMatrix& matrix2);
SparseDoubleLinkedMatrix inverseMatrix(SparseDoubleLinkedMatrix& mainMatrix);
void joinMatrix(SparseDoubleLinkedMatrix& matrix1, SparseDoubleLinkedMatrix& matrix2);
SparseDoubleLinkedMatrix multiply(SparseDoubleLinkedMatrix& matrix1, SparseDoubleLinkedMatrix& matrix2);
SparseDoubleLinkedMatrix generateUnitMatrix(std::size_t n);
SparseDoubleLinkedMatrix generateRndFifty(const int n, const int m);

#endif //OPERATIONS_H
