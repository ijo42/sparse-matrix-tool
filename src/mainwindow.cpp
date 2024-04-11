#include "../headers/mainwindow.h"
#include "shared.h"
#include "ui_mainwindow.h"
#include "../headers/inmatrix.h"
#include "../headers/explorer.h"

#include <headers/sparsematrixmodel.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->matrixAView->hide();
    ui->matrixBView->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_exlporerButton_clicked()
{
    explorer w1;
    w1.setModal(true);
    w1.exec();
}


void MainWindow::on_matrixAButton_clicked()
{
    inmatrix w1;
    w1.setModal(true);
    w1.exec();
}


void MainWindow::on_pushButton_8_clicked()
{
    auto model = new SparseMatrixModel(explorer::getMatrixs()[0]);
    ui->matrixAView->setModel(model);
    ui->matrixAView->show();
    ui->matrixAButton->hide();
}


void MainWindow::on_pushButton_9_clicked()
{
    auto model = SparseMatrixModel(explorer::getMatrixs()[1]);
    ui->matrixBView->setModel(&model);
    ui->matrixBView->show();
    ui->matrixAButton->hide();

}

