#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "main.h"
void printMatrix(const SparseDoubleLinkedMatrix& matrix);
SparseDoubleLinkedMatrix deepCopy(const SparseDoubleLinkedMatrix& matrix);
void deepDelete(SparseDoubleLinkedMatrix& matrix);
SparseDoubleLinkedMatrixElement *initElement(double value);
SparseDoubleLinkedMatrix generateRnd(int n, int m);

#endif //OPERATIONS_H
