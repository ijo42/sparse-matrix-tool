#include "mainwindow.h"
#include "shared.h"
#include "ui_mainwindow.h"
#include "inmatrix.h"
#include "explorer.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    auto matrix = SparseDoubleLinkedMatrix();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_7_clicked()//создать матрицу А
{
    inmatrix w1;
    w1.setModal(true);
    w1.exec();
}


void MainWindow::on_pushButton_10_clicked()//Создать матрицу Б
{
    inmatrix w2;
    w2.setModal(true);
    w2.exec();
}


void MainWindow::on_pushButton_8_clicked()//сохранить мартицу А
{

}


void MainWindow::on_pushButton_9_clicked()//сохранить матрицу Б
{
    explorer w3;
    w3.setModal(true);
    w3.exec();
}


void MainWindow::on_pushButton_clicked()//открыть проводник матриц
{

}


void MainWindow::on_pushButton_2_clicked()//произвести обмен матриц
{

}


void MainWindow::on_pushButton_3_clicked()//произвести сложение матриц
{

}


void MainWindow::on_pushButton_4_clicked()//произвести вычитание матриц
{

}


void MainWindow::on_pushButton_5_clicked()//произвести умножение матриц
{

}


void MainWindow::on_pushButton_6_clicked()//вычислить обратную матрицу
{

}

