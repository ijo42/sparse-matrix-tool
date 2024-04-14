#include "headers/util.h"

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

size_t maxElements(const size_t n, const size_t m) {
    return n * m / 10;
}
/* определяет максимальное кол-во элементов матрицы на основе процента заполнения
 * используется для отладки, целевое значение: 10%  */
size_t maxElements(const SparseDoubleLinkedMatrix &matrix) {
    return maxElements(matrix.linePointer.size(), matrix.columnPointer.size());
}


std::vector<SparseDoubleLinkedMatrixElement*> listElements(const SparseDoubleLinkedMatrix &matrix, std::vector<size_t> &lineIds, std::vector<size_t> &columnIds) {
    std::vector<SparseDoubleLinkedMatrixElement*>
            elements(maxElements(matrix)),
            columnPointer(matrix.columnPointer);
    size_t k = 1, i = 0;

    for(auto lineHead = matrix.linePointer.begin(); lineHead != matrix.linePointer.end(); ++i, ++lineHead){
        auto lineTail = *lineHead;
        size_t j = 0;
        for (auto columnHead = columnPointer.begin(); columnHead != columnPointer.end(); ++j, ++columnHead) {
            if(lineTail && lineTail == *columnHead) {
                elements[k-1] = (*columnHead);
                lineTail = lineTail->nextLine;
                *columnHead = (*columnHead)->nextColumn;
                if(columnIds[j] == 0)
                    columnIds[j] = k;
                if(lineIds[i] == 0)
                    lineIds[i] = k;
                k++;
            }
        }
    }

    elements.resize(k-1);
    return elements;
}

size_t countElements(const SparseDoubleLinkedMatrix &matrix) {
    size_t k = 0;
    std::vector<SparseDoubleLinkedMatrixElement*> columnPointer(matrix.columnPointer);

    for(auto lineTail : matrix.linePointer){
        for (auto & columnHead : columnPointer) {
            if(lineTail && lineTail == columnHead) {
                lineTail = lineTail->nextLine;
                columnHead = columnHead->nextColumn;
                k++;
            }
        }
    }
    return k;
}
