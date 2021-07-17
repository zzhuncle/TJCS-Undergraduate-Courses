#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>

#define MAX_V 21
#define MAX_E 101
#define INF 100000000

struct edge {
    int from;
    int to;
    double cost;
};

double d[MAX_V][MAX_V];//记录最短距离
int p[MAX_V][MAX_V];//记录路径
string city[MAX_V];//存储点
edge e[MAX_E];//存储边

set<int> vertexs;//记录路径的点
set<pair<int, int>> edges;//记录路径的边

int V, E;//顶点数、边数

//初始化
void init()
{
    for (int i = 1; i <= V; i++)
        for (int j = 1; j <= V; j++)
            d[i][j] = INF;
    for (int i = 1; i <= V; i++)
        d[i][i] = 0;
    for (int i = 1; i <= V; i++)
        for (int j = 1; j <= V; j++)
            p[i][j] = j;
}

//输入数据
void input(string path)
{
    ifstream in(path, ios::in);
    string temp = " ";
    while (temp != "[NodeNum EdgeNum]")
        getline(in, temp);
    in >> V >> E;
    init();
    while (temp != "[node]")
        getline(in, temp);
    int s;
    for (int i = 1; i <= V; i++) {
        in >> s;
        in >> city[s];
    }
    while (temp != "[line]")
        getline(in, temp);
    for (int i = 1; i <= E; i++) {
        in >> e[i].from >> e[i].to >> e[i].cost;
        d[e[i].from][e[i].to] = e[i].cost;
    }
    in.close();//关闭文件
}

//弗洛伊德算法
void floyd()
{
    for (int k = 1; k <= V; k++)
        for (int i = 1; i <= V; i++)
            for (int j = 1; j <= V; j++)
                if (d[i][k] + d[k][j] < d[i][j]) {
                    d[i][j] = d[i][k] + d[k][j];//更新最短路径
                    p[i][j] = p[i][k];//更新路径
                }
}

//输出数据
void output()
{
    ofstream out("out.dat", ios::out);
    for (int i = 1; i <= V; i++)
        for (int j = 1; j <= V; j++) {
            if (i == j)
                continue;
            out << city[i] << " ---> " << city[j] << " : ";
            if (fabs(d[i][j] - INF) < 1e-6)
                out << "INF";
            else
                out << d[i][j];
            out << endl;
        }
    out.close();
}

//绘制初始图像
void paintinit()
{
    ofstream out("out.dot", ios::out);
    out << "digraph G {" <<endl;
    out << "graph [autosize = false, size = \"8, 8\"];" << endl;
    out << "node [fontname = \"FangSong\"];" <<endl;
    for (int i = 1;i <= V;i++)
        out << city[i] << "[shape = box];" << endl;
    for (int i = 1;i <= E; i++)
        out << city[e[i].from] << " -> " << city[e[i].to] << "[style = bold, label = \"" << e[i].cost <<"\"];" << endl;
    out << "}" << endl;
    out.close();
    QProcess run;
    run.execute("dot -Tpng out.dot -o out.png");
    //system("dot -Tpng out.dot -o out.png");
}

//打印路径
QString path(const int& s,const int& t)
{
    vertexs.clear();
    edges.clear();
    stringstream io;
    string text = "", temp = "";

    vertexs.insert(s);
    vertexs.insert(t);

    if (d[s][t] == INF) {
        text += "无法到达";
        return QString::fromStdString(text);
    }
    text += "最短距离为 : ";
    io << d[s][t];
    io >> temp;
    text += temp;
    text += "\n";
    text += "真实路径为 : ";
    int mid = s;
    for (; mid != t; mid = p[mid][t]) {
        text += city[mid] + " -> ";
        vertexs.insert(mid);
        edges.insert(make_pair(mid, p[mid][t]));
    }
    text += city[t] + "\n";
    return QString::fromStdString(text);
}

//绘制路径图像
void paintpath()
{
    ofstream out("path.dot", ios::out);
    out << "digraph G {" <<endl;
    out << "graph [autosize = false, size = \"8, 8\"];" << endl;
    out << "node [fontname = \"FangSong\"];" <<endl;
    for (int i = 1;i <= V;i++) {
        if (vertexs.find(i) == vertexs.end())
            out << city[i] << "[shape = box];" << endl;
        else
            out << city[i] << "[shape = box, color = blue];" << endl;
    }
    for (int i = 1;i <= E; i++) {
        if (edges.find(make_pair(e[i].from, e[i].to)) == edges.end())
            out << city[e[i].from] << " -> " << city[e[i].to] << "[style = bold, label = \"" << e[i].cost <<"\"];" << endl;
        else
            out << city[e[i].from] << " -> " << city[e[i].to] << "[color = red, style = bold, label = \"" << e[i].cost <<"\"];" << endl;
    }
    out << "}" << endl;
    out.close();
    QProcess run;
    run.execute("dot -Tpng path.dot -o path.png");
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //点击选取文件按钮
    connect(ui->pushButton, &QPushButton::clicked, [=](){
        QString path = QFileDialog::getOpenFileName(this, "打开文件/初始化", QString::fromStdString("D:/Qt/code/Path/test"));
        //将路径放入到lineEdit中
        ui->lineEdit->setText(path);
        ui->textBrowser->clear();
        input(path.toStdString());//QString转化为String
        paintinit();//绘制初始图像
        floyd();
        output();

        QImage image;
        image.load("out.png");
        ui->label_3->setPixmap(QPixmap::fromImage(image));
        ui->label_3->setAlignment(Qt::AlignCenter);
        ui->label_3->update();

        //首先清除上次的选项
        ui->comboBox->clear();
        ui->comboBox_2->clear();
        //插入最新的选项
        for (int i = 1;i <= V;i++)
            ui->comboBox->addItem(QString::fromStdString(city[i]));
        for (int i = 1;i <= V;i++)
            ui->comboBox_2->addItem(QString::fromStdString(city[i]));
    });

    //点击显示路径按钮
    connect(ui->pushButton_2, &QPushButton::clicked, [=](){
        int s, t;
        s = ui->comboBox->currentIndex();
        t = ui->comboBox_2->currentIndex();
        ui->textBrowser->clear();
        ui->textBrowser->insertPlainText(path(s + 1, t + 1));

        //绘制路径图像
        paintpath();

        QImage image;
        image.load("path.png");
        ui->label_3->setPixmap(QPixmap::fromImage(image));
        ui->label_3->setAlignment(Qt::AlignCenter);
        ui->label_3->update();
    });

    //点击复位按钮
    connect(ui->pushButton_3, &QPushButton::clicked, [=](){
        QImage image;
        image.load("out.png");
        ui->label_3->setPixmap(QPixmap::fromImage(image));
        ui->label_3->setAlignment(Qt::AlignCenter);
        ui->label_3->show();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
