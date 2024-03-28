#include <cmath>
#include <format>
#include <fstream>
#include <iostream>
#include <unordered_map>
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

SparseDoubleLinkedMatrixElement *initElement(const double value) {
    auto *element = new SparseDoubleLinkedMatrixElement;
    element->value = value;
    element->nextLine = element->nextColumn = nullptr;
    return element;
}

/* n - кол-во столбцов, m - кол-во строк */
SparseDoubleLinkedMatrix generateRnd(int n, int m) {
    int k = n * m / 10; // максимальное кол-во ненулевых
    srand(time(0));

    auto matrix = SparseDoubleLinkedMatrix{};
    matrix.columnPointer = std::vector<SparseDoubleLinkedMatrixElement*>(n);
    matrix.linePointer = std::vector<SparseDoubleLinkedMatrixElement*>(m);
    auto columnTail = std::vector<SparseDoubleLinkedMatrixElement*>(n, nullptr),
            lineTail = std::vector<SparseDoubleLinkedMatrixElement*>(m, nullptr);

    for (int i = 0; i < n && k > 0; ++i) {      // вместо полного прохода прегенерировать непустые индексы
        for (int j = 0; j < m && k > 0; ++j) {
            if(rand() % 10 < 10){
                auto element = initElement(rand() % 100 / 10.0);
                if(!matrix.columnPointer[i])
                    matrix.columnPointer[i] = columnTail[i] = element;
                else {
                    columnTail[i]->nextColumn = element;
                    columnTail[i] = columnTail[i]->nextColumn;
                }

                if(!matrix.linePointer[j])
                    matrix.linePointer[j] = lineTail[j] = element;
                else {
                    lineTail[j]->nextLine = element;
                    lineTail[j] = lineTail[j]->nextLine;
                }

                k--;
            }
        }
    }
    return matrix;
}

SparseDoubleLinkedMatrixElement *createIfNotExists(std::unordered_map<unsigned int, SparseDoubleLinkedMatrixElement *>& table, unsigned int id,
                                                   const double value = NAN) {
    if(id == 0)
        return nullptr;

    if(!table.contains(id)) {
        auto *element = initElement(value);
        table.insert({id,element});
    } else if(value != NAN)
        table.at(id)->value = value;
    return table.at(id);
}

void loadPointers(const std::vector<std::string>& line, std::unordered_map<unsigned int, SparseDoubleLinkedMatrixElement*>& elements, std::vector<SparseDoubleLinkedMatrixElement *>& pointers) {
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
void loadElement(const std::vector<std::string>& line, std::unordered_map<unsigned int, SparseDoubleLinkedMatrixElement*>& elements, int id) {

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
    std::ranges::copy(matrix.columnPointer, columnTail.begin());
    for (const auto lineHead: matrix.linePointer) {
        auto lineTail = lineHead;
        for (auto & i : columnTail) {
            if(lineTail && lineTail == i) {
                std::cout << std::format("{:6.2f}", i->value);
                lineTail = lineTail->nextLine;
                i = i->nextColumn;
            } else
                std::cout << std::format("{:6.0f}", 0.0);
        }
        std::cout << "\n";
    }
}

SparseDoubleLinkedMatrix loadFromFile(const std::string &path) {
    std::ifstream input(path);
    std::string lineString;
    SparseDoubleLinkedMatrix matrix{};
    std::unordered_map<unsigned int, SparseDoubleLinkedMatrixElement*> elements{};

    std::getline(input,lineString);
    std::vector<std::string> line = split(lineString, DLSMDelimiter);

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

std::string join(const std::vector<unsigned int> &lst, const char delim) {
    std::string ret;
    for(const auto &s : lst) {
        if(!ret.empty())
            ret += delim;
        ret.append(std::to_string(s));
    }
    return ret;
}

/*
 * поиск элемента `searched` начиная от текущей позиции итератора
 * возвращает кол-во элементов между текущим элементом и искомым
 */
unsigned int findElementNext(std::vector<SparseDoubleLinkedMatrixElement *>::iterator current, const SparseDoubleLinkedMatrixElement* searched) {
    int i = 0;
    for (; *current != searched; ++current, ++i) {}
    return i;
}

void saveToFile(const std::string &path, const SparseDoubleLinkedMatrix& matrix) {
    std::vector<SparseDoubleLinkedMatrixElement*> elements(matrix.linePointer.size()*matrix.columnPointer.size() / 10);
    std::vector<unsigned int> lineIds(matrix.linePointer.size(), 0), columnIds(matrix.columnPointer.size(), 0);
    unsigned int k = 1, i = 0;

    std::vector<SparseDoubleLinkedMatrixElement*> columnPointer(matrix.columnPointer.size());
    std::ranges::copy(matrix.columnPointer, columnPointer.begin());
    for(auto lineHead = matrix.linePointer.begin(); lineHead != matrix.linePointer.end(); ++i, ++lineHead){
        auto lineTail = *lineHead;
        unsigned int j = 0;
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
    auto outputText = std::vector<std::string>(elements.size() + 2);

    outputText[0] = join(columnIds, DLSMDelimiter);
    outputText[1] =  join(lineIds, DLSMDelimiter);

    k = 1;
    for (auto current = elements.begin(); current != elements.end(); ++current, ++k) {
        outputText[k+1] = std::format("{:.2f};{};{}",
            (*current)->value,
            (*current)->nextLine ? k+1 : 0,
            (*current)->nextColumn ? k + findElementNext(current, (*current)->nextColumn) : 0
        );
    }

    std::ofstream output(path, std::ios::trunc);
    std::ranges::copy(outputText, std::ostream_iterator<std::string>(output, "\n"));
}


int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    auto matrix = generateRnd(4, 7);
    printMatrix(matrix);
    saveToFile(R"(..\examples\ex0.txt)", matrix);

    return 0;
}
