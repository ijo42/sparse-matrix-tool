#include "../headers/creatematrix.h"
#include "operations.h"
#include "ui_creatematrix.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QString>
#include <QDebug>
#include <headers/explorer.h>
#include <QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>
#include <QProgressDialog>

creatematrix::creatematrix(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::creatematrix)
{
    ui->setupUi(this);
}

creatematrix::~creatematrix()
{
    delete ui;
}
bool checkLineEditValue(QLineEdit* lineEdit) {
    bool ok;
    int value = lineEdit->text().toInt(&ok);

    if (ok && value >= 1 && value <= 10000) {
        //qDebug() << "Значение в допустимом диапазоне:" << value;
        return true;
    } else {
        //qDebug() << "Значение вне допустимого диапазона или не является числом";
        return false;
    }
}



void creatematrix::on_pushButton_clicked() {
    bool rowOk, columnOk;
    int row = ui->Stroka->text().toInt(&rowOk);
    int column = ui->Stolbec->text().toInt(&columnOk);

    if (rowOk && columnOk) {
        // Показать диалог прогресса
        auto progress = new QProgressDialog("Генерация матрицы...", "Отмена", 0, 0, this);
        progress->setWindowModality(Qt::WindowModal);
        progress->show();
        int type = ui->type->currentIndex();

        // Асинхронное выполнение создания матрицы
        QFuture<SparseDoubleLinkedMatrix*> future = QtConcurrent::run([type, row, column]() -> SparseDoubleLinkedMatrix* {
            switch(type) {
            case 0:
                return generateRnd(row,column);
            case 1:
                return generateUnitMatrix(row);
            case 2:
            default:
                return generateEmpty(row,column);
            }
        });

        // Создание QFutureWatcher для отслеживания завершения задачи
        QFutureWatcher<SparseDoubleLinkedMatrix*> *watcher = new QFutureWatcher<SparseDoubleLinkedMatrix*>(this);
        connect(watcher, &QFutureWatcher<SparseDoubleLinkedMatrix*>::finished, this, [this, watcher, progress]() {
            progress->close(); // Скрываем диалог прогресса
            SparseDoubleLinkedMatrix* matrix = watcher->result();
            watcher->deleteLater();
            if (matrix) {
                matrix->name = QString("Матрица %1").arg(1 + explorer::getMatrixs().size()).toStdString();
                explorer::getMatrixs().append(matrix);
                if (explorer* v = dynamic_cast<explorer*>(parent()->parent())) {
                    v->refresh();
                }

                QMessageBox msgBox(this);
                msgBox.setWindowTitle("Создание матрицы");
                msgBox.setText("Матрица успешно создана!");
                msgBox.setInformativeText("Хотите создать еще одну матрицу?");
                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                msgBox.setDefaultButton(QMessageBox::Yes);
                if(msgBox.exec() == QMessageBox::No){
                    ((QWidget*)parent())->close();
                    (this)->close();
                }
            } else
                QMessageBox::warning(this, "Ошибка", "Произошла непредвиденная ошибка");
        });
        watcher->setFuture(future);
    } else {
        QMessageBox::warning(this, "Ошибка", "Значения строк и столбцов могут быть только целые числа от 1 до 10000");
    }
}

void creatematrix::on_type_currentIndexChanged(int index)
{
    if(index == 1) {
        ui->Stolbec->setReadOnly(true);
        if(QString::compare(ui->Stroka->text(), ui->Stolbec->text(), Qt::CaseInsensitive) != 0){
            QMessageBox::information(this, "Внимание", "Лишь квадратная единичная матрица может быть создана");
        }
        ui->Stolbec->setText(ui->Stroka->text());
    } else {
        ui->Stolbec->setReadOnly(false);
    }
}

