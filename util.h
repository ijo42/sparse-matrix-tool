#ifndef UTIL_H
#define UTIL_H

#include "main.h"

std::vector<std::string> split(const std::string &s, char delim);
std::string join(const std::vector<unsigned int> &lst, char delim);
unsigned int countLines(const std::string& path);

#endif //UTIL_H