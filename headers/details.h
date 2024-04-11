#ifndef DETAILS_H
#define DETAILS_H

#include <QWidget>
#include <shared.h>

namespace Ui {
class Details;
}

class Details : public QWidget
{
    Q_OBJECT

public:
    explicit Details(SparseDoubleLinkedMatrix *matrix, QWidget *parent = nullptr);
    ~Details();

private:
    Ui::Details *ui;
};

#endif // DETAILS_H
