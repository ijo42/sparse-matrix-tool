#include "../headers/inmatrix.h"
#include "io.h"
#include "ui_inmatrix.h"
#include "../headers/creatematrix.h"
#include "QFileDialog"

#include <headers/explorer.h>
#include <headers/mainwindow.h>
#include <headers/preview.h>
#include <QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>

inmatrix::inmatrix(QWidget *parent)
        : QDialog(parent), ui(new Ui::inmatrix) {
    ui->setupUi(this);
}

inmatrix::~inmatrix() {
    delete ui;
}

void inmatrix::on_pushButton_2_clicked()//открыть окно создания матрицы
{
    creatematrix w3(this);
    w3.setModal(true);
    w3.exec();
}

QString inmatrix::getLastSubstringOrLastFive(const QString &input) {
    // Проверяем, содержит ли строка символ "/"
    int lastSlashIndex = input.lastIndexOf('/');
    if (lastSlashIndex != -1) {
        // Если содержит, возвращаем подстроку после последнего символа "/"
        return input.mid(lastSlashIndex + 1, input.lastIndexOf(".") - (lastSlashIndex + 1));
    } else {
        // Если не содержит, возвращаем последние 5 символов
        if (input.length() > 5) {
            return input.right(5);
        } else {
            // Если строка короче 5 символов, возвращаем всю строку
            return input;
        }
    }
}

void inmatrix::load(QString filename, QWidget *widget) {


    if(!MainWindow::isCompatibleWithStdString(filename)) {
        QMessageBox::warning(widget, "Ошибка", "В пути загружаемого файла обнаружены сиволы не латинского алфавита. Загрузка невозможно");
        return;
    }

    // Показать диалог прогресса
    auto progress = new QProgressDialog("Загрузка матрицы...", "Отмена", 0, 0, widget);
    progress->setWindowModality(Qt::WindowModal);
    progress->show();

    QFuture <std::pair<SparseDoubleLinkedMatrix *, bool>> future = QtConcurrent::run(
            [filename]() -> std::pair < SparseDoubleLinkedMatrix * , bool > {
                    bool isSuccess = false;
                    std::string path = filename.toStdString();
                    auto matrix = loadFromFileValidate(isSuccess, path);
                    return { matrix, isSuccess };
            });

    // Создание QFutureWatcher для отслеживания завершения задачи
    QFutureWatcher <std::pair<SparseDoubleLinkedMatrix *, bool>> *watcher = new QFutureWatcher <std::pair<SparseDoubleLinkedMatrix *, bool>>(
            widget);
    connect(watcher, &QFutureWatcher < std::pair < SparseDoubleLinkedMatrix * , bool >> ::finished, widget,
            [widget, watcher, filename, progress]() {
                progress->close(); // Скрываем диалог прогресса
                auto result = watcher->result();
                watcher->deleteLater();
                if (result.second) {
                    if (countElements(*result.first) > maxElements(*result.first)) {
                        QMessageBox::warning(widget, "Предупреждение",
                                             "Загружаемая матрица не является разряженной. Обработка невозможна.");
                    } else {
                        result.first->name = getLastSubstringOrLastFive(filename).toStdString();
                        explorer::getMatrixs().append(result.first);

                        if (explorer * v = dynamic_cast<explorer *>(widget)) { // drag'n'drop
                            v->refresh();
                            QMessageBox::information(widget, "Загрузка матрицы", "Матрица успешно загружена!");

                        } else {    // загрузка через окно выбора файла
                            if (explorer * v = dynamic_cast<explorer *>(widget->parent())) {
                                v->refresh();
                            }
                            QMessageBox msgBox(widget);
                            msgBox.setWindowTitle("Создание матрицы");
                            msgBox.setText("Матрица успешно загружена!");
                            msgBox.setInformativeText("Хотите импортировать еще одну матрицу?");
                            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                            msgBox.setDefaultButton(QMessageBox::Yes);
                            if (msgBox.exec() == QMessageBox::No) {
                                (widget)->close();
                            }
                        }
                    }
                } else {
                    QMessageBox::warning(widget, "Предупреждение",
                                         "Данный файл не содержит матрицу в поддерживаемом формате.");
                }
            });
    watcher->setFuture(future);
}

void inmatrix::on_pushButton_clicked() {
    QString filename = QFileDialog::getOpenFileName(this, "Выберите файл матрицы", "",
                                                    "Double Linked Sparse Matrix Files (*.dlsm)");
    if (filename.isEmpty())
        return;


    // Асинхронное выполнение загрузки
    load(filename, this);
}
