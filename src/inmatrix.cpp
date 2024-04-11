#include "../headers/inmatrix.h"
#include "io.h"
#include "ui_inmatrix.h"
#include "../headers/creatematrix.h"
#include "QFileDialog"

#include <headers/explorer.h>
#include <headers/preview.h>
#include <headers/warring.h>

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
        return input.mid(lastSlashIndex + 1);
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

void inmatrix::on_pushButton_clicked()//открыть окно импорта
{
    QString filename = QFileDialog::getOpenFileName(this, "Выберите файл матрицы", "", "*.dlsm");
    if(filename.isEmpty())
        return;
    bool isSuccess = false;
    std::string path = filename.toStdString();
    auto matrix = loadFromFileValidate(isSuccess, path);
    if(isSuccess){
        matrix->name = getLastSubstringOrLastFive(filename).toStdString();
        explorer::getMatrixs().append(matrix);
        if(explorer* v = dynamic_cast<explorer*>(parent())) {
            v->refresh();
        }
    } else  {
        warring w;
        w.setModal(true);
        w.exec();
    }
}

