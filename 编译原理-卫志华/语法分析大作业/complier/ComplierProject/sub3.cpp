#include "sub3.h"
#include "ui_sub3.h"

sub3::sub3(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sub3)
{
    ui->setupUi(this);
    setWindowTitle("规约过程和语法树展示");

}

sub3::~sub3()
{
    delete ui;
}

void sub3::showTreePic(){
    //显示树的图片
    QImage img;
    img.load("./MyStoreFolder/SynTaxTree.png");
    QPixmap pixmap = QPixmap::fromImage(img);
    int with = ui->label_3->width();
    int height = ui->label_3->height();
    QPixmap fitpixmap = pixmap.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
    ui->label_3->setPixmap(fitpixmap);

}

void sub3::showLR1Process(){

    //显示语法分析表
    ifstream in("./MyStoreFolder/6.AnalyzeProcess.txt");
    if(!in.is_open()){
        cerr<<"can not open file ./MyStoreFolder/6.AnalyzeProcess.txt\n";
        return;
    }
    int row=1000,col=5;
    ui->tableWidget->setColumnCount(col);
    ui->tableWidget->setRowCount(row);
    QStringList m_Header;
    m_Header.append("步骤");
    m_Header.append("状态栈");
    m_Header.append("符号栈");
    m_Header.append("输入串");
    m_Header.append("动作说明");
    ui->tableWidget->setHorizontalHeaderLabels(m_Header);

    string t;
    bool flag=true;
    for(int i=0;flag;i++){

        for(int j=0;j<col&&flag;j++){
            in>>t;
            ui->tableWidget->setColumnWidth(j,200+30*j);
            ui->tableWidget->setRowHeight(i,25);
            ui->tableWidget->setItem(i,j,new QTableWidgetItem(t.data()));
            ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignCenter);
            if(t=="acc!")
                flag=false;
        }
    }

}

void sub3::on_pushButton_released()
{
    //关闭这个界面
    close();
}
