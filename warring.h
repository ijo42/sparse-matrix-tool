#ifndef WARRING_H
#define WARRING_H

#include <QDialog>

namespace Ui {
class warring;
}

class warring : public QDialog
{
    Q_OBJECT

public:
    explicit warring(QWidget *parent = nullptr);
    ~warring();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::warring *ui;
};

#endif // WARRING_H
