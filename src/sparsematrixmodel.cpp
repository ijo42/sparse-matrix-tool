#include <headers/sparsematrixmodel.h>

SparseMatrixModel::SparseMatrixModel(SparseDoubleLinkedMatrix *matrix, QObject* parent)
    : QAbstractTableModel(parent), _matrix(matrix) {
}

int SparseMatrixModel::rowCount(const QModelIndex &parent) const {
    // Возвращает количество строк в матрице
    return (_matrix)->linePointer.size();
}

int SparseMatrixModel::columnCount(const QModelIndex &parent) const {
    // Возвращает количество столбцов в матрице
    return (_matrix)->columnPointer.size();
}

QVariant SparseMatrixModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    // Проверка существования элемента на пересечении строки и столбца
    auto rowTail = (_matrix)->linePointer[index.row()];

    while (rowTail != nullptr) {
        // Сброс columnTail на начало столбца для каждой новой строки
        auto columnTail = (_matrix)->columnPointer[index.column()];

        while (columnTail != nullptr) {
            if (columnTail == rowTail) {
                return rowTail->value; // Элемент на пересечении найден
            }

            columnTail = columnTail->nextColumn; // Перемещение к следующему элементу в столбце
        }

        rowTail = rowTail->nextLine; // Перемещение к следующему элементу в строке
    }

    // Элемент на пересечении не найден
    return 0.0;
}
