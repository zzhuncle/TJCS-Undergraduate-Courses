#include "Syn_main.h"


map<int, string> V;  //产生映射表的关系map



int table[MAX_N][MAX_N];         //预测分析表 -1
int tb_s_r[MAX_N][MAX_N];        //是移进项还是规约项,-1,-2.
int numvt;                       //终结符的数目
int num;

vector<vector<Item>> itemSet;//项目集族
int edge[MAX_N][3];
int head[MAX_N];            //第i个项目集的头
int nume = 0;               //边数
vector<int> word;

/* 依据格式输入文法 */
//前一个参数输入，后三个参数输出
void inputGrammar(string filePath, map<int, int>& getNum, map<int, int>& getString, vector<vector<int>>& production)
{
    memset(table, -1, sizeof(table));
    int n1, n2, n3;//终结符、非终结符、产生式的数目
    int n = 0;
    ifstream in(filePath, ios::in);

    getNum[INT_MAX] = n;
    getString[n++] = INT_MAX; //INT_MAX 代表#号

    in >> n1 >> n2 >> n3;
    /************************/
    numvt = n1 + 1;
    num = n1 + n2 + 1;
    /************************/
    int temp;
    //终结符
    for (int i = 0; i < n1; i++, n++) {
        in >> temp;
        getNum[temp] = n;
        getString[n] = temp;
    }
    getNum[-1000] = n;
    getString[n++] = -1000; //INT_MAX 代表空字符
    //非终结符
    for (int i = 0; i < n2; i++, n++) {
        in >> temp;
        getNum[temp] = n;
        getString[n] = temp;
    }
    //产生式
    vector<int> temp1;
    for (int i = 0; i < n3; i++) {
        int n4;
        in >> n4;
        temp1.clear();
        for (int j = 0; j < n4; j++) {
            in >> temp;
            temp1.emplace_back(temp);
        }
        production.emplace_back(temp1);
    }
    in.close();

    /*
    cout << n << endl;
    for (auto it = getNum.begin(); it != getNum.end(); it++)
        cout << it->first << " " << it->second << endl;
    for (auto it = getString.begin(); it != getString.end(); it++)
        cout << it->first << " " << it->second << endl;
    for (auto it = production.begin(); it != production.end(); it++) {
        for (int i = 0; i < it->size(); i++) {
            cout << (*it)[i] << " ";
        }
        cout << endl;
    }
    */
}

/* 合并左式相同的产生式 */
//前两个参数输入，后一个参数输出
void getProduction(const vector<vector<int>>& production, map<int, int>& getNum, string getProduce[MAX_N])
{
    for (int i = 0; i < production.size(); i++) {
        int temp = getNum[production[i][0]];  //获得产生式左边的非终结符的编号
        getProduce[temp] += char('\0' + i);   //记录这个非终结符对应的产生式编号
    }
}

void dfsGetFirst(const vector<vector<int>>& production, map<int, int>& getNum, string getProduce[MAX_N],
    string first[MAX_N], int nv, int nump, bool getfirst[MAX_N])  //当前的符号，和对应产生式编号
{
    int temp = getNum[production[nump][1]];  //产生式推出来的首符
    getfirst[nump] = true;                   //标记
    if (temp <= numvt)                       //是终结符，直接加入返回
        first[nv] += char('\0' + temp);
    else {
        for (int j = 1; j < production[nump].size(); j++) {
            temp = getNum[production[nump][j]];
            for (int i = 0; i < getProduce[temp].size(); i++)  //所有temp可以推出来的符号对应的产生式
            {
                if (production[nump][0] == production[nump][1])
                    continue; //左递归的产生式不用 不影响求fisrt集
                dfsGetFirst(production, getNum, getProduce, first, temp, getProduce[temp][i] - '\0', getfirst);
            }
            int pos = first[temp].find('\0' + numvt);
            if (pos == -1) {  //没有空字符
                first[nv] += first[temp];
                break;
            }
            first[nv] += first[temp].substr(0, pos);
            first[nv] += first[temp].substr(pos + 1);
            if (j == production[nump].size() - 1)
                first[nv] += '\0' + numvt;                     //加上空字符
        }
        /*for (int i = 0; i < getProduce[temp].size(); i++)  //所有temp可以推出来的符号对应的产生式
        {
            if (production[nump][0] == production[nump][1])
                continue; //左递归的产生式不用 不影响求fisrt集
            dfsGetFirst(production, getNum, getProduce, first, temp, getProduce[temp][i] - '\0', getfirst);
        }
        first[nv] += first[temp];                  //回溯时候沿途保存*/
    }
}

/* 求项目的FIRST集 */
void getFirst(const vector<vector<int>>& production, map<int, int>& getNum, string getProduce[MAX_N], string first[MAX_N])
{
    bool getfirst[MAX_N];
    memset(getfirst, 0, sizeof(getfirst));         //初始化
    for (int i = 1; i <= numvt; i++)               //终结符first集合是它自己。 first集合通过终结符的编号来存储
        first[i] = char('\0' + i);
    for (int i = 0; i < production.size(); i++) {
        if (production[i][0] == production[i][1])
            continue; //左递归的产生式不用 不影响求fisrt集
        if (getfirst[i])
            continue; //已经生成
        int temp = getNum[production[i][0]];
        dfsGetFirst(production, getNum, getProduce, first, temp, i, getfirst);
    }
}

void change_first(string first[MAX_N])              //FIRST集去重
{
    for (int i = numvt + 1; i <= num; i++) {
        int len = first[i].length();
        set<char> temp;
        for (int j = 0; j < len; j++)
            temp.insert(first[i][j]);
        first[i].clear();
        for (auto it = temp.begin(); it != temp.end(); it++)
            first[i] += *it;
    }
}

void change_production(vector<vector<int>>& production) {
    for (int i = 0; i < production.size(); i++) {
        if (production[i][1] == -1000)
            production[i].pop_back();
    }
}

void init()                 //初始化函数
{
    for (int i = 0; i < MAX_N; i++)
        head[i] = -1;
    for (int i = 0; i < MAX_N; i++)
        for (int j = 0; j < MAX_N; j++)
            tb_s_r[i][j] = table[i][j] = -1;
    nume = 0;
}

void addegde(int from, int to, int w)          //添加边
{
    edge[nume][0] = to;
    edge[nume][1] = head[from];
    head[from] = nume;
    edge[nume++][2] = w;
}

/* 一些项目、项目集运算 */
inline bool itemEqual(Item a, Item b)
{
    if (a.forward == b.forward && a.ppos == b.ppos && a.nump == b.nump)
        return 1;
    return 0;
}

inline bool itemInSet(Item a, vector<Item> b)
{
    for (int i = 0; i < b.size(); i++)
        if (itemEqual(a, b[i]))
            return 1;
    return 0;
}

vector<Item> itemSetMerge(vector<Item> a, vector<Item> b)  //合并项目集 a,b 复给 a
{
    for (int i = 0; i < b.size(); i++)
        if (!itemInSet(b[i], a))
            a.push_back(b[i]);
    return a;
}

inline bool itemSetEqual(vector<Item> a, vector<Item> b)  //两个项目集是否相等
{
    if (a.size() != b.size())
        return 0;
    for (int i = 0; i < a.size(); i++)
        if (!itemInSet(a[i], b))
            return 0;
    return 1;
}

int findItemSet(vector<Item> a, vector<vector<Item>> b)  //查找项目集，若有，则返回编号,一举俩得
{
    for (int i = 0; i < b.size(); i++)
        if (itemSetEqual(a, b[i]))
            return i;
    return -1;
}

/* 求解项目集闭包 */
vector<Item> getItemClosure(Item t, map<int, int>& getNum, string getProduce[MAX_N], vector<vector<int>>& production, string first[MAX_N])             //对项目T作闭包
{
    vector<Item> temp;
    temp.push_back(t);
    queue<Item> q;
    q.push(t);
    while (!q.empty()) {
        Item cur = q.front();
        q.pop();
        if (cur.ppos == production[cur.nump].size())           //归约项舍去
            continue;
        int tt = getNum[production[cur.nump][cur.ppos]];       //是'.'之后的符号
        if (tt <= numvt)
            continue;                                          //若是终结符，不必寻找
        for (int i = 0; i < getProduce[tt].size(); i++)        //若是非终结符，对应产生式的编号
        {
            Item c;
            c.ppos = 1;
            c.nump = getProduce[tt][i] - '\0';
            if (production[cur.nump].size() - cur.ppos == 1)
                c.forward += cur.forward;
            else  {
                for (int j = 1; j < production[cur.nump].size(); j++) {
                    int tttnum = getNum[production[cur.nump][cur.ppos + j]];
                    int pos = first[tttnum].find('\0' + numvt);
                    if (pos == -1) {  //没有空字符
                        c.forward += first[tttnum];
                        break;
                    }
                    c.forward += first[tttnum].substr(0, pos);
                    c.forward += first[tttnum].substr(pos + 1);
                    if (cur.ppos + j == production[cur.nump].size() - 1) {
                        c.forward += cur.forward;
                        break;
                    }
                }
                /*int tttnum = getNum[production[cur.nump][cur.ppos + 1]];
                c.forward += first[tttnum];*/
            }
            if (!itemInSet(c, temp)) {
                q.push(c);
                temp.push_back(c);
            }
        }
    }
    return temp;
}

/* 获得项目集族 */
void getItemSet(map<int, int>& getNum, string getProduce[MAX_N], vector<vector<int>>& production, string first[MAX_N])
{
    vector<Item> temp;
    Item t;
    t.nump = 0;
    t.ppos = 1;
    t.forward += '\0';    //初始的项目集：0
    temp = getItemClosure(t, getNum, getProduce, production, first);

    queue<vector<Item>> q;
    q.push(temp);
    itemSet.push_back(temp);//S -> .BB, #

    while (!q.empty()) {
        vector<Item> cur = q.front();
        q.pop();
        for (int i = 1; i <= num; i++) //所有符号
        {
            if (i == numvt)
                continue;      //空字符
            vector<Item> temp;
            for (int j = 0; j < cur.size(); j++)                    //该项目集中的所有项目
            {
                if (cur[j].ppos == production[cur[j].nump].size())  //是规约项目，无法再读入
                    continue;
                int tt = getNum[production[cur[j].nump][cur[j].ppos]];
                if (tt == i) {
                    Item tempt;
                    tempt.forward = cur[j].forward;
                    tempt.ppos = cur[j].ppos + 1;
                    tempt.nump = cur[j].nump;
                    temp = itemSetMerge(temp, getItemClosure(tempt, getNum, getProduce, production, first));
                }
            }
            if (temp.size() == 0)
                continue;                             //该符号无法读入
            int numcur = findItemSet(cur, itemSet);   //当前节点标号
            int tttnum = findItemSet(temp, itemSet);  //新目标标号
            if (tttnum == -1)                         //新的项目集
            {
                itemSet.push_back(temp);
                q.push(temp);
                addegde(numcur, itemSet.size() - 1, i);   //添加边，权为读入的符号
            }
            else
                addegde(numcur, tttnum, i);
        }
    }
}

/* 打印项目集族 */
void printItemSet(vector<vector<int>>& production, map<int, int>& getString)
{
    for (int i = 0; i < itemSet.size(); i++) {
        cout << "项目集" << i << ":" << endl;
        for (int j = 0; j < itemSet[i].size(); j++){
            cout << "{";
            for (int k = 0; k < production[itemSet[i][j].nump].size(); k++) {
                cout << production[itemSet[i][j].nump][k] << ",";
            }
            cout << "}";
            cout << " " << itemSet[i][j].ppos << " " << itemSet[i][j].forward << endl;
        }
        cout << endl;
    }
    for (int i = 0; i < itemSet.size(); i++)
    {
        for (int j = head[i]; j != -1; j = edge[j][1])
        {
            cout << "  " << getString[edge[j][2]] << endl;
            cout << i << "--->" << edge[j][0] << endl;
        }
    }
}

/* 获得LR1分析表 table[i][j] = w:状态i --> j,读入符号W */
bool getLR1Table(vector<vector<int>>& production)
{
    cout << "*********************************" << endl;
    cout << itemSet.size() << endl;
    cout << "*********************************" << endl;
    for (int i = 0; i < itemSet.size(); i++)        //遍历图
        for (int j = head[i]; j != -1; j = edge[j][1]) {
            if (table[i][edge[j][2]] != -1)
                return 0;                           //多重入口，报错.
            table[i][edge[j][2]] = edge[j][0];
            tb_s_r[i][edge[j][2]] = -1;             //移近项-1
        }
    for (int i = 0; i < itemSet.size(); i++)        //遍历所有项目
        for (int j = 0; j < itemSet[i].size(); j++)
            if (itemSet[i][j].ppos == production[itemSet[i][j].nump].size())        //归约项
            {
                for (int k = 0; k < itemSet[i][j].forward.size(); k++) {
                    if (table[i][(itemSet[i][j].forward)[k] - '\0'] != -1)
                        return 0;           //多重入口，报错.
                    if ((itemSet[i][j].forward)[k] == '\0' && itemSet[i][j].nump == 0)
                        table[i][(itemSet[i][j].forward)[k] - '\0'] = -3;           //接受态
                    else {
                        table[i][(itemSet[i][j].forward)[k] - '\0'] = itemSet[i][j].nump;
                        tb_s_r[i][(itemSet[i][j].forward)[k] - '\0'] = -2;          //归约态
                    }
                }
            }
    return true;
}

/* 打印LR1分析表 */
void printLR1Table(map<int, int>& getString)
{
    ofstream out("./MyStoreFolder/5.LR1Table.txt");
    if(!out.is_open()){
        cerr<<"can not open file 5.LR1Table.txt\n";
        return;
    }
    out << itemSet.size() << " " << num << endl;

    for (int j = 0; j <= num; j++) {
        if (j == numvt)
            continue;
        out << "    " << getString[j];
    }
    out << endl;
    for (int i = 0; i < itemSet.size(); i++) {
        for (int j = 0; j <= num; j++)
        {
            if (j == numvt)
                continue;
            if (table[i][j] == -3)
                out << "acc" << ' ';       //接受
            else if (table[i][j] == -1)
                out << "*"<<' ';        //空
            else if (tb_s_r[i][j] == -1)
                out << "s" << table[i][j] <<' ';  //移近
            else if (tb_s_r[i][j] == -2)
                out << "r" << table[i][j] <<' ';  //归约
        }
        out << endl;
    }
}

/* 打印当前状态 */
void printCurState(int count, stack<int> state, stack<int> wd, int i, map<int, int>& getString,ofstream &out)
{
    out << count <<"        ";     //步骤  0 1 2 3 4 5 6 ……
    bool flag;

    flag=true;
    stack<int> temp;
    while (!state.empty()) {
        temp.push(state.top());
        state.pop();
    }
    while (!temp.empty()) {
        out << temp.top();
        out<<',';         //输出状态栈的状态，用，分割
        flag=false;
        temp.pop();
    }
    if(flag)
        out<<"NUL";


    flag=true;
    out <<"        ";
    while (!wd.empty()) {
        temp.push(wd.top());
        wd.pop();
    }
    while (!temp.empty()) {
        int i=getString[temp.top()];
        if(i==INT_MAX)
            out<<"#,";
        else
            out<<V[i]<<',';
        temp.pop();
        flag=false;
    }
    if(flag)
        out<<"NUL";




    flag=true;
    out << "        ";
    for (int j = i; j < word.size()-1; j++){
        out<<V[word[j]];
        flag=false;
    }
    if(flag)
        out<<"NUL";
    out << "        ";

}

/* 打印树函数 */
void printTree(Tree*& root, int depth) {
    for (int i = 0; i < depth; i++)
        cout << " ";
    cout << root->value << endl;
    for (int i = 0; i < root->children.size(); i++)
        printTree(root->children[i], depth + 2);
}

/* 绘制树函数 */
void paintTree1(ofstream& out, Tree*& root) {
    if (root->children.size())
        out << root->key << "[label=\"" << V[root->value] << "\"];" << endl;
    else
        out << root->key << "[shape=polygon,sides=4,peripheries=2,color=lightblue,style=filled,label=\""
        << V[root->value] <<"\"];" << endl;
    for (int i = 0; i < root->children.size(); i++)
        paintTree1(out, root->children[i]);
}
void paintTree2(ofstream& out, Tree*& root) {
    for (int i = 0; i < root->children.size(); i++)
        out << root->key << "->" << (root->children[i])->key << ";" << endl;
    for (int i = 0; i < root->children.size(); i++)
        paintTree2(out, root->children[i]);
}

/* 生成树函数 */
void getTree(stack<vector<int>>& regularset) {
    int key = 0;//用来唯一标识某个树节点

    Tree* root;
    stack<Tree*> Var;//变量存储栈

    vector<int> cur = regularset.top(); regularset.pop();
    if (cur.size() == 1)//空产生式
        cur.emplace_back(-1000);
    root = new Tree(key++, cur[0]);
    for (int i = 1; i < cur.size(); i++) {
        Tree* t = new Tree(key++, cur[i]);
        root->children.emplace_back(t);
        Var.push(t);
    }
    while (regularset.size()) {//LR1分析是最左规约，对应最右推导
        vector<int> cur = regularset.top(); regularset.pop();
        if (cur.size() == 1)//空产生式
            cur.emplace_back(-1000);
        Tree* p = Var.top();
        while (p->value >= 0 || p->value == -1000) {
            Var.pop();
            if (!Var.size()) break;
            p = Var.top();
        }
        if (!Var.size()) break;
        if (p->value != cur[0]) {
            cout << "出错！" << endl;
            return;
        }
        Var.pop();//弹出p
        for (int i = 1; i < cur.size(); i++) {
            Tree* t = new Tree(key++, cur[i]);
            p->children.emplace_back(t);
            Var.push(t);
        }
    }
    //printTree(root, 0);  //这个函数可以打印文字版的tree，但是标号类型的
    ofstream out("./MyStoreFolder/SynTaxTree.dot", ios::out);
    out << "digraph G {" << endl;
    out << "rankdir = TB;" << endl;
    paintTree1(out, root);
    paintTree2(out, root);
    out << "}" << endl;
    QProcess run;
    run.execute("dot -Tpng ./MyStoreFolder/SynTaxTree.dot -o ./MyStoreFolder/SynTaxTree.png");
//    system("dot -Tpng ./MyStoreFolder/SynTaxTree.dot -o ./MyStoreFolder/SynTaxTree.png");
}

/* 总控程序 */
bool totalControl(map<int, int>& getString, map<int, int>& getNum, vector<vector<int>>& production)
{
    ofstream out("./MyStoreFolder/6.AnalyzeProcess.txt",ios::out);
    if(!out.is_open()){
        cerr<<"can not open file ./MyStoreFolder/4.SyntaxGrammar.txt\n";
        return 0;
    }

    /**************************************/
    stack<vector<int>> regularset;//规约顺序
    /**************************************/
    cout << "步骤 " << "状态栈 " << "符号栈 " << "输入串 " << "动作说明 " << endl;
    stack<int> state;   //状态栈和符号栈
    stack<int> wd;
    int count = 0;
    state.push(0);     //初始化
    wd.push(0);        //'#'
    for (int i = 0;;)
    {
        int cur = state.top();
        if (table[cur][getNum[word[i]]] == -1)  //空白，报错误
            return 0;
        if (table[cur][getNum[word[i]]] == -3)  //接受态
        {
            /**************************************/
            regularset.push(production[0]);
            /**************************************/
            printCurState(count++, state, wd, i, getString,out);
            out << "acc!" << endl;
            getTree(regularset);

            return 1;
        }
        if (tb_s_r[cur][getNum[word[i]]] == -1)       //移进项
        {
            printCurState(count++, state, wd, i, getString,out);
            int newstate = table[cur][getNum[word[i]]];
            out << "action[" << cur << "," << V[getNum[word[i]]] << "]=" << newstate;
            out << ",状态" << newstate << "入栈" << endl;
            wd.push(getNum[word[i]]);
            state.push(newstate);
            i++;
        }
        else if (tb_s_r[cur][getNum[word[i]]] == -2)         //归约
        {
            printCurState(count++, state, wd, i, getString,out);

            int numpro = table[cur][getNum[word[i]]];   //用该产生式归约
            int len = production[numpro].size() - 1;
            for (int ii = 0; ii < len; ii++)                 //弹栈
            {
                wd.pop();
                state.pop();
            }
            wd.push(getNum[production[numpro][0]]);    //新入
            int cur = state.top();
            out << "用" << V[production[numpro][0]] << "->";
            for (int ii = 1; ii <= len; ii++)
                out << V[production[numpro][ii]]<<',';
            /**************************************/
            regularset.push(production[numpro]);
            /**************************************/
            out << "进行归约," << "goto[" << cur << "," << V[getNum[word[i]]] << "]=" << table[cur][getNum[production[numpro][0]]];
            out << "入栈" << endl;
            state.push(table[cur][getNum[production[numpro][0]]]);
        }
    }
    return 1;
}

bool isEnd(string s) {
    if (s == "int" || s == "void" || s == "if" || s == "while" || s == "else" || s == "return") {
        return true;
    }
    if (s == "+" || s == "-" || s == "*" || s == "/" || s == "=" || s == "==" || s == ">" || s == "<" || s == "!=" || s == ">=" || s == "<=") {
        return true;
    }
    if (s == ";" || s == "," || s == "(" || s == ")" || s == "{" || s == "}" || s == "ID" || s == "NUM") {
        return true;
    }
    if (s == "a" || s == "b" || s == "c" || s == "d")
        return true;
    return false;
}


void LRTableBackup1(const char* filename, const int row, const int col) {
    ofstream out(filename);
    out << row << " " << col << endl;
    for (int i = 0;i < row;i++) {
        for (int j = 0;j < col;j++) {
            out << table[i][j] << " ";
        }
        out << endl;
    }
}

void LRTableBackup2(const char* filename, const int row, const int col) {
    ofstream out(filename);
    out << row << " " << col << endl;
    for (int i = 0;i < row;i++) {
        for (int j = 0;j < col;j++) {
            out << tb_s_r[i][j] << " ";
        }
        out << endl;
    }
}


void LRTableReturn1(const char* filename) {
    int row, col;
    ifstream in(filename, ios::in);
    in >> row >> col;
    for (int i = 0;i < row;i++) {
        for (int j = 0;j < col;j++) {
            in >> table[i][j];
        }
    }
}

void LRTableReturn2(const char* filename) {
    int row, col;
    ifstream in(filename,ios::in);
    in >> row >> col;
    for (int i = 0;i < row;i++) {
        for (int j = 0;j < col;j++) {
            in >> tb_s_r[i][j];
        }
    }
}


void LRProductionBackup(const char* filename,vector<vector<int>>pro) {
    ofstream out(filename);
    out << pro.size() << endl;
    for (auto it = pro.begin();it!=pro.end();it++) {
        out << it->size() << ' ';
        for (auto itt = it->begin();itt != it->end();itt++) {
            out << *itt << " ";
        }
        out << endl;
    }

}
void LRProductionReturn(const char* filename, vector<vector<int>> &pro) {
    ifstream in(filename);
    pro.clear();
    int row,num,k;
    in >> row;
    for (int i = 0;i < row;i++) {
        vector<int> tmp;
        in >> num;
        for (int j = 0;j < num;j++) {
            in >> k;
            tmp.push_back(k);
        }
        pro.push_back(tmp);
    }
}



int Syn_main(int &isfirst)
{

    string filePath;
    map<int, int> getNum;
    map<int, int> getString;
    vector<vector<int>> production;
    string getProduce[MAX_N];
    string first[MAX_N];
    init();
    inputGrammar("./MyStoreFolder/4.SyntaxGrammar.txt", getNum, getString, production);

    if(isfirst){
        {
            string t1;
            int t2;
            ifstream in1("./MyStoreFolder/0.LexSynCheckList.txt", ios::in);
            while (!in1.eof()) {
                in1 >> t1;
                in1 >> t2;
                V.insert({ t2,t1 });
            }
            in1.close();
        }

        getProduction(production, getNum, getProduce);
        getFirst(production, getNum, getProduce, first);
        change_first(first);
        change_production(production);
        getItemSet(getNum, getProduce, production, first);
        if (!getLR1Table(production))
        {
            cerr << "This Grammar is not a LR(1) Grammar !" << endl;
            return -2;
        }

        //printItemSet(production, getString);
        printLR1Table(getString);  //这个函数我魔改了，将LR1Table输出文件里面

        LRTableBackup1("./MyStoreFolder/backup_Table.txt",itemSet.size()+1,num+2);
        LRTableBackup2("./MyStoreFolder/backup_tb_s_r.txt",itemSet.size()+1,num+2);
        LRProductionBackup("./MyStoreFolder/backup_production.txt",production);
        isfirst=0;
    }
    else{
        LRProductionReturn("./MyStoreFolder/backup_production.txt", production);
        LRTableReturn1("./MyStoreFolder/backup_Table.txt");
        LRTableReturn2("./MyStoreFolder/backup_tb_s_r.txt");
    }

    cout << "请输入字：" << endl;

    int temp;
    ifstream inpu("./MyStoreFolder/2.LexResultList.txt",ios::in);
    if(!inpu.is_open()){
        cerr<<"can not open 2.LexResultList.txt \n";
        return -1;
    }
    while(true){
        inpu>>temp;
        if(temp==-1)
            break;
        word.emplace_back(temp);
    }
    word.emplace_back(INT_MAX);


    if (!totalControl(getString, getNum, production)){
        cout << "error!" << endl;
        return -1;
    }
    else
        return 0;
}
