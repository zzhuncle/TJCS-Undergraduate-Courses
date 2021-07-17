#define _CRT_SECURE_NO_WARNINGS
#include "DiskDriver.h"
#include "CacheManager.h"
#include "OpenFileManager.h"
#include "SystemCall.h"
#include "UserCall.h"
#include <iostream>
#include <sstream>
#include <unordered_map>
using namespace std;

#include "help.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>

//一些全局变量
DiskDriver myDiskDriver;
CacheManager myCacheManager;
OpenFileTable myOpenFileTable;
SuperBlock mySuperBlock;
FileSystem myFileSystem;
INodeTable myINodeTable;
SystemCall mySystemCall;
UserCall myUserCall;
QTextBrowser *gCmd;
UserCall* User = &myUserCall;
const int N = 46;

QString cmdlines[] =
{
    "mkdir /bin",
    "mkdir /etc",
    "mkdir /home",
    "mkdir /dev",
    "ls",
    "mkdir /home/texts",
    "mkdir /home/reports",
    "mkdir /home/photos",
    "ftree /",
    "cd /home/texts",
    "fcreate Readme.txt",
    "fopen Readme.txt",
    "fwrite 8 Readme.txt 2609",
    "fseek 8 0 begin",
    "fread 8 ReadmeOut.txt 2609",
    "fclose 8",
    "cd /home/reports",
    "fcreate Report.pdf",
    "fopen Report.pdf",
    "fwrite 9 Report.pdf 1604288",
    "fseek 9 0 begin",
    "fread 9 ReportOut.pdf 1604288",
    "fclose 9",
    "cd /home/photos",
    "fcreate DennisRitchie.jpg",
    "fopen DennisRitchie.jpg",
    "fwrite 10 DennisRitchie.jpg 7402",
    "fseek 10 0 begin",
    "fread 10 DennisRitchieOut.jpg 7402",
    "fclose 10",
    "mkdir /test",
    "cd /test",
    "fcreate Jerry",
    "fopen Jerry",
    "fwrite 12 input.txt 800",
    "fseek 12 500 begin",
    "fread 12 std 500",
    "fseek 12 500 begin",
    "fread 12 abc.txt 500",
    "fwrite 12 abc.txt 300",
    "fclose 12",
    "ftree /",
    "cd /test",
    "frename Jerry Larry",
    "ls",
    "ftree /"
};

//解析输入参数
void cmdArgs(const string& cmd, vector<string>& args)
{
    args.clear();
    string str;
    unsigned int p, q;
    for (p = 0, q = 0; q < cmd.length(); p = q + 1) {
        q = cmd.find_first_of(" \n", p);
        str = cmd.substr(p, q - p);
        if (!str.empty())
            args.push_back(str);
        if (q == string::npos)
            return;
    }
}

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gCmd = ui->textBrowser;
    ui->textBrowser->setFontPointSize(qreal(12));
    ui->textBrowser->setFontFamily(QString("KaiTi"));
    QPalette pal;
    pal = ui->textBrowser->palette();
    pal.setColor(QPalette::Base, QColor(0, 0, 0));//改变背景色
    ui->textBrowser->setPalette(pal);
    ui->textBrowser->setTextColor(QColor(255, 255, 255));
    QString input = "[1853790-ZZH-OS ";
    input = input + QString::fromStdString(User->curDirPath)+" ]$ ";
    ui->textBrowser->append(input);

    //命令行执行
    connect(ui->pushButton, &QPushButton::clicked, [=](){
        completeClicked();
    });
    //自动测试命令
    connect(ui->pushButton_2, &QPushButton::clicked, [=](){
        autoTestClicked();
    });
    //点击帮助按钮
    connect(ui->pushButton_3, &QPushButton::clicked, [=](){
        helpClicked();
    });
    //磁盘格式化按钮
    connect(ui->pushButton_4, &QPushButton::clicked, [=](){
        fformatClicked();
    });
    //保存退出按钮
    connect(ui->pushButton_5, &QPushButton::clicked, [=](){
        exitClicked();
    });
    //当前目录按钮
    connect(ui->pushButton_6, &QPushButton::clicked, [=](){
        lsClicked();
    });
    //创建目录按钮
    connect(ui->pushButton_7, &QPushButton::clicked, [=](){
        mkdirClicked();
    });
    //创建文件按钮
    connect(ui->pushButton_8, &QPushButton::clicked, [=](){
        fcreateClicked();
    });
    //打开文件按钮
    connect(ui->pushButton_9, &QPushButton::clicked, [=](){
        fopenClicked();
    });
    //关闭文件按钮
    connect(ui->pushButton_10, &QPushButton::clicked, [=](){
        fcloseClicked();
    });
    //删除文件按钮
    connect(ui->pushButton_11, &QPushButton::clicked, [=](){
        fdeleteClicked();
    });
    //读取文件按钮
    connect(ui->pushButton_12, &QPushButton::clicked, [=](){
        freadClicked();
    });
    //指针移动按钮
    connect(ui->pushButton_13, &QPushButton::clicked, [=](){
        fseekClicked();
    });
    //写入文件按钮
    connect(ui->pushButton_14, &QPushButton::clicked, [=](){
        fwriteClicked();
    });
    //文件重命名
    connect(ui->pushButton_15, &QPushButton::clicked, [=](){
        frenameClicked();
    });
    //文件目录树
    connect(ui->pushButton_16, &QPushButton::clicked, [=](){
        ftreeClicked();
    });
    //切换目录按钮
    connect(ui->pushButton_17, &QPushButton::clicked, [=](){
        cdClicked();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

/****  一些工具函數  ****/
void MainWindow::autoTest()
{
    vector<string> args;
    string cmd, arg1, arg2, arg3;
    QString outline;
    outline = "[1853790-ZZH-OS " + QString::fromStdString(User->curDirPath)+" ]$ 自动测试开始...";
    gCmd->append(outline);
    for (int i = 0;i < N;i++) {
        string line = cmdlines[i].toStdString();
        cmdArgs(line, args);
        cmd = args[0];
        arg1 = args.size() > 1 ? args[1] : "";
        arg2 = args.size() > 2 ? args[2] : "";
        arg3 = args.size() > 3 ? args[3] : "";
        outline = "[1853790-ZZH-OS " + QString::fromStdString(User->curDirPath)+" ]$ " + cmdlines[i];
        gCmd->append(outline);
        //格式化文件系统
        if (cmd == "fformat") {
            myOpenFileTable.Reset();
            myINodeTable.Reset();
            myCacheManager.FormatBuffer();
            myFileSystem.FormatDevice();
            QString infoStr = "格式化完毕，文件系统已退出，请重新启动！\n";
            ui->textBrowser->append(infoStr);
            exit(0);
        }
        //退出系统，并将缓存内容存至磁盘
        else if (cmd == "exit")
            exit(0);
        //自动测试
        else if (cmd == "test")
            autoTest();
        //生成文件夹
        else if (cmd == "mkdir") {
            if (arg1[0] != '/')
                arg1 = User->curDirPath + arg1;
            User->userMkDir(arg1);
        }
        //查看当前目录内容
        else if (cmd == "ls")
            User->userLs();
        //进入目录
        else if (cmd == "cd")
            User->userCd(arg1);
        //创建文件
        else if (cmd == "fcreate") {
            if (arg1[0] != '/')
                arg1 = User->curDirPath + arg1;
            User->userCreate(arg1);
        }
        //删除文件文件名为filename的文件或者文件夹
        else if (cmd == "fdelete") {
            if (arg1[0] != '/')
                arg1 = User->curDirPath + arg1;
            User->userDelete(arg1);
        }
        //打开文件名为filename的文件
        else if (cmd == "fopen") {
            if (myUserCall.ar0[UserCall::EAX] == 0) {
                User->userMkDir("demo");
                User->userDelete("demo");
            }
            if (arg1[0] != '/')
                arg1 = User->curDirPath + arg1;
            User->userOpen(arg1);
        }
        //关闭文件
        else if (cmd == "fclose")
            User->userClose(arg1);
        else if (cmd == "fseek") {
            if (arg3 == "begin")
                User->userSeek(arg1, arg2, string("0"));
            //以cur模式把fd文件指针偏移step
            else if (arg3 == "cur")
                User->userSeek(arg1, arg2, string("1"));
            //以end模式把fd文件指针偏移step
            else if (arg3 == "end")
                User->userSeek(arg1, arg2, string("2"));
        }
        //从fd文件读取size字节，输出到outfile
        //从fd文件读取size字节，输出到屏幕
        else if (cmd == "fread") {
            User->userRead(arg1, arg2, arg3);
        }
        else if (cmd == "fwrite") {
            User->userWrite(arg1, arg2, arg3);
        }
        //重命名文件或文件夹
        else if (cmd == "frename") {
            User->userRename(arg1, arg2);
        }
        else if (cmd == "ftree") {
            User->userTree(arg1);
        }
        else if (cmd != "") {
            QString inputStr = QString::fromStdString(cmd) + ": 找不到这条指令！";
            ui->textBrowser->append(inputStr);
        }
    }
    outline = "[1853790-ZZH-OS " + QString::fromStdString(User->curDirPath) + " ]$ 自动测试结束，请继续输入指令...";
    gCmd->append(outline);

    outline = "[1853790-ZZH-OS " + QString::fromStdString(User->curDirPath)+" ]$ ";
    gCmd->append(outline);
}

/****  一些槽函數  ****/
//命令行执行
void MainWindow::completeClicked()
{
    QString input = ui->lineEdit->text();
    string line = (input == "") ? "" : input.toStdString();
    vector<string> args;
    string cmd, arg1, arg2, arg3, arg4;
    if (line == "")
    {
        QString input = "[1853790-ZZH-OS ";
        input = input + QString::fromStdString(User->curDirPath)+" ]$ "+"";
        ui->textBrowser->append(input);
    }
    else {
        cmdArgs(line, args);
        cmd = args[0];
        arg1 = args.size() > 1 ? args[1] : "";
        arg2 = args.size() > 2 ? args[2] : "";
        arg3 = args.size() > 3 ? args[3] : "";
        arg4 = args.size() > 4 ? args[4] : "";
        ui->textBrowser->insertPlainText(input);

        //格式化文件系统
        if (cmd == "fformat") {
            myOpenFileTable.Reset();
            myINodeTable.Reset();
            myCacheManager.FormatBuffer();
            myFileSystem.FormatDevice();
            QString infoStr = "格式化完毕，文件系统已退出，请重新启动！\n";
            ui->textBrowser->append(infoStr);
            exit(0);
        }
        //退出系统，并将缓存内容存至磁盘
        else if (cmd == "exit")
            exit(0);
        //自动测试
        else if (cmd == "test")
            autoTest();
        //生成文件夹
        else if (cmd == "mkdir") {
            if (arg1[0] != '/')
                arg1 = User->curDirPath + arg1;
            User->userMkDir(arg1);
        }
        //查看当前目录内容
        else if (cmd == "ls")
            User->userLs();
        //进入目录
        else if (cmd == "cd")
            User->userCd(arg1);
        //创建文件
        else if (cmd == "fcreate") {
            if (arg1[0] != '/')
                arg1 = User->curDirPath + arg1;
            User->userCreate(arg1);
        }
        //删除文件文件名为filename的文件或者文件夹
        else if (cmd == "fdelete") {
            if (arg1[0] != '/')
                arg1 = User->curDirPath + arg1;
            User->userDelete(arg1);
        }
        //打开文件名为filename的文件
        else if (cmd == "fopen") {
            if (myUserCall.ar0[UserCall::EAX] == 0) {
                User->userMkDir("demo");
                User->userDelete("demo");
            }
            if (arg1[0] != '/')
                arg1 = User->curDirPath + arg1;
            User->userOpen(arg1);
        }
        //关闭文件
        else if (cmd == "fclose")
            User->userClose(arg1);
        else if (cmd == "flseek") {
            if (arg3 == "begin")
                User->userSeek(arg1, arg2, string("0"));
            //以cur模式把fd文件指针偏移step
            else if (arg3 == "cur")
                User->userSeek(arg1, arg2, string("1"));
            //以end模式把fd文件指针偏移step
            else if (arg3 == "end")
                User->userSeek(arg1, arg2, string("2"));
        }
        //从fd文件读取size字节，输出到outfile
        //从fd文件读取size字节，输出到屏幕
        else if (cmd == "fread") {
            User->userRead(arg1, arg2, arg3);
        }
        else if (cmd == "fwrite") {
            User->userWrite(arg1, arg2, arg3);
        }
        //重命名文件或文件夹
        else if (cmd == "frename") {
            User->userRename(arg1, arg2);
        }
        else if (cmd == "ftree") {
            User->userTree(arg1);
        }
        else if (cmd != "") {
            QString inputStr = QString::fromStdString(cmd) + ": 找不到这条指令！";
            ui->textBrowser->append(inputStr);
        }
    }
    QString inputStr = "[1853790-ZZH-OS ";
    inputStr = inputStr + QString::fromStdString(User->curDirPath)+" ]$ ";
    gCmd->append(inputStr);
    QTextCursor text_cursor(ui->textBrowser->textCursor());
    text_cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(text_cursor);
    ui->lineEdit->setText("");
}

//自动测试命令
void MainWindow::autoTestClicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("自动测试操作"),
                                  "由于程序中的文件句柄为写定形式, 假如前面打开过文件可能会执行出错，还要继续执行吗？",
                                  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if(reply == QMessageBox::Yes) {
        vector<string> args;
        string cmd, arg1, arg2, arg3;
        QString outline;
        outline = "[1853790-ZZH-OS " + QString::fromStdString(User->curDirPath)+" ]$ 自动测试开始...";
        gCmd->append(outline);
        for (int i = 0;i < N;i++) {
            string line = cmdlines[i].toStdString();
            cmdArgs(line, args);
            cmd = args[0];
            arg1 = args.size() > 1 ? args[1] : "";
            arg2 = args.size() > 2 ? args[2] : "";
            arg3 = args.size() > 3 ? args[3] : "";
            outline = "[1853790-ZZH-OS " + QString::fromStdString(User->curDirPath)+" ]$ " + cmdlines[i];
            gCmd->append(outline);
            //格式化文件系统
            if (cmd == "fformat") {
                myOpenFileTable.Reset();
                myINodeTable.Reset();
                myCacheManager.FormatBuffer();
                myFileSystem.FormatDevice();
                QString infoStr = "格式化完毕，文件系统已退出，请重新启动！\n";
                ui->textBrowser->append(infoStr);
                exit(0);
            }
            //退出系统，并将缓存内容存至磁盘
            else if (cmd == "exit")
                exit(0);
            //自动测试
            else if (cmd == "test")
                autoTest();
            //生成文件夹
            else if (cmd == "mkdir") {
                if (arg1[0] != '/')
                    arg1 = User->curDirPath + arg1;
                User->userMkDir(arg1);
            }
            //查看当前目录内容
            else if (cmd == "ls")
                User->userLs();
            //进入目录
            else if (cmd == "cd")
                User->userCd(arg1);
            //创建文件
            else if (cmd == "fcreate") {
                if (arg1[0] != '/')
                    arg1 = User->curDirPath + arg1;
                User->userCreate(arg1);
            }
            //删除文件文件名为filename的文件或者文件夹
            else if (cmd == "fdelete") {
                if (arg1[0] != '/')
                    arg1 = User->curDirPath + arg1;
                User->userDelete(arg1);
            }
            //打开文件名为filename的文件
            else if (cmd == "fopen") {
                if (myUserCall.ar0[UserCall::EAX] == 0) {
                    User->userMkDir("demo");
                    User->userDelete("demo");
                }
                if (arg1[0] != '/')
                    arg1 = User->curDirPath + arg1;
                User->userOpen(arg1);
            }
            //关闭文件
            else if (cmd == "fclose")
                User->userClose(arg1);
            else if (cmd == "fseek") {
                if (arg3 == "begin")
                    User->userSeek(arg1, arg2, string("0"));
                //以cur模式把fd文件指针偏移step
                else if (arg3 == "cur")
                    User->userSeek(arg1, arg2, string("1"));
                //以end模式把fd文件指针偏移step
                else if (arg3 == "end")
                    User->userSeek(arg1, arg2, string("2"));
            }
            //从fd文件读取size字节，输出到outfile
            //从fd文件读取size字节，输出到屏幕
            else if (cmd == "fread") {
                User->userRead(arg1, arg2, arg3);
            }
            else if (cmd == "fwrite") {
                User->userWrite(arg1, arg2, arg3);
            }
            //重命名文件或文件夹
            else if (cmd == "frename") {
                User->userRename(arg1, arg2);
            }
            else if (cmd == "ftree") {
                User->userTree(arg1);
            }
            else if (cmd != "") {
                QString inputStr = QString::fromStdString(cmd) + ": 找不到这条指令！";
                ui->textBrowser->append(inputStr);
            }
        }
        outline = "[1853790-ZZH-OS " + QString::fromStdString(User->curDirPath) + " ]$ 自动测试结束，请继续输入指令...";
        gCmd->append(outline);

        outline = "[1853790-ZZH-OS " + QString::fromStdString(User->curDirPath)+" ]$ ";
        gCmd->append(outline);
    }
}

//点击帮助按钮
void MainWindow::helpClicked()
{
    Help* helpWindow = new Help();
    helpWindow->show();
    helpWindow->setWindowIcon(QIcon(":/Resources/tju.png"));
}

//格式化按钮
void MainWindow::fformatClicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("磁盘格式化操作"),
                                  "是否格式化磁盘？执行此操作后会自动退出...",
                                  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (reply == QMessageBox::Yes) {
        QString input = "fformat";
        ui->textBrowser->insertPlainText(input);

        myOpenFileTable.Reset();
        myINodeTable.Reset();
        myCacheManager.FormatBuffer();
        myFileSystem.FormatDevice();

        QString infoStr = "格式化完毕，文件系统已退出，请重新启动！\n";
        ui->textBrowser->append(infoStr);
        exit(0);
    }
    else;
}

//保存退出按钮
void MainWindow::exitClicked()
{
    exit(0);
}

//当前目录按钮
void MainWindow::lsClicked()
{
    QString input = "ls";
    ui->textBrowser->insertPlainText(input);
    QTextCursor text_cursor(ui->textBrowser->textCursor());
    text_cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(text_cursor);
    User->userLs();
    QString outline;
    outline = "[1853790-ZZH-OS " + QString::fromStdString(User->curDirPath)+" ]$ ";
    ui->textBrowser->append(outline);
}

//建立目录按钮
void MainWindow::mkdirClicked()
{
    QString arg = ui->lineEdit_2->text();
    ui->lineEdit_2->setText("");
    string arg_mkdir = arg.toStdString();
    if (arg_mkdir[0] != '/')
        arg_mkdir = User->curDirPath + arg_mkdir;

    QString input = "mkdir " + QString::fromStdString(arg_mkdir);
    ui->textBrowser->insertPlainText(input);
    QTextCursor text_cursor(ui->textBrowser->textCursor());
    text_cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(text_cursor);
    User->userMkDir(arg_mkdir);
    QString outline;
    outline = "[1853790-ZZH-OS " + QString::fromStdString(User->curDirPath)+" ]$ ";
    gCmd->append(outline);
}

//建立文件按钮
void MainWindow::fcreateClicked()
{
    QString filename = ui->lineEdit_3->text();
    ui->lineEdit_3->setText("");
    QString input = "fcreate "+ filename;
    ui->textBrowser->insertPlainText(input);

    string arg1 = filename.toStdString();
    if (arg1[0] != '/')
        arg1 = User->curDirPath + arg1;
    User->userCreate(arg1);

    QString outline;
    outline = "[1853790-ZZH-OS " + QString::fromStdString(User->curDirPath)+" ]$ ";
    gCmd->append(outline);
}

//打开文件按钮
void MainWindow::fopenClicked()
{
    QString filename = ui->lineEdit_4->text();
    ui->lineEdit_4->setText("");

    QString inputStr = "fopen " + filename;
    ui->textBrowser->insertPlainText(inputStr);
    QTextCursor text_cursor(ui->textBrowser->textCursor());
    text_cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(text_cursor);
    string arg1 = filename.toStdString();
    if (myUserCall.ar0[UserCall::EAX] == 0) {
        User->userMkDir("demo");
        User->userDelete("demo");
    }
    if (arg1[0] != '/')
        arg1 = User->curDirPath + arg1;
    User->userOpen(arg1);

    QString outline;
    outline = "[1853790-ZZH-OS " + QString::fromStdString(User->curDirPath)+" ]$ ";
    gCmd->append(outline);
}

//关闭文件按钮
void MainWindow::fcloseClicked()
{
    QString arg = ui->lineEdit_5->text();
    ui->lineEdit_5->setText("");
    string arg1 = arg.toStdString();

    QString inputStr= "fclose " + arg;
    ui->textBrowser->insertPlainText(inputStr);
    QTextCursor text_cursor(ui->textBrowser->textCursor());
    text_cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(text_cursor);
    User->userClose(arg1);
    QString outline;
    outline = "[1853790-ZZH-OS " + QString::fromStdString(User->curDirPath)+" ]$ ";
    gCmd->append(outline);
}

//删除文件按钮
void MainWindow::fdeleteClicked()
{
    string arg1 = ui->lineEdit_6->text().toStdString();
    ui->lineEdit_6->setText("");

    QString inputStr = "fdelete " + QString::fromStdString(arg1);
    ui->textBrowser->insertPlainText(inputStr);
    QTextCursor text_cursor(ui->textBrowser->textCursor());
    text_cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(text_cursor);
    if (arg1[0] != '/')
        arg1 = User->curDirPath + arg1;
    User->userDelete(arg1);
    QString outline;
    outline = "[1853790-ZZH-OS " + QString::fromStdString(User->curDirPath)+" ]$ ";
    gCmd->append(outline);
}

//读取文件按钮
void MainWindow::freadClicked()
{
    string arg1, arg2, arg3;
    arg1 = ui->lineEdit_7->text().toStdString();
    arg2 = ui->lineEdit_8->text().toStdString();
    arg3 = ui->lineEdit_9->text().toStdString();
    ui->lineEdit_7->setText("");
    ui->lineEdit_8->setText("");
    ui->lineEdit_9->setText("");

    QString inputStr = "fread " + QString::fromStdString(arg1) + " " + QString::fromStdString(arg2) + " " + QString::fromStdString(arg3);
    ui->textBrowser->insertPlainText(inputStr);
    QTextCursor text_cursor(ui->textBrowser->textCursor());
    text_cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(text_cursor);
    User->userRead(arg1, arg2, arg3);
    QString outline;
    outline = "[1853790-ZZH-OS " + QString::fromStdString(User->curDirPath)+" ]$ ";
    gCmd->append(outline);
}

//指针移动按钮
void MainWindow::fseekClicked()
{
    string arg1, arg2, arg3;
    arg1 = ui->lineEdit_10->text().toStdString();
    arg2 = ui->lineEdit_11->text().toStdString();
    arg3 = ui->comboBox->currentText().toStdString();
    ui->lineEdit_10->setText("");
    ui->lineEdit_11->setText("");

    QString inputStr = "fseek " + QString::fromStdString(arg1) + " " + QString::fromStdString(arg2) + " " + QString::fromStdString(arg3);
    ui->textBrowser->insertPlainText(inputStr);
    QTextCursor text_cursor(ui->textBrowser->textCursor());
    text_cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(text_cursor);
    if (arg3 == "begin")
        User->userSeek(arg1, arg2, string("0"));
    else if (arg3 == "cur")
        User->userSeek(arg1, arg2, string("1"));
    else if (arg3 == "end")
        User->userSeek(arg1, arg2, string("2"));
    QString outline;
    outline = "[1853790-ZZH-OS " + QString::fromStdString(User->curDirPath)+" ]$ ";
    gCmd->append(outline);
}

//写入文件按钮
void MainWindow::fwriteClicked()
{
    string arg1, arg2, arg3;
    arg1 = ui->lineEdit_12->text().toStdString();
    arg2 = ui->lineEdit_13->text().toStdString();
    arg3 = ui->lineEdit_14->text().toStdString();
    ui->lineEdit_12->setText("");
    ui->lineEdit_13->setText("");
    ui->lineEdit_14->setText("");

    QString inputStr = "fwrite " + QString::fromStdString(arg1) + " " + QString::fromStdString(arg2) + " " + QString::fromStdString(arg3);
    ui->textBrowser->insertPlainText(inputStr);
    QTextCursor text_cursor(ui->textBrowser->textCursor());
    text_cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(text_cursor);
    User->userWrite(arg1, arg2, arg3);
    QString outline;
    outline = "[1853790-ZZH-OS " + QString::fromStdString(User->curDirPath)+" ]$ ";
    gCmd->append(outline);
}

//改变路径按钮
void MainWindow::cdClicked()
{
    string arg1 = ui->lineEdit_18->text().toStdString();
    ui->lineEdit_18->setText("");

    QString inputStr = "cd " + QString::fromStdString(arg1);
    ui->textBrowser->insertPlainText(inputStr);
    QTextCursor text_cursor(ui->textBrowser->textCursor());
    text_cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(text_cursor);
    User->userCd(arg1);
    QString outline;
    outline = "[1853790-ZZH-OS " + QString::fromStdString(User->curDirPath)+" ]$ ";
    gCmd->append(outline);
}

//重命名文件按钮
void MainWindow::frenameClicked()
{
    string arg1 = ui->lineEdit_15->text().toStdString();
    string arg2 = ui->lineEdit_16->text().toStdString();
    ui->lineEdit_15->setText("");
    ui->lineEdit_16->setText("");

    QString inputStr = "frename " + QString::fromStdString(arg1) + " " + QString::fromStdString(arg2);
    ui->textBrowser->insertPlainText(inputStr);
    QTextCursor text_cursor(ui->textBrowser->textCursor());
    text_cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(text_cursor);
    User->userRename(arg1, arg2);
    QString outline;
    outline = "[1853790-ZZH-OS " + QString::fromStdString(User->curDirPath)+" ]$ ";
    gCmd->append(outline);
}

//文件目录树按钮
void MainWindow::ftreeClicked()
{
    string arg1 = ui->lineEdit_17->text().toStdString();
    ui->lineEdit_17->setText("");

    QString inputStr = "ftree " + QString::fromStdString(arg1);
    ui->textBrowser->insertPlainText(inputStr);
    User->userTree(arg1);
    QString outline;
    outline = "[1853790-ZZH-OS " + QString::fromStdString(User->curDirPath)+" ]$ ";
    gCmd->append(outline);
    QTextCursor text_cursor(gCmd->textCursor());
    text_cursor.movePosition(QTextCursor::End);
    gCmd->setTextCursor(text_cursor);
}
