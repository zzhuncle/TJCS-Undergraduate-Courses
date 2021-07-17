#ifndef SUB3_H
#define SUB3_H

#include <QWidget>
#include <iostream>
#include <fstream>
#include <QWheelEvent>
#include <QMouseEvent>
using namespace std;

namespace Ui {
class sub3;
}

class sub3 : public QWidget
{
    Q_OBJECT

public:
    explicit sub3(QWidget *parent = nullptr);
    ~sub3();

public:
    void showTreePic();
    void showLR1Process();


private slots:
    void on_pushButton_released();

private:
    Ui::sub3 *ui;
};

#endif // SUB3_H
