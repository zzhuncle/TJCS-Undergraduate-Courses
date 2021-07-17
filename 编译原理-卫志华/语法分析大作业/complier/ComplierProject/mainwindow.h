#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <string>
#include <fstream>
#include <QFileDialog>
#include <QMessageBox>
#include <QDialog>
#include <direct.h> //创建文件目录的文件夹
#include "LexAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include "Highlighter.h"
#include "sublex.h"
#include "subsyn.h"

#include "Lex_main.h"
#include "Syn_main.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    string src;
    sublex winsub1;
    subsyn winsub2;
    int isfirst=1;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void ErrorInfo(QString promt);


private slots:
    void on_pBSources_released();
    void showMainWindow();
    void CloseAllWindows();
    void on_pBLexAnalyze_released();


    void on_pBSynAnalyze_released();

    void on_pushButton_released();

    void on_pB3_released();

    void on_pB4_released();

    void on_pB5_released();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
