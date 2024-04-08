/********************************************************************************
** Form generated from reading UI file 'creatematrix.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATEMATRIX_H
#define UI_CREATEMATRIX_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_creatematrix
{
public:
    QCheckBox *checkBox;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLabel *label;
    QLabel *label_2;
    QPushButton *pushButton;

    void setupUi(QDialog *creatematrix)
    {
        if (creatematrix->objectName().isEmpty())
            creatematrix->setObjectName("creatematrix");
        creatematrix->resize(400, 300);
        checkBox = new QCheckBox(creatematrix);
        checkBox->setObjectName("checkBox");
        checkBox->setGeometry(QRect(120, 130, 171, 71));
        lineEdit = new QLineEdit(creatematrix);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(170, 80, 113, 26));
        lineEdit_2 = new QLineEdit(creatematrix);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(170, 120, 113, 26));
        label = new QLabel(creatematrix);
        label->setObjectName("label");
        label->setGeometry(QRect(60, 90, 63, 20));
        label_2 = new QLabel(creatematrix);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(60, 130, 91, 20));
        pushButton = new QPushButton(creatematrix);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(140, 210, 93, 29));

        retranslateUi(creatematrix);

        QMetaObject::connectSlotsByName(creatematrix);
    } // setupUi

    void retranslateUi(QDialog *creatematrix)
    {
        creatematrix->setWindowTitle(QCoreApplication::translate("creatematrix", "Dialog", nullptr));
        checkBox->setText(QCoreApplication::translate("creatematrix", "\320\267\320\260\320\277\320\276\320\273\320\275\320\270\321\202\321\214 \321\201\320\273\321\203\321\207\320\260\320\271\320\275\320\276", nullptr));
        lineEdit->setText(QString());
        lineEdit_2->setText(QString());
        label->setText(QCoreApplication::translate("creatematrix", "\320\241\321\202\321\200\320\276\320\272", nullptr));
        label_2->setText(QCoreApplication::translate("creatematrix", "\320\241\321\202\320\276\320\273\320\261\321\206\320\276\320\262", nullptr));
        pushButton->setText(QCoreApplication::translate("creatematrix", "\321\201\320\276\320\267\320\264\320\260\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class creatematrix: public Ui_creatematrix {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATEMATRIX_H
