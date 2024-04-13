#include "../headers/mainwindow.h"
#include "shared.h"
#include "operations.h"
#include "ui_mainwindow.h"
#include "../headers/explorer.h"

#include <headers/sparsematrixmodel.h>

#include <headers/details.h>
#include <headers/preview.h>

#include <QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>
#include <QProgressDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->matrixAView->hide();
    ui->matrixBView->hide();
    ui->pushButton_8->hide();
    ui->pushButton_9->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_exlporerButton_clicked()
{
    explorer w1;
    w1.setModal(true);
    w1.setWindowModality(Qt::NonModal);
    w1.exec();
}


void MainWindow::on_matrixAButton_clicked()
{
    explorer w1(nullptr, ui->matrixAView, ui->matrixAButton, ui->pushButton_8, ui->matrixALabel, &matrixA);
    w1.setModal(true);
    w1.setWindowModality(Qt::NonModal);
    w1.exec();
}

void MainWindow::on_matrixBButton_clicked()
{
    explorer w1(nullptr, ui->matrixBView, ui->matrixBButton, ui->pushButton_9, ui->matrixBLabel, &matrixB);
    w1.setModal(true);
    w1.setWindowModality(Qt::NonModal);
    w1.exec();
}

void MainWindow::on_pushButton_8_clicked()
{
    ui->matrixAView->hide();
    ui->pushButton_8->hide();
    ui->matrixAButton->show();
    ui->matrixALabel->show();
    matrixA = nullptr;
}


void MainWindow::on_pushButton_9_clicked()
{
    ui->matrixBView->hide();
    ui->pushButton_9->hide();
    ui->matrixBButton->show();
    ui->matrixBLabel->show();
    matrixB = nullptr;
}

void MainWindow::on_swapButton_clicked(){
    std::swap(matrixA, matrixB);
    if (matrixA) {
        auto model = new SparseMatrixModel(matrixA);
        ui->matrixAView->setModel(model);
        ui->matrixAButton->hide();
        ui->matrixAView->show();
        ui->pushButton_8->show();
        ui->matrixALabel->hide();
    }
    else{
        ui->matrixAView->hide();
        ui->matrixAButton->show();
        ui->pushButton_8->hide();
        ui->matrixALabel->show();
    }
    if (matrixB){
        auto model = new SparseMatrixModel(matrixB);
        ui->matrixBView->setModel(model);
        ui->matrixBButton->hide();
        ui->matrixBView->show();
        ui->pushButton_9->show();
        ui->matrixBLabel->hide();
    }
    else{
        ui->matrixBView->hide();
        ui->matrixBButton->show();
        ui->pushButton_9->hide();
        ui->matrixBLabel->show();
    }
}

void MainWindow::on_sumButton_clicked(){
    if (!(matrixA && matrixB)) QMessageBox::warning(this, "Предупреждение", "Недостаточно матриц для обработки");
    else{
        SparseDoubleLinkedMatrix * m = add(*matrixA, *matrixB);
        if (m){
            m->name = m->name = matrixA->name + " + " + matrixB->name;
            explorer::getMatrixs().append(m);
            auto preview = new Preview(m);
            preview->show();
        }
        else {
            QMessageBox::warning(this, "Предупреждение", "Разная размерность матриц");
        }
    }
}

void MainWindow::on_subButton_clicked(){
    if (!(matrixA && matrixB))
        QMessageBox::warning(this, "Предупреждение", "Недостаточно матриц для обработки");
    else{
        SparseDoubleLinkedMatrix * m = sub(*matrixA, *matrixB);
        if (m){
            m->name = m->name = matrixA->name + " - " + matrixB->name;
            explorer::getMatrixs().append(m);
            auto preview = new Preview(m);
            preview->show();
        }
        else {
            QMessageBox::warning(this, "Предупреждение", "Разная размерность матриц");
        }
    }
}


void MainWindow::on_multiplyButton_clicked(){
    if (!(matrixA && matrixB)) {
        QMessageBox::warning(this, "Предупреждение", "Недостаточно матриц для обработки");
        return;
    }

    // Показать диалог прогресса
    auto progress = new QProgressDialog("Умножение матриц...", "Отмена", 0, 0, this);
    progress->setWindowModality(Qt::NonModal);
    progress->show();

    // Асинхронное выполнение умножения
    QFuture<SparseDoubleLinkedMatrix*> future = QtConcurrent::run([this]() -> SparseDoubleLinkedMatrix* {
        return multiply(*matrixA, *matrixB);
    });

    // Создание QFutureWatcher для отслеживания завершения задачи
    QFutureWatcher<SparseDoubleLinkedMatrix*> *watcher = new QFutureWatcher<SparseDoubleLinkedMatrix*>(this);
    connect(watcher, &QFutureWatcher<SparseDoubleLinkedMatrix*>::finished, this, [this, watcher, progress]() {
        progress->close(); // Скрываем диалог прогресса
        SparseDoubleLinkedMatrix *m = watcher->result();
        watcher->deleteLater();
        if (m) {
            m->name = matrixA->name + " * " + matrixB->name;
            explorer::getMatrixs().append(m);
            auto preview = new Preview(m);
            preview->show();
        } else {
            QMessageBox::warning(this, "Предупреждение", "Матрицы имеют не сопостовимые размерности");
        }
    });
    watcher->setFuture(future);
}

void MainWindow::on_reverseButton_clicked(){

    auto progress = new QProgressDialog("Нахождение обратной матрицы...", "Отмена", 0, 0, this);
    progress->setWindowModality(Qt::NonModal);
    progress->show();

    QFuture<SparseDoubleLinkedMatrix*> future;
    if (matrixA){
        future = QtConcurrent::run([this]() -> SparseDoubleLinkedMatrix* {
            return inverseMatrix(*matrixA);
        });
    }
    else if (matrixB) {
        future = QtConcurrent::run([this]() -> SparseDoubleLinkedMatrix* {
            return inverseMatrix(*matrixB);
        });
    }

    QFutureWatcher<SparseDoubleLinkedMatrix*> *watcher = new QFutureWatcher<SparseDoubleLinkedMatrix*>(this);
    connect(watcher, &QFutureWatcher<SparseDoubleLinkedMatrix*>::finished, this, [this, watcher, progress]() {
        progress->close(); // Скрываем диалог прогресса
        SparseDoubleLinkedMatrix *m = watcher->result();
        watcher->deleteLater();
        if (m) {
            m->name = matrixA->name + " * " + matrixB->name;
            explorer::getMatrixs().append(m);
            auto preview = new Preview(m);
            preview->show();
        } else {
            if (matrixA||matrixB) QMessageBox::warning(this, "Предупреждение", "Определитель равен 0");
            else QMessageBox::warning(this, "Предупреждение", "Матрицы имеют не сопостовимые размерности");
        }
    });
}
