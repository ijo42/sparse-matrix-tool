#include "inmatrix.h"
#include "ui_inmatrix.h"
#include "creatematrix.h"
#include "explorer.h"

inmatrix::inmatrix(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::inmatrix)
{
    ui->setupUi(this);
}

inmatrix::~inmatrix()
{
    delete ui;
}

void inmatrix::on_pushButton_2_clicked()//открыть окно создания матрицы
{
    creatematrix w3;
    w3.setModal(true);
    w3.exec();
}


void inmatrix::on_pushButton_clicked()//открыть окно импорта
{

}

