#include "warring.h"
#include "ui_warring.h"
#include "explorer.h"

warring::warring(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::warring)
{
    ui->setupUi(this);
}

warring::~warring()
{
    delete ui;
}

void warring::on_pushButton_clicked()//перезаписать
{

}


void warring::on_pushButton_2_clicked()//отменить
{

}

