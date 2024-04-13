#include "qlineedit.h"
#include "ui_explorer.h"
#include <headers/explorer.h>
#include <headers/inmatrix.h>
#include "QMessageBox"
#include <QHBoxLayout>
#include <qlabel.h>

explorer::explorer(QWidget *parent, QTableView* matrixPlace, QPushButton* button, QPushButton* rmButton, QLabel *matrixLabel, SparseDoubleLinkedMatrix** Matrix)
    : QDialog(parent)
    , matrixLabel(matrixLabel)
    , mPlace(matrixPlace)
    , btn(button)
    , rmbtn(rmButton)
    , matrix(Matrix)
    , ui(new Ui::explorer)
{
    ui->setupUi(this);

    this->populateList();

    if (matrixPlace) connect(ui->listWidget, &QListWidget::itemClicked, this, &explorer::onItemClicked);

}

void explorer::populateList() {
    ui->listWidget->clear();

    for (auto &matrix : explorer::getMatrixs()) {
        QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(matrix->name) + QString(" [%0 x %1]").arg(matrix->columnPointer.size()).arg(matrix->linePointer.size()));
        QVariant variant;
        variant.setValue(matrix);
        item->setData(Qt::UserRole, variant);
        ui->listWidget->addItem(item);
    }
}

void explorer::onItemClicked(QListWidgetItem *item){
    QVariant variant = item->data(Qt::UserRole);
    SparseDoubleLinkedMatrix *selectedMatrix = variant.value<SparseDoubleLinkedMatrix*>();
    auto model = new SparseMatrixModel(selectedMatrix);
    mPlace->setModel(model);
    mPlace->show();
    btn->hide();
    rmbtn->show();
    matrixLabel->hide();
    *matrix = selectedMatrix;
}

void explorer::refresh() {
    this->populateList();

    if (mPlace) connect(ui->listWidget, &QListWidget::itemClicked, this, &explorer::onItemClicked);
}

void explorer::on_btnAdd_clicked()
{
    inmatrix w1(this);
    w1.setModal(true);
    w1.exec();
}

void explorer::on_btnRemove_clicked() {
    auto selectedItems = ui->listWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Выберите объект для удаления");
        return;
    }
    auto *item = selectedItems.first();
    QVariant variant = item->data(Qt::UserRole);
    SparseDoubleLinkedMatrix *selectedMatrix = variant.value<SparseDoubleLinkedMatrix*>();
    deepDelete(*selectedMatrix);
    delete item;
}

void explorer::on_btnRename_clicked() {
    auto selectedItems = ui->listWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Выберите объект для переименования");
        return;
    }
    auto *item = selectedItems.first();
    bool ok;
    QString newName = QInputDialog::getText(this, "Переименовать", "Новое название:", QLineEdit::Normal, item->text(), &ok);
    if (ok && !newName.isEmpty()) {
        QVariant variant = item->data(Qt::UserRole);
        SparseDoubleLinkedMatrix *selectedMatrix = variant.value<SparseDoubleLinkedMatrix*>();
        selectedMatrix->name = newName.toStdString();
        item->setText(newName);
    }
}

void explorer::on_btnMoreinf_clicked(){
    auto selectedItems = ui->listWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Выберите объект для переименования");
        return;
    }
    auto *item = selectedItems.first();
    QVariant variant = item->data(Qt::UserRole);
    SparseDoubleLinkedMatrix *selectedMatrix = variant.value<SparseDoubleLinkedMatrix*>();
    auto detail = new Details(selectedMatrix);
    detail->show();
}

explorer::~explorer()
{
    delete ui;
}
