#ifndef SPARSEMATRIXMODEL_H
#define SPARSEMATRIXMODEL_H

#include <QObject>

#include <QAbstractTableModel>
#include "shared.h"

class SparseMatrixModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit SparseMatrixModel(const std::string &path, QObject* parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
private:
    SparseDoubleLinkedMatrix _matrix; // Указатель на матрицу
};


#endif // SPARSEMATRIXMODEL_H
