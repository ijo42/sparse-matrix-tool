#ifndef SPARSEMATRIXMODEL_H
#define SPARSEMATRIXMODEL_H

#include <QObject>

#include <QAbstractTableModel>
#include "shared.h"
#include <operations.h>

class SparseMatrixModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit SparseMatrixModel(SparseDoubleLinkedMatrix *matrix, QObject* parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
private:
    SparseDoubleLinkedMatrix *_matrix; // Указатель на матрицу
};


#endif // SPARSEMATRIXMODEL_H
