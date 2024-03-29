#include "operations.h"

/*
 * иницилизирует новый элемент, заполняя значение из параметра, либо NAN при отсутствии параметра
 * выставляет ссылки в NULL
 */
SparseDoubleLinkedMatrixElement *initElement(const double value) {
    auto *element = new SparseDoubleLinkedMatrixElement;
    element->value = value;
    element->nextLine = element->nextColumn = nullptr;
    return element;
}


/*
 * генерирует матрицу
 * каждый элемент генерируется с вероятностью 0.1,
 * общее колличество сгенерированных элементов строго меньше 10% размера матрицы
 * n - кол-во столбцов, m - кол-во строк
 */
SparseDoubleLinkedMatrix generateRnd(const int n, const int m) {
    int k = n * m / 10; // максимальное кол-во ненулевых
    srand(time(0));

    auto matrix = SparseDoubleLinkedMatrix{};
    matrix.columnPointer = std::vector<SparseDoubleLinkedMatrixElement*>(n);
    matrix.linePointer = std::vector<SparseDoubleLinkedMatrixElement*>(m);
    auto columnTail = std::vector<SparseDoubleLinkedMatrixElement*>(n, nullptr),
            lineTail = std::vector<SparseDoubleLinkedMatrixElement*>(m, nullptr);

    for (int i = 0; i < n && k > 0; ++i) {      // вместо полного прохода прегенерировать непустые индексы
        for (int j = 0; j < m && k > 0; ++j) {
            if(rand() % 10 == 0){
                auto element = initElement(rand() % 1000 / 100.0);
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

void printMatrix(const SparseDoubleLinkedMatrix& matrix) {
    if (matrix.columnPointer.empty())
        std::cout << "Matrix is empty";

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


SparseDoubleLinkedMatrix deepCopy(const SparseDoubleLinkedMatrix& matrix) {
    auto newMatrix = SparseDoubleLinkedMatrix{};

    // нули авто иницилизированы, т.к. без счётчика не обойтись
    newMatrix.columnPointer = std::vector<SparseDoubleLinkedMatrixElement*>(matrix.columnPointer.size(), nullptr);
    newMatrix.linePointer = std::vector<SparseDoubleLinkedMatrixElement*>{};
    newMatrix.linePointer.reserve(matrix.linePointer.size());           // нули будут заполнены вручную, что бы не вводить счетчик

    std::vector<SparseDoubleLinkedMatrixElement*> columnPointer(matrix.columnPointer.size());
    std::ranges::copy(matrix.columnPointer, columnPointer.begin());

    std::vector<SparseDoubleLinkedMatrixElement*> newColumnPointer(matrix.columnPointer.size(), nullptr);

    for (const auto linePointer: matrix.linePointer) {
        SparseDoubleLinkedMatrixElement* newLineTail = nullptr;
        unsigned int columnI = 0;
        for (auto & columnTail : columnPointer) {
            if(linePointer && linePointer == columnTail) {
                auto element = initElement(columnTail->value);
                if(newLineTail) {   // если первый элемент в строке существует связываем предыдущий элемент с текущим
                    newLineTail->nextLine = element;
                } else {            // если это первый элемент в строке, записываем в указатели
                    newMatrix.linePointer.push_back(element);
                }
                newLineTail = element; // двигаем хвост строк

                if(!newColumnPointer[columnI]) { // если это первый элемент в столбце, записываем в указатели
                    newMatrix.columnPointer[columnI] = newColumnPointer[columnI] = element;
                } else {                         // если первый элемент в столбце существует, связываем с предыдущим
                    newColumnPointer[columnI]->nextColumn = element;
                }
                newColumnPointer[columnI] = element; // двигаем хвост столбца

                columnTail = columnTail->nextColumn; // двигаем хвост оригинальной матрицы
            }
            columnI++;
        }
        if(!newLineTail)
            newMatrix.linePointer.push_back(newLineTail);
    }
    return newMatrix;
}
/*
 * удаляет матрицу, а так же все элементы из ОЗУ
 * после операции класс матрицы имеет нулевые векторы
 */
void deepDelete(SparseDoubleLinkedMatrix& matrix) {
    std::vector<SparseDoubleLinkedMatrixElement*> columnPointer(matrix.columnPointer.size());
    std::ranges::copy(matrix.columnPointer, columnPointer.begin());
    unsigned int i = 0;
    for(auto lineHead = matrix.linePointer.begin(); lineHead != matrix.linePointer.end(); ++i, ++lineHead){
        auto lineTail = *lineHead;
        unsigned int j = 0;
        for (auto columnHead = columnPointer.begin(); columnHead != columnPointer.end(); ++j, ++columnHead) {
            if(lineTail && lineTail == *columnHead) {
                auto element = lineTail;
                lineTail = lineTail->nextLine;
                *columnHead = (*columnHead)->nextColumn;
                element->nextColumn = element->nextLine = nullptr;
                delete element;
            }
        }
    }
    matrix.columnPointer.clear();
    matrix.linePointer.clear();
}
