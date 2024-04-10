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
    if (id == 0) {
        return nullptr;
    }

    auto it = table.find(id); // Attempt to find the id in the table
    if (it == table.end()) { // If id is not found, create new element
        auto* element = initElement(value); // Assuming initElement is defined elsewhere
        table[id] = element; // Insert element into the table
    } else if (!std::isnan(value)) { // If the id is found and value is not NAN, update the element's value
        it->second->value = value;
    }
    return table[id]; // Return the element
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
    std::vector<SparseDoubleLinkedMatrixElement*> elements;
    elements.reserve(maxElements(matrix.linePointer.size(), matrix.columnPointer.size()));

    std::vector<size_t> lineIds(matrix.linePointer.size(), 0);
    std::vector<size_t> columnIds(matrix.columnPointer.size(), 0);

    std::vector<SparseDoubleLinkedMatrixElement*> columnPointer = matrix.columnPointer;
    size_t k = 1;

    for (size_t i = 0; i < matrix.linePointer.size(); ++i) {
        auto lineTail = matrix.linePointer[i];
        for (size_t j = 0; j < columnPointer.size(); ++j) {
            if (lineTail && lineTail == columnPointer[j]) {
                elements.push_back(columnPointer[j]);
                lineTail = lineTail->nextLine;
                columnPointer[j] = columnPointer[j]->nextColumn;

                if (columnIds[j] == 0) columnIds[j] = k;
                if (lineIds[i] == 0) lineIds[i] = k;
                k++;
            }
        }
    }

    // Использование строкового потока для формирования итогового текста
    std::ostringstream oss;
    oss.precision(6);
    // Функция для преобразования векторов в строку с разделителем ";"
    auto vectorToStream = [](std::ostringstream& oss, const std::vector<size_t>& ids) {
        for (size_t i = 0; i < ids.size(); ++i) {
            if (i > 0) oss << ";";
            oss << ids[i];
        }
        oss << "\n";
    };

    // Добавление идентификаторов строк и столбцов
    vectorToStream(oss, lineIds);
    vectorToStream(oss, columnIds);

    // Добавление элементов
    for (size_t index = 0; index < elements.size(); ++index) {
        const auto& elem = elements[index];
        oss << std::fixed << elem->value << ";";
        oss << (elem->nextLine ? std::to_string(index + 2) : "0") << ";";
        oss << (elem->nextColumn ? std::to_string(index + 2) : "0") << "\n";
    }

    // Запись в файл
    std::ofstream output(path, std::ios::trunc);
    if (output) {
        output << oss.str();
    }
}
