#include "../headers/creatematrix.h"
#include "operations.h"
#include "ui_creatematrix.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QString>
#include <QDebug>
#include <headers/explorer.h>

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

    if (ok && value >= 1 && value <= 10000) {
        //qDebug() << "Значение в допустимом диапазоне:" << value;
        return true;
    } else {
        //qDebug() << "Значение вне допустимого диапазона или не является числом";
        return false;
    }
}

void creatematrix::on_pushButton_clicked()//создать матрицу
{
    bool rowOk, columnOk;
    int row = ui->Stroka->text().toInt(&rowOk), column = ui->Stolbec->text().toInt(&columnOk);

    if (rowOk&&columnOk)
    {
        auto matrix = !ui->checkBox->checkState() ? generateEmpty(row,column) : generateRnd(row, column);;
        matrix->name = QString("Матрица %1").arg(1+explorer::getMatrixs().size()).toStdString();
        explorer::getMatrixs().append(matrix);
        if(explorer* v = dynamic_cast<explorer*>(parent()->parent())) {
            v->refresh();
        }
    }
    else
    {
        QMessageBox::warning(this,"Ошибка","Значения строк и стобцов могут быть только целые числа от 1 до 10000");
            switch(type) {
            case 0:
                return generateRnd(row,column);
            case 1:
                return generateUnitMatrix(row);
            case 2:
            default:
                return generateEmpty(row,column);
            }
void creatematrix::on_type_currentIndexChanged(int index)
{
    if(index == 1) {
        ui->Stolbec->setReadOnly(true);
        if(QString::compare(ui->Stroka->text(), ui->Stolbec->text(), Qt::CaseInsensitive) != 0){
            QMessageBox::information(this, "Внимание", "Лишь квадратная единичная матрица может быть создана");
        }
        ui->Stolbec->setText(ui->Stroka->text());
    } else {
        ui->Stolbec->setReadOnly(false);
    }
}

