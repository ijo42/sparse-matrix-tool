#include <headers/sparsematrixmodel.h>

#include <qmessagebox.h>

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

bool SparseMatrixModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || role != Qt::EditRole || value.toString().isEmpty())
        return false;

    int row = index.row();
    int column = index.column();
    bool isSuccess;
    double newValue = value.toDouble(&isSuccess);

    if(!isSuccess){
        QMessageBox::warning((QWidget*)this->parent(), "Ошибка", "Введите вещественное число.");
        return false;
    }

    if (countElements(*_matrix) >= maxElements(*_matrix) && newValue != 0.0){
        QMessageBox::warning((QWidget*)this->parent(), "Предупреждение", "В матрицу уже добавлено максимальное количество элементов.");
        return false;
    }

    if(setElement(_matrix, row, column, newValue) == nullptr)
        return false;

    emit dataChanged(index, index, {Qt::EditRole});
    return true;
}

Qt::ItemFlags SparseMatrixModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

