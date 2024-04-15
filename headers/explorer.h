#ifndef EXPLORER_H
#define EXPLORER_H

#include "ui_explorer.h"
#include <QDialog>
#include <qlistwidget.h>
#include <qmessagebox.h>
#include <shared.h>
#include "QInputDialog"
#include "QTableView"
#include "qlabel.h"

namespace Ui {
class explorer;
}

class explorer : public QDialog
{
    Q_OBJECT

public:
    explicit explorer(QWidget *parent = nullptr, QTableView* matrixPlace = nullptr,
                      QPushButton* button = nullptr, QWidget* pane = nullptr,
                      QLabel *matrixLabel = nullptr,
                      SparseDoubleLinkedMatrix** matrix = nullptr);
    ~explorer();
    static QList<SparseDoubleLinkedMatrix*>& getMatrixs() {
        static QList<SparseDoubleLinkedMatrix*> list;
        return list;
    }
    void refresh();


private slots:
    void on_btnAdd_clicked();
    void on_btnRemove_clicked();
    void on_btnRename_clicked();
    void on_btnMoreinf_clicked();

    void on_btnSpareSave_clicked();

    void on_btnFullSave_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    QLabel *matrixLabel;
    QTableView* mPlace;
    QPushButton* btn;
    QWidget* pane;
    std::string* matrixName;
    SparseDoubleLinkedMatrix** matrix;
    Ui::explorer *ui;
    void populateList();
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // EXPLORER_H
