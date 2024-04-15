#ifndef OPERATIONS_H
#define OPERATIONS_H
#define MIN_ZNACH 1e-10
#include "main.h"
void printMatrix(const SparseDoubleLinkedMatrix* matrix);
SparseDoubleLinkedMatrix *deepCopy(const SparseDoubleLinkedMatrix& input);
void deepDelete(SparseDoubleLinkedMatrix& matrix);
SparseDoubleLinkedMatrixElement *initElement(double value);
SparseDoubleLinkedMatrix *generateRnd(int n, int m);
SparseDoubleLinkedMatrix *generateEmpty(int n, int m);
SparseDoubleLinkedMatrix *add(const SparseDoubleLinkedMatrix& matrix1, const SparseDoubleLinkedMatrix& matrix2);
SparseDoubleLinkedMatrix *sub(const SparseDoubleLinkedMatrix& matrix1, const SparseDoubleLinkedMatrix& matrix2);
SparseDoubleLinkedMatrix *inverseMatrix(SparseDoubleLinkedMatrix& mainMatrix);
void joinMatrix(SparseDoubleLinkedMatrix& matrix1, SparseDoubleLinkedMatrix& matrix2);
SparseDoubleLinkedMatrix *multiply(SparseDoubleLinkedMatrix& matrix1, SparseDoubleLinkedMatrix& matrix2);
SparseDoubleLinkedMatrix *generateUnitMatrix(size_t n);
SparseDoubleLinkedMatrix *generateRndFifty(int n, int m);
SparseDoubleLinkedMatrix *setElement(SparseDoubleLinkedMatrix *&matrix, int row, int column, double value);
void deleteZeroElements(SparseDoubleLinkedMatrix& matrix);
#endif //OPERATIONS_H
