/********************************************************************************
** Form generated from reading UI file 'sub3.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUB3_H
#define UI_SUB3_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_sub3
{
public:
    QTableWidget *tableWidget;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *pushButton;

    void setupUi(QWidget *sub3)
    {
        if (sub3->objectName().isEmpty())
            sub3->setObjectName(QStringLiteral("sub3"));
        sub3->resize(1451, 934);
        tableWidget = new QTableWidget(sub3);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(10, 30, 1431, 201));
        label = new QLabel(sub3);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(670, 10, 131, 16));
        label_2 = new QLabel(sub3);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(690, 240, 101, 20));
        label_3 = new QLabel(sub3);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 260, 1421, 661));
        pushButton = new QPushButton(sub3);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(80, 0, 93, 28));

        retranslateUi(sub3);

        QMetaObject::connectSlotsByName(sub3);
    } // setupUi

    void retranslateUi(QWidget *sub3)
    {
        sub3->setWindowTitle(QApplication::translate("sub3", "Form", Q_NULLPTR));
        label->setText(QApplication::translate("sub3", "LR(1)\345\210\206\346\236\220\350\277\207\347\250\213", Q_NULLPTR));
        label_2->setText(QApplication::translate("sub3", "\350\257\255\346\263\225\345\210\206\346\236\220\346\240\221", Q_NULLPTR));
        label_3->setText(QString());
        pushButton->setText(QApplication::translate("sub3", "\345\205\263\351\227\255\346\255\244\347\225\214\351\235\242", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class sub3: public Ui_sub3 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUB3_H
