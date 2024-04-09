#include "explorer.h"
#include "ui_explorer.h"
#include "inmatrix.h"

#include "QMessageBox"

explorer::explorer(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::explorer)
{
    ui->setupUi(this);
}

explorer::~explorer()
{
    delete ui;
}

void explorer::on_pushButton_clicked()//добавиление матрицы в проводник
{
    int k=0;
    if (ui->label->isEnabled()==true)
    {
        k++;
    }
    if (ui->label_2->isEnabled()==true)
    {
        k++;
    }
    if (ui->label_3->isEnabled()==true)
    {
        k++;
    }
    if (k==3)
    {
        QMessageBox::warning(this,"Ошибка","добавленное максимальное число матриц");
    }
    else
    {
    inmatrix w4;
    w4.setModal(true);
    w4.exec();
    }
}


void explorer::on_pushButton_4_clicked()//удалить матрицу 3
{
    ui->label_3->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    //добавить функцию удаления из памяти
}


void explorer::on_pushButton_7_clicked()//удалить матрицу 2
{
    ui->label_2->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
    ui->pushButton_7->setEnabled(false);
    ui->pushButton_8->setEnabled(false);
    ui->pushButton_9->setEnabled(false);
    //добавить функцию удаления из памяти
}


void explorer::on_pushButton_11_clicked()//удалить матрицу 1
{
    ui->label->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    //добавить функцию удаления из памяти
}


void explorer::on_pushButton_3_clicked()//посмотреть матрицу 3
{

}


void explorer::on_pushButton_6_clicked()//посмотреть матрицу 2
{

}


void explorer::on_pushButton_10_clicked()//посмотреть матрицу 1
{

}

