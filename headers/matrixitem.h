#ifndef MATRIXITEM_H
#define MATRIXITEM_H

#include <QWidget>
#include <shared.h>

namespace Ui {
class MatrixItem;
}

class MatrixItem : public QWidget
{
    Q_OBJECT

public:
    explicit MatrixItem(SparseDoubleLinkedMatrix *matrix, QWidget *parent = nullptr);
    ~MatrixItem();

private:
    Ui::MatrixItem *ui;
    SparseDoubleLinkedMatrix *matrix;
};

#endif // MATRIXITEM_H
