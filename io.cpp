#include "io.h"

/*
 * поиск элемента `searched` начиная от текущей позиции итератора
 * возвращает кол-во элементов между текущим элементом и искомым
 */
size_t findElementNext(std::vector<SparseDoubleLinkedMatrixElement *>::iterator current,
                             const SparseDoubleLinkedMatrixElement *searched) {
    int i = 0;
    for (; *current != searched; ++current, ++i) {}
    return i;
}

SparseDoubleLinkedMatrixElement *createIfNotExists(std::unordered_map<size_t, SparseDoubleLinkedMatrixElement *>& table, size_t id,
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

void loadPointers(const std::vector<std::string>& line, std::unordered_map<size_t, SparseDoubleLinkedMatrixElement*>& elements, std::vector<SparseDoubleLinkedMatrixElement *>& pointers) {
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

void loadElement(const std::vector<std::string>& line, std::unordered_map<size_t, SparseDoubleLinkedMatrixElement*>& elements, int id) {

    const double value = std::stod(line[0]);
    const int nextLineId = std::stoi(line[1]), nextColumnId = std::stoi(line[2]);
    SparseDoubleLinkedMatrixElement *nextLine   = createIfNotExists(elements, nextLineId),
                                    *nextColumn = createIfNotExists(elements, nextColumnId),
    *current = createIfNotExists(elements, id, value);

    current->nextLine   = nextLine;
    current->nextColumn = nextColumn;

}

SparseDoubleLinkedMatrix loadFromFile(const std::string &path) {
    std::ifstream input(path);
    std::string lineString;
    SparseDoubleLinkedMatrix matrix{};
    const auto lines = countLines(path);
    if(lines < 3)
        return matrix;
    std::unordered_map<size_t, SparseDoubleLinkedMatrixElement*> elements(lines-2);

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

void saveToFile(const std::string &path, const SparseDoubleLinkedMatrix& matrix) {
    std::vector<SparseDoubleLinkedMatrixElement*> elements(maxElements(matrix.linePointer.size(), matrix.columnPointer.size()));
    std::vector<size_t> lineIds(matrix.linePointer.size(), 0), columnIds(matrix.columnPointer.size(), 0);
    size_t k = 1, i = 0;

    std::vector<SparseDoubleLinkedMatrixElement*> columnPointer(matrix.columnPointer.size());
    std::ranges::copy(matrix.columnPointer, columnPointer.begin());
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
    auto outputText = std::vector<std::string>(elements.size() + 2);

    outputText[0] =  join(lineIds, DLSMDelimiter);
    outputText[1] = join(columnIds, DLSMDelimiter);

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
