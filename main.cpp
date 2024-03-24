#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <ranges>
#include <string>
#include <sstream>
#include <vector>

struct SparseDoubleLinkedMatrixElement {
    double value;
    SparseDoubleLinkedMatrixElement *nextLine, *nextColumn;
};

class SparseDoubleLinkedMatrix {
public:
    std::vector<SparseDoubleLinkedMatrixElement*> linePointer{}, columnPointer{};
};

constexpr char DLSMDelimiter = ';';

std::vector<std::string> split(const std::string &s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

SparseDoubleLinkedMatrixElement *createIfNotExists(std::map<unsigned int, SparseDoubleLinkedMatrixElement *>& table, unsigned int id,
                                                   const double value = NAN) {
    if(id == 0)
        return nullptr;

    if(!table.contains(id)) {
        auto *element = new SparseDoubleLinkedMatrixElement;
        element->value = value;
        element->nextLine = element->nextColumn = nullptr;
        table.insert({id,element});
    } else if(value != NAN)
        table.at(id)->value = value;
    return table.at(id);
}

void loadPointers(const std::vector<std::string>& line, std::map<unsigned int, SparseDoubleLinkedMatrixElement*>& elements, std::vector<SparseDoubleLinkedMatrixElement *>& pointers) {
    for (const auto& number: line) {
        int id = std::stoi(number);
        if(id != 0) {
            auto element = createIfNotExists(elements, id);
            pointers.push_back(element);
        } else {
            pointers.push_back(nullptr);
        }
    }
}
void loadElement(const std::vector<std::string>& line, std::map<unsigned int, SparseDoubleLinkedMatrixElement*>& elements, int id) {

    const double value = std::stod(line[0]);
    const int nextLineId = std::stoi(line[1]), nextColumnId = std::stoi(line[2]);
    SparseDoubleLinkedMatrixElement *nextLine   = createIfNotExists(elements, nextLineId),
                                    *nextColumn = createIfNotExists(elements, nextColumnId),
    *current = createIfNotExists(elements, id, value);

    current->nextLine   = nextLine;
    current->nextColumn = nextColumn;

}

void printMatrix(const SparseDoubleLinkedMatrix& matrix) {
    std::vector<SparseDoubleLinkedMatrixElement*> columnTail(matrix.columnPointer.size());
    std::copy(matrix.columnPointer.begin(), matrix.columnPointer.end(), columnTail.begin());
    for (const auto lineHead: matrix.linePointer) {
        auto lineTail = lineHead;
        for (int i = 0; i < columnTail.size(); i++) {
            if(lineTail && lineTail == columnTail.at(i)) {
                printf("%6.1f", columnTail.at(i)->value);
                lineTail = lineTail->nextLine;
                columnTail.at(i) = columnTail.at(i)->nextColumn;
            }else
                printf("%6.1f", 0.0);
        }
        printf("\n");
    }
}

SparseDoubleLinkedMatrix loadFromFile(const std::string &path) {
    std::ifstream input(path);
    std::string lineString;
    std::vector<std::string> line;
    SparseDoubleLinkedMatrix matrix{};
    std::map<unsigned int, SparseDoubleLinkedMatrixElement*> elements{};

    std::getline(input,lineString);
    line = split(lineString, DLSMDelimiter);

    loadPointers(line, elements, matrix.linePointer);    // загрузка указателей на строки

    std::getline(input,lineString);
    line = split(lineString, DLSMDelimiter);
    loadPointers(line, elements, matrix.columnPointer);  // загрузка указателей на столбцы


    for(int i = 1; std::getline(input,lineString); i++) {
        line = split(lineString, DLSMDelimiter);
        loadElement(line, elements, i);                    // загрузка элементов
    }
    return matrix;
}

std::string join(const std::vector<unsigned int> &lst, const char delim)
{
    std::string ret;
    for(const auto &s : lst) {
        if(!ret.empty())
            ret += delim;
        ret += std::to_string(s);
    }
    return ret;
}

unsigned int findElementNext(std::vector<SparseDoubleLinkedMatrixElement *>::iterator current, const SparseDoubleLinkedMatrixElement* searched) {
    int i = 0;
    for (; *current != searched; ++current, ++i) {}
    return i;
}

void saveToFile(const std::string &path, const SparseDoubleLinkedMatrix& matrix) {
    std::ofstream output(path, std::ios::trunc);
    std::vector<SparseDoubleLinkedMatrixElement*> elements{};
    std::vector<unsigned int> lineIds(matrix.linePointer.size(), 0), columnIds(matrix.columnPointer.size(), 0);
    unsigned int k = 1, i = 0, j;

    std::vector<SparseDoubleLinkedMatrixElement*> columnPointer(matrix.columnPointer.size());
    std::copy(matrix.columnPointer.begin(), matrix.columnPointer.end(), columnPointer.begin());
    for(auto lineHead = matrix.linePointer.begin(); lineHead != matrix.linePointer.end(); ++i, ++lineHead){
        auto lineTail = *lineHead;
        j = 0;
        for (auto columnHead = columnPointer.begin(); columnHead != columnPointer.end(); ++j, ++columnHead) {
            if(lineTail && lineTail == *columnHead) {
                elements.push_back(*columnHead);
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

    output << join(columnIds, DLSMDelimiter) << "\n"
            << join(lineIds, DLSMDelimiter) << "\n";
    k = 1;
    for (auto current = elements.begin(); current != elements.end(); ++current, ++k) {
        output << (*current)->value << DLSMDelimiter <<
            ((*current)->nextLine ? k+1 : 0)
                                         << DLSMDelimiter <<
            ((*current)->nextColumn ? k + findElementNext(current, (*current)->nextColumn) : 0)
                                         << "\n";
    }
}


int main() {
    auto matrix = loadFromFile(R"(..\examples\ex1.txt)");
    printMatrix(matrix);
    saveToFile(R"(..\examples\ex0.txt)", matrix);

    return 0;
}
