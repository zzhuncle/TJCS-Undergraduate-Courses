#ifndef SUBSYN_H
#define SUBSYN_H

#include <QWidget>
#include "sub3.h"
#include "Highlighter.h"
#include "LexAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include <QMessageBox>
#include <QDialog>

namespace Ui {
    class subsyn;
}

class subsyn : public QWidget
{
    Q_OBJECT
signals:
    void RETURNTOMAINWINDOW();  //返回主界面
    void CLOSEALL();    //直接退出演示

public:
    QString srcc;
    int flag=0;
    sub3 winsub3;
    int errorlines;


public:
    explicit subsyn(QWidget *parent = nullptr);
    ~subsyn();

private slots:
    void on_pushButton_5_released();

    void on_pushButton_released();

    void on_pushButton_3_released();

    void on_pushButton_2_released();

    void on_pushButton_8_released();

    void on_pushButton_4_released();

    void on_pushButton_6_released();

    void on_pushButton_7_released();

private:
    Ui::subsyn *ui;

};

#endif // SUBSYN_H
