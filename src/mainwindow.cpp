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
    w1.setWindowModality(Qt::ApplicationModal);
    w1.exec();
}

void MainWindow::on_matrixBButton_clicked()
{
    explorer w1(nullptr, ui->matrixBView, ui->matrixBButton, ui->matrixBPane, ui->matrixBLabel, &matrixB);
    w1.setModal(true);
    w1.setWindowModality(Qt::ApplicationModal);
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
    SparseDoubleLinkedMatrix *matrix = nullptr;

    if(matrixA && matrixB){
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Выбор матрицы");
        msgBox.setText("Выберите матрицу для операции:");
        msgBox.setIcon(QMessageBox::Question);

        // Добавление кнопок для выбора матриц
        QAbstractButton* buttonA = msgBox.addButton("Матрица A", QMessageBox::AcceptRole);
        QAbstractButton* buttonB = msgBox.addButton("Матрица B", QMessageBox::RejectRole);

        msgBox.exec();  // Отображение диалогового окна

        if (msgBox.clickedButton() == buttonA) {
            matrix = matrixA;
        } else if (msgBox.clickedButton() == buttonB) {
            matrix = matrixB;
        }
    } else if(matrixA) {
        matrix = matrixA;
    } else if(matrixB) {
        matrix = matrixB;
    } else {
        QMessageBox::warning(this, "Предупреждение", "Недостаточно матриц для обработки");
        return;
    }

    if(matrix->columnPointer.size() != matrix->linePointer.size()){
        QMessageBox::warning(this, "Предупреждение", "Обратная матрица существует только для квадратной матрицы");
        return;
    }

    auto progress = new QProgressDialog("Нахождение обратной матрицы...", "Отмена", 0, 0, this);
    progress->setWindowModality(Qt::WindowModal);
    progress->show();

    QFuture<SparseDoubleLinkedMatrix*> future = QtConcurrent::run([matrix]() -> SparseDoubleLinkedMatrix* {
        return inverseMatrix(*matrix);
    });
    

    QFutureWatcher<SparseDoubleLinkedMatrix*> *watcher = new QFutureWatcher<SparseDoubleLinkedMatrix*>(this);
    connect(watcher, &QFutureWatcher<SparseDoubleLinkedMatrix*>::finished, this, [this, watcher, progress, matrix]() {
        progress->close(); // Скрываем диалог прогресса
        SparseDoubleLinkedMatrix *m = watcher->result();
        watcher->deleteLater();
        if (m) {
            m->name = matrix->name + " ^-1 ";
            explorer::getMatrixs().append(m);
            auto preview = new Preview(m);
            preview->show();
        } else {
            QMessageBox::warning(this, "Предупреждение", "Определитель равен 0");
        }
    });
    watcher->setFuture(future);
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
    MainWindow::save(matrixB, this, false);

}


void MainWindow::on_btnSaveA_clicked()
{
    MainWindow::save(matrixA, this, false);
}

void MainWindow::save(SparseDoubleLinkedMatrix *matrix, QWidget *widget, bool isFull)
{
    QString filename = QFileDialog::getSaveFileName(widget, "Сохранить матрицу как", "", isFull ? "Matrix Content (*.txt)" : "Double Linked Sparse Matrix Files (*.dlsm)");
    if (filename.isEmpty())
        return;  // Пользователь отменил сохранение

    try {
        std::string path = filename.toStdString();  // Преобразуем QString в std::string
        if(isFull) {
            saveFullToFile(path, *matrix);
        } else {
            saveToFile(path, *matrix);  // Вызываем функцию сохранения, определенную в другом месте
        }
        QMessageBox::information(widget, "Сохранение", "Матрица успешно сохранена!");
    } catch (const std::exception& e) {
        // Обработка возможных исключений при сохранении файла
        QMessageBox::warning(widget, "Ошибка", QString("Произошла ошибка при сохранении файла: %1").arg(e.what()));
    }
}

