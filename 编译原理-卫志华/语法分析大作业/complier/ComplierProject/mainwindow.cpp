#include "mainwindow.h"
#include "ui_mainwindow.h"


//下面是一些全局的文件夹名称和文件名称
string folderPath = "./MyStoreFolder";
string LexResIndex="1.LexResIndex.txt";
string LexResList="2.LexResList.txt";
string LexResWord="3.LexResWord.txt";
string SynGrammar="4.SynTaxGrammar.txt";






MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("编译原理演示程序");
    connect(&winsub1,SIGNAL(RETURNTOMAINWINDOW()),this,SLOT(showMainWindow()));
    connect(&winsub1,SIGNAL(CLOSEALL()),this,SLOT(CloseAllWindows()));

    connect(&winsub2,SIGNAL(RETURNTOMAINWINDOW()),this,SLOT(showMainWindow()));
    connect(&winsub2,SIGNAL(CLOSEALL()),this,SLOT(CloseAllWindows()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pBSources_released()
{

    QString path=QFileDialog::getOpenFileName(this,"open","./","TXT(*.txt *.dat *.c *.cpp)");
    QByteArray file_mid = path.toLocal8Bit();
    path = file_mid.data(); //这个仅仅用作处理输入路径为中文字符的例子

    if(path.isEmpty()==false){
        QByteArray ba = path.toLatin1(); // must
        char filename[256];
        this->src.clear();

        strcpy(filename,ba.data());
        ifstream input(filename, ios::in);
        if (!input.is_open()) {
            cerr << "打开 "<< filename <<" 文件失败" << endl;
            return ;
        }

        this->src = read(input);
        src += ' ';
        winsub2.srcc=src.data();
        ui->textEdit->clear();
        ui->textEdit->append(src.data());
        Highlighter * h = new Highlighter(ui->textEdit->document());//传一个QTextDocument

    }

    //下面是为了做一些铺垫工作，新创建一个存储所有临时文件的文件夹
        if (0 != access(folderPath.c_str(), 0))
        {
            // if this folder not exist, create a new one.
            mkdir(folderPath.c_str());   // 返回 0 表示创建成功，-1 表示失败
            //换成 ::_mkdir  ::_access 也行，不知道什么意思
        }



}

void MainWindow::on_pBLexAnalyze_released(){
    //进行词法分析
    Lex_main(src);

    hide();     //这个地方不要用close（）会把内存删掉
    winsub1.show();
    winsub1.LexShow();
}


void MainWindow::showMainWindow(){
    show();
}
void MainWindow::CloseAllWindows(){
    close();
}

void MainWindow::on_pBSynAnalyze_released(){
    //开始语法分析的步骤
    hide();
    int flag=Syn_main(isfirst);
    winsub2.flag=flag;
    if(flag==-2){
        QMessageBox::information(this,"错误弹窗","此文法不是LR（1）文法");
    }
    else
        winsub2.show(); //先生成页面

}

void MainWindow::on_pushButton_released()
{
    //退出演示
    close();
}

void MainWindow::ErrorInfo(QString promt){
    QString dlgTitle="错误弹窗";
    QMessageBox::information(this,dlgTitle,promt,QMessageBox::Ok);}

void MainWindow::on_pB3_released()
{
    ErrorInfo("语义分析还未实现");
}

void MainWindow::on_pB4_released()
{
    ErrorInfo("中间代码生成还未实现");
}

void MainWindow::on_pB5_released()
{
    ErrorInfo("目标代码生成还未实现");
}
