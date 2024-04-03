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

void swapRows(SparseDoubleLinkedMatrix& matrix, int rowIndex1, int rowIndex2) {
    // Проверяем, что индексы не выходят за пределы размера матрицы
    if (rowIndex1 < 0 || rowIndex2 < 0 || rowIndex1 >= matrix.linePointer.size() || rowIndex2 >= matrix.linePointer.size()) {
        std::cerr << "Index out of bounds" << std::endl;
        return;
    }
    if (rowIndex1 == rowIndex2) {
        // Если индексы одинаковы, ничего делать не нужно
        return;
    }

    // Меняем указатели на строки местами
    std::swap(matrix.linePointer[rowIndex1], matrix.linePointer[rowIndex2]);

    // Теперь нужно обновить связи в колонках, проходясь по всем элементам обоих строк
    for (auto colPtr = matrix.columnPointer.begin(); colPtr != matrix.columnPointer.end(); ++colPtr) {
        SparseDoubleLinkedMatrixElement* currentElement = *colPtr;
        while (currentElement != nullptr) {
            if (currentElement->nextLine == matrix.linePointer[rowIndex1]) {
                currentElement->nextLine = matrix.linePointer[rowIndex2];
            }
            else if (currentElement->nextLine == matrix.linePointer[rowIndex2]) {
                currentElement->nextLine = matrix.linePointer[rowIndex1];
            }
            currentElement = currentElement->nextLine;
        }
    }
}

SparseDoubleLinkedMatrix generateUnitMatrix(size_t n) {
    auto matrix = SparseDoubleLinkedMatrix{};
    matrix.columnPointer.reserve(n);
    matrix.linePointer.reserve(n);
    for (int i = 0; i < n; ++i) {
        // Create the diagonal element
        SparseDoubleLinkedMatrixElement* element = new SparseDoubleLinkedMatrixElement();
        element->value = 1.0;

        // For a unit matrix, each diagonal element is the only non-zero element in its line and column
        matrix.linePointer.push_back(element);
        matrix.columnPointer.push_back(element);
    }
    return matrix;
}



void subtractRow(SparseDoubleLinkedMatrix& matrix, int rowIndex1, int rowIndex2) {
    // Verify that the row indices are within bounds
    if (rowIndex1 < 0 || rowIndex2 < 0 || rowIndex1 >= matrix.linePointer.size() || rowIndex2 >= matrix.linePointer.size()) {
        return;
    }
    if (rowIndex1 == rowIndex2) {
        // If the indices are the same, there's no need to perform any operation
        return;
    }

    SparseDoubleLinkedMatrixElement* row2ElementFirst = matrix.linePointer[rowIndex2];

    if (row2ElementFirst == NULL) return;

    std::vector<SparseDoubleLinkedMatrixElement*> columnTail(matrix.columnPointer.size());
    std::vector<SparseDoubleLinkedMatrixElement*> column1Tail(matrix.columnPointer.size());
    std::vector<SparseDoubleLinkedMatrixElement*> column2Tail(matrix.columnPointer.size());
    std::vector<double> row1els;
    std::vector<double> row2els;
    std::ranges::copy(matrix.columnPointer, columnTail.begin());
    std::ranges::copy(columnTail, column1Tail.begin());
    for (size_t rowit = 0; rowit < matrix.linePointer.size(); rowit++) {
        auto lineTail = matrix.linePointer[rowit];
        if (rowit == rowIndex1 - 1) std::ranges::copy(columnTail, column1Tail.begin());
        if (rowit == rowIndex1) std::ranges::copy(columnTail, column2Tail.begin());
        for (auto& i : columnTail) {
            if (lineTail && lineTail == i) {
                if (rowit == rowIndex1) row1els.push_back(i->value);
                if (rowit == rowIndex2) row2els.push_back(i->value);
                lineTail = lineTail->nextLine;
                i = i->nextColumn;
            }
            else {
                if (rowit == rowIndex1) row1els.push_back(0);
                if (rowit == rowIndex2) row2els.push_back(0);
            }
        }
    }

    for (int i = 0; i < row1els.size(); i++) row1els[i] -= row2els[i];

    size_t rowit = rowIndex1;
    auto lineTail = matrix.linePointer[rowit];
    auto prevLineTail = matrix.linePointer[rowit];
    for (size_t i = 0; i < column1Tail.size(); i++) {
        if (column1Tail[i] != nullptr) {
            if (lineTail && lineTail == column1Tail[i]->nextColumn) {
                if (row1els[i] != 0) column1Tail[i]->nextColumn->value = row1els[i];
                prevLineTail = lineTail;
                lineTail = lineTail->nextLine;
            }
            else if (lineTail && lineTail == matrix.columnPointer[i]) {
                if (row1els[i] != 0) matrix.columnPointer[i]->value = row1els[i];
                prevLineTail = lineTail;
                lineTail = lineTail->nextLine;
            }
            else {
                if (row1els[i] != 0) {
                    auto element = initElement(row1els[i]);
                    if (lineTail != matrix.linePointer[rowit]) { // не первый элемент в строке, связываем
                        element->nextLine = lineTail;
                        prevLineTail->nextLine = element;
                    }
                    else {       // первый элемент в строке, сохраняем
                        element->nextLine = matrix.linePointer[rowIndex1];
                        matrix.linePointer[rowIndex1] = element;
                    }
                    lineTail = element;

                    if (column2Tail[i] != matrix.columnPointer[i]) {
                        element->nextColumn = column2Tail[i];
                        column1Tail[i]->nextColumn = element;
                    }
                    else {
                        element->nextColumn = matrix.columnPointer[i];
                        matrix.columnPointer[i] = element;
                    }
                    prevLineTail = lineTail;
                    lineTail = lineTail->nextLine;
                }
            }
        }
    }
}


void joinmatrix(SparseDoubleLinkedMatrix& matrix1, SparseDoubleLinkedMatrix& matrix2) {
    if (matrix1.linePointer.size() != matrix2.linePointer.size()) {
        return;
    }
    matrix1.columnPointer.insert(matrix1.columnPointer.end(),
        matrix2.columnPointer.begin(), matrix2.columnPointer.end());
    for (size_t i = 0; i < matrix1.linePointer.size(); i++) {
        auto lineTail = matrix1.linePointer[i];
        if (!lineTail) matrix1.linePointer[i] = matrix2.linePointer[i];
        else {
            while (lineTail->nextLine) lineTail = lineTail->nextLine;
            lineTail->nextLine = matrix2.linePointer[i];
        }
    }
    return;
}


// много лишних действий буду дорабатывать
SparseDoubleLinkedMatrix inverseMatrix(SparseDoubleLinkedMatrix& mainMatrix) {

    if (matrixSize(mainMatrix).first != matrixSize(mainMatrix).second) return mainMatrix;//проверяем матрицу на валидность

    for (size_t rowit = 0; rowit < mainMatrix.linePointer.size(); rowit++) {
        auto lineTail = mainMatrix.linePointer[rowit];
        if (lineTail == nullptr || mainMatrix.columnPointer[rowit] == nullptr) return mainMatrix; //проверяем матрицу на валидность
    }

    auto matrix = deepCopy(mainMatrix);
    matrix = mainMatrix; // нужно закомментировать

    auto UnitMatrix = generateUnitMatrix(matrix.linePointer.size());
    joinmatrix(matrix, UnitMatrix);

    int temp = 0;
    bool b = true;
    auto lineTail = matrix.linePointer[0];
    auto prevlineTail = lineTail;
    std::vector<SparseDoubleLinkedMatrixElement*> columnTail(matrix.columnPointer.size());
    std::vector<double> columnTailAvEl(matrix.columnPointer.size());
    std::vector<SparseDoubleLinkedMatrixElement*> column1Tail(matrix.columnPointer.size());
    std::vector<SparseDoubleLinkedMatrixElement*> column2Tail(matrix.columnPointer.size());
    double koef;





    //тут мы подготавливаем матрицу так чтобы на главной диагонали были элементы
    // 
    // 
    // можно сказать РАБОЧИЙ И БЫСТРЫЙ КОНЦЕПТ НО НЕ БЕЗ ПИЗДЫ (кратко мы приводим к треугольному виду чисто заменяя нули почти-нулями)
    // в принципе скорее всего будут ошибки 
    std::ranges::copy(matrix.columnPointer, columnTail.begin());
    std::ranges::copy(columnTail, column1Tail.begin());
    for (size_t rowit = 0; rowit < matrix.linePointer.size(); rowit++) {
        auto lineTail = matrix.linePointer[rowit];
        if (lineTail == nullptr || matrix.columnPointer[rowit] == nullptr) return matrix; //одновременно мы проверяем матрицу на валидность
        for (size_t columnit = 0; columnit < matrix.columnPointer.size(); columnit++) {
            if (lineTail && lineTail == columnTail[columnit]) {
                prevlineTail = lineTail;
                lineTail = lineTail->nextLine;
                column1Tail[columnit] = columnTail[columnit];
                columnTail[columnit] = columnTail[columnit]->nextColumn;
            }
            else {
                if (columnit == rowit) {
                    auto element = initElement(MIN_ZNACH);
                    //я так подумал а зачем нам менять строки подвязывать там
                    //если можно просто представить ноль по другому, я не думаю что разницу после 300 знаков после запятой заметят
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
                        column1Tail[columnit]->nextColumn = element;
                    }
                    else {
                        element->nextColumn = matrix.columnPointer[columnit];
                        matrix.columnPointer[columnit] = element;
                    }

                    column1Tail[columnit] = element;
                    columnTail[columnit] = element->nextColumn;
                    prevlineTail = lineTail;
                    lineTail = lineTail->nextLine;
                }
            }
        }
    }


    //другой концепт если мы просто последовательно складывать предидущую строку в следующую, то как минимум на главной диагонали будут все элементы, иначе нельзя
    //ужасный концепт, но максимально правильный (очень много - памяти и скорости)

    //std::ranges::copy(matrix.columnPointer, columnTail.begin());
    //std::ranges::copy(columnTail, column1Tail.begin());
    //for (size_t rowit = 0; rowit < matrix.linePointer.size(); rowit++) {

    //    auto lineHead = matrix.linePointer[rowit];

    //    if (lineTail == nullptr || matrix.columnPointer[rowit] == nullptr) return matrix; //проверка на возможность 

    //    for (size_t column1it = 0; column1it < matrix.columnPointer.size(); column1it++) {
    //        if (lineHead && lineHead == columnTail[column1it]) {// если элемент найден  
    //            columnTailAvEl[column1it] = lineHead->value;
    //            lineHead = lineHead->nextLine;
    //        }
    //        else {
    //            columnTailAvEl[column1it] = 0;
    //        }
    //    }


    //    koef = 1;
    //    auto lineTail = matrix.linePointer[rowit];
    //    auto prevLineTail = matrix.linePointer[rowit];
    //    for (size_t columnit = 0; columnit < matrix.columnPointer.size(); columnit++) {
    //        if (lineTail && lineTail == columnTail[columnit]) {// если элемент найден  
    //            lineTail->value -= koef * columnTailAvEl[columnit];
    //            prevLineTail = lineTail;
    //            lineTail = lineTail->nextLine;
    //            column1Tail[columnit] = columnTail[columnit];
    //            columnTail[columnit] = columnTail[columnit]->nextColumn;
    //        }
    //        else {
    //            if (koef && columnTailAvEl[columnit]) {
    //                auto element = initElement(-columnTailAvEl[columnit] * koef);
    //                if (lineTail != matrix.linePointer[rowit]) { // не первый элемент в строке, связываем
    //                    element->nextLine = lineTail;
    //                    prevLineTail->nextLine = element;
    //                }
    //                else {       // первый элемент в строке, сохраняем
    //                    element->nextLine = matrix.linePointer[rowit];
    //                    matrix.linePointer[rowit] = element;
    //                }
    //                lineTail = element;

    //                if (columnTail[columnit] != matrix.columnPointer[columnit]) {
    //                    element->nextColumn = columnTail[columnit];
    //                    column1Tail[columnit]->nextColumn = element;
    //                }
    //                else {
    //                    element->nextColumn = matrix.columnPointer[columnit];
    //                    matrix.columnPointer[columnit] = element;
    //                }

    //                column1Tail[columnit] = element;
    //                columnTail[columnit] = element->nextColumn;
    //                prevLineTail = lineTail;
    //                lineTail = lineTail->nextLine;
    //            }
    //        }
    //        printMatrix(matrix);
    //    }
    //}


    //printMatrix(matrix);
    //вычитаем вниз
    for (size_t counter = 0; counter < matrix.linePointer.size(); counter++) {

        std::ranges::copy(matrix.columnPointer, columnTail.begin());
        std::ranges::copy(columnTail, column1Tail.begin());
        for (size_t rowit = 0; rowit < matrix.linePointer.size(); rowit++) {

            auto lineHead = matrix.linePointer[rowit];
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

            koef = 0;
            auto lineTail = matrix.linePointer[rowit];
            auto prevLineTail = matrix.linePointer[rowit];
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
                    column1Tail[columnit] = columnTail[columnit];
                    columnTail[columnit] = columnTail[columnit]->nextColumn;
                }
                else {
                    if (rowit > counter) {
                        if (koef && columnTailAvEl[columnit]) {
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
                                column1Tail[columnit]->nextColumn = element;
                            }
                            else {
                                element->nextColumn = matrix.columnPointer[columnit];
                                matrix.columnPointer[columnit] = element;
                            }

                            column1Tail[columnit] = element;
                            columnTail[columnit] = element->nextColumn;
                            prevLineTail = lineTail;
                            lineTail = lineTail->nextLine;
                        }
                    }
                }
                //printMatrix(matrix);
            }
        }
    }
    //printMatrix(matrix);
    //ну вообще предидущих вычислений хватит для того чтобы найти определитель

    //вычитаем вверх
    for (size_t counter = 0; counter < matrix.linePointer.size() - 1; counter++) {

        std::ranges::copy(matrix.columnPointer, columnTail.begin());
        std::ranges::copy(columnTail, column1Tail.begin());
        for (size_t rowit = 0; rowit <= counter + 1; rowit++) {

            auto lineHead = matrix.linePointer[rowit];
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
        std::ranges::copy(columnTail, column1Tail.begin());

        for (size_t rowit = 0; rowit < counter + 1; rowit++) {
            koef = 0;
            auto lineTail = matrix.linePointer[rowit];
            auto prevLineTail = matrix.linePointer[rowit];
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
                    column1Tail[columnit] = columnTail[columnit];
                    columnTail[columnit] = columnTail[columnit]->nextColumn;
                }
                else {
                    if (koef && columnTailAvEl[columnit]) {
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
                            column1Tail[columnit]->nextColumn = element;
                        }
                        else {
                            element->nextColumn = matrix.columnPointer[columnit];
                            matrix.columnPointer[columnit] = element;
                        }

                        column1Tail[columnit] = element;
                        columnTail[columnit] = element->nextColumn;
                        prevLineTail = lineTail;
                        lineTail = lineTail->nextLine;
                    }
                }
            }
            //printMatrix(matrix);
        }


    }

    //printMatrix(matrix);

    //делим
    std::ranges::copy(matrix.columnPointer, columnTail.begin());
    size_t count = 0;
    for (size_t rowit = 0; rowit < matrix.linePointer.size(); rowit++) {

        auto lineHead = matrix.linePointer[rowit];
        for (size_t columnit = 0; columnit < matrix.columnPointer.size(); columnit++) {
            if (lineHead && lineHead == columnTail[columnit]) { // если элемент найден  
                if (rowit == columnit) {
                    koef = lineHead->value;
                    auto lineTail = matrix.linePointer[rowit];
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
        auto lineTail = matrix.linePointer[rowit];
        for (size_t columnit = 0; columnit < matrix.columnPointer.size(); columnit++) {
            if (lineTail && lineTail == columnTail[columnit]) {
                if (columnit == matrix.linePointer.size()) UnitMatrix.linePointer[rowit] = lineTail;
                lineTail = lineTail->nextLine;
                columnTail[columnit] = columnTail[columnit]->nextColumn;
            }
        }
        UnitMatrix.columnPointer[rowit] = matrix.columnPointer[matrix.linePointer.size() + rowit];
    }


    //все что ниже я не знаю нужно ли это это цикл который убирает прктически-нули из матрицы
    //вообще можно списать на погрешность компьютерных вычислений чтобы не терять скорость
    bool sw1, sw2;
    std::ranges::copy(UnitMatrix.columnPointer, columnTail.begin());
    std::ranges::copy(UnitMatrix.columnPointer, column1Tail.begin());
    for (size_t rowit = 0; rowit < UnitMatrix.linePointer.size(); rowit++) {
        auto lineTail = UnitMatrix.linePointer[rowit];
        prevlineTail = lineTail;
        for (size_t columnit = 0; columnit < UnitMatrix.columnPointer.size(); columnit++) {
            sw1 = 0;
            sw2 = 0;
            if (lineTail && lineTail == columnTail[columnit]) {
                if (abs(lineTail->value) < MORE_THEN_MIN_ZNACH) {
                    if (prevlineTail != lineTail) prevlineTail->nextLine = lineTail->nextLine;
                    else { UnitMatrix.linePointer[rowit] = lineTail->nextLine; prevlineTail = UnitMatrix.linePointer[rowit]; sw1 = 1; }
                    if (column1Tail[columnit] != columnTail[columnit]) column1Tail[columnit]->nextColumn = columnTail[columnit]->nextColumn;
                    else { UnitMatrix.columnPointer[columnit] = columnTail[columnit]->nextColumn; column1Tail[columnit] = UnitMatrix.columnPointer[columnit]; sw2 = 1; }
                    lineTail->nextColumn = nullptr;
                    lineTail->nextLine = nullptr;
                    delete lineTail;
                    if (sw1) lineTail = prevlineTail;
                    else lineTail = prevlineTail->nextLine;
                    if (sw2)columnTail[columnit] = column1Tail[columnit];
                    else columnTail[columnit] = column1Tail[columnit]->nextLine;
                }
                else {
                    prevlineTail = lineTail;
                    lineTail = lineTail->nextLine;
                    column1Tail[columnit] = columnTail[columnit];
                    columnTail[columnit] = columnTail[columnit]->nextColumn;
                }
            }
        }
    }



    matrix.columnPointer.resize(matrix.linePointer.size());


    deepDelete(matrix);

    return UnitMatrix;
}
