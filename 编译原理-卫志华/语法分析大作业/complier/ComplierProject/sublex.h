#ifndef SUBLEX_H
#define SUBLEX_H

#include <QWidget>
#include <string>
#include <fstream>
using namespace std;

namespace Ui {
class sublex;
}

class sublex : public QWidget
{
    Q_OBJECT
signals:
    void RETURNTOMAINWINDOW();  //返回主界面
    void CLOSEALL();    //直接退出演示

public:
    explicit sublex(QWidget *parent = nullptr);
    ~sublex();
    void LexShow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_released();

private:
    Ui::sublex *ui;
};

#endif // SUBLEX_H
