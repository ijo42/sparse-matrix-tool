#include "../headers/mainwindow.h"
#include "shared.h"
#include "operations.h"
#include "ui_mainwindow.h"
#include "../headers/inmatrix.h"
#include "../headers/explorer.h"

#include <headers/sparsematrixmodel.h>

#include <headers/details.h>


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
    explorer w1(nullptr, ui->matrixAView, ui->matrixAButton, ui->matrixALabel, &matrixA);
    w1.setModal(true);
    w1.exec();
}

void MainWindow::on_matrixBButton_clicked()
{
    explorer w1(nullptr, ui->matrixBView, ui->matrixBButton, ui->matrixBLabel, &matrixB);
    w1.setModal(true);
    w1.exec();
}

void MainWindow::on_pushButton_8_clicked()
{
    auto detail = new Details(matrixA);
    detail->show();
}


void MainWindow::on_pushButton_9_clicked()
{
    auto detail = new Details(matrixB);
    detail->show();
}

void MainWindow::on_swapButton_clicked(){
    std::swap(ui->matrixAView, ui->matrixBView);
    std::swap(matrixA, matrixB);
}

void MainWindow::on_sumButton_clicked() {
    if (!(matrixA && matrixB)) QMessageBox::warning(this, "Предупреждение", "Недостаточно матриц для обработки");
    else {
        SparseDoubleLinkedMatrix* m = add(*matrixA, *matrixB);
        m->name = m->name = matrixA->name + " + " + matrixB->name;
        explorer::getMatrixs().append(m);
    }
}

void MainWindow::on_subButton_clicked() {
    if (!(matrixA && matrixB))
        QMessageBox::warning(this, "Предупреждение", "Недостаточно матриц для обработки");
    else {
        SparseDoubleLinkedMatrix* m = sub(*matrixA, *matrixB);
        m->name = matrixA->name + " - " + matrixB->name;
        explorer::getMatrixs().append(m);
    }
}

void MainWindow::on_multiplyButton_clicked() {
    if (!(matrixA && matrixB)) QMessageBox::warning(this, "Предупреждение", "Недостаточно матриц для обработки");
    else {
        SparseDoubleLinkedMatrix* m = multiply(*matrixA, *matrixB);
        m->name = matrixA->name + " * " + matrixB->name;
        explorer::getMatrixs().append(m);
    }
}

void MainWindow::on_reverseButton_clicked() {
    SparseDoubleLinkedMatrix* m = nullptr;
    if (matrixA) m = inverseMatrix(*matrixA);
    else if (matrixB) m = inverseMatrix(*matrixB);
    if (m == nullptr) {
        if (matrixA || matrixB) QMessageBox::warning(this, "Предупреждение", "Определитель равен нулю");
        else QMessageBox::warning(this, "Предупреждение", "Недостаточно матриц для обработки");
    }
    else {
        m->name = matrixA->name + " ^-1";
        explorer::getMatrixs().append(m);
    }
}