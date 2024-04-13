#include "../headers/inmatrix.h"
#include "io.h"
#include "ui_inmatrix.h"
#include "../headers/creatematrix.h"
#include "QFileDialog"

#include <headers/explorer.h>
#include <headers/preview.h>
#include <QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>
#include <QProgressDialog>

inmatrix::inmatrix(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::inmatrix)
{
    ui->setupUi(this);
}

inmatrix::~inmatrix()
{
    delete ui;
}

void inmatrix::on_pushButton_2_clicked()//открыть окно создания матрицы
{
    creatematrix w3(this);
    w3.setModal(true);
    w3.exec();
}

QString getLastSubstringOrLastFive(const QString &input) {
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

void inmatrix::on_pushButton_clicked() {
    QString filename = QFileDialog::getOpenFileName(this, "Выберите файл матрицы", "", "*.dlsm");
    if (filename.isEmpty())
        return;

    // Показать диалог прогресса
    auto progress = new QProgressDialog("Загрузка матрицы...", "Отмена", 0, 0, this);
    progress->setWindowModality(Qt::WindowModal);
    progress->show();

    // Асинхронное выполнение загрузки
    QFuture<std::pair<SparseDoubleLinkedMatrix*, bool>> future = QtConcurrent::run([filename]() -> std::pair<SparseDoubleLinkedMatrix*, bool> {
        bool isSuccess = false;
        std::string path = filename.toStdString();
        auto matrix = loadFromFileValidate(isSuccess, path);
        return {matrix, isSuccess};
    });

    // Создание QFutureWatcher для отслеживания завершения задачи
    QFutureWatcher<std::pair<SparseDoubleLinkedMatrix*, bool>> *watcher = new QFutureWatcher<std::pair<SparseDoubleLinkedMatrix*, bool>>(this);
    connect(watcher, &QFutureWatcher<std::pair<SparseDoubleLinkedMatrix*, bool>>::finished, this, [this, watcher, filename, progress]() {
        progress->close(); // Скрываем диалог прогресса
        auto result = watcher->result();
        watcher->deleteLater();
        if (result.second) {
            result.first->name = getLastSubstringOrLastFive(filename).toStdString();
            explorer::getMatrixs().append(result.first);
            if (explorer* v = dynamic_cast<explorer*>(parent())) {
                v->refresh();
            }
        } else {
            QMessageBox::warning(this, "Предупреждение", "Данный файл не содержит матрицу");
        }
    });
    watcher->setFuture(future);
}
