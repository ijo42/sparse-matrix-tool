#include "../headers/preview.h"
#include "../headers/sparsematrixmodel.h"
#include "ui_preview.h"

#include <qtableview.h>

Preview::Preview(SparseDoubleLinkedMatrix *matrix,QWidget *parent)
    : QWidget(parent), ui(new Ui::Preview)
{
    ui->setupUi(this);
    QTableView tableView;
    auto model = new SparseMatrixModel(matrix);
    tableView.setModel(model);
    tableView.show(); // Отображение таблицы

    //ui->tableView->setModel(&model);
    //ui->tableView->show();
}

Preview::~Preview()
{
    delete ui;
}
