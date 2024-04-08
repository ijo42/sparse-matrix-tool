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
    int k = maxElements(n,m); // максимальное кол-во ненулевых

    auto matrix = SparseDoubleLinkedMatrix{};
    matrix.columnPointer = std::vector<SparseDoubleLinkedMatrixElement*>(n);
    matrix.linePointer = std::vector<SparseDoubleLinkedMatrixElement*>(m);
    auto columnTail = std::vector<SparseDoubleLinkedMatrixElement*>(n, nullptr),
            lineTail = std::vector<SparseDoubleLinkedMatrixElement*>(m, nullptr);

    for (int i = 0; i < n && k > 0; ++i) {      // вместо полного прохода прегенерировать непустые индексы
        for (int j = 0; j < m && k > 0; ++j) {
            if(rand() % 10 == 0){
                auto element = initElement((1.0 + rand() % 1000) / 101.0);
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


SparseDoubleLinkedMatrix generateRndFifty(const int n, const int m) {
    int k = n * m / 2; // максимальное кол-во ненулевых

    auto matrix = SparseDoubleLinkedMatrix{};
    matrix.columnPointer = std::vector<SparseDoubleLinkedMatrixElement*>(n);
    matrix.linePointer = std::vector<SparseDoubleLinkedMatrixElement*>(m);
    auto columnTail = std::vector<SparseDoubleLinkedMatrixElement*>(n, nullptr),
        lineTail = std::vector<SparseDoubleLinkedMatrixElement*>(m, nullptr);

    for (int i = 0; i < n && k > 0; ++i) {      // вместо полного прохода прегенерировать непустые индексы
        for (int j = 0; j < m && k > 0; ++j) {
            if (rand() % 2 == 0) {
                auto element = initElement((1.0 + rand() % 1000) / 101.0);
                if (!matrix.columnPointer[i])
                    matrix.columnPointer[i] = columnTail[i] = element;
                else {
                    columnTail[i]->nextColumn = element;
                    columnTail[i] = columnTail[i]->nextColumn;
                }

                if (!matrix.linePointer[j])
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


SparseDoubleLinkedMatrix deepCopy(const SparseDoubleLinkedMatrix& input) {
    auto output = SparseDoubleLinkedMatrix();

    output.columnPointer = std::vector<SparseDoubleLinkedMatrixElement*>(input.columnPointer.size(), nullptr);
    output.linePointer = std::vector<SparseDoubleLinkedMatrixElement*>{};
    output.linePointer.reserve(input.linePointer.size());

    std::vector<SparseDoubleLinkedMatrixElement*> inputColumnTails(input.columnPointer.size());
    std::ranges::copy(input.columnPointer, inputColumnTails.begin());

    std::vector<SparseDoubleLinkedMatrixElement*> outputColumnTails(input.columnPointer.size(), nullptr);

    for (auto lineHead : input.linePointer) {
        if (lineHead != nullptr) {
            auto outputLineTail = (SparseDoubleLinkedMatrixElement *) nullptr,
                inputLineTail = lineHead;
            for (int j = 0; j < inputColumnTails.size(); ++j) {
                if (inputColumnTails[j] && inputColumnTails[j] == inputLineTail){
                    auto element = initElement(inputLineTail->value);

                    if(outputLineTail) {
                        outputLineTail->nextLine = element;
                    } else {
                        output.linePointer.push_back(element);
                    }

                    if (outputColumnTails[j]) {
                        outputColumnTails[j]->nextColumn = element;
                    } else {
                        output.columnPointer[j] = element;
                    }

                    outputLineTail = outputColumnTails[j] = element;
                    inputLineTail = inputLineTail->nextLine;
                    inputColumnTails[j] = inputColumnTails[j]->nextColumn;
                }
            }
        } else {
            output.linePointer.push_back(nullptr);
        }
    }

    return output;
}

size_t countElement(const SparseDoubleLinkedMatrix &matrix) {
    return matrix.columnPointer.size() * matrix.linePointer.size();
}

std::pair<size_t, size_t> matrixSize(const SparseDoubleLinkedMatrix &matrix) {
    return std::pair{matrix.columnPointer.size(), matrix.linePointer.size()};
}

/*
 * удаляет матрицу, а так же все элементы из ОЗУ
 * после операции класс матрицы имеет нулевые векторы
 */
void deepDelete(SparseDoubleLinkedMatrix& matrix) {
    std::vector<SparseDoubleLinkedMatrixElement*> columnHeads(matrix.columnPointer.size());
    std::ranges::copy(matrix.columnPointer, columnHeads.begin());
    for(auto lineTail : matrix.linePointer){
        for (auto & columnHead : columnHeads) {
            if(lineTail && lineTail == columnHead) {
                auto element = lineTail;
                lineTail = lineTail->nextLine;
                columnHead = columnHead->nextColumn;
                element->nextColumn = element->nextLine = nullptr;
                delete element;
            }
        }
    }
    matrix.columnPointer.clear();
    matrix.linePointer.clear();
}

void addElement(int i, int j, SparseDoubleLinkedMatrix &output, std::vector<SparseDoubleLinkedMatrixElement *> &outputColumnTail,
           std::vector<SparseDoubleLinkedMatrixElement *> &outputColumnPrevTail,
           SparseDoubleLinkedMatrixElement *&outputLinePrevTail, SparseDoubleLinkedMatrixElement *&outputLineTail,
           SparseDoubleLinkedMatrixElement *&element) {

    if (outputLineTail && outputLineTail == outputColumnTail[j]) { // элемент есть в обоих матрицах
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

        if (outputColumnPrevTail[j] == nullptr || outputColumnPrevTail[j] == outputColumnTail[j]) {
            element->nextColumn = output.columnPointer[j];
            output.columnPointer[j] = element;
        } else {
            outputColumnPrevTail[j]->nextColumn = element;
            element->nextColumn = outputColumnTail[j];
        }
        outputColumnPrevTail[j] = outputColumnTail[j] = element;
    }
}

SparseDoubleLinkedMatrix twoMatrixAccumulateOperation(const SparseDoubleLinkedMatrix& matrix1, const SparseDoubleLinkedMatrix& matrix2, const std::function<double(double, double)> &accumulateFunc) {
    SparseDoubleLinkedMatrix output{};
    if(matrixSize(matrix1) != matrixSize(matrix2)) { // неквадратные матрицы нельзя складывать / вычитать
        return output;
    }

    output = deepCopy(matrix1);

    std::vector<SparseDoubleLinkedMatrixElement*>
                outputColumnTail(output.columnPointer.size()),
                outputColumnPrevTail(output.columnPointer.size()),
                input2ColumnTail(matrix2.columnPointer.size());

    std::ranges::copy(output.columnPointer, outputColumnTail.begin());
    std::ranges::copy(output.columnPointer, outputColumnPrevTail.begin());
    std::ranges::copy(matrix2.columnPointer, input2ColumnTail.begin());

    for (int i = 0; i < matrix1.linePointer.size(); i++) {
        auto input2LineTail = matrix2.linePointer[i],
             outputLinePrevTail = output.linePointer[i],    // хвост отстающий
             outputLineTail = outputLinePrevTail; // хвост строки

        if (input2LineTail == nullptr && outputLineTail) {
            /* для пустой строки matrix2 необходимо пропустить указатели данной строки для matrix1 */
            for (int j = 0; j < output.columnPointer.size(); ++j) {
                if (outputLineTail && outputLineTail == outputColumnTail[j]) {
                    if (outputColumnTail[j] != outputColumnPrevTail[j]) {
                        outputColumnPrevTail[j] = outputColumnPrevTail[j]->nextColumn;
                    }
                    outputColumnTail[j] = outputColumnTail[j]->nextColumn;
                    outputLineTail = outputLineTail->nextLine;
                }
            }
        } else {
            for (int j = 0; j < output.columnPointer.size(); ++j) {
                bool flag = false;
                // есть элемент в 2 матрице
                if (input2LineTail && input2LineTail == input2ColumnTail[j]) {
                    flag = true;
                    SparseDoubleLinkedMatrixElement *element;
                    addElement(i, j, output, outputColumnTail, outputColumnPrevTail, outputLinePrevTail, outputLineTail,
                               element);
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
    return output;
}

SparseDoubleLinkedMatrix add(const SparseDoubleLinkedMatrix& matrix1, const SparseDoubleLinkedMatrix& matrix2) {
    return twoMatrixAccumulateOperation(matrix1, matrix2, [&](const double a, const double o) { return a+o; });
}

SparseDoubleLinkedMatrix sub(const SparseDoubleLinkedMatrix& matrix1, const SparseDoubleLinkedMatrix& matrix2) {
    return twoMatrixAccumulateOperation(matrix1, matrix2, [&](const double a, const double o) { return a-o; });
}

/* генерация единичной матрицы размера n * n */
SparseDoubleLinkedMatrix generateUnitMatrix(size_t n) {
    auto matrix = SparseDoubleLinkedMatrix{};
    matrix.columnPointer.reserve(n);
    matrix.linePointer.reserve(n);
    for (int i = 0; i < n; ++i) {
        auto element = initElement(1.0);

        matrix.linePointer.push_back(element);
        matrix.columnPointer.push_back(element);
    }
    return matrix;
}

/* добавляет все столбцы из матрицы matrix2 справа к матрице matrix1 */
void joinMatrix(SparseDoubleLinkedMatrix& matrix1, SparseDoubleLinkedMatrix& matrix2) {
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
SparseDoubleLinkedMatrix inverseMatrix(SparseDoubleLinkedMatrix& mainMatrix) {

    if (matrixSize(mainMatrix).first != matrixSize(mainMatrix).second) {
        // обратная матрицы существует лишь для квадратной матрицы
        return mainMatrix;
    }

    for (size_t rowit = 0; rowit < mainMatrix.linePointer.size(); rowit++)
        if (mainMatrix.linePointer[rowit] == nullptr ||
            mainMatrix.columnPointer[rowit] == nullptr) {
            // если существует хоть один нулевой столбце или строка, обратной матрицы не существует
            return mainMatrix;
        }

    auto matrix = deepCopy(mainMatrix);

    auto unitMatrix = generateUnitMatrix(matrix.linePointer.size());
    joinMatrix(matrix, unitMatrix);

    SparseDoubleLinkedMatrixElement *prevLineTail, *lineTail, *lineHead;
    std::vector<SparseDoubleLinkedMatrixElement*> columnTail(matrix.columnPointer.size()),
                                                prevColumnTail(matrix.columnPointer.size());
    std::vector<double> columnTailAvEl(matrix.columnPointer.size());
    double koef;


    std::ranges::copy(matrix.columnPointer, columnTail.begin());
    std::ranges::copy(columnTail, prevColumnTail.begin());

    for (size_t rowit = 0; rowit < matrix.linePointer.size(); rowit++) {
        lineTail = matrix.linePointer[rowit];
        prevLineTail = lineTail;
        for (size_t columnit = 0; columnit < matrix.columnPointer.size(); columnit++) {
            if (lineTail && lineTail == columnTail[columnit]) {
                prevLineTail = lineTail;
                lineTail = lineTail->nextLine;
                prevColumnTail[columnit] = columnTail[columnit];
                columnTail[columnit] = columnTail[columnit]->nextColumn;
            }
            else {
                if (columnit == rowit) {
                    auto element = initElement(MIN_ZNACH);
                    if (lineTail != matrix.linePointer[rowit]) { // не первый элемент в строке, связываем
                        element->nextLine = lineTail;
                        prevLineTail->nextLine = element;
                    }
                    else {       // первый элемент в строке, сохраняем
                        element->nextLine = matrix.linePointer[rowit];
                        matrix.linePointer[rowit] = element;
                    }
                    lineTail = element;

                    if (columnTail[columnit] != matrix.columnPointer[columnit]) {
                        element->nextColumn = columnTail[columnit];
                        prevColumnTail[columnit]->nextColumn = element;
                    }
                    else {
                        element->nextColumn = matrix.columnPointer[columnit];
                        matrix.columnPointer[columnit] = element;
                    }

                    prevColumnTail[columnit] = element;
                    columnTail[columnit] = element->nextColumn;
                    prevLineTail = lineTail;
                    lineTail = lineTail->nextLine;
                }
            }
        }
    }



    //вычитаем вниз
    for (size_t counter = 0; counter < matrix.linePointer.size(); counter++) {

        std::ranges::copy(matrix.columnPointer, columnTail.begin());
        std::ranges::copy(columnTail, prevColumnTail.begin());

        for (size_t rowit = 0; rowit < matrix.linePointer.size(); rowit++) {
            lineHead = matrix.linePointer[rowit];
            if (rowit == counter) {
                for (size_t column1it = 0; column1it < matrix.columnPointer.size(); column1it++) {
                    if (lineHead && lineHead == columnTail[column1it]) {// если элемент найден
                        columnTailAvEl[column1it] = lineHead->value;
                        lineHead = lineHead->nextLine;
                    }
                    else {
                        columnTailAvEl[column1it] = 0;
                    }
                }
            }

            koef = 0.0;
            lineTail = matrix.linePointer[rowit];
            prevLineTail = matrix.linePointer[rowit];
            for (size_t columnit = 0; columnit < matrix.columnPointer.size(); columnit++) {
                if (lineTail && lineTail == columnTail[columnit]) {// если элемент найден
                    if (rowit > counter && columnit == counter) {
                        if (columnTail[counter]) {
                            koef = columnTail[columnit]->value / columnTailAvEl[columnit];
                        }
                    }
                    lineTail->value -= koef * columnTailAvEl[columnit];
                    prevLineTail = lineTail;
                    lineTail = lineTail->nextLine;
                    prevColumnTail[columnit] = columnTail[columnit];
                    columnTail[columnit] = columnTail[columnit]->nextColumn;
                }
                else {
                    if (rowit > counter) {
                        if (koef != 0.0 && columnTailAvEl[columnit] != 0.0) {
                            auto element = initElement(-columnTailAvEl[columnit] * koef);
                            if (lineTail != matrix.linePointer[rowit]) { // не первый элемент в строке, связываем
                                element->nextLine = lineTail;
                                prevLineTail->nextLine = element;
                            }
                            else {       // первый элемент в строке, сохраняем
                                element->nextLine = matrix.linePointer[rowit];
                                matrix.linePointer[rowit] = element;
                            }
                            lineTail = element;

                            if (columnTail[columnit] != matrix.columnPointer[columnit]) {
                                element->nextColumn = columnTail[columnit];
                                prevColumnTail[columnit]->nextColumn = element;
                            }
                            else {
                                element->nextColumn = matrix.columnPointer[columnit];
                                matrix.columnPointer[columnit] = element;
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
    }
    //ну вообще предидущих вычислений хватит для того чтобы найти определитель

    //вычитаем вверх
    for (size_t counter = 0; counter < matrix.linePointer.size() - 1; counter++) {

        std::ranges::copy(matrix.columnPointer, columnTail.begin());
        std::ranges::copy(columnTail, prevColumnTail.begin());
        for (size_t rowit = 0; rowit <= counter + 1; rowit++) {

            lineHead = matrix.linePointer[rowit];
            for (size_t column1it = 0; column1it < matrix.columnPointer.size(); column1it++) {
                if (lineHead && lineHead == columnTail[column1it]) { // если элемент найден
                    columnTailAvEl[column1it] = lineHead->value;
                    lineHead = lineHead->nextLine;
                    columnTail[column1it] = columnTail[column1it]->nextColumn;
                }
                else {
                    columnTailAvEl[column1it] = 0;
                }
            }
        }

        std::ranges::copy(matrix.columnPointer, columnTail.begin());
        std::ranges::copy(columnTail, prevColumnTail.begin());

        for (size_t rowit = 0; rowit < counter + 1; rowit++) {
            koef = 0;
            lineTail = matrix.linePointer[rowit];
            prevLineTail = matrix.linePointer[rowit];
            for (size_t columnit = 0; columnit < matrix.columnPointer.size(); columnit++) {
                if (lineTail && lineTail == columnTail[columnit]) {// если элемент найден
                    if (columnit == counter + 1) {
                        if (columnTail[counter + 1]) {
                            koef = columnTail[columnit]->value / columnTailAvEl[columnit];
                        }
                    }
                    lineTail->value -= koef * columnTailAvEl[columnit];
                    prevLineTail = lineTail;
                    lineTail = lineTail->nextLine;
                    prevColumnTail[columnit] = columnTail[columnit];
                    columnTail[columnit] = columnTail[columnit]->nextColumn;
                } else {
                    if (koef != 0.0 && columnTailAvEl[columnit] != 0.0) {
                        auto element = initElement(-columnTailAvEl[columnit] * koef);
                        if (lineTail != matrix.linePointer[rowit]) { // не первый элемент в строке, связываем
                            element->nextLine = lineTail;
                            prevLineTail->nextLine = element;
                        }
                        else {       // первый элемент в строке, сохраняем
                            element->nextLine = matrix.linePointer[rowit];
                            matrix.linePointer[rowit] = element;
                        }
                        lineTail = element;

                        if (columnTail[columnit] != matrix.columnPointer[columnit]) {
                            element->nextColumn = columnTail[columnit];
                            prevColumnTail[columnit]->nextColumn = element;
                        }
                        else {
                            element->nextColumn = matrix.columnPointer[columnit];
                            matrix.columnPointer[columnit] = element;
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

    //делим
    std::ranges::copy(matrix.columnPointer, columnTail.begin());
    for (size_t rowit = 0; rowit < matrix.linePointer.size(); rowit++) {

        lineHead = matrix.linePointer[rowit];
        for (size_t columnit = 0; columnit < matrix.columnPointer.size(); columnit++) {
            if (lineHead && lineHead == columnTail[columnit]) { // если элемент найден
                if (rowit == columnit) {
                    koef = lineHead->value;
                    //if (abs(koef) < MORE_THEN_MIN_ZNACH) { deepDelete(matrix); std::cout << "error" << std::endl; return mainMatrix; } //находим ошибку мол определитель равен нулю
                    //нужно сделать проверку чтобы определитель не был равен нулю!!
                    lineTail = matrix.linePointer[rowit];
                    while (lineTail) {
                        lineTail->value /= koef;
                        lineTail = lineTail->nextLine;
                    }
                }
                lineHead = lineHead->nextLine;
                columnTail[columnit] = columnTail[columnit]->nextColumn;
            }
        }
    }

    //обрезаем лишнее
    std::ranges::copy(matrix.columnPointer, columnTail.begin());
    for (size_t rowit = 0; rowit < matrix.linePointer.size(); rowit++) {
        lineTail = matrix.linePointer[rowit];
        for (size_t columnit = 0; columnit < matrix.columnPointer.size(); columnit++) {
            if (lineTail && lineTail == columnTail[columnit]) {
                if (columnit == matrix.linePointer.size()) unitMatrix.linePointer[rowit] = lineTail;
                lineTail = lineTail->nextLine;
                columnTail[columnit] = columnTail[columnit]->nextColumn;
            }
        }
        unitMatrix.columnPointer[rowit] = matrix.columnPointer[matrix.linePointer.size() + rowit];
    }


    bool sw1, sw2;
    std::ranges::copy(unitMatrix.columnPointer, columnTail.begin());
    std::ranges::copy(unitMatrix.columnPointer, prevColumnTail.begin());
    for (size_t rowit = 0; rowit < unitMatrix.linePointer.size(); rowit++) {
        lineTail = unitMatrix.linePointer[rowit];
        prevLineTail = lineTail;
        for (size_t columnit = 0; columnit < unitMatrix.columnPointer.size(); columnit++) {
            sw1 = false;
            sw2 = false;
            if (lineTail && lineTail == columnTail[columnit]) {
                if (fabs(lineTail->value) < MORE_THEN_MIN_ZNACH) {
                    if (prevLineTail != lineTail) prevLineTail->nextLine = lineTail->nextLine;
                    else { unitMatrix.linePointer[rowit] = lineTail->nextLine; prevLineTail = unitMatrix.linePointer[rowit]; sw1 = true; }
                    if (prevColumnTail[columnit] != columnTail[columnit]) prevColumnTail[columnit]->nextColumn = columnTail[columnit]->nextColumn;
                    else { unitMatrix.columnPointer[columnit] = columnTail[columnit]->nextColumn; prevColumnTail[columnit] = unitMatrix.columnPointer[columnit]; sw2 = true; }
                    lineTail->nextColumn = nullptr;
                    lineTail->nextLine = nullptr;
                    delete lineTail;
                    if (sw1) lineTail = prevLineTail;
                    else lineTail = prevLineTail->nextLine;
                    if (sw2)columnTail[columnit] = prevColumnTail[columnit];
                    else columnTail[columnit] = prevColumnTail[columnit]->nextLine;
                }
                else {
                    prevLineTail = lineTail;
                    lineTail = lineTail->nextLine;
                    prevColumnTail[columnit] = columnTail[columnit];
                    columnTail[columnit] = columnTail[columnit]->nextColumn;
                }
            }
        }
    }

    matrix.columnPointer.resize(matrix.linePointer.size());

    deepDelete(matrix);
    return unitMatrix;
}


SparseDoubleLinkedMatrix inverseMatrixSecondMethod(SparseDoubleLinkedMatrix& mainMatrix) { //обратная матрица использующая идеи перестановки строк

    if (matrixSize(mainMatrix).first != matrixSize(mainMatrix).second) return mainMatrix;//проверяем матрицу на валидность

    for (size_t rowit = 0; rowit < mainMatrix.linePointer.size(); rowit++)
        if (mainMatrix.linePointer[rowit] == nullptr || mainMatrix.columnPointer[rowit] == nullptr) return mainMatrix; //проверяем матрицу на валидность

    auto matrix = deepCopy(mainMatrix);
    matrix = mainMatrix; // нужно закомментировать

    auto UnitMatrix = generateUnitMatrix(matrix.linePointer.size());
    joinMatrix(matrix, UnitMatrix);

    int temp = 0;
    SparseDoubleLinkedMatrixElement* prevlineTail, * lineTail, * lineHead;
    std::vector<SparseDoubleLinkedMatrixElement*> columnTail(matrix.columnPointer.size());
    std::vector<double> columnTailAvEl(matrix.columnPointer.size());
    std::vector<double> columnTailDopAvEl(matrix.columnPointer.size());
    std::vector<SparseDoubleLinkedMatrixElement*> prevColumnTail(matrix.columnPointer.size());
    std::vector<SparseDoubleLinkedMatrixElement*> dopColumnTail(matrix.columnPointer.size());
    std::vector<SparseDoubleLinkedMatrixElement*> SkibidiColumnTail(matrix.columnPointer.size());
    double koef;



    bool found = true;

    for (size_t counter = 0; counter < matrix.linePointer.size(); counter++) {

        std::ranges::copy(matrix.columnPointer, columnTail.begin());
        std::ranges::copy(columnTail, prevColumnTail.begin());
        found = true;
        for (size_t rowit = 0; rowit < matrix.linePointer.size(); rowit++) {
            found = true;
            for (auto& it : columnTailDopAvEl) it = 0;
            auto lineHead = matrix.linePointer[rowit];
            if (rowit == counter) {
                for (size_t column1it = 0; column1it < matrix.columnPointer.size(); column1it++) {
                    if ((lineHead && lineHead == columnTail[column1it]) && (lineHead->value * lineHead->value) > MIN_ZNACH) {// если элемент найден  
                        lineHead = lineHead->nextLine;
                    }
                    else {
                        if ((lineHead && lineHead == columnTail[column1it]) && (lineHead->value * lineHead->value) < MIN_ZNACH) {
                            lineHead = lineHead->nextLine;
                        }
                        if (column1it == counter) { // если диагональный элемент отсутсвует то ищем ниже
                            found = false;
                            std::ranges::copy(columnTail, dopColumnTail.begin());
                            size_t doprowit = rowit;
                            while (doprowit < matrix.linePointer.size() && !found) {
                                auto dopLineTail = matrix.linePointer[doprowit];
                                size_t dopcolumnit = 0;
                                std::ranges::copy(dopColumnTail, SkibidiColumnTail.begin());
                                while (dopcolumnit < matrix.columnPointer.size() && !found) {
                                    if (dopLineTail && dopLineTail == dopColumnTail[dopcolumnit]) {// если элемент найден  
                                        if ((dopcolumnit == counter && dopLineTail->value * dopLineTail->value > MIN_ZNACH) && doprowit > rowit) { //нашли нужную строку
                                            auto dopLineHead = matrix.linePointer[doprowit];
                                            for (size_t col = 0; col < matrix.columnPointer.size(); col++) {
                                                if (dopLineHead && dopLineHead == SkibidiColumnTail[col]) {// если элемент найден  
                                                    columnTailDopAvEl[col] = dopLineHead->value;
                                                    dopLineHead = dopLineHead->nextLine;
                                                }
                                                else {
                                                    columnTailDopAvEl[col] = 0;
                                                }

                                            }
                                            found = true;
                                        }

                                        dopLineTail = dopLineTail->nextLine;
                                        dopColumnTail[dopcolumnit] = dopColumnTail[dopcolumnit]->nextColumn;
                                    }
                                    dopcolumnit++;
                                }

                                doprowit++;
                            }

                        }

                    }
                }
            }

            if (!found) { deepDelete(matrix); return mainMatrix; }


            if (rowit == counter) {
                auto blyatskiy = matrix.linePointer[rowit];
                auto prevblyatskiy = blyatskiy;
                for (size_t i = 0; i < columnTail.size(); i++) {
                    if (blyatskiy && blyatskiy == columnTail[i]) {// если элемент найден  
                        blyatskiy->value += columnTailDopAvEl[i] * 1.5;
                        columnTailAvEl[i] = blyatskiy->value;
                        prevblyatskiy = blyatskiy;
                        blyatskiy = blyatskiy->nextLine;

                    }
                    else {
                        if (columnTailDopAvEl[i] * columnTailDopAvEl[i] > MIN_ZNACH) {
                            auto element = initElement(columnTailDopAvEl[i] * 1.5);
                            if (blyatskiy != matrix.linePointer[rowit]) { // не первый элемент в строке, связываем
                                element->nextLine = blyatskiy;
                                prevblyatskiy->nextLine = element;
                            }
                            else {       // первый элемент в строке, сохраняем
                                element->nextLine = matrix.linePointer[rowit];
                                matrix.linePointer[rowit] = element;
                            }
                            blyatskiy = element;

                            if (columnTail[i] != matrix.columnPointer[i]) {
                                element->nextColumn = columnTail[i];
                                prevColumnTail[i]->nextColumn = element;
                            }
                            else {
                                element->nextColumn = matrix.columnPointer[i];
                                matrix.columnPointer[i] = element;
                            }
                            columnTailAvEl[i] = blyatskiy->value;
                            columnTail[i] = element;
                            prevblyatskiy = blyatskiy;
                            blyatskiy = blyatskiy->nextLine;
                        }
                        else
                            columnTailAvEl[i] = 0;
                    }
                }
            }

            koef = 0;
            lineTail = matrix.linePointer[rowit];
            prevlineTail = matrix.linePointer[rowit];
            for (size_t columnit = 0; columnit < matrix.columnPointer.size(); columnit++) {
                if (lineTail && lineTail == columnTail[columnit]) {// если элемент найден  
                    if (rowit > counter && columnit == counter) {
                        koef = columnTail[columnit]->value / columnTailAvEl[columnit];
                        if (columnTailAvEl[columnit] == 0) { deepDelete(matrix);  return mainMatrix; }
                    }
                    lineTail->value -= koef * columnTailAvEl[columnit];
                    prevlineTail = lineTail;
                    lineTail = lineTail->nextLine;
                    prevColumnTail[columnit] = columnTail[columnit];
                    columnTail[columnit] = columnTail[columnit]->nextColumn;
                }
                else {
                    if (rowit > counter) {
                        if (koef && columnTailAvEl[columnit]) {
                            auto element = initElement(-columnTailAvEl[columnit] * koef);
                            if (lineTail != matrix.linePointer[rowit]) { // не первый элемент в строке, связываем
                                element->nextLine = lineTail;
                                prevlineTail->nextLine = element;
                            }
                            else {       // первый элемент в строке, сохраняем
                                element->nextLine = matrix.linePointer[rowit];
                                matrix.linePointer[rowit] = element;
                            }
                            lineTail = element;

                            if (columnTail[columnit] != matrix.columnPointer[columnit]) {
                                element->nextColumn = columnTail[columnit];
                                prevColumnTail[columnit]->nextColumn = element;
                            }
                            else {
                                element->nextColumn = matrix.columnPointer[columnit];
                                matrix.columnPointer[columnit] = element;
                            }

                            prevColumnTail[columnit] = element;
                            columnTail[columnit] = element->nextColumn;
                            prevlineTail = lineTail;
                            lineTail = lineTail->nextLine;
                        }
                    }
                }
            }
        }
    }
    //ну вообще предидущих вычислений хватит для того чтобы найти определитель
    //вычитаем вверх




    std::ranges::copy(matrix.columnPointer, dopColumnTail.begin());

    lineHead = matrix.linePointer[0];
    for (size_t column1it = 0; column1it < matrix.columnPointer.size(); column1it++) {
        if (lineHead && lineHead == dopColumnTail[column1it]) { // если элемент найден  
            columnTailAvEl[column1it] = lineHead->value;
            lineHead = lineHead->nextLine;
            dopColumnTail[column1it] = dopColumnTail[column1it]->nextColumn;
        }
        else {
            columnTailAvEl[column1it] = 0;
        }
    }

    for (size_t counter = 0; counter < matrix.linePointer.size() - 1; counter++) {


        lineHead = matrix.linePointer[counter + 1];
        for (size_t column1it = 0; column1it < matrix.columnPointer.size(); column1it++) {
            if (lineHead && lineHead == dopColumnTail[column1it]) { // если элемент найден  
                columnTailAvEl[column1it] = lineHead->value;
                lineHead = lineHead->nextLine;
                dopColumnTail[column1it] = dopColumnTail[column1it]->nextColumn;
            }
            else {
                columnTailAvEl[column1it] = 0;
            }
        }


        std::ranges::copy(matrix.columnPointer, columnTail.begin());
        std::ranges::copy(columnTail, prevColumnTail.begin());

        for (size_t rowit = 0; rowit < counter + 1; rowit++) {
            koef = 0;
            lineTail = matrix.linePointer[rowit];
            prevlineTail = matrix.linePointer[rowit];
            for (size_t columnit = 0; columnit < matrix.columnPointer.size(); columnit++) {
                if (lineTail && lineTail == columnTail[columnit]) {// если элемент найден  
                    if (columnit == counter + 1) {
                        if (columnTail[counter + 1]) {
                            koef = columnTail[columnit]->value / columnTailAvEl[columnit];
                        }
                    }
                    lineTail->value -= koef * columnTailAvEl[columnit];
                    prevlineTail = lineTail;
                    lineTail = lineTail->nextLine;
                    prevColumnTail[columnit] = columnTail[columnit];
                    columnTail[columnit] = columnTail[columnit]->nextColumn;
                }
                else {
                    if (koef && columnTailAvEl[columnit]) {
                        auto element = initElement(-columnTailAvEl[columnit] * koef);
                        if (lineTail != matrix.linePointer[rowit]) { // не первый элемент в строке, связываем
                            element->nextLine = lineTail;
                            prevlineTail->nextLine = element;
                        }
                        else {       // первый элемент в строке, сохраняем
                            element->nextLine = matrix.linePointer[rowit];
                            matrix.linePointer[rowit] = element;
                        }
                        lineTail = element;

                        if (columnTail[columnit] != matrix.columnPointer[columnit]) {
                            element->nextColumn = columnTail[columnit];
                            prevColumnTail[columnit]->nextColumn = element;
                        }
                        else {
                            element->nextColumn = matrix.columnPointer[columnit];
                            matrix.columnPointer[columnit] = element;
                        }

                        prevColumnTail[columnit] = element;
                        columnTail[columnit] = element->nextColumn;
                        prevlineTail = lineTail;
                        lineTail = lineTail->nextLine;
                    }
                }
            }
        }


    }


    //делим
    std::ranges::copy(matrix.columnPointer, columnTail.begin());
    size_t count = 0;
    for (size_t rowit = 0; rowit < matrix.linePointer.size(); rowit++) {

        lineHead = matrix.linePointer[rowit];
        for (size_t columnit = 0; columnit < matrix.columnPointer.size(); columnit++) {
            if (lineHead && lineHead == columnTail[columnit]) { // если элемент найден  
                if (rowit == columnit) {
                    koef = lineHead->value;
                    if (koef * koef < MORE_THEN_MIN_ZNACH) { deepDelete(matrix); std::cout << "error" << std::endl; return mainMatrix; } //находим ошибку мол определитель равен нулю
                    //нужно сделать проверку чтобы определитель не был равен нулю!!
                    lineTail = matrix.linePointer[rowit];
                    while (lineTail) {
                        lineTail->value /= koef;
                        lineTail = lineTail->nextLine;
                    }
                }
                lineHead = lineHead->nextLine;
                columnTail[columnit] = columnTail[columnit]->nextColumn;
            }
        }
    }

    //обрезаем лишнее
    std::ranges::copy(matrix.columnPointer, columnTail.begin());
    for (size_t rowit = 0; rowit < matrix.linePointer.size(); rowit++) {
        lineTail = matrix.linePointer[rowit];
        for (size_t columnit = 0; columnit < matrix.columnPointer.size(); columnit++) {
            if (lineTail && lineTail == columnTail[columnit]) {
                if (columnit == matrix.linePointer.size()) UnitMatrix.linePointer[rowit] = lineTail;
                lineTail = lineTail->nextLine;
                columnTail[columnit] = columnTail[columnit]->nextColumn;
            }
        }
        UnitMatrix.columnPointer[rowit] = matrix.columnPointer[matrix.linePointer.size() + rowit];
    }


    bool sw1, sw2;
    std::ranges::copy(UnitMatrix.columnPointer, columnTail.begin());
    std::ranges::copy(UnitMatrix.columnPointer, prevColumnTail.begin());
    for (size_t rowit = 0; rowit < UnitMatrix.linePointer.size(); rowit++) {
        lineTail = UnitMatrix.linePointer[rowit];
        prevlineTail = lineTail;
        for (size_t columnit = 0; columnit < UnitMatrix.columnPointer.size(); columnit++) {
            sw1 = 0;
            sw2 = 0;
            if (lineTail && lineTail == columnTail[columnit]) {
                if (lineTail->value * lineTail->value < MORE_THEN_MIN_ZNACH) {
                    if (prevlineTail != lineTail) prevlineTail->nextLine = lineTail->nextLine;
                    else { UnitMatrix.linePointer[rowit] = lineTail->nextLine; prevlineTail = UnitMatrix.linePointer[rowit]; sw1 = 1; }
                    if (prevColumnTail[columnit] != columnTail[columnit]) prevColumnTail[columnit]->nextColumn = columnTail[columnit]->nextColumn;
                    else { UnitMatrix.columnPointer[columnit] = columnTail[columnit]->nextColumn; prevColumnTail[columnit] = UnitMatrix.columnPointer[columnit]; sw2 = 1; }
                    lineTail->nextColumn = nullptr;
                    lineTail->nextLine = nullptr;
                    delete lineTail;
                    if (sw1) lineTail = prevlineTail;
                    else lineTail = prevlineTail->nextLine;
                    if (sw2)columnTail[columnit] = prevColumnTail[columnit];
                    else columnTail[columnit] = prevColumnTail[columnit]->nextLine;
                }
                else {
                    prevlineTail = lineTail;
                    lineTail = lineTail->nextLine;
                    prevColumnTail[columnit] = columnTail[columnit];
                    columnTail[columnit] = columnTail[columnit]->nextColumn;
                }
            }
        }
    }



    matrix.columnPointer.resize(matrix.linePointer.size());


    deepDelete(matrix);

    return UnitMatrix;
}





SparseDoubleLinkedMatrix multiply(SparseDoubleLinkedMatrix& matrix1, SparseDoubleLinkedMatrix& matrix2) {

    if (matrixSize(matrix1).second != matrixSize(matrix2).first) return matrix1; // вот мне не нравится как тут ошибка выводится, мб написать функцию которая делает пустую матрицу?

    auto output = SparseDoubleLinkedMatrix{};
    output.columnPointer = std::vector<SparseDoubleLinkedMatrixElement*>(matrix1.columnPointer.size(), nullptr);
    output.linePointer = std::vector<SparseDoubleLinkedMatrixElement*>(matrix2.linePointer.size(), nullptr);

    std::vector<SparseDoubleLinkedMatrixElement*> columnTails(matrix1.columnPointer.size()),
        lineTails(matrix2.linePointer.size()),
        resColumnTails(matrix2.columnPointer.size(), nullptr),
        resColumn1Tails(matrix2.columnPointer.size(), nullptr);

    SparseDoubleLinkedMatrixElement* lineTail,
        * reslineTail,
        * prevreslineTail,
        * element,
        * columnTail;

    std::vector<double> matrix1LineEls(matrix1.columnPointer.size());
    double elValue;

    size_t rowit, columnit, column, line;
    std::ranges::copy(matrix1.columnPointer, columnTails.begin());

    for (rowit = 0; rowit < matrix1.linePointer.size(); rowit++) {

        lineTail = matrix1.linePointer[rowit];
        reslineTail = nullptr;
        prevreslineTail = nullptr;

        for (column = 0; column < matrix1.columnPointer.size(); column++) {
            if (lineTail && lineTail == columnTails[column]) {// если элемент найден
                matrix1LineEls[column] = lineTail->value;
                lineTail = lineTail->nextLine;
                columnTails[column] = columnTails[column]->nextColumn;
            }
            else {
                matrix1LineEls[column] = 0.0;
            }
        }

        std::ranges::copy(matrix2.linePointer, lineTails.begin());
        for (columnit = 0; columnit < matrix2.columnPointer.size(); columnit++) {

            columnTail = matrix2.columnPointer[columnit];
            elValue = 0.0;

            for (line = 0; line < matrix2.linePointer.size(); line++) {
                if (columnTail && columnTail == lineTails[line]) {// если элемент найден
                    elValue += columnTail->value*matrix1LineEls[line];
                    columnTail = columnTail->nextColumn;
                    lineTails[line] = lineTails[line]->nextLine;
                }
            }


            if (elValue != 0) {
                element = initElement(elValue);
                if (reslineTail != output.linePointer[rowit]) { // не первый элемент в строке, связываем
                    element->nextLine = lineTail;
                    prevreslineTail->nextLine = element; // TODO: may by null (по логике не может быть иначе киньте тесты)
                }
                else {       // первый элемент в строке, сохраняем
                    element->nextLine = output.linePointer[rowit];
                    output.linePointer[rowit] = element;
                }
                reslineTail = element;

                if (resColumnTails[columnit] != output.columnPointer[columnit]) {
                    element->nextColumn = resColumnTails[columnit];
                    resColumn1Tails[columnit]->nextColumn = element;
                }
                else {
                    element->nextColumn = output.columnPointer[columnit];
                    output.columnPointer[columnit] = element;
                }

                resColumn1Tails[columnit] = element;
                resColumnTails[columnit] = element->nextColumn;
                prevreslineTail = reslineTail;
                reslineTail = reslineTail->nextLine;
            }

        }
    }

    return output;
}
