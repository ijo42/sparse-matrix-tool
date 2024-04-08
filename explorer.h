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

private:
    Ui::explorer *ui;
};

#endif // EXPLORER_H
