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

private slots:
    void on_pushButton_clicked();

    void on_type_currentIndexChanged(int index);

    void on_Stroka_textChanged(const QString &arg1);

private:
    Ui::creatematrix *ui;
};

#endif // CREATEMATRIX_H
