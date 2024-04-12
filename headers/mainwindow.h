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

private slots:

    void on_exlporerButton_clicked();

    void on_matrixAButton_clicked();

    void on_matrixBButton_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_swapButton_clicked();

    void on_sumButton_clicked();

    void on_subButton_clicked();

    void on_multiplyButton_clicked();

    void on_reverseButton_clicked();

private:
    Ui::MainWindow *ui;
    SparseDoubleLinkedMatrix *matrixA, *matrixB;
};
#endif // MAINWINDOW_H
