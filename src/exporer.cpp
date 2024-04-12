#include "qlineedit.h"
#include "ui_explorer.h"
#include <headers/explorer.h>
#include <headers/inmatrix.h>
#include "QMessageBox"
#include <QHBoxLayout>
#include <qlabel.h>


explorer::explorer(QWidget *parent, QTableView* matrixPlace, QPushButton* button,QPushButton* rmButton, SparseDoubleLinkedMatrix** Matrix)
    : QDialog(parent)
    , ui(new Ui::explorer)
{
    ui->setupUi(this);
    // auto *layout = new QVBoxLayout(this);
    auto listWidget = ui->listWidget;
    // auto *btnRemove = new QPushButton("Удалить", this);
    // auto *btnRename = new QPushButton("Переименовать", this);

    for (auto &matrix : explorer::getMatrixs()) {
        QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(matrix->name));
        QVariant variant;
        variant.setValue(matrix);
        item->setData(Qt::UserRole, variant);
        listWidget->addItem(item);
    }

    if (matrixPlace) connect(listWidget, &QListWidget::itemClicked, this, &explorer::onItemClicked);

    mPlace = matrixPlace;
    btn = button;
    matrix = Matrix;
    rmbtn = rmButton;
}

void explorer::onItemClicked(QListWidgetItem *item){
    QVariant variant = item->data(Qt::UserRole);
    SparseDoubleLinkedMatrix *selectedMatrix = variant.value<SparseDoubleLinkedMatrix*>();
    auto model = new SparseMatrixModel(selectedMatrix);
    mPlace->setModel(model);
    mPlace->show();
    btn->hide();
    rmbtn->show();
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
