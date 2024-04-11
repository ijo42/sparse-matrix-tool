#ifndef PREVIEW_H
#define PREVIEW_H

#include <QWidget>
#include <shared.h>

namespace Ui {
class Preview;
}

class Preview : public QWidget
{
    Q_OBJECT

public:
    explicit Preview(SparseDoubleLinkedMatrix *matrix, QWidget *parent = nullptr);
    ~Preview();

private:
    Ui::Preview *ui;
    SparseDoubleLinkedMatrix *matrix;
};

#endif // PREVIEW_H
