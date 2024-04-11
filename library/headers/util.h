#ifndef UTIL_H
#define UTIL_H

#include "main.h"

std::vector<std::string> split(const std::string &s, char delim);
std::string join(const std::vector<size_t> &lst, char delim);
size_t countLines(const std::string& path);
size_t maxElements(size_t n, size_t m);
size_t maxElements(const SparseDoubleLinkedMatrix &matrix);
std::vector<SparseDoubleLinkedMatrixElement*> listElements(const SparseDoubleLinkedMatrix &matrix, std::vector<size_t> &lineIds, std::vector<size_t> &columnIds);

#endif //UTIL_H
