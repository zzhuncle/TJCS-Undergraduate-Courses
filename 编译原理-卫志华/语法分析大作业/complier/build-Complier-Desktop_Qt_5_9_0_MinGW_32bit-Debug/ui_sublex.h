/********************************************************************************
** Form generated from reading UI file 'sublex.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUBLEX_H
#define UI_SUBLEX_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_sublex
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QTextEdit *sub_TextEdit;
    QLabel *label;

    void setupUi(QWidget *sublex)
    {
        if (sublex->objectName().isEmpty())
            sublex->setObjectName(QStringLiteral("sublex"));
        sublex->resize(918, 544);
        verticalLayoutWidget = new QWidget(sublex);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(710, 50, 181, 151));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(verticalLayoutWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(verticalLayoutWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        verticalLayout->addWidget(pushButton_2);

        sub_TextEdit = new QTextEdit(sublex);
        sub_TextEdit->setObjectName(QStringLiteral("sub_TextEdit"));
        sub_TextEdit->setGeometry(QRect(10, 30, 691, 501));
        label = new QLabel(sublex);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(260, 10, 141, 16));

        retranslateUi(sublex);

        QMetaObject::connectSlotsByName(sublex);
    } // setupUi

    void retranslateUi(QWidget *sublex)
    {
        sublex->setWindowTitle(QApplication::translate("sublex", "Form", Q_NULLPTR));
        pushButton->setText(QApplication::translate("sublex", "\350\277\224\345\233\236\344\270\273\347\225\214\351\235\242", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("sublex", "\347\233\264\346\216\245\351\200\200\345\207\272\346\274\224\347\244\272", Q_NULLPTR));
        label->setText(QApplication::translate("sublex", "\350\257\215\346\263\225\345\210\206\346\236\220\347\273\223\346\236\234", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class sublex: public Ui_sublex {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUBLEX_H
