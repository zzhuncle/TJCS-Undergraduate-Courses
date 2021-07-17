#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
    class MainWindow;
}

class MainWindow:public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void autoTest();       //自动测试

private slots://槽函数
    void completeClicked();//命令行输入执行
    void autoTestClicked();//自动测试
    void helpClicked();    //功能帮助
    void fformatClicked(); //磁盘格式化
    void exitClicked();    //保存退出
    void lsClicked();      //当前目录
    void mkdirClicked();   //创建目录
    void fcreateClicked(); //创建文件
    void fopenClicked();   //打开文件
    void fcloseClicked();  //关闭文件
    void fdeleteClicked(); //删除文件
    void freadClicked();   //读取文件
    void fseekClicked();   //指针移动
    void fwriteClicked();  //写入文件
    void cdClicked();      //切换目录
    void frenameClicked(); //文件重命名
    void ftreeClicked();   //文件目录树
};

#endif // MAINWINDOW_H
