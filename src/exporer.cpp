#include "qlineedit.h"
#include "qtconcurrentrun.h"
#include "ui_explorer.h"
#include <headers/explorer.h>
#include <headers/inmatrix.h>
#include <headers/mainwindow.h>
#include "QMessageBox"
#include <QHBoxLayout>
#include <qlabel.h>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <qprogressdialog.h>
#include <qfuture.h>
#include <qfuturewatcher.h>
#include "io.h"

explorer::explorer(QWidget *parent, QTableView* matrixPlace, QPushButton* button, QWidget* pane, QLabel *matrixLabel, SparseDoubleLinkedMatrix** Matrix)
    : QDialog(parent)
    , matrixLabel(matrixLabel)
    , mPlace(matrixPlace)
    , btn(button)
    , pane(pane)
    , matrix(Matrix)
    , ui(new Ui::explorer)
{
    ui->setupUi(this);

    this->populateList();
    setAcceptDrops(true);

    if (matrixPlace) {
        connect(ui->listWidget, &QListWidget::itemClicked, this, &explorer::onItemClicked);
        this->setWindowTitle("Выберите матрицу для добавления");
    }

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
    pane->show();
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
    explorer::getMatrixs().removeOne(selectedMatrix);
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
    QVariant variant = item->data(Qt::UserRole);
    SparseDoubleLinkedMatrix *selectedMatrix = variant.value<SparseDoubleLinkedMatrix*>();

    bool ok;
    QString newName = QInputDialog::getText(this, "Переименовать", "Новое название:", QLineEdit::Normal, QString::fromStdString(selectedMatrix->name), &ok);
    if (ok && !newName.isEmpty()) {
        selectedMatrix->name = newName.toStdString();
        item->setText(QString::fromStdString(selectedMatrix->name) + QString(" [%0 x %1]").arg(selectedMatrix->columnPointer.size()).arg(selectedMatrix->linePointer.size()));
    }
}

void explorer::on_btnMoreinf_clicked(){
    auto selectedItems = ui->listWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Выберите объект для подробностей");
        return;
    }
    auto *item = selectedItems.first();
    QVariant variant = item->data(Qt::UserRole);
    SparseDoubleLinkedMatrix *selectedMatrix = variant.value<SparseDoubleLinkedMatrix*>();
    auto detail = new Details(selectedMatrix);
    detail->show();
}


void explorer::on_btnSpareSave_clicked() {
    auto selectedItems = ui->listWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Выберите объект для сохранения");
        return;
    }
    auto *item = selectedItems.first();
    QVariant variant = item->data(Qt::UserRole);
    SparseDoubleLinkedMatrix *selectedMatrix = variant.value<SparseDoubleLinkedMatrix*>();
    MainWindow::save(selectedMatrix, this, false);
}

void explorer::on_btnFullSave_clicked() {
    auto selectedItems = ui->listWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Выберите объект для сохранения");
        return;
    }
    auto *item = selectedItems.first();
    QVariant variant = item->data(Qt::UserRole);
    SparseDoubleLinkedMatrix *selectedMatrix = variant.value<SparseDoubleLinkedMatrix*>();
    MainWindow::save(selectedMatrix, this, true);
}

void explorer::dragEnterEvent(QDragEnterEvent *event) {
    // Разрешаем событие, если это перетаскивание файла
    if (event->mimeData()->hasFormat("text/uri-list")) {
        event->acceptProposedAction();
    }
}

void explorer::dropEvent(QDropEvent *event) {
    // Получаем список URL-ов, которые были перетащены в виджет
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) return;

    QString filePath = urls.first().toLocalFile();  // Получаем путь к первому файлу
    if (filePath.isEmpty()) return;

    // Показать диалог прогресса
    auto progress = new QProgressDialog("Загрузка матрицы...", "Отмена", 0, 0, this);
    progress->setWindowModality(Qt::WindowModal);
    progress->show();

    inmatrix::load(filePath, progress, this);
}


explorer::~explorer()
{
    delete ui;
}
