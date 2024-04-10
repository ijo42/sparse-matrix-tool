#include "../headers/creatematrix.h"
#include "ui_creatematrix.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QString>
#include <QDebug>

creatematrix::creatematrix(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::creatematrix)
{
    ui->setupUi(this);
}

creatematrix::~creatematrix()
{
    delete ui;
}
bool checkLineEditValue(QLineEdit* lineEdit) {
    bool ok;
    int value = lineEdit->text().toInt(&ok);

    if (ok && value >= 2 && value <= 10000) {
        //qDebug() << "Значение в допустимом диапазоне:" << value;
        return true;
    } else {
        //qDebug() << "Значение вне допустимого диапазона или не является числом";
        return false;
    }
}

void creatematrix::on_pushButton_clicked()//создать матрицу
{

    if ((checkLineEditValue(ui->Stroka))&&(checkLineEditValue(ui->Stolbec)))
    {
        //вызов функции создания
    }
    else
    {
        QMessageBox::warning(this,"Ошибка","Строки/стобцы могут быть только целые числа от 2 до 10000");
    }
}

