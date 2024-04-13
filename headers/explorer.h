#ifndef EXPLORER_H
#define EXPLORER_H

#include "operations.h"
#include "qlineedit.h"
#include "ui_explorer.h"
#include <QDialog>
#include <qlistwidget.h>
#include <qmessagebox.h>
#include <shared.h>
#include "QInputDialog"
#include "QTableView"
#include "sparsematrixmodel.h"
#include "details.h"
#include "qlabel.h"

namespace Ui {
class explorer;
}

class explorer : public QDialog
{
    Q_OBJECT

public:
    explicit explorer(QWidget *parent = nullptr, QTableView* matrixPlace = nullptr,
                      QPushButton* button = nullptr, QPushButton* rmButton = nullptr,
                      QLabel *matrixLabel = nullptr,
                      SparseDoubleLinkedMatrix** matrix = nullptr);
    ~explorer();
    static QList<SparseDoubleLinkedMatrix*>& getMatrixs() {
        static QList<SparseDoubleLinkedMatrix*> list;
        return list;
    }
    void onItemClicked(QListWidgetItem *item);
    void refresh();


private slots:
    void on_btnAdd_clicked();
    void on_btnRemove_clicked();
    void on_btnRename_clicked();
    void on_btnMoreinf_clicked();

private:
    QLabel *matrixLabel;
    QTableView* mPlace;
    QPushButton* btn;
    QPushButton* rmbtn;
    std::string* matrixName;
    SparseDoubleLinkedMatrix** matrix;
    Ui::explorer *ui;
    void populateList();
};

#endif // EXPLORER_H
