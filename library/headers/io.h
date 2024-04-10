//
// Created by ijo42 on 29.03.2024.
//

#ifndef IO_H
#define IO_H

#include "main.h"

SparseDoubleLinkedMatrix loadFromFileValidate(bool &isSuccess, std::string &path);
SparseDoubleLinkedMatrix loadFromFile(const std::string &path);
void saveToFile(const std::string &path, const SparseDoubleLinkedMatrix& matrix);
SparseDoubleLinkedMatrix loadFromFileValidate(bool& isSuccess, std::string& path);
#endif //IO_H
