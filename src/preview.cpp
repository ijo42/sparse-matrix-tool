#include "../headers/preview.h"
#include "../headers/sparsematrixmodel.h"
#include "ui_preview.h"

#include <qlabel.h>
#include <qtableview.h>

Preview::Preview(SparseDoubleLinkedMatrix *matrix, QWidget *parent)
        : QWidget(parent), ui(new Ui::Preview) {
    ui->setupUi(this);
    this->setWindowTitle(QString("Предпросмотр %1").arg(QString::fromStdString(matrix->name)));
    auto model = new SparseMatrixModel(matrix);
    ui->tableView->setModel(model);
    ui->tableView->show();
}

Preview::~Preview() {
    delete ui;
}
