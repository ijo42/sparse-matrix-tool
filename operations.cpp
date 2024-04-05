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

SparseDoubleLinkedMatrix twoMatrixAccumulateOperation(const SparseDoubleLinkedMatrix& matrix1, const SparseDoubleLinkedMatrix& matrix2, const std::function<double(double,double)> &accumulateFunc) {
    SparseDoubleLinkedMatrix output{};
    if(matrixSize(matrix1) != matrixSize(matrix2)) { // неквадратные матрицы нельзя складывать
        return output;
    }

    output = deepCopy(matrix1);

    std::vector<SparseDoubleLinkedMatrixElement*>
                columnTail(output.columnPointer.size()),
                columnTail2(matrix2.columnPointer.size());

    std::ranges::copy(output.columnPointer, columnTail.begin());
    std::ranges::copy(matrix2.columnPointer, columnTail2.begin());

    for (int i = 0; i < matrix1.linePointer.size(); i++) {
        auto lineTail = output.linePointer[i],
             lineTail2 = matrix2.linePointer[i];

        for (int j = 0; j < output.columnPointer.size(); ++j) {
            if(lineTail2 && lineTail2 == columnTail2[j]) {
                SparseDoubleLinkedMatrixElement *element;
                if(lineTail && lineTail == columnTail[j]) { // элемент есть в обоих матрицах
                    element = lineTail;
                } else {    // элемент есть только в 2 матрице
                    element = initElement(0.0);
                    if(lineTail) { // не первый элемент в строке, связываем
                        lineTail->nextLine = element;
                    } else {       // первый элемент в строке, сохраняем
                        output.linePointer[i] = element;
                    }
                    lineTail = element;

                    if(columnTail[j]) {
                        columnTail[j]->nextColumn = element;
                    } else {
                        output.columnPointer[j] = element;
                    }
                    columnTail[j] = element;
                }


                element->value = accumulateFunc(element->value, lineTail2->value);
                lineTail2 = lineTail2->nextLine;
                columnTail2[j] = columnTail2[j]->nextColumn;
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

SparseDoubleLinkedMatrix multiply(SparseDoubleLinkedMatrix& matrix1, SparseDoubleLinkedMatrix& matrix2) {

    if (matrixSize(matrix1).second != matrixSize(matrix2).first) return matrix1;

    auto output = SparseDoubleLinkedMatrix{};
    output.columnPointer = std::vector<SparseDoubleLinkedMatrixElement*>(matrix1.columnPointer.size(), nullptr);
    output.linePointer = std::vector<SparseDoubleLinkedMatrixElement*>(matrix2.linePointer.size(), nullptr);

    std::vector<SparseDoubleLinkedMatrixElement*> columnTails(matrix1.columnPointer.size());
    std::vector<SparseDoubleLinkedMatrixElement*> lineTails(matrix2.linePointer.size());
    std::vector<SparseDoubleLinkedMatrixElement*> resColumnTails(matrix2.columnPointer.size(), nullptr);
    std::vector<SparseDoubleLinkedMatrixElement*> resColumn1Tails(matrix2.columnPointer.size(), nullptr);
    std::vector<double> matrix1LineEls(matrix1.columnPointer.size());
    std::vector<double> matrix2ColumnEls(matrix1.columnPointer.size());
    double elValue;
    std::ranges::copy(matrix1.columnPointer, columnTails.begin());

    for (size_t rowit = 0; rowit < matrix1.linePointer.size(); rowit++) {

        SparseDoubleLinkedMatrixElement* lineTail = matrix1.linePointer[rowit];
        SparseDoubleLinkedMatrixElement* reslineTail = nullptr;
        SparseDoubleLinkedMatrixElement* prevreslineTail = nullptr;

        for (size_t column = 0; column < matrix1.columnPointer.size(); column++) {
            if (lineTail && lineTail == columnTails[column]) {// если элемент найден
                matrix1LineEls[column] = lineTail->value;
                lineTail = lineTail->nextLine;
                columnTails[column] = columnTails[column]->nextColumn;
            }
            else {
                matrix1LineEls[column] = 0;
            }
        }

        std::ranges::copy(matrix2.linePointer, lineTails.begin());
        for (size_t columnit = 0; columnit < matrix2.columnPointer.size(); columnit++) {

            auto columnTail = matrix2.columnPointer[columnit];

            for (size_t line = 0; line < matrix2.linePointer.size(); line++) {
                if (columnTail && columnTail == lineTails[line]) {// если элемент найден
                    matrix2ColumnEls[line] = columnTail->value;
                    columnTail = columnTail->nextColumn;
                    lineTails[line] = lineTails[line]->nextLine;
                }
                else {
                    matrix2ColumnEls[line] = 0;
                }
            }
            elValue = 0;
            for (size_t i = 0; i < matrix2ColumnEls.size(); i++)
                elValue += matrix1LineEls[i] * matrix2ColumnEls[i];


            if (elValue != 0) {
                auto element = initElement(elValue);
                if (reslineTail != output.linePointer[rowit]) { // не первый элемент в строке, связываем
                    element->nextLine = lineTail;
                    prevreslineTail->nextLine = element; // TODO: may by null
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
