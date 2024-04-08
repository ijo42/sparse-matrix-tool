#include "explorer.h"
#include "ui_explorer.h"

explorer::explorer(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::explorer)
{
    ui->setupUi(this);
}

explorer::~explorer()
{
    delete ui;
}
