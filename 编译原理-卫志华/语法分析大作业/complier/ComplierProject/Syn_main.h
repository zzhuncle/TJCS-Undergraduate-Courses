#ifndef SYN_MAIN_H
#define SYN_MAIN_H

#include <string.h>
#include <map>
#include <set>
#include <queue>
#include <vector>
#include <stack>
#include <string>
#include <fstream>
#include <sstream>
#include <QProcess>
#include <iostream>
using namespace std;


#define MAX_N 1000

/* 项目集结构体 */
//S -> .BB, #
struct Item       //单个项目
{
    int nump;     //产生式编号
    int ppos;     //.的位置
    string forward;   //预测的非终结符集合
};

class Tree {//绘制树结构体
public:
    int key;
    int value;
    vector<Tree*> children;
    Tree(int k, int v) {
        key = k;
        value = v;
    }
};
int Syn_main(int &isfirst);


#endif // SYN_MAIN_H
