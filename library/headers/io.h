//
// Created by ijo42 on 29.03.2024.
//

#ifndef IO_H
#define IO_H

#include "main.h"

SparseDoubleLinkedMatrix *loadFromFileValidate(bool &isSuccess, std::string &path);
void saveToFile(const std::string &path, const SparseDoubleLinkedMatrix& matrix);

#endif //IO_H
