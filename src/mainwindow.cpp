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
#include <qfiledialog.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->matrixAView->hide();
    ui->matrixBView->hide();
    ui->matrixAPane->hide();
    ui->matrixBPane->hide();
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
    explorer w1(nullptr, ui->matrixAView, ui->matrixAButton, ui->matrixAPane, ui->matrixALabel, &matrixA);
    w1.setModal(true);
    w1.setWindowModality(Qt::NonModal);
    w1.exec();
}

void MainWindow::on_matrixBButton_clicked()
{
    explorer w1(nullptr, ui->matrixBView, ui->matrixBButton, ui->matrixBPane, ui->matrixBLabel, &matrixB);
    w1.setModal(true);
    w1.setWindowModality(Qt::NonModal);
    w1.exec();
}

void MainWindow::on_swapButton_clicked(){
    std::swap(matrixA, matrixB);
    if (matrixA) {
        auto model = new SparseMatrixModel(matrixA);
        ui->matrixAView->setModel(model);
        ui->matrixAButton->hide();
        ui->matrixAView->show();
        ui->matrixAPane->show();
        ui->matrixALabel->hide();
    }
    else{
        ui->matrixAView->hide();
        ui->matrixAButton->show();
        ui->matrixAPane->hide();
        ui->matrixALabel->show();
    }
    if (matrixB){
        auto model = new SparseMatrixModel(matrixB);
        ui->matrixBView->setModel(model);
        ui->matrixBButton->hide();
        ui->matrixBView->show();
        ui->matrixBPane->show();
        ui->matrixBLabel->hide();
    }
    else{
        ui->matrixBView->hide();
        ui->matrixBButton->show();
        ui->matrixBPane->hide();
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
    progress->setWindowModality(Qt::WindowModal);
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
    progress->setWindowModality(Qt::WindowModal);
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
            if (matrixA) m->name = matrixA->name + " ^-1 ";
            else if (matrixB) m->name = matrixB->name + " ^-1 ";
            explorer::getMatrixs().append(m);
            auto preview = new Preview(m);
            preview->show();
        } else {
            if (matrixA||matrixB) QMessageBox::warning(this, "Предупреждение", "Определитель равен 0");
            else QMessageBox::warning(this, "Предупреждение", "Матрицы имеют не сопостовимые размерности");
        }
    });
}

void MainWindow::on_btnDeleteA_clicked()
{
    ui->matrixAView->hide();
    ui->matrixAPane->hide();
    ui->matrixAButton->show();
    ui->matrixALabel->show();
    matrixA = nullptr;
}


void MainWindow::on_btnDeleteB_clicked()
{
    ui->matrixBView->hide();
    ui->matrixBPane->hide();
    ui->matrixBButton->show();
    ui->matrixBLabel->show();
    matrixB = nullptr;
}


void MainWindow::on_btnSaveB_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "Сохранить матрицу как", "", "Double Linked Sparse Matrix Files (*.dlsm)");
    if (filename.isEmpty())
        return;  // Пользователь отменил сохранение

    try {
        std::string path = filename.toStdString();  // Преобразуем QString в std::string
        saveToFile(path, *matrixB);  // Вызываем функцию сохранения, определенную в другом месте
        QMessageBox::information(this, "Сохранение", "Матрица успешно сохранена!");
    } catch (const std::exception& e) {
        // Обработка возможных исключений при сохранении файла
        QMessageBox::warning(this, "Ошибка", QString("Произошла ошибка при сохранении файла: %1").arg(e.what()));
    }
}


void MainWindow::on_btnSaveA_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "Сохранить матрицу как", "", "Double Linked Sparse Matrix Files (*.dlsm)");
    if (filename.isEmpty())
        return;  // Пользователь отменил сохранение

    try {
        std::string path = filename.toStdString();  // Преобразуем QString в std::string
        saveToFile(path, *matrixA);  // Вызываем функцию сохранения, определенную в другом месте
        QMessageBox::information(this, "Сохранение", "Матрица успешно сохранена!");
    } catch (const std::exception& e) {
        // Обработка возможных исключений при сохранении файла
        QMessageBox::warning(this, "Ошибка", QString("Произошла ошибка при сохранении файла: %1").arg(e.what()));
    }
}

