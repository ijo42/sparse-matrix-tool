#include "qlineedit.h"
#include "ui_explorer.h"
#include <headers/explorer.h>
#include <headers/inmatrix.h>
#include "QMessageBox"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <qlabel.h>

explorer::explorer(QWidget *parent, QTableView *Place)
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
        item->setData(Qt::UserRole, QVariant::fromValue(reinterpret_cast<void*>(&matrix)));
        listWidget->addItem(item);
    }

    if (Place) connect(listWidget, &QListWidget::itemClicked, this, &explorer::onItemClicked);

    matrixPlace = Place;
}

void explorer::onItemClicked(QListWidgetItem *item) {
    // Получение указателя на матрицу
    QVariant variant = item->data(Qt::UserRole);
    SparseDoubleLinkedMatrix* matrix = variant.value<SparseDoubleLinkedMatrix*>();
    auto model = new SparseMatrixModel(matrix);
    matrixPlace->setModel(model);
    matrixPlace->show();
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

    if (matrixPlace) connect(listWidget, &QListWidget::itemClicked, this, &explorer::onItemClicked);
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
