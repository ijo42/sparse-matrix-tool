#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sparsematrixmodel.h"

#include <QMainWindow>
#include <shared.h>
#include <operations.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static void save(SparseDoubleLinkedMatrix *matrixA, QWidget *widget, bool isFull);

private slots:

    void on_exlporerButton_clicked();

    void on_matrixAButton_clicked();

    void on_matrixBButton_clicked();

    void on_swapButton_clicked();

    void on_sumButton_clicked();

    void on_subButton_clicked();

    void on_multiplyButton_clicked();

    void on_reverseButton_clicked();

    void on_btnDeleteA_clicked();

    void on_btnDeleteB_clicked();

    void on_btnSaveB_clicked();

    void on_btnSaveA_clicked();


private:
    Ui::MainWindow *ui;
    SparseDoubleLinkedMatrix *matrixA = nullptr, *matrixB =nullptr;
};
#endif // MAINWINDOW_H
