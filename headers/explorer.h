#ifndef EXPLORER_H
#define EXPLORER_H

#include <QDialog>

namespace Ui {
class explorer;
}

class explorer : public QDialog
{
    Q_OBJECT

public:
    explicit explorer(QWidget *parent = nullptr);
    ~explorer();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_10_clicked();

private:
    Ui::explorer *ui;
};

#endif // EXPLORER_H
