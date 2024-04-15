#include "ui_matrixitem.h"
#include <shared.h>
#include <headers/matrixitem.h>

MatrixItem::~MatrixItem() {
    delete ui;
}

MatrixItem::MatrixItem(SparseDoubleLinkedMatrix *matrix, QWidget *parent) : QWidget(parent),
                                                                            ui(new Ui::MatrixItem),
                                                                            matrix(matrix) {
    ui->setupUi(this);
    QString name = QString::fromStdString(matrix->name);
    ui->label->setText(name);
}
