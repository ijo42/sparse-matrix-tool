#ifndef ERROR_H
#define ERROR_H

#include <QDialog>
#include <QLabel>

namespace Ui {
class error;
}

class error : public QDialog
{
    Q_OBJECT

public:
    explicit error(QWidget *parent = nullptr);
    ~error();
    QLabel errormessage;

private slots:
    void on_pushButton_clicked();

private:
    Ui::error *ui;
};

#endif // ERROR_H
