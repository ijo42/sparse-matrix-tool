#include "qlineedit.h"
#include "ui_explorer.h"
#include <headers/explorer.h>
#include <headers/inmatrix.h>
#include "QMessageBox"
#include <QHBoxLayout>
#include <qlabel.h>


explorer::explorer(QWidget *parent, QTableView* matrixPlace, QPushButton* button, QLabel *matrixLabel, SparseDoubleLinkedMatrix** Matrix)
    : QDialog(parent), matrixLabel(matrixLabel), ui(new Ui::explorer)
{
    ui->setupUi(this);
    auto listWidget = ui->listWidget;
    for (auto &matrix : explorer::getMatrixs()) {
        QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(matrix->name));
        QVariant variant;
        variant.setValue(matrix);
        item->setData(Qt::UserRole, variant);
        listWidget->addItem(item);
    }

    if (matrixPlace) {
        connect(listWidget, &QListWidget::itemClicked, this, &explorer::onItemClicked);
    }

    mPlace = matrixPlace;
    btn = button;
    matrix = Matrix;
}

void explorer::onItemClicked(QListWidgetItem *item){
    QVariant variant = item->data(Qt::UserRole);
    SparseDoubleLinkedMatrix *selectedMatrix = variant.value<SparseDoubleLinkedMatrix*>();
    auto model = new SparseMatrixModel(selectedMatrix);
    mPlace->setModel(model);
    mPlace->show();
    btn->hide();
    matrixLabel->hide();
    *matrix = selectedMatrix;
}

void explorer::refresh() {
    auto listWidget = ui->listWidget;
    listWidget->clear();

    for (auto &matrix : explorer::getMatrixs()) {
        QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(matrix->name));
        QVariant variant;
        variant.setValue(matrix);
        item->setData(Qt::UserRole, variant);
        listWidget->addItem(item);
    }

    if (mPlace) connect(listWidget, &QListWidget::itemClicked, this, &explorer::onItemClicked);
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
    SparseDoubleLinkedMatrix *matrix = reinterpret_cast<SparseDoubleLinkedMatrix*>(item->data(Qt::UserRole).value<void*>());

    deepDelete(*matrix);
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
        SparseDoubleLinkedMatrix *matrix = reinterpret_cast<SparseDoubleLinkedMatrix*>(item->data(Qt::UserRole).value<void*>());
        matrix->name = newName.toStdString();
        item->setText(newName);
    }
}

explorer::~explorer()
{
    delete ui;
}
