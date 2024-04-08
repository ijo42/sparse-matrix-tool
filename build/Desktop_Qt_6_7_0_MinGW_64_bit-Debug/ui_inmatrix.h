/********************************************************************************
** Form generated from reading UI file 'inmatrix.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INMATRIX_H
#define UI_INMATRIX_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_inmatrix
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *inmatrix)
    {
        if (inmatrix->objectName().isEmpty())
            inmatrix->setObjectName("inmatrix");
        inmatrix->resize(400, 300);
        pushButton = new QPushButton(inmatrix);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(120, 70, 93, 29));
        pushButton_2 = new QPushButton(inmatrix);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(120, 140, 93, 29));

        retranslateUi(inmatrix);

        QMetaObject::connectSlotsByName(inmatrix);
    } // setupUi

    void retranslateUi(QDialog *inmatrix)
    {
        inmatrix->setWindowTitle(QCoreApplication::translate("inmatrix", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("inmatrix", "\320\270\320\274\320\277\320\276\321\200\321\202", nullptr));
        pushButton_2->setText(QCoreApplication::translate("inmatrix", "\321\201\320\276\320\267\320\264\320\260\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class inmatrix: public Ui_inmatrix {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INMATRIX_H
