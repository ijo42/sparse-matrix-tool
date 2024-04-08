#ifndef CREATEMATRIX_H
#define CREATEMATRIX_H

#include <QDialog>

namespace Ui {
class creatematrix;
}

class creatematrix : public QDialog
{
    Q_OBJECT

public:
    explicit creatematrix(QWidget *parent = nullptr);
    ~creatematrix();

private:
    Ui::creatematrix *ui;
};

#endif // CREATEMATRIX_H
