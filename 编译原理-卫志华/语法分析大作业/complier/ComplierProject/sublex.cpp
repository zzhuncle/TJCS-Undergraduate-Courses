#include "sublex.h"
#include "ui_sublex.h"

sublex::sublex(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sublex)
{
    ui->setupUi(this);
    setWindowTitle("词法分析页面");
}

sublex::~sublex()
{
    delete ui;
}

void sublex::on_pushButton_clicked(){
    //返回主界面
    emit RETURNTOMAINWINDOW();
    close();
}

void sublex::on_pushButton_2_released()
{
    //退出整个程序
    emit CLOSEALL();
    close();
}

void sublex::LexShow(){
    string tmp,s1,s2;
    string file="./MyStoreFolder/3.LexResultWord.txt";
    ifstream infile(file,ios::in);
    ui->sub_TextEdit->clear();
    while(!infile.eof()){
        infile>>s1;
        infile>>s2;
        if(!infile.good()||infile.eof()){
            break;
        }
        tmp=s1+"  --->   "+s2;
        ui->sub_TextEdit->append(tmp.data());
    }




   // tmp=string(src.substr(seg.left_index, seg.right_index - seg.left_index)) << " " << to_string(seg.component_type) << endl;





}
