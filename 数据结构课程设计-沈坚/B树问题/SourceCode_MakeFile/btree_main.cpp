#define _CRT_SECURE_NO_WARNINGS
#define MAXM 10                     //定义B树的最大的阶数
#include <fstream>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;

const int M = 3;                      //设定B树的阶数 
const int MAX = M - 1;                //结点的最大关键字数量 
const int MIN = (M - 1) / 2;          //结点的最小关键字数量 

typedef struct node {               //B树和B树结点类型 
    int keynum;                     //结点关键字个数
    int key[MAXM];                  //关键字数组，key[0]不使用 
    struct node* parent;            //双亲结点指针
    struct node* ptr[MAXM];         //孩子结点指针数组 
} BTNode, * BTree;

typedef struct {                    //B树查找结果类型 
    BTNode* pt;                     //指向找到的结点
    int i;                          //在结点中的关键字位置; 
    bool tag;                       //查找成功与否标志
} Result;

typedef enum status {               //枚举类型（依次递增） 
    TRUE,
    FALSE,
    OK,
    ERROR,
    LOVERFLOW,
    EMPTY
} Status;

//函数用法
void Usage()
{
    cout << "Usage : ./btree --input in.dat --output out.dat 或 ./btree --output out.dat --input in.dat" << endl;
}

bool isnumber(char* ch_m, int& m)
{
    m = 0;
    for (int i = 0; ch_m[i]; i++) {
        if (!(ch_m[i] >= '0' && ch_m[i] <= '9'))
            return false;
        m *= 10;
        m += ch_m[i] - '0';
    }
    return true;
}

//初始化B树 
Status InitBTree(BTree& t) {
    t = NULL;
    return OK;
}

//在结点p中查找关键字k的插入位置i 
int SearchBTNode(BTNode* p, int k) {
    int i = 0;
    for (i = 0; i < p->keynum && p->key[i + 1] <= k; i++);
    return i;
}

//在树t上查找关键字k, 返回结果(pt, i, tag)
//若查找成功, 则特征值tag = 1, 关键字k是指针pt所指结点中第i个关键字
//否则特征值tag = 0, 关键字k的插入位置为pt结点的第i个
Result SearchBTree(BTree t, int k) {
    BTNode* p = t, * q = NULL;                      //初始化结点p和结点q, p指向待查结点,q指向p的双亲
    bool found_tag = false;                        //设定查找成功与否标志 
    int i = 0;
    Result r;                                      //设定返回的查找结果 

    while (p && found_tag == false) {
        i = SearchBTNode(p, k);                        //在结点p中查找关键字k, 使得p->key[i] <= k < p->key[i+1]
        if (i > 0 && p->key[i] == k)                   //找到待查关键字 
            found_tag = true;                          //查找成功 
        else {                                         //查找失败 
            q = p;
            p = p->ptr[i];
        }
    }

    if (found_tag == true) {                         //查找成功
        r.pt = p;
        r.i = i;
        r.tag = true;
    }
    else {                                           //查找失败
        r.pt = q;
        r.i = i;
        r.tag = false;
    }

    return r;                                       //返回关键字k的位置(或插入位置)
}

//将关键字k和结点q分别插入到p->key[i + 1]和p->ptr[i + 1]中
void InsertBTNode(BTNode*& p, int i, int k, BTNode* q) {
    int j;
    for (j = p->keynum; j > i; j--) {                       //整体后移空出一个位置
        p->key[j + 1] = p->key[j];
        p->ptr[j + 1] = p->ptr[j];
    }
    p->key[i + 1] = k;
    p->ptr[i + 1] = q;
    if (q)
        q->parent = p;
    p->keynum++;
}

//将结点p分裂成两个结点,前一半保留,后一半移入结点q
void SplitBTNode(BTNode*& p, BTNode*& q) {
    int i;
    int s = (M + 1) / 2;
    q = (BTNode*)malloc(sizeof(BTNode));             //给结点q分配空间

    q->ptr[0] = p->ptr[s];                            //后一半移入结点q
    for (i = s + 1; i <= M; i++) {
        q->key[i - s] = p->key[i];
        q->ptr[i - s] = p->ptr[i];
    }
    q->keynum = p->keynum - s;
    q->parent = p->parent;
    for (i = 0; i <= p->keynum - s; i++)                     //修改双亲指针 
        if (q->ptr[i])
            q->ptr[i]->parent = q;
    p->keynum = s - 1;                                  //结点p的前一半保留,修改结点p的keynum
}

//生成新的根结点t,原p和q为子树指针
void NewRoot(BTNode*& t, int k, BTNode* p, BTNode* q) {
    t = (BTNode*)malloc(sizeof(BTNode));            //分配空间 
    t->keynum = 1;
    t->ptr[0] = p;
    t->ptr[1] = q;
    t->key[1] = k;
    if (p)                                          //调整结点p和结点q的双亲指针 
        p->parent = t;
    if (q)
        q->parent = t;
    t->parent = NULL;
}

//在树t上结点p的key[i]与key[i + 1]之间插入关键字k。若引起结点过大,则沿双亲链进行必要的结点分裂调整,使t仍是B树
void InsertBTree(BTree& t, int i, int k, BTNode* p) {
    BTNode* q;
    bool finish_tag, newroot_tag;
    int s;                   //设定需要新结点标志和插入完成标志 
    int x;
    if (p == NULL)                                     //t是空树
        NewRoot(t, k, NULL, NULL);                     //生成仅含关键字k的根结点t
    else {
        x = k;
        q = NULL;
        finish_tag = false;
        newroot_tag = false;
        while (finish_tag == false && newroot_tag == false) {
            InsertBTNode(p, i, x, q);                  //将关键字x和结点q分别插入到p->key[i+1]和p->ptr[i+1]
            if (p->keynum <= MAX)
                finish_tag = true;                     //插入完成
            else {
                s = (M + 1) / 2;
                SplitBTNode(p, q);                    //分裂结点 
                x = p->key[s];
                if (p->parent) {                      //查找x的插入位置
                    p = p->parent;
                    i = SearchBTNode(p, x);
                }
                else                                //没找到x，需要新结点 
                    newroot_tag = true;
            }
        }
        if (newroot_tag == true)                          //根结点已分裂为结点p和q 
            NewRoot(t, x, p, q);                       //生成新根结点t,p和q为子树指针
    }
}

//从叶子结点p删除key[i]和它的孩子指针ptr[i]
void DeleteBTLeaf(BTNode* p, int i) {
    int j;
    for (j = i + 1; j <= p->keynum; j++) {                    //前移删除key[i]和ptr[i]
        p->key[j - 1] = p->key[j];
        p->ptr[j - 1] = p->ptr[j];
    }
    p->keynum--;
}

//查找被删关键字p->key[i](在非叶子结点中)的替代叶子结点(右子树中值最小的关键字) 
void Substitution(BTNode* p, int i) {
    BTNode* q;
    for (q = p->ptr[i]; q->ptr[0] != NULL; q = q->ptr[0]);
    p->key[i] = q->key[1];                            //复制关键字值
}

//将双亲结点p中的最后一个关键字移入右结点q中，将左结点aq中的最后一个关键字移入双亲结点p中
void BorrowLeft(BTNode* p, int i) {
    int j;
    BTNode* q = p->ptr[i];
    BTNode* aq = p->ptr[i - 1];

    for (j = q->keynum; j > 0; j--) {                       //将右兄弟q中所有关键字向后移动一位
        q->key[j + 1] = q->key[j];
        q->ptr[j + 1] = q->ptr[j];
    }

    q->ptr[1] = q->ptr[0];                            //从双亲结点p移动关键字到右兄弟q中
    q->key[1] = p->key[i];
    q->keynum++;

    p->key[i] = aq->key[aq->keynum];                  //将左兄弟aq中最后一个关键字移动到双亲结点p中
    p->ptr[i]->ptr[0] = aq->ptr[aq->keynum];
    aq->keynum--;
}

//将双亲结点p中的第一个关键字移入左结点aq中，将右结点q中的第一个关键字移入双亲结点p中
void BorrowRight(BTNode* p, int i) {
    int j;
    BTNode* aq = p->ptr[i - 1];
    BTNode* q = p->ptr[i];

    aq->keynum++;                                   //把双亲结点p中的关键字移动到左兄弟aq中
    aq->key[aq->keynum] = p->key[i];
    aq->ptr[aq->keynum] = p->ptr[i]->ptr[0];

    p->key[i] = q->key[1];                            //把右兄弟q中的关键字移动到双亲节点p中
    q->ptr[0] = q->ptr[1];
    q->keynum--;

    for (j = 1; j <= q->keynum; j++) {                     //将右兄弟q中所有关键字向前移动一位
        q->key[j] = q->key[j + 1];
        q->ptr[j] = q->ptr[j + 1];
    }
}

//将双亲结点p、右结点q合并入左结点aq，并调整双亲结点p中的剩余关键字的位置
void MergeBTNode(BTNode* p, int i) {
    int j;
    BTNode* q = p->ptr[i];
    BTNode* aq = p->ptr[i - 1];

    aq->keynum++;                                  //将双亲结点的关键字p->key[i]插入到左结点aq     
    aq->key[aq->keynum] = p->key[i];
    aq->ptr[aq->keynum] = q->ptr[0];

    for (j = 1; j <= q->keynum; j++) {                      //将右结点q中的所有关键字插入到左结点aq 
        aq->keynum++;
        aq->key[aq->keynum] = q->key[j];
        aq->ptr[aq->keynum] = q->ptr[j];
    }

    for (j = i; j < p->keynum; j++) {                       //将双亲结点p中的p->key[i]后的所有关键字向前移动一位 
        p->key[j] = p->key[j + 1];
        p->ptr[j] = p->ptr[j + 1];
    }
    p->keynum--;                                    //修改双亲结点p的keynum值 
    free(q);                                        //释放空右结点q的空间
}

//删除结点p中的第i个关键字后,调整B树
void AdjustBTree(BTNode* p, int i) {
    if (i == 0)                                        //删除的是最左边关键字
        if (p->ptr[1]->keynum > MIN)                   //右结点可以借
            BorrowRight(p, 1);
        else                                        //右兄弟不够借 
            MergeBTNode(p, 1);
    else if (i == p->keynum)                           //删除的是最右边关键字
        if (p->ptr[i - 1]->keynum > MIN)                 //左结点可以借 
            BorrowLeft(p, i);
        else                                        //左结点不够借 
            MergeBTNode(p, i);
    else if (p->ptr[i - 1]->keynum > MIN)                //删除关键字在中部且左结点够借 
        BorrowLeft(p, i);
    else if (p->ptr[i + 1]->keynum > MIN)                //删除关键字在中部且右结点够借 
        BorrowRight(p, i + 1);
    else                                            //删除关键字在中部且左右结点都不够借
        MergeBTNode(p, i);
}

//反映是否在结点p中是否查找到关键字k 
bool FindBTNode(BTNode* p, int k, int& i) {
    if (k < p->key[1]) {                               //结点p中查找关键字k失败 
        i = 0;
        return false;
    }
    else {                                             //在p结点中查找
        i = p->keynum;
        while (k < p->key[i] && i > 1)
            i--;
        if (k == p->key[i])                            //结点p中查找关键字k成功 
            return true;
    }
    return false;
}

//在结点p中查找并删除关键字k
bool BTNodeDelete(BTNode* p, int k) {
    int i;
    bool found_tag;                                  //查找标志 
    if (p == NULL)
        return false;
    else {
        found_tag = FindBTNode(p, k, i);                //返回查找结果 
        if (found_tag == true) {                           //查找成功 
            if (p->ptr[i - 1] != NULL) {                  //删除的是非叶子结点
                Substitution(p, i);                  //寻找相邻关键字(右子树中最小的关键字) 
                BTNodeDelete(p->ptr[i], p->key[i]);  //执行删除操作 
            }
            else
                DeleteBTLeaf(p, i);                        //从结点p中位置i处删除关键字
        }
        else
            found_tag = BTNodeDelete(p->ptr[i], k);    //沿孩子结点递归查找并删除关键字k
        if (p->ptr[i] != NULL)                         //p不是叶子节点
            if (p->ptr[i]->keynum < MIN)               //删除后关键字个数小于MIN
                AdjustBTree(p, i);                     //调整B树 
        return found_tag;
    }
}

//执行删除操作  
bool BTreeDelete(BTree& t, int k) {
    BTNode* p;
    bool flag = BTNodeDelete(t, k);                   //删除关键字k 
    if (!flag)                                        //查找失败 
        return false;
    else if (t->keynum == 0) {                        //调整 
        p = t;
        t = t->ptr[0];
        free(p);
    }
    return true;
}

//递归释放B树 
void DestroyBTree(BTree& t) {
    int i;
    BTNode* p = t;
    if (p) {                                    //B树不为空  
        for (i = 0; i <= p->keynum; i++)        //递归释放每一个结点 
            DestroyBTree(*&p->ptr[i]);
        free(p);
    }
    t = NULL;
}

//打印B树
void PrintBTree(BTree t, int count, ofstream& out)
{
    if (t) {
        int i, j;
        for (i = 1; i <= t->keynum; i++) {
            for (j = count; j > 0; j--)
                out << " ";
            out << t->key[i] << endl;
        }
        for (i = 0; i <= t->keynum; i++)
            if (t->ptr[i])
                PrintBTree(t->ptr[i], count + 2, out);
    }
}

//打印查找路径
void PrintSearchBTree(BTNode* t, int value, ofstream& out)
{
    if (t == NULL)
        return;
    else {
        int i;
        for (i = 1; i < t->keynum; i++)
            out << t->key[i] << " ";
        out << t->key[t->keynum];
        //在比它小和比它大的中间子节点中寻找相等的
        for (i = 1; i <= t->keynum; i++)
            if (value <= t->key[i])
                break;
        //校验当前的关键字是否等于查找的关键字t
        if (i <= t->keynum && value == t->key[i])
            out << endl;
        else {
            out << "->";
            PrintSearchBTree(t->ptr[i - 1], value, out);//这里的return别忘了
        }
    }
}

int main(int argc, char** argv)
{
    if (argc != 5) {
        Usage();
        return 0;
    }
    if (!((!strcmp(argv[1], "--input") && !strcmp(argv[3], "--output")) || (!strcmp(argv[3], "--input") && !strcmp(argv[1], "--output")))) {
        Usage();
        return 0;
    }
    char* indata, * outdata;
    indata = (!strcmp(argv[1], "--input") ? argv[2] : argv[4]);
    outdata = (!strcmp(argv[1], "--output") ? argv[2] : argv[4]);

    //判断文件是否可以打开
    ifstream in(indata, ios::in);
    ofstream out(outdata, ios::out);
    if (!in.is_open()) {
        cout << "打开文件" << indata << "失败" << endl;
        return 0;
    }
    in.close();

    freopen(indata, "r", stdin);
    BTree t = NULL;
    Result s;
    InitBTree(t);
    char ch_m[1024];
    char ch[1024];
    int m;
    int cnt = scanf("%s %s", ch, ch_m);
    if (1) {
        while (1) {
            cnt = scanf("%s %s", ch, ch_m);
            if (cnt == -1)
                break;
            if (cnt == 2 && isnumber(ch_m, m)) {
                if (!strcmp(ch, "insert")) {
                    s = SearchBTree(t, m);
                    if (s.tag == true)
                        out << "要插入的" << m << "已存在" << endl;
                    else {
                        InsertBTree(t, s.i, m, s.pt);
                        PrintBTree(t, 0, out);
                    }
                }
                else if (!strcmp(ch, "delete")) {
                    bool flag = BTreeDelete(t, m);
                    if (!flag)
                        out << "要删除的" << m << "不存在" << endl;
                    else
                        PrintBTree(t, 0, out);
                }
                else if (!strcmp(ch, "find")) {
                    s = SearchBTree(t, m);
                    if (s.tag == false)
                        out << "未找到节点" << m << endl;
                    else
                        PrintSearchBTree(t, m, out);
                }
            }
        }
    }
    else {
        while (1) {
            cnt = scanf("%s %s", ch, ch_m);
            if (cnt == -1)
                break;
            if (cnt == 2 && isnumber(ch_m, m)) {
                if (!strcmp(ch, "insert")) {
                    s = SearchBTree(t, m);
                    if (s.tag == true)
                        out << "要插入的节点" << m << "已存在" << endl;
                    else {
                        InsertBTree(t, s.i, m, s.pt);
                        PrintBTree(t, 0, out);
                    }
                }
                else if (!strcmp(ch, "delete")) {
                    bool flag = BTreeDelete(t, m);
                    if (!flag)
                        out << "要删除的节点" << m << "不存在" << endl;
                    else
                        PrintBTree(t, 0, out);
                }
                else if (!strcmp(ch, "find")) {
                    s = SearchBTree(t, m);
                    if (s.tag == false)
                        out << "未找到节点" << m << endl;
                    else {
                        out << "找到节点" << m << "的路径为：" << endl;
                        PrintSearchBTree(t, m, out);
                    }
                }
                else
                    out << "操作指令错误" << endl;
            }
            else
                out << "操作指令错误" << endl;
            out << endl;
        }
    }
    DestroyBTree(t);
    return 0;
}