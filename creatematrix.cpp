#include "creatematrix.h"
#include "ui_creatematrix.h"

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

void creatematrix::on_pushButton_clicked()//создать матрицу
{

}

