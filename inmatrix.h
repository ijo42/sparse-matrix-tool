#ifndef INMATRIX_H
#define INMATRIX_H

#include <QDialog>

namespace Ui {
class inmatrix;
}

class inmatrix : public QDialog
{
    Q_OBJECT

public:
    explicit inmatrix(QWidget *parent = nullptr);
    ~inmatrix();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::inmatrix *ui;
};

#endif // INMATRIX_H
