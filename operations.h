#ifndef OPERATIONS_H
#define OPERATIONS_H
#define MIN_ZNACH 1e-15 //почти ноль
#define MORE_THEN_MIN_ZNACH 1e-10 //почти ноль
#include "main.h"
void printMatrix(const SparseDoubleLinkedMatrix& matrix);
SparseDoubleLinkedMatrix deepCopy(const SparseDoubleLinkedMatrix& matrix);
void deepDelete(SparseDoubleLinkedMatrix& matrix);
SparseDoubleLinkedMatrixElement *initElement(double value);
SparseDoubleLinkedMatrix generateRnd(int n, int m);
SparseDoubleLinkedMatrix add(const SparseDoubleLinkedMatrix& matrix1, const SparseDoubleLinkedMatrix& matrix2);
SparseDoubleLinkedMatrix sub(const SparseDoubleLinkedMatrix& matrix1, const SparseDoubleLinkedMatrix& matrix2);

#endif //OPERATIONS_H
