#include "subsyn.h"
#include "ui_subsyn.h"

subsyn::subsyn(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::subsyn)
{
    ui->setupUi(this);
    setWindowTitle("语法分析界面");
}

subsyn::~subsyn()
{
    delete ui;
}


void subsyn::on_pushButton_5_released()
{
    //返回主页面的函数
    close();
    emit RETURNTOMAINWINDOW();
}

void subsyn::on_pushButton_released()
{
    //点击按钮显示终结符
    //显示终结符
    ui->tB_VT->clear();
    for(int i=VT_MIN;i<=VT_MAX;i++){
        QString tmp;
        tmp=VT_to_string(i-VT_MIN).data();
        ui->tB_VT->append(tmp);
    }

}

void subsyn::on_pushButton_3_released()
{
    //显示非终结符
    ui->tB_VN->clear();
    for(int i=VN_MIN;i<=VN_MAX;i++){
        QString tmp;
        tmp=VN_to_string(i-VN_MIN).data();
        ui->tB_VN->append(tmp);
    }
}

void subsyn::on_pushButton_2_released()
{
    //显示生成文法
    ui->tB_grammar->clear();
    string filename="./MyStoreFolder/4.SynTaxGrammar.txt";
    int n1, n2, n3;//终结符、非终结符、产生式的数目
    int n = 0;
    ifstream in(filename, ios::in);
    in>>n1>>n2>>n3;
    //终结符
    for (int i = 0; i < n1; i++, n++) {
        in >> n;
    }
    //非终结符
    for (int i = 0; i < n2; i++, n++) {
        in >> n;
    }
    string tmp;
    for (int i = 0; i < n3; i++) {
        tmp="";
        int n4;
        in >> n4;
        for (int j = 0; j < n4; j++) {
            in >> n;
            if(j==0){
                tmp+=VN_to_string(n-VN_MIN);
                tmp+=" --> ";
            }
            else{
                if(n>=0){
                    tmp+=VT_to_string(n-VT_MIN);

                }
                else{
                    tmp+=VN_to_string(n-VN_MIN);

                }
                if(j<n4-1)
                    tmp+=" ";
            }


        }
        ui->tB_grammar->append(tmp.data());
    }

}


void subsyn::on_pushButton_8_released()
{

    //显示待分析的句子
    ui->tB_sentence->clear();
    ui->tB_sentence->append(srcc);
    Highlighter * h2 = new Highlighter(ui->tB_sentence->document());//传一个QTextDocument

}

void subsyn::on_pushButton_4_released()
{
    //显示语法分析表
    ifstream in("./MyStoreFolder/5.LR1Table.txt");
    if(!in.is_open()){
        cerr<<"can not open file ./MyStoreFolder/5.LR1Table.txt\n";
        return;
    }
    int row,col,tmp;
    in>>row>>col;


    ui->tableWidget->setColumnCount(col);
    ui->tableWidget->setRowCount(row);
    ui->tableWidget->setWindowTitle("LR(1) Table");
    QStringList m_Header;
    for(int i=0;i<col;i++){
        in>>tmp;
        if(tmp==INT_MAX)
            m_Header.append(" # ");
        else if(tmp>=0)
            m_Header.append(VT_to_string(tmp-VT_MIN).data());
        else
            m_Header.append(VN_to_string(tmp-VN_MIN).data());
    }
    ui->tableWidget->setHorizontalHeaderLabels(m_Header);

    string t;
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            ui->tableWidget->setColumnWidth(j,100);
            ui->tableWidget->setRowHeight(i,30);
            in>>t;
            if(t=="*")
                continue;

            ui->tableWidget->setItem(i,j,new QTableWidgetItem(t.data()));
            ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignCenter);

        }
    }
}

void subsyn::on_pushButton_6_released()
{
    //这个函数能够调用出新的窗体显示规约过程和图片
    if(!flag){
        winsub3.show();
        winsub3.showTreePic();
        winsub3.showLR1Process();
    }
    else{
        QMessageBox::information(this,"错误弹窗","语句分析失败！",QMessageBox::Ok);
    }
}

void subsyn::on_pushButton_7_released()
{
    //直接退出演示，的发送信号
    emit CLOSEALL();
    close();

}
