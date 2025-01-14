#include "headers/operations.h"
#include <random>

/*
 * иницилизирует новый элемент, заполняя значение из параметра, либо NAN при отсутствии параметра
 * выставляет ссылки в NULL
 */
SparseDoubleLinkedMatrixElement *initElement(const double value) {
    auto element = new SparseDoubleLinkedMatrixElement;
    element->value = value;
    element->nextLine = element->nextColumn = nullptr;
    return element;
}

SparseDoubleLinkedMatrix *generateEmpty(const int n, const int m) {
    auto matrix = new SparseDoubleLinkedMatrix;
    matrix->columnPointer = std::vector<SparseDoubleLinkedMatrixElement *>(n, nullptr);
    matrix->linePointer = std::vector<SparseDoubleLinkedMatrixElement *>(m, nullptr);
    return matrix;
}


std::random_device rd;  // Используется только для инициализации
std::mt19937 gen(rd()); // Генератор, используемый для всех случайных чисел в программе
std::uniform_real_distribution<> distrib(1.0, 100.0);
std::uniform_int_distribution<> genDistrib(0, 10);

/*
 * генерирует матрицу
 * каждый элемент генерируется с вероятностью 0.1,
 * общее колличество сгенерированных элементов строго меньше 10% размера матрицы
 * n - кол-во столбцов, m - кол-во строк
 */
SparseDoubleLinkedMatrix *generateRnd(const int n, const int m) {
    size_t k = softMaxElements(n, m); // максимальное кол-во ненулевых


    auto matrix = new SparseDoubleLinkedMatrix;
    matrix->columnPointer = std::vector<SparseDoubleLinkedMatrixElement *>(n);
    matrix->linePointer = std::vector<SparseDoubleLinkedMatrixElement *>(m);
    auto columnTail = std::vector<SparseDoubleLinkedMatrixElement *>(n, nullptr),
            lineTail = std::vector<SparseDoubleLinkedMatrixElement *>(m, nullptr);

    for (int i = 0; i < n && k > 0; ++i) {      // вместо полного прохода прегенерировать непустые индексы
        for (int j = 0; j < m && k > 0; ++j) {
            if (genDistrib(gen) == 0) {
                auto element = initElement(distrib(gen));
                if (!matrix->columnPointer[i])
                    matrix->columnPointer[i] = columnTail[i] = element;
                else {
                    columnTail[i]->nextColumn = element;
                    columnTail[i] = columnTail[i]->nextColumn;
                }

                if (!matrix->linePointer[j])
                    matrix->linePointer[j] = lineTail[j] = element;
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


SparseDoubleLinkedMatrix *generateRndFifty(const int n, const int m) {
    int k = n * m / 2; // максимальное кол-во ненулевых

    auto matrix = new SparseDoubleLinkedMatrix;
    matrix->columnPointer = std::vector<SparseDoubleLinkedMatrixElement *>(n);
    matrix->linePointer = std::vector<SparseDoubleLinkedMatrixElement *>(m);
    auto columnTail = std::vector<SparseDoubleLinkedMatrixElement *>(n, nullptr),
            lineTail = std::vector<SparseDoubleLinkedMatrixElement *>(m, nullptr);

    for (int i = 0; i < n && k > 0; ++i) {      // вместо полного прохода прегенерировать непустые индексы
        for (int j = 0; j < m && k > 0; ++j) {
            if (rand() % 2 == 0) {
                auto element = initElement((1.0 + rand() % 1000) / 101.0);
                if (!matrix->columnPointer[i])
                    matrix->columnPointer[i] = columnTail[i] = element;
                else {
                    columnTail[i]->nextColumn = element;
                    columnTail[i] = columnTail[i]->nextColumn;
                }

                if (!matrix->linePointer[j])
                    matrix->linePointer[j] = lineTail[j] = element;
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

void printMatrix(const SparseDoubleLinkedMatrix *matrix) {
    if (!(matrix) || matrix->columnPointer.empty()) {
        std::cout << "Matrix is empty" << std::endl;
        return;
    }

    std::vector<SparseDoubleLinkedMatrixElement *> columnTail(matrix->columnPointer.begin(),
                                                              matrix->columnPointer.end());

    for (const auto lineHead: matrix->linePointer) {
        auto lineTail = lineHead;
        for (auto &i: columnTail) {
            if (lineTail && lineTail == i) {
                std::cout << std::setw(6) << std::fixed << std::setprecision(2) << i->value;
                lineTail = lineTail->nextLine;
                i = i->nextColumn;
            } else {
                std::cout << std::setw(6) << "NULL";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


SparseDoubleLinkedMatrix *deepCopy(const SparseDoubleLinkedMatrix &input) {
    auto output = new SparseDoubleLinkedMatrix();

    output->columnPointer = std::vector<SparseDoubleLinkedMatrixElement *>(input.columnPointer.size(), nullptr);
    output->linePointer = std::vector<SparseDoubleLinkedMatrixElement *>{};
    output->linePointer.reserve(input.linePointer.size());

    std::vector<SparseDoubleLinkedMatrixElement *> inputColumnTails(input.columnPointer.size());
    std::copy(input.columnPointer.begin(), input.columnPointer.end(), inputColumnTails.begin());

    std::vector<SparseDoubleLinkedMatrixElement *> outputColumnTails(input.columnPointer.size(), nullptr);

    for (auto lineHead: input.linePointer) {
        if (lineHead != nullptr) {
            auto outputLineTail = (SparseDoubleLinkedMatrixElement *) nullptr,
                    inputLineTail = lineHead;
            for (int j = 0; j < inputColumnTails.size(); ++j) {
                if (inputColumnTails[j] && inputColumnTails[j] == inputLineTail) {
                    auto element = initElement(inputLineTail->value);

                    if (outputLineTail) {
                        outputLineTail->nextLine = element;
                    } else {
                        output->linePointer.push_back(element);
                    }

                    if (outputColumnTails[j]) {
                        outputColumnTails[j]->nextColumn = element;
                    } else {
                        output->columnPointer[j] = element;
                    }

                    outputLineTail = outputColumnTails[j] = element;
                    inputLineTail = inputLineTail->nextLine;
                    inputColumnTails[j] = inputColumnTails[j]->nextColumn;
                }
            }
        } else {
            output->linePointer.push_back(nullptr);
        }
    }

    return output;
}

inline std::pair<size_t, size_t> matrixSize(const SparseDoubleLinkedMatrix &matrix) {
    return std::pair{matrix.columnPointer.size(), matrix.linePointer.size()};
}

/*
 * удаляет матрицу, а так же все элементы из ОЗУ
 * после операции класс матрицы имеет нулевые векторы
 */
void deepDelete(SparseDoubleLinkedMatrix &matrix) {
    std::vector<SparseDoubleLinkedMatrixElement *> columnHeads(matrix.columnPointer.size());
    std::copy(matrix.columnPointer.begin(), matrix.columnPointer.end(), columnHeads.begin());
    for (auto lineTail: matrix.linePointer) {
        for (auto &columnHead: columnHeads) {
            if (lineTail && lineTail == columnHead) {
                auto element = lineTail;
                lineTail = lineTail->nextLine;
                columnHead = columnHead->nextColumn;
                element->nextColumn = element->nextLine = nullptr;
                delete element;
            }
        }
    }
    matrix.columnPointer.clear(); // тут была проблема что мы просто очищали массив от элементов и все не удаляя сами массивы
    matrix.linePointer.clear();
    std::vector<SparseDoubleLinkedMatrixElement *>().swap(
            matrix.columnPointer); //здесь мы свапаем его с новым пустым вектором
    std::vector<SparseDoubleLinkedMatrixElement *>().swap(matrix.linePointer);

}

inline void addElement(int i, int j, SparseDoubleLinkedMatrix &output, SparseDoubleLinkedMatrixElement *&outputColumnTail,
                SparseDoubleLinkedMatrixElement *&outputColumnPrevTail,
                SparseDoubleLinkedMatrixElement *&outputLinePrevTail, SparseDoubleLinkedMatrixElement *&outputLineTail,
                SparseDoubleLinkedMatrixElement *&element) {

    if (outputLineTail && outputLineTail == outputColumnTail) { // элемент есть в обоих матрицах
        element = outputLineTail;
    } else {    // элемент есть только в 2 матрице
        element = initElement(0.0);
        /* 3 сценария: до головы С/С; между отстающим и хвостом; после хвоста*/
        if (outputLinePrevTail == nullptr ||
            outputLinePrevTail == outputLineTail) { // до отстающей головы
            element->nextLine = output.linePointer[i];
            output.linePointer[i] = element;
        } else {
            outputLinePrevTail->nextLine = element;
            element->nextLine = outputLineTail;
        }
        outputLinePrevTail = outputLineTail = element;

        if (outputColumnPrevTail == nullptr || outputColumnPrevTail == outputColumnTail) {
            element->nextColumn = output.columnPointer[j];
            output.columnPointer[j] = element;
        } else {
            outputColumnPrevTail->nextColumn = element;
            element->nextColumn = outputColumnTail;
        }
        outputColumnPrevTail = outputColumnTail = element;
    }
}

inline SparseDoubleLinkedMatrix *
twoMatrixAccumulateOperation(const SparseDoubleLinkedMatrix &matrix1, const SparseDoubleLinkedMatrix &matrix2,
                             const std::function<double(double, double)> &accumulateFunc) {
    if (matrixSize(matrix1) != matrixSize(matrix2)) { // неквадратные матрицы нельзя складывать / вычитать
        return nullptr;
    }

    auto output = deepCopy(matrix1);

    std::vector<SparseDoubleLinkedMatrixElement *>
            outputColumnTail(output->columnPointer),
            outputColumnPrevTail(output->columnPointer),
            input2ColumnTail(matrix2.columnPointer);

    for (int i = 0; i < matrix1.linePointer.size(); i++) {
        auto input2LineTail = matrix2.linePointer[i],
                outputLinePrevTail = output->linePointer[i],    // хвост отстающий
        outputLineTail = outputLinePrevTail; // хвост строки

        if (input2LineTail == nullptr && outputLineTail) {
            /* для пустой строки matrix2 необходимо пропустить указатели данной строки для matrix1 */
            for (int j = 0; j < output->columnPointer.size(); ++j) {
                if (outputLineTail && outputLineTail == outputColumnTail[j]) {
                    if (outputColumnTail[j] != outputColumnPrevTail[j]) {
                        outputColumnPrevTail[j] = outputColumnPrevTail[j]->nextColumn;
                    }
                    outputColumnTail[j] = outputColumnTail[j]->nextColumn;
                    outputLineTail = outputLineTail->nextLine;
                }
            }
        } else {
            for (int j = 0; j < output->columnPointer.size(); ++j) {
                bool flag = false;
                // есть элемент в 2 матрице
                if (input2LineTail && input2LineTail == input2ColumnTail[j]) {
                    flag = true;
                    SparseDoubleLinkedMatrixElement *element;
                    addElement(i, j, *output, outputColumnTail[j], outputColumnPrevTail[j], outputLinePrevTail,
                               outputLineTail, element);
                    element->value = accumulateFunc(element->value, input2LineTail->value);

                    input2LineTail = input2LineTail->nextLine;
                    input2ColumnTail[j] = input2ColumnTail[j]->nextColumn;

                }
                if (flag || (outputLineTail && outputLineTail == outputColumnTail[j])) {
                    // сдвиг хвостов при обработке элементов matrix2 / при пропуске элементов matrix1
                    if (outputLineTail) {
                        if (outputLinePrevTail != outputLineTail) {
                            outputLinePrevTail = outputLineTail;
                        }
                        outputLineTail = outputLineTail->nextLine;
                    }
                    if (outputColumnTail[j]) {
                        if (outputColumnPrevTail[j] != outputColumnTail[j]) {
                            outputColumnPrevTail[j] = outputColumnTail[j];
                        }
                        outputColumnTail[j] = outputColumnTail[j]->nextColumn;
                    }
                }
            }
        }
    }

    deleteZeroElements(*output);
    return output;
}

SparseDoubleLinkedMatrix *add(const SparseDoubleLinkedMatrix &matrix1, const SparseDoubleLinkedMatrix &matrix2) {
    return twoMatrixAccumulateOperation(matrix1, matrix2, [&](const double a, const double o) { return a + o; });
}

SparseDoubleLinkedMatrix *sub(const SparseDoubleLinkedMatrix &matrix1, const SparseDoubleLinkedMatrix &matrix2) {
    return twoMatrixAccumulateOperation(matrix1, matrix2, [&](const double a, const double o) { return a - o; });
}

/* генерация единичной матрицы размера n * n */
SparseDoubleLinkedMatrix *generateUnitMatrix(size_t n) {
    auto matrix = new SparseDoubleLinkedMatrix;
    matrix->columnPointer.reserve(n);
    matrix->linePointer.reserve(n);
    for (int i = 0; i < n; ++i) {
        auto element = initElement(1.0);

        matrix->linePointer.push_back(element);
        matrix->columnPointer.push_back(element);
    }
    return matrix;
}

/* добавляет все столбцы из матрицы matrix2 справа к матрице matrix1 */
inline void joinMatrix(SparseDoubleLinkedMatrix &matrix1, SparseDoubleLinkedMatrix &matrix2) {
    if (matrix1.linePointer.size() != matrix2.linePointer.size()) {
        return;
    }
    matrix1.columnPointer.insert(matrix1.columnPointer.end(),
                                 matrix2.columnPointer.begin(), matrix2.columnPointer.end());
    for (size_t i = 0; i < matrix1.linePointer.size(); i++) {
        auto lineTail = matrix1.linePointer[i];
        if (!lineTail) {
            matrix1.linePointer[i] = matrix2.linePointer[i];
        } else {
            while (lineTail->nextLine) {
                lineTail = lineTail->nextLine;
            }
            lineTail->nextLine = matrix2.linePointer[i];
        }
    }
}

/* генерация обратной матрицы */
SparseDoubleLinkedMatrix *
inverseMatrix(SparseDoubleLinkedMatrix &mainMatrix) { //обратная матрица использующая идеи перестановки строк

    if (matrixSize(mainMatrix).first != matrixSize(mainMatrix).second) return nullptr;//проверяем матрицу на валидность

    size_t rowit;

    for (rowit = 0; rowit < mainMatrix.linePointer.size(); rowit++)
        if (mainMatrix.linePointer[rowit] == nullptr || mainMatrix.columnPointer[rowit] == nullptr)
            return nullptr; //проверяем матрицу на валидность

    auto matrix = deepCopy(mainMatrix),
            UnitMatrix = generateUnitMatrix(matrix->linePointer.size());
    joinMatrix(*matrix, *UnitMatrix);

    SparseDoubleLinkedMatrixElement *prevLineTail, *lineTail, *lineHead, *element, *searchedElement, *doplineTail, *rowTail, *prevRowTail;
    std::vector<double> columnTailAvEl(matrix->columnPointer.size()),
            columnTailDopAvEl(matrix->columnPointer.size());
    std::vector<SparseDoubleLinkedMatrixElement *> columnTail(matrix->columnPointer.size()),
            prevColumnTail(matrix->columnPointer.size()),
            dopColumnTail(matrix->columnPointer.size()),
            SkibidiColumnTail(matrix->columnPointer.size()),
            midColumnTail(matrix->columnPointer),
            midPrevColumnTail(columnTail);
    double koef, delitelexist, delitel;

    bool found, stopflag;

    size_t counter, columnit, column1it, doprowit, dopcolumnit, count, i;

    for (counter = 0; counter < matrix->linePointer.size(); counter++) {

        std::copy(midColumnTail.begin(), midColumnTail.end(), columnTail.begin());
        std::copy(midPrevColumnTail.begin(), midPrevColumnTail.end(), prevColumnTail.begin());

        stopflag = false;
        lineHead = matrix->linePointer[counter];
        delitelexist = 1;
        for (column1it = 0; column1it < matrix->columnPointer.size(); column1it++) {
            if ((lineHead && lineHead == columnTail[column1it]) && fabs(lineHead->value) != 0) {// если элемент найден
                if (counter == column1it) delitelexist = lineHead->value;
                lineHead->value /= delitelexist;
                columnTailAvEl[column1it] = lineHead->value;
                lineHead = lineHead->nextLine;
            } else {
                if ((lineHead && lineHead == columnTail[column1it]) && fabs(lineHead->value) == 0) {
                    lineHead = lineHead->nextLine;
                    columnTailAvEl[column1it] = 0.0;
                }
                if (column1it == counter) { // если диагональный элемент отсутсвует то ищем ниже
                    found = false;
                    searchedElement = columnTail[counter];
                    while (searchedElement && !found) {
                        if (searchedElement->value != 0.0) found = true;
                        else searchedElement = searchedElement->nextColumn;
                    }

                    if (!found) {
                        deepDelete(*matrix);
                        return nullptr;
                    } //определитель равен нулю удобная проверка

                    delitel = searchedElement->value;

                    std::copy(columnTail.begin(), columnTail.end(), dopColumnTail.begin());
                    doprowit = counter;
                    while (doprowit < matrix->linePointer.size() && !stopflag) {
                        doplineTail = matrix->linePointer[doprowit];
                        dopcolumnit = 0;
                        while (dopcolumnit < matrix->columnPointer.size()) {
                            if (doplineTail && doplineTail == dopColumnTail[dopcolumnit]) {
                                if (doplineTail == searchedElement) stopflag = true;
                                columnTailDopAvEl[dopcolumnit] = doplineTail->value;
                                doplineTail = doplineTail->nextLine;
                                dopColumnTail[dopcolumnit] = dopColumnTail[dopcolumnit]->nextColumn;
                            } else {
                                columnTailDopAvEl[dopcolumnit] = 0.0;
                            }
                            dopcolumnit++;
                        }
                        doprowit++;
                    }
                }

            }

        }

        if (stopflag) { //если на главной диагонали не было то нам нужно прибавить одну строку на другую, которую мы нашли в предидущем цикле
            rowTail = matrix->linePointer[counter];
            prevRowTail = rowTail;
            for (i = 0; i < columnTail.size(); i++) {
                if (rowTail && rowTail == columnTail[i]) {// если элемент найден
                    rowTail->value = (rowTail->value + columnTailDopAvEl[i]) / delitel;
                    columnTailAvEl[i] = rowTail->value;
                    prevRowTail = rowTail;
                    rowTail = rowTail->nextLine;

                } else {
                    if (columnTailDopAvEl[i] != 0.0) {
                        element = initElement(columnTailDopAvEl[i] / delitel);
                        if (rowTail != matrix->linePointer[counter]) { // не первый элемент в строке, связываем
                            element->nextLine = rowTail;
                            prevRowTail->nextLine = element;
                        } else {       // первый элемент в строке, сохраняем
                            element->nextLine = matrix->linePointer[counter];
                            matrix->linePointer[counter] = element;
                        }
                        rowTail = element;

                        if (columnTail[i] != matrix->columnPointer[i]) {
                            element->nextColumn = columnTail[i];
                            prevColumnTail[i]->nextColumn = element;
                        } else {
                            element->nextColumn = matrix->columnPointer[i];
                            matrix->columnPointer[i] = element;
                        }
                        columnTailAvEl[i] = rowTail->value;
                        columnTail[i] = element;
                        prevRowTail = rowTail;
                        rowTail = rowTail->nextLine;
                    } else
                        columnTailAvEl[i] = 0;
                }
            }
        }

        lineTail = matrix->linePointer[counter];
        for (columnit = 0; columnit < matrix->columnPointer.size(); columnit++) {
            if (lineTail && lineTail == columnTail[columnit]) {// если элемент найден
                lineTail = lineTail->nextLine;
                prevColumnTail[columnit] = columnTail[columnit];
                columnTail[columnit] = columnTail[columnit]->nextColumn;
            }
        }

        std::copy(columnTail.begin(), columnTail.end(), midColumnTail.begin());
        std::copy(prevColumnTail.begin(), prevColumnTail.end(), midPrevColumnTail.begin());


        for (rowit = counter + 1; rowit < matrix->linePointer.size(); rowit++) {

            koef = 0;
            lineTail = matrix->linePointer[rowit];
            prevLineTail = matrix->linePointer[rowit];
            for (columnit = 0; columnit < matrix->columnPointer.size(); columnit++) {
                if (lineTail && lineTail == columnTail[columnit]) {// если элемент найден
                    if (columnit == counter) {
                        koef = columnTail[columnit]->value / columnTailAvEl[columnit];
                    }
                    lineTail->value -= koef * columnTailAvEl[columnit];
                    prevLineTail = lineTail;
                    lineTail = lineTail->nextLine;
                    prevColumnTail[columnit] = columnTail[columnit];
                    columnTail[columnit] = columnTail[columnit]->nextColumn;
                } else {
                    if (columnTailAvEl[columnit] && koef) {
                        element = initElement(-columnTailAvEl[columnit] * koef);
                        if (lineTail != matrix->linePointer[rowit]) { // не первый элемент в строке, связываем
                            element->nextLine = lineTail;
                            prevLineTail->nextLine = element;
                        } else {       // первый элемент в строке, сохраняем
                            element->nextLine = matrix->linePointer[rowit];
                            matrix->linePointer[rowit] = element;
                        }
                        lineTail = element;

                        if (columnTail[columnit] != matrix->columnPointer[columnit]) {
                            element->nextColumn = columnTail[columnit];
                            prevColumnTail[columnit]->nextColumn = element;
                        } else {
                            element->nextColumn = matrix->columnPointer[columnit];
                            matrix->columnPointer[columnit] = element;
                        }

                        prevColumnTail[columnit] = element;
                        columnTail[columnit] = element->nextColumn;
                        prevLineTail = lineTail;
                        lineTail = lineTail->nextLine;
                        midColumnTail[columnit] = midPrevColumnTail[columnit]->nextColumn;
                    }
                }
            }
        }
    }
    //ну вообще предидущих вычислений хватит для того чтобы найти определитель
    //вычитаем вверх




    std::copy(matrix->columnPointer.begin(), matrix->columnPointer.end(), dopColumnTail.begin());

    lineHead = matrix->linePointer[0];
    for (column1it = 0; column1it < matrix->columnPointer.size(); column1it++) {
        if (lineHead && lineHead == dopColumnTail[column1it]) { // если элемент найден
            columnTailAvEl[column1it] = lineHead->value;
            lineHead = lineHead->nextLine;
            dopColumnTail[column1it] = dopColumnTail[column1it]->nextColumn;
        } else {
            columnTailAvEl[column1it] = 0;
        }
    }

    for (counter = 0; counter < matrix->linePointer.size() - 1; counter++) {


        lineHead = matrix->linePointer[counter + 1];
        for (column1it = 0; column1it < matrix->columnPointer.size(); column1it++) {
            if (lineHead && lineHead == dopColumnTail[column1it]) { // если элемент найден
                columnTailAvEl[column1it] = lineHead->value;
                lineHead = lineHead->nextLine;
                dopColumnTail[column1it] = dopColumnTail[column1it]->nextColumn;
            } else {
                columnTailAvEl[column1it] = 0;
            }
        }


        std::copy(matrix->columnPointer.begin(), matrix->columnPointer.end(), columnTail.begin());
        std::copy(columnTail.begin(), columnTail.end(), prevColumnTail.begin());

        for (rowit = 0; rowit < counter + 1; rowit++) {
            koef = 0;
            lineTail = matrix->linePointer[rowit];
            prevLineTail = matrix->linePointer[rowit];
            for (columnit = 0; columnit < matrix->columnPointer.size(); columnit++) {
                if (lineTail && lineTail == columnTail[columnit]) {// если элемент найден
                    if (columnit == counter + 1 && columnTail[counter + 1]) {
                        if (columnTailAvEl[columnit] == 0) {
                            deepDelete(*matrix);
                            return nullptr;
                        }
                        koef = columnTail[columnit]->value / columnTailAvEl[columnit];
                    }
                    lineTail->value -= koef * columnTailAvEl[columnit];
                    prevLineTail = lineTail;
                    lineTail = lineTail->nextLine;
                    prevColumnTail[columnit] = columnTail[columnit];
                    columnTail[columnit] = columnTail[columnit]->nextColumn;
                } else {
                    if (koef && columnTailAvEl[columnit]) {
                        element = initElement(-columnTailAvEl[columnit] * koef);
                        if (lineTail != matrix->linePointer[rowit]) { // не первый элемент в строке, связываем
                            element->nextLine = lineTail;
                            prevLineTail->nextLine = element;
                        } else {       // первый элемент в строке, сохраняем
                            element->nextLine = matrix->linePointer[rowit];
                            matrix->linePointer[rowit] = element;
                        }
                        lineTail = element;

                        if (columnTail[columnit] != matrix->columnPointer[columnit]) {
                            element->nextColumn = columnTail[columnit];
                            prevColumnTail[columnit]->nextColumn = element;
                        } else {
                            element->nextColumn = matrix->columnPointer[columnit];
                            matrix->columnPointer[columnit] = element;
                        }

                        prevColumnTail[columnit] = element;
                        columnTail[columnit] = element->nextColumn;
                        prevLineTail = lineTail;
                        lineTail = lineTail->nextLine;
                    }
                }
            }
        }


    }



    //обрезаем лишнее
    std::copy(matrix->columnPointer.begin(), matrix->columnPointer.end(), columnTail.begin());
    for (rowit = 0; rowit < matrix->linePointer.size(); rowit++) {
        lineTail = matrix->linePointer[rowit];
        for (columnit = 0; columnit < matrix->columnPointer.size(); columnit++) {
            if (lineTail && lineTail == columnTail[columnit]) {
                if (columnit == matrix->linePointer.size()) UnitMatrix->linePointer[rowit] = lineTail;
                lineTail = lineTail->nextLine;
                columnTail[columnit] = columnTail[columnit]->nextColumn;
            }
        }
        UnitMatrix->columnPointer[rowit] = matrix->columnPointer[matrix->linePointer.size() + rowit];
    }

    matrix->columnPointer.resize(matrix->linePointer.size());


    deepDelete(*matrix);


    //удаляем нули (почему мы убираем только в самом конце)
    //если убирать походу программы будут постоянно возникать нули и на эти нули записываться элементы по несколько раз что скажется на производительности
    deleteZeroElements(*UnitMatrix);

    return UnitMatrix;
}


SparseDoubleLinkedMatrix *multiply(SparseDoubleLinkedMatrix &matrix1, SparseDoubleLinkedMatrix &matrix2) {

    if (matrixSize(matrix1).second != matrixSize(matrix2).first)
        return nullptr; // вот мне не нравится как тут ошибка выводится, мб написать функцию которая делает пустую матрицу?

    auto output = new SparseDoubleLinkedMatrix;
    output->columnPointer = std::vector<SparseDoubleLinkedMatrixElement *>(matrix1.linePointer.size(), nullptr);
    output->linePointer = std::vector<SparseDoubleLinkedMatrixElement *>(matrix2.columnPointer.size(), nullptr);

    std::vector<SparseDoubleLinkedMatrixElement *> columnTails(matrix1.columnPointer),
            lineTails(matrix2.linePointer.size()),
            resColumnTails(matrix2.columnPointer.size(), nullptr),
            resColumn1Tails(matrix2.columnPointer.size(), nullptr);

    SparseDoubleLinkedMatrixElement *lineTail,
            *reslineTail,
            *prevreslineTail,
            *element,
            *columnTail;

    std::vector<double> matrix1LineEls(matrix1.columnPointer.size());
    double elValue;

    size_t rowit, columnit, column, line;

    for (rowit = 0; rowit < output->linePointer.size(); rowit++) {

        lineTail = matrix1.linePointer[rowit];
        reslineTail = nullptr;
        prevreslineTail = nullptr;

        for (column = 0; column < matrix1.columnPointer.size(); column++) {
            if (lineTail && lineTail == columnTails[column]) {// если элемент найден
                matrix1LineEls[column] = lineTail->value;
                lineTail = lineTail->nextLine;
                columnTails[column] = columnTails[column]->nextColumn;
            } else {
                matrix1LineEls[column] = 0.0;
            }
        }

        std::copy(matrix2.linePointer.begin(), matrix2.linePointer.end(), lineTails.begin());
        for (columnit = 0; columnit < output->columnPointer.size(); columnit++) {

            columnTail = matrix2.columnPointer[columnit];
            elValue = 0.0;

            for (line = 0; line < matrix2.linePointer.size(); line++) {
                if (columnTail && columnTail == lineTails[line]) {// если элемент найден
                    elValue += columnTail->value * matrix1LineEls[line];
                    columnTail = columnTail->nextColumn;
                    lineTails[line] = lineTails[line]->nextLine;
                }
            }


            if (elValue != 0.0) {
                element = initElement(elValue);
                if (reslineTail != output->linePointer[rowit]) { // не первый элемент в строке, связываем
                    element->nextLine = lineTail;
                    prevreslineTail->nextLine = element; // TODO: may by null (по логике не может быть иначе киньте тесты)
                } else {       // первый элемент в строке, сохраняем
                    element->nextLine = output->linePointer[rowit];
                    output->linePointer[rowit] = element;
                }
                reslineTail = element;

                if (resColumnTails[columnit] != output->columnPointer[columnit]) {
                    element->nextColumn = resColumnTails[columnit];
                    resColumn1Tails[columnit]->nextColumn = element;
                } else {
                    element->nextColumn = output->columnPointer[columnit];
                    output->columnPointer[columnit] = element;
                }

                resColumn1Tails[columnit] = element;
                resColumnTails[columnit] = element->nextColumn;
                prevreslineTail = reslineTail;
                reslineTail = reslineTail->nextLine;
            }

        }
    }

    deleteZeroElements(*output);
    return output;
}

SparseDoubleLinkedMatrix *setElement(SparseDoubleLinkedMatrix *&matrix, int row, int column, double value) {
    if (column > matrix->columnPointer.size() || row > matrix->linePointer.size())
        return nullptr;

    std::vector<SparseDoubleLinkedMatrixElement *>
            outputColumnTail(matrix->columnPointer),
            outputColumnPrevTail(matrix->columnPointer);

    for (int i = 0; i < matrix->linePointer.size(); i++) {
        auto outputLinePrevTail = matrix->linePointer[i],    // хвост отстающий
        outputLineTail = outputLinePrevTail;            // хвост строки
        for (int j = 0; j < matrix->columnPointer.size(); ++j) {
            // есть элемент в 2 матрице
            if (row == i && column == j) {
                SparseDoubleLinkedMatrixElement *element;
                addElement(i, j, *matrix, outputColumnTail[j], outputColumnPrevTail[j], outputLinePrevTail,
                           outputLineTail, element);
                element->value = value;
                break;
            }
            if (outputLineTail && outputLineTail == outputColumnTail[j]) {
                // сдвиг хвостов при обработке элементов matrix2 / при пропуске элементов matrix1
                if (outputLineTail) {
                    if (outputLinePrevTail != outputLineTail) {
                        outputLinePrevTail = outputLineTail;
                    }
                    outputLineTail = outputLineTail->nextLine;
                }
                if (outputColumnTail[j]) {
                    if (outputColumnPrevTail[j] != outputColumnTail[j]) {
                        outputColumnPrevTail[j] = outputColumnTail[j];
                    }
                    outputColumnTail[j] = outputColumnTail[j]->nextColumn;
                }
            }
        }
    }

    deleteZeroElements(*matrix);
    return matrix;
}


void deleteZeroElements(SparseDoubleLinkedMatrix &matrix) {
    size_t rowit, columnit;
    bool sw1, sw2;
    std::vector<SparseDoubleLinkedMatrixElement *> columnTail(matrix.columnPointer.size()),
            prevColumnTail(matrix.columnPointer.size());
    std::copy(matrix.columnPointer.begin(), matrix.columnPointer.end(), columnTail.begin());
    std::copy(matrix.columnPointer.begin(), matrix.columnPointer.end(), prevColumnTail.begin());
    for (rowit = 0; rowit < matrix.linePointer.size(); rowit++) {
        auto lineTail = matrix.linePointer[rowit];
        auto prevLineTail = lineTail;
        for (columnit = 0; columnit < matrix.columnPointer.size(); columnit++) {
            if (lineTail && lineTail == columnTail[columnit]) {
                if (fabs(lineTail->value) < MIN_ZNACH) {
                    sw1 = false;
                    sw2 = false;
                    if (prevLineTail != lineTail) prevLineTail->nextLine = lineTail->nextLine;
                    else {
                        matrix.linePointer[rowit] = lineTail->nextLine;
                        prevLineTail = matrix.linePointer[rowit];
                        sw1 = 1;
                    }
                    if (prevColumnTail[columnit] != columnTail[columnit])
                        prevColumnTail[columnit]->nextColumn = columnTail[columnit]->nextColumn;
                    else {
                        matrix.columnPointer[columnit] = columnTail[columnit]->nextColumn;
                        prevColumnTail[columnit] = matrix.columnPointer[columnit];
                        sw2 = 1;
                    }
                    lineTail->nextColumn = nullptr;
                    lineTail->nextLine = nullptr;
                    free(lineTail);
                    if (sw1) lineTail = prevLineTail;
                    else lineTail = prevLineTail->nextLine;
                    if (sw2)columnTail[columnit] = prevColumnTail[columnit];
                    else columnTail[columnit] = prevColumnTail[columnit]->nextLine;
                } else {
                    prevLineTail = lineTail;
                    lineTail = lineTail->nextLine;
                    prevColumnTail[columnit] = columnTail[columnit];
                    columnTail[columnit] = columnTail[columnit]->nextColumn;
                }
            }
        }
    }
}
