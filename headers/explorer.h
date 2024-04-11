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

namespace Ui {
class explorer;
}

class explorer : public QDialog
{
    Q_OBJECT

public:
    explicit explorer(QWidget *parent = nullptr);
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

private:
    Ui::explorer *ui;
};

#endif // EXPLORER_H
