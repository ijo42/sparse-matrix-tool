#include "../headers/details.h"
#include "ui_details.h"
#include "util.h"

Details::Details(SparseDoubleLinkedMatrix *matrix, QWidget *parent)
        : QWidget(parent), ui(new Ui::Details) {
    ui->setupUi(this);
    this->setWindowTitle(QString("Подробности матрицы %1").arg(QString::fromStdString(matrix->name)));
    std::vector <size_t> lineIds(matrix->linePointer.size(), 0), columnIds(matrix->columnPointer.size(), 0);
    std::stringstream ss;
    ui->tableWidget->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);

    auto elements = listElements(*matrix, lineIds, columnIds);
    ui->tableWidget->setRowCount(elements.size());
    for (size_t r = 0; r < elements.size(); r++) {
        ss << elements[r]->value;
        ui->tableWidget->setItem(r, 0, new QTableWidgetItem(QString::fromStdString(ss.str())));
        ss.str(std::string());

        if (elements[r] == nullptr) {
            ss << "NULL";
        } else {
            ss << elements[r];
        }
        ui->tableWidget->setItem(r, 1, new QTableWidgetItem(QString::fromStdString(ss.str())));
        ss.str(std::string());
        if (elements[r]->nextColumn == nullptr) {
            ss << "NULL";
        } else {
            ss << (elements[r]->nextColumn);
        }
        ui->tableWidget->setItem(r, 2, new QTableWidgetItem(QString::fromStdString(ss.str())));
        ss.str(std::string());
        if (elements[r]->nextLine == nullptr) {
            ss << "NULL";
        } else {
            ss << (elements[r]->nextLine);
        }
        ui->tableWidget->setItem(r, 3, new QTableWidgetItem(QString::fromStdString(ss.str())));
        ss.str(std::string());
    }
}

Details::~Details() {
    delete ui;
}
