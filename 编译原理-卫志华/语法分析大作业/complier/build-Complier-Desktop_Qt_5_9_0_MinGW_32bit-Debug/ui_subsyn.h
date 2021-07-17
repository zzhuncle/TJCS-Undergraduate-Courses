/********************************************************************************
** Form generated from reading UI file 'subsyn.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUBSYN_H
#define UI_SUBSYN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_subsyn
{
public:
    QLabel *label_3;
    QLabel *label;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_3;
    QPushButton *pushButton_2;
    QPushButton *pushButton_4;
    QTextBrowser *tB_VN;
    QLabel *label_2;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pushButton_8;
    QPushButton *pushButton_6;
    QPushButton *pushButton_5;
    QPushButton *pushButton_7;
    QTextBrowser *tB_VT;
    QTextBrowser *tB_grammar;
    QLabel *label_4;
    QTableWidget *tableWidget;
    QLabel *label_6;
    QTextBrowser *tB_sentence;

    void setupUi(QWidget *subsyn)
    {
        if (subsyn->objectName().isEmpty())
            subsyn->setObjectName(QStringLiteral("subsyn"));
        subsyn->resize(1529, 792);
        label_3 = new QLabel(subsyn);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(710, 220, 121, 16));
        label = new QLabel(subsyn);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(90, 0, 101, 16));
        verticalLayoutWidget = new QWidget(subsyn);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(1189, 1, 171, 231));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(verticalLayoutWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout->addWidget(pushButton);

        pushButton_3 = new QPushButton(verticalLayoutWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        verticalLayout->addWidget(pushButton_3);

        pushButton_2 = new QPushButton(verticalLayoutWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        verticalLayout->addWidget(pushButton_2);

        pushButton_4 = new QPushButton(verticalLayoutWidget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        verticalLayout->addWidget(pushButton_4);

        tB_VN = new QTextBrowser(subsyn);
        tB_VN->setObjectName(QStringLiteral("tB_VN"));
        tB_VN->setGeometry(QRect(10, 380, 241, 401));
        label_2 = new QLabel(subsyn);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(80, 360, 121, 16));
        verticalLayoutWidget_2 = new QWidget(subsyn);
        verticalLayoutWidget_2->setObjectName(QStringLiteral("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(1370, 0, 160, 231));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        pushButton_8 = new QPushButton(verticalLayoutWidget_2);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));

        verticalLayout_2->addWidget(pushButton_8);

        pushButton_6 = new QPushButton(verticalLayoutWidget_2);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));

        verticalLayout_2->addWidget(pushButton_6);

        pushButton_5 = new QPushButton(verticalLayoutWidget_2);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));

        verticalLayout_2->addWidget(pushButton_5);

        pushButton_7 = new QPushButton(verticalLayoutWidget_2);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));

        verticalLayout_2->addWidget(pushButton_7);

        tB_VT = new QTextBrowser(subsyn);
        tB_VT->setObjectName(QStringLiteral("tB_VT"));
        tB_VT->setGeometry(QRect(10, 20, 241, 331));
        tB_grammar = new QTextBrowser(subsyn);
        tB_grammar->setObjectName(QStringLiteral("tB_grammar"));
        tB_grammar->setGeometry(QRect(260, 20, 501, 191));
        label_4 = new QLabel(subsyn);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(510, 0, 101, 16));
        tableWidget = new QTableWidget(subsyn);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(270, 240, 1251, 541));
        label_6 = new QLabel(subsyn);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(940, 0, 101, 16));
        tB_sentence = new QTextBrowser(subsyn);
        tB_sentence->setObjectName(QStringLiteral("tB_sentence"));
        tB_sentence->setGeometry(QRect(770, 20, 411, 191));

        retranslateUi(subsyn);

        QMetaObject::connectSlotsByName(subsyn);
    } // setupUi

    void retranslateUi(QWidget *subsyn)
    {
        subsyn->setWindowTitle(QApplication::translate("subsyn", "Form", Q_NULLPTR));
        label_3->setText(QApplication::translate("subsyn", "LR(1)\350\257\255\346\263\225\345\210\206\346\236\220\350\241\250", Q_NULLPTR));
        label->setText(QApplication::translate("subsyn", "\347\273\210\347\273\223\347\254\246\345\210\227\350\241\250", Q_NULLPTR));
        pushButton->setText(QApplication::translate("subsyn", "\346\230\276\347\244\272\347\273\210\347\273\223\347\254\246", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("subsyn", "\346\230\276\347\244\272\351\235\236\347\273\210\347\273\223\347\254\246", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("subsyn", "\346\230\276\347\244\272\347\224\237\346\210\220\346\226\207\346\263\225", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("subsyn", "\346\230\276\347\244\272LR(1)\350\257\255\346\263\225\345\210\206\346\236\220\350\241\250", Q_NULLPTR));
        label_2->setText(QApplication::translate("subsyn", "\351\235\236\347\273\210\347\273\223\347\254\246\345\210\227\350\241\250", Q_NULLPTR));
        pushButton_8->setText(QApplication::translate("subsyn", "\346\230\276\347\244\272\345\276\205\345\210\206\346\236\220\345\217\245\345\255\220", Q_NULLPTR));
        pushButton_6->setText(QApplication::translate("subsyn", "\345\274\200\345\247\213LR\345\210\206\346\236\220", Q_NULLPTR));
        pushButton_5->setText(QApplication::translate("subsyn", "\350\277\224\345\233\236\344\270\273\351\241\265\351\235\242", Q_NULLPTR));
        pushButton_7->setText(QApplication::translate("subsyn", "\347\233\264\346\216\245\351\200\200\345\207\272\346\274\224\347\244\272", Q_NULLPTR));
        label_4->setText(QApplication::translate("subsyn", "\347\224\237\346\210\220\346\226\207\346\263\225", Q_NULLPTR));
        label_6->setText(QApplication::translate("subsyn", "\345\276\205\345\210\206\346\236\220\345\217\245\345\255\220", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class subsyn: public Ui_subsyn {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUBSYN_H
