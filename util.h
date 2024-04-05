#ifndef UTIL_H
#define UTIL_H

#include "main.h"

std::vector<std::string> split(const std::string &s, char delim);
std::string join(const std::vector<size_t> &lst, char delim);
size_t countLines(const std::string& path);

#endif //UTIL_H
