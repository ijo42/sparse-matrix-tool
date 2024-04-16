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
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);


    ui->matrixAView->hide();
    ui->matrixBView->hide();
    ui->matrixAPane->hide();
    ui->matrixBPane->hide();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_exlporerButton_clicked() {
    explorer w1;
    w1.setModal(true);
    w1.setWindowModality(Qt::NonModal);
    w1.exec();
}


void MainWindow::on_matrixAButton_clicked() {
    explorer w1(nullptr, ui->matrixAView, ui->matrixAButton, ui->matrixAPane, ui->matrixALabel, &matrixA);
    w1.setModal(true);
    w1.setWindowModality(Qt::ApplicationModal);
    w1.exec();
}

void MainWindow::on_matrixBButton_clicked() {
    explorer w1(nullptr, ui->matrixBView, ui->matrixBButton, ui->matrixBPane, ui->matrixBLabel, &matrixB);
    w1.setModal(true);
    w1.setWindowModality(Qt::ApplicationModal);
    w1.exec();
}

void MainWindow::on_swapButton_clicked() {
    std::swap(matrixA, matrixB);
    refreshWindow();
}

void MainWindow::on_sumButton_clicked() {
    if (!(matrixA && matrixB)) {
        QMessageBox::warning(this, "Предупреждение", "Недостаточно матриц для обработки.");
    } else {
        // Показать диалог прогресса
        auto progress = new QProgressDialog("Сложение матриц...", "Отмена", 0, 0, this);
        progress->setWindowModality(Qt::WindowModal);
        progress->show();

        // Асинхронное выполнение умножения
        QFuture < SparseDoubleLinkedMatrix * > future = QtConcurrent::run([this]() -> SparseDoubleLinkedMatrix * {
            return add(*matrixA, *matrixB);
        });

        // Создание QFutureWatcher для отслеживания завершения задачи
        QFutureWatcher < SparseDoubleLinkedMatrix * > *watcher = new QFutureWatcher<SparseDoubleLinkedMatrix *>(this);
        connect(watcher, &QFutureWatcher<SparseDoubleLinkedMatrix *>::finished, this, [this, watcher, progress]() {
            progress->close(); // Скрываем диалог прогресса
            SparseDoubleLinkedMatrix *m = watcher->result();
            watcher->deleteLater();
            if (m) {
                if (countElements(*m) > maxElements(*m)) {
                    QMessageBox::warning(this, "Предупреждение",
                                         "Получившаяся в ходе операции матрица не является разряженной. Обработка невозможна.");
                } else {
                    m->name = matrixA->name + " + " + matrixB->name;
                    explorer::getMatrixs().append(m);
                    QMessageBox::information(this, "Операция над матрицами",
                                             "Операция успешна! Новая матрица была добавлена в проводник.");
                }
            } else {
                QMessageBox::warning(this, "Предупреждение", "Матрицы разной размерности не могут сложены");
            }
        });
        watcher->setFuture(future);
    }
}

void MainWindow::on_subButton_clicked() {
    if (!(matrixA && matrixB)) {
        QMessageBox::warning(this, "Предупреждение", "Недостаточно матриц для обработки.");
    } else {
        // Показать диалог прогресса
        auto progress = new QProgressDialog("Вычитание матриц...", "Отмена", 0, 0, this);
        progress->setWindowModality(Qt::WindowModal);
        progress->show();

        // Асинхронное выполнение умножения
        QFuture < SparseDoubleLinkedMatrix * > future = QtConcurrent::run([this]() -> SparseDoubleLinkedMatrix * {
            return sub(*matrixA, *matrixB);
        });

        // Создание QFutureWatcher для отслеживания завершения задачи
        QFutureWatcher < SparseDoubleLinkedMatrix * > *watcher = new QFutureWatcher<SparseDoubleLinkedMatrix *>(this);
        connect(watcher, &QFutureWatcher<SparseDoubleLinkedMatrix *>::finished, this, [this, watcher, progress]() {
            progress->close(); // Скрываем диалог прогресса
            SparseDoubleLinkedMatrix *m = watcher->result();
            watcher->deleteLater();
            if (m) {
                if (countElements(*m) > maxElements(*m)) {
                    QMessageBox::warning(this, "Предупреждение",
                                         "Получившаяся в ходе операции матрица не является разряженной. Обработка невозможна.");
                } else {
                    m->name = matrixA->name + " - " + matrixB->name;
                    explorer::getMatrixs().append(m);
                    QMessageBox::information(this, "Операция над матрицами",
                                             "Операция успешна! Новая матрица была добавлена в проводник.");
                }
            } else {
                QMessageBox::warning(this, "Предупреждение", "Матрицы разной размерности не могут вычитаться");
            }
        });
        watcher->setFuture(future);
    }
}


void MainWindow::on_multiplyButton_clicked() {
    if (!(matrixA && matrixB)) {
        QMessageBox::warning(this, "Предупреждение", "Недостаточно матриц для обработки.");
    } else {
        // Показать диалог прогресса
        auto progress = new QProgressDialog("Умножение матриц...", "Отмена", 0, 0, this);
        progress->setWindowModality(Qt::WindowModal);
        progress->show();

        // Асинхронное выполнение умножения
        QFuture < SparseDoubleLinkedMatrix * > future = QtConcurrent::run([this]() -> SparseDoubleLinkedMatrix * {
            return multiply(*matrixA, *matrixB);
        });

        // Создание QFutureWatcher для отслеживания завершения задачи
        QFutureWatcher < SparseDoubleLinkedMatrix * > *watcher = new QFutureWatcher<SparseDoubleLinkedMatrix *>(this);
        connect(watcher, &QFutureWatcher<SparseDoubleLinkedMatrix *>::finished, this, [this, watcher, progress]() {
            progress->close(); // Скрываем диалог прогресса
            SparseDoubleLinkedMatrix *m = watcher->result();
            watcher->deleteLater();
            if (m) {
                if (countElements(*m) > maxElements(*m)) {
                    QMessageBox::warning(this, "Предупреждение",
                                         "Получившаяся в ходе операции матрица не является разряженной. Обработка невозможна.");
                } else {
                    m->name = matrixA->name + " * " + matrixB->name;
                    explorer::getMatrixs().append(m);
                    QMessageBox::information(this, "Операция над матрицами",
                                             "Операция успешна! Новая матрица была добавлена в проводник.");
                }
            } else {
                QMessageBox::warning(this, "Предупреждение",
                                     "Число столбцов А не равно числу строк B, произведения A*B не существует.");
            }
        });
        watcher->setFuture(future);
    }
}

void MainWindow::on_reverseButton_clicked() {
    SparseDoubleLinkedMatrix *matrix = nullptr;

    if (matrixA && matrixB) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Выбор матрицы");
        msgBox.setText("Выберите матрицу для операции:");
        msgBox.setIcon(QMessageBox::Question);

        // Добавление кнопок для выбора матриц
        QAbstractButton *buttonA = msgBox.addButton("Матрица A", QMessageBox::AcceptRole);
        QAbstractButton *buttonB = msgBox.addButton("Матрица B", QMessageBox::RejectRole);

        msgBox.exec();  // Отображение диалогового окна

        if (msgBox.clickedButton() == buttonA) {
            matrix = matrixA;
        } else if (msgBox.clickedButton() == buttonB) {
            matrix = matrixB;
        }
    } else if (matrixA) {
        matrix = matrixA;
    } else if (matrixB) {
        matrix = matrixB;
    } else {
        QMessageBox::warning(this, "Предупреждение", "Недостаточно матриц для обработки");
        return;
    }

    if (matrix->columnPointer.size() != matrix->linePointer.size()) {
        QMessageBox::warning(this, "Предупреждение", "Обратная матрица существует только для квадратной матрицы");
        return;
    }

    auto progress = new QProgressDialog("Нахождение обратной матрицы...", "Отмена", 0, 0, this);
    progress->setWindowModality(Qt::WindowModal);
    progress->show();

    QFuture < SparseDoubleLinkedMatrix * > future = QtConcurrent::run([matrix]() -> SparseDoubleLinkedMatrix * {
        return inverseMatrix(*matrix);
    });


    QFutureWatcher < SparseDoubleLinkedMatrix * > *watcher = new QFutureWatcher<SparseDoubleLinkedMatrix *>(this);
    connect(watcher, &QFutureWatcher<SparseDoubleLinkedMatrix *>::finished, this, [this, watcher, progress, matrix]() {
        progress->close(); // Скрываем диалог прогресса
        SparseDoubleLinkedMatrix *m = watcher->result();
        watcher->deleteLater();
        if (m) {
            if (countElements(*m) > maxElements(*m)) {
                QMessageBox::warning(this, "Предупреждение",
                                     "Получившаяся в ходе операции матрица не является разряженной. Обработка невозможна.");
            } else {
                m->name = matrix->name + " ^-1 ";
                explorer::getMatrixs().append(m);
                QMessageBox::information(this, "Операция над матрицей",
                                         "Операция успешна! Новая матрица была добавлена в проводник.");
            }
        } else {
            QMessageBox::warning(this, "Предупреждение",
                                 "Определитель матрицы равен 0, обратной матрицы не существует");
        }
    });
    watcher->setFuture(future);
}

void MainWindow::on_btnDeleteA_clicked() {
    ui->matrixAView->hide();
    ui->matrixAPane->hide();
    ui->matrixAButton->show();
    ui->matrixALabel->show();
    matrixA = nullptr;
}


void MainWindow::on_btnDeleteB_clicked() {
    ui->matrixBView->hide();
    ui->matrixBPane->hide();
    ui->matrixBButton->show();
    ui->matrixBLabel->show();
    matrixB = nullptr;
}


void MainWindow::on_btnSaveB_clicked() {
    MainWindow::save(matrixB, this, false);

}


void MainWindow::on_btnSaveA_clicked() {
    MainWindow::save(matrixA, this, false);
}

bool MainWindow::isCompatibleWithStdString(const QString &str) {
    // Проверяем каждый символ в QString
    for (int i = 0; i < str.length(); ++i) {
        QChar ch = str[i];
        // Если символ выходит за пределы ASCII (0-127), возвращаем false
        if (ch.unicode() > 127) {
            return false;
        }
    }
    // Все символы в строке совместимы с ASCII
    return true;
}


void MainWindow::save(SparseDoubleLinkedMatrix *matrix, QWidget *widget, bool isFull) {
    QString filename = QFileDialog::getSaveFileName(widget, "Сохранить матрицу как", "",
                                                    isFull ? "Matrix Content (*.txt)"
                                                           : "Double Linked Sparse Matrix Files (*.dlsm)");
    if (filename.isEmpty())
        return;  // Пользователь отменил сохранение


    if(!isCompatibleWithStdString(filename)) {
        QMessageBox::warning(widget, "Ошибка", "В пути сохраняемого файла обнаружены сиволы не латинского алфавита. Сохранение невозможно");
        return;
    }

    std::string path = filename.toStdString();  // Преобразуем QString в std::string


    auto progress = new QProgressDialog("Сохранение файла...", "Отмена", 0, 0, widget);
    progress->setWindowModality(Qt::WindowModal);
    progress->show();

    // Используем QtConcurrent::run для асинхронного выполнения сохранения
    QFuture<void> future = QtConcurrent::run([matrix, isFull, path]() -> void {
        try {
            if (isFull) {
                //saveFullToFile(path, *matrix);  // Предполагается, что это функция сохраняет всю матрицу
                int i;
            } else {
                saveToFile(path, *matrix);  // Вызываем функцию сохранения
            }
        } catch (const std::exception &e) {
            throw std::runtime_error(e.what());  // Переброс исключения для обработки в основном потоке
        }
    });

    // Отслеживаем завершение асинхронной операции
    QFutureWatcher<void> *watcher = new QFutureWatcher<void>(widget);
    connect(watcher, &QFutureWatcher<void>::finished, widget, [widget, watcher, progress, path]() {
        watcher->deleteLater();
        progress->close();  // Закрываем прогресс-бар
        QMessageBox::information(widget, "Сохранение", "Матрица успешно сохранена!");

    });
    watcher->setFuture(future);
}


void MainWindow::refreshWindow(){
    if (matrixA && !matrixA->columnPointer.empty()) {
        auto model = new SparseMatrixModel(matrixA);
        ui->matrixAView->setModel(model);

        QModelIndex firstRowIndex = model->index(0, 0);
        ui->matrixAView->scrollTo(firstRowIndex, QAbstractItemView::PositionAtTop);

        QLabel* label = ui->matrixAPane->findChild<QLabel*>();
        if(label) {
            label->setText(QString::fromStdString(matrixA->name) +
                           QString(" [%0 x %1]").arg(matrixA->columnPointer.size()).arg(
                               matrixA->linePointer.size()));
        }
        ui->matrixAButton->hide();
        ui->matrixAView->show();
        ui->matrixAPane->show();
        ui->matrixALabel->hide();
    } else {
        ui->matrixAView->hide();
        ui->matrixAButton->show();
        ui->matrixAPane->hide();
        ui->matrixALabel->show();
    }
    if (matrixB&& !matrixB->columnPointer.empty()) {
        auto model = new SparseMatrixModel(matrixB);
        ui->matrixBView->setModel(model);

        QModelIndex firstRowIndex = model->index(0, 0);
        ui->matrixBView->scrollTo(firstRowIndex, QAbstractItemView::PositionAtTop);
        QLabel* label = ui->matrixBPane->findChild<QLabel*>();
        if(label) {
            label->setText(QString::fromStdString(matrixB->name) +
                           QString(" [%0 x %1]").arg(matrixB->columnPointer.size()).arg(
                               matrixB->linePointer.size()));
        }

        ui->matrixBButton->hide();
        ui->matrixBView->show();
        ui->matrixBPane->show();
        ui->matrixBLabel->hide();
    } else {
        ui->matrixBView->hide();
        ui->matrixBButton->show();
        ui->matrixBPane->hide();
        ui->matrixBLabel->show();
    }
}
