#include "creatematrix.h"
#include "ui_creatematrix.h"
#include "string"
#include "error.h"
#include <QMessageBox>

creatematrix::creatematrix(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::creatematrix)
{
    ui->setupUi(this);
}

creatematrix::~creatematrix()
{
    delete ui;
}

void creatematrix::on_pushButton_clicked()//создать матрицу
{
    QString str=ui->Stroka->text();
    QString stl=ui->Stolbec->text();
    bool q=true;
    int a=0;
    int b=0;
    int temp=0;
    for (int i=0;i<str.length();i++)
    {
        if (str[i].isDigit())
        {
            if (str[i]=='0')
            {
                temp=0;
            } else if(str[i]=='1')
            {
                temp=1;
            } else if(str[i]=='2')
            {
                temp=2;
            } else if(str[i]=='3')
            {
                temp=3;
            }
            else if(str[i]=='4')
            {
                temp=4;
            } else if(str[i]=='5')
            {
                temp=5;
            } else if(str[i]=='6')
            {
                temp=6;
            } else if(str[i]=='7')
            {
                temp=7;
            } else if(str[i]=='8')
            {
                temp=8;
            }
            else if(str[i]=='9')
            {
                temp=9;
            }
            a=a*10+temp;
        }
        else
        {
            q=false;
            break;
        }
    }
    for (int i=0;i<stl.length();i++)
    {
        if (stl[i].isDigit())
        {
            if (stl[i]=='0')
            {
                temp=0;
            } else if(stl[i]=='1')
            {
                temp=1;
            } else if(stl[i]=='2')
            {
                temp=2;
            } else if(stl[i]=='3')
            {
                temp=3;
            }
            else if(stl[i]=='4')
            {
                temp=4;
            } else if(stl[i]=='5')
            {
                temp=5;
            } else if(stl[i]=='6')
            {
                temp=6;
            } else if(stl[i]=='7')
            {
                temp=7;
            } else if(stl[i]=='8')
            {
                temp=8;
            }
            else if(stl[i]=='9')
            {
                temp=9;
            }
            b=b*10+temp;
        }
        else
        {
            q=false;
            break;
        }
    }
    if ((a<2) || (a>10000)||(b<2)||(b>10000))
    {
        q=false;
    }
    else
    {

    }
    if (q)
    {
        //вызов функции создания
    }
    else
    {
        QMessageBox::warning(this,"Ошибка","Строки/стобцы могут быть только целые числа от 2 до 10000");
    }
}

