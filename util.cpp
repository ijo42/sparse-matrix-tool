#include "util.h"

std::vector<std::string> split(const std::string &s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}



std::string join(const std::vector<size_t> &lst, const char delim) {
    std::string ret;
    for(const auto &s : lst) {
        if(!ret.empty())
            ret += delim;
        ret.append(std::to_string(s));
    }
    return ret;
}

/*
 * возвращает кол-во строк в файле
 */
size_t countLines(const std::string& path) {
    size_t k = 0;
    std::ifstream input(path);
    std::string line;
    while(std::getline(input,line)) {
        k++;
    }
    return k;
}

/* определяет максимальное кол-во элементов матрицы на основе процента заполнения
 * используется для отладки, целевое значение: 10%  */
size_t maxElements(const size_t n, const size_t m) {
    return n * m / 10;
}
