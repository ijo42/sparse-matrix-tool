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
    std::cout << std::endl;
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
        if(!newLineTail)    // сохранение пустой строки
            newMatrix.linePointer.push_back(newLineTail);
    }
    return newMatrix;
}

unsigned int countElement(const SparseDoubleLinkedMatrix &matrix) {
    return matrix.columnPointer.size() * matrix.linePointer.size();
}

std::pair<long long unsigned int, long long unsigned int> matrixSize(const SparseDoubleLinkedMatrix &matrix) {
    return std::pair{matrix.columnPointer.size(), matrix.linePointer.size()};
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

SparseDoubleLinkedMatrix twoMatrixAccumulateOperation(const SparseDoubleLinkedMatrix& matrix1, const SparseDoubleLinkedMatrix& matrix2, const std::function<double(double,double)> &accumulateFunc) {
    SparseDoubleLinkedMatrix output{};
    if(matrixSize(matrix1) != matrixSize(matrix2)) { // неквадратные матрицы нельзя складывать
        return output;
    }

    // нули авто иницилизированы, т.к. без счётчика не обойтись
    output.columnPointer = std::vector<SparseDoubleLinkedMatrixElement*>(matrix1.columnPointer.size(), nullptr);
    output.linePointer = std::vector<SparseDoubleLinkedMatrixElement*>{};
    output.linePointer.reserve(matrix1.linePointer.size());           // нули будут заполнены вручную, что бы не вводить счетчик

    std::vector<SparseDoubleLinkedMatrixElement*>
                columnTail(matrix1.columnPointer.size()),
                columnTail1(matrix1.columnPointer.size()),
                columnTail2(matrix2.columnPointer.size());

    std::ranges::copy(matrix1.columnPointer, columnTail1.begin());
    std::ranges::copy(matrix2.columnPointer, columnTail2.begin());

    for (int i = 0; i < matrix1.linePointer.size(); i++) {
        auto lineTail = static_cast<SparseDoubleLinkedMatrixElement *>(nullptr),
            lineTail1 = matrix1.linePointer[i],
            lineTail2 = matrix2.linePointer[i];

        for (int j = 0; j < output.columnPointer.size(); ++j) {
            double value = 0.0;
            if(lineTail1 && lineTail1 == columnTail1[j]) {
                value = accumulateFunc(value, lineTail1->value);
                lineTail1 = lineTail1->nextLine;
                columnTail1[j] = columnTail1[j]->nextColumn;
            }
            if(lineTail2 && lineTail2 == columnTail2[j]) {
                value = accumulateFunc(value, lineTail2->value);
                lineTail2 = lineTail2->nextLine;
                columnTail2[j] = columnTail2[j]->nextColumn;
            }


            if(value != 0.0) {
                auto element = initElement(value);

                if(lineTail) { // не первый элемент в строке, связываем
                    lineTail->nextLine = element;
                } else {       // первый элемент в строке, сохраняем
                    output.linePointer.push_back(element);
                }
                lineTail = element;

                if(columnTail[j]) {
                    columnTail[j]->nextColumn = element;
                } else {
                    output.columnPointer[j] = element;
                }
                columnTail[j] = element;

            }
        }
        if(!lineTail) { // пустая строка
            output.linePointer.push_back(lineTail);
        }

    }
    return output;
}

SparseDoubleLinkedMatrix add(const SparseDoubleLinkedMatrix& matrix1, const SparseDoubleLinkedMatrix& matrix2) {
    return twoMatrixAccumulateOperation(matrix1, matrix2, [&](const double a, const double o) { return a+o; });
}

SparseDoubleLinkedMatrix sub(const SparseDoubleLinkedMatrix& matrix1, const SparseDoubleLinkedMatrix& matrix2) {
    return twoMatrixAccumulateOperation(matrix1, matrix2, [&](const double a, const double o) { return a-o; });
}
