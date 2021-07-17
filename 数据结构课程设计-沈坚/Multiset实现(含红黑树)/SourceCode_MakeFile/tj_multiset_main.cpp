// 1851909 聂尧 数强
#include <iostream>
#include <set>
#include <string>
#include <functional>  // for std::less

#include "tj_student.h"
#include "tj_multiset.cpp"

using namespace std;

// 打开注释，自己的tj_multiset测试
// 关闭注释，STL的multiset测试
#define multiset tj_multiset

// 打开注释，进行对该数据类型的测试
// 关闭注释，不进行对该数据类型的测试
#define INT
#define STRING
#define STUDENT

// 测试函数
void test_set();

int main()
{
    std::cout << "测试开始...\n" << std::endl;
    test_set();
    std::cout << "测试结束！\n" << std::endl;
    return 0;
}

// 任意键继续
void continue_key(std::string prompt, int num = 1);
// 打印标准输出与实际输出的比较（集合）
template <class K, class C = std::less<K>>
void print_compare(std::string std_out, multiset<K, C>& s);
// 打印标准输出与实际输出的比较（元素）
template <class T>
void print_compare(std::string std_out, T ele);
// 打印标准输出与实际输出的比较（student）
template <class K, class C>
void print_compare_stu(std::string std_out, multiset<K, C>& s);

struct IntCmp {
    bool operator()(const int& lhs, const int& rhs) const {
        return lhs > rhs;
    }
};

void test_set()
{
#ifdef INT
    continue_key("int数据类型");

    int index_int[6] = { 1, 1, 1, 1, 1, 1 };

    if (index_int[0]) {  //初始化器：构造/析构/=
        continue_key("第1组：初始化器");

        continue_key("构造函数");

        // 无参构造函数
        multiset<int> a;
        print_compare("<None>", a);

        // 迭代器构造函数
        for (int i = 1; i <= 5; i++)
            a.insert(i);
        multiset<int> b(a.find(3), a.end());
        print_compare("3 4 5", b);

        // 复制构造函数
        multiset<int> c(a);
        print_compare("1 2 3 4 5", c);

        // 移动构造函数
        // 暂不要求

        // initializer_list 构造函数
        multiset<int> d{ 6, 7, 8, 9, 10 };
        print_compare("6 7 8 9 10", d);

        multiset<int> e{ 10, 9, 8, 7, 6 };  //倒序
        print_compare("6 7 8 9 10", e);

        multiset<int> f{ 1,2,1,3 };  //含重复元素
        print_compare("1 1 2 3", f);

        // 自定义比较器的构造函数
        multiset<int, IntCmp> g{ 10, 9, 8, 7, 6 };
        print_compare("10 9 8 7 6", g);

        continue_key("析构函数");
        // 析构函数
        for (int i = 0; i < 1e5; i++) {  //循环次数可调整
            if (1)
                multiset<int> s{ 1,2,3,4,5,6,7,8 };
        }

        continue_key("复制构造");
        // 复制构造=
        multiset<int> h;
        h = f;
        print_compare("1 1 2 3", h);

        // 复制构造=（initializer_list）
        multiset<int> i;
        i = { 1,2,3 };
        print_compare("1 2 3", i);
    }
    if (index_int[1]) {  //迭代器：begin/cbegin/end/cend/rbegin/crbegin/rend/crend
        continue_key("第2组：迭代器");

        multiset<int> a{ 1,2,3,4,5 }, b{ 5,1,4,1,3,2 };
        multiset<int, IntCmp> c{ 5,1,4,1,3,2 };

        // begin/cbegin
        continue_key("begin/cbegin");

        print_compare("1", *a.begin());

        print_compare("1", *b.cbegin());

        print_compare("5", *c.begin());

        // end/cend
        continue_key("end/cend");

        print_compare("5", *--a.end());

        print_compare("5", *--b.end());

        print_compare("1", *--c.cend());

        // rbegin/crbegin
        continue_key("rbegin/crbegin");

        print_compare("5", *a.crbegin());

        print_compare("5", *b.rbegin());

        print_compare("1", *c.rbegin());

        // rend/crend
        continue_key("rend/crend");

        print_compare("1", *--a.rend());

        print_compare("1", *--b.crend());

        print_compare("5", *--c.crend());
    }
    if (index_int[2]) {  //容量：empty/size/max_size
        continue_key("第3组：容量");

        // empty
        continue_key("empty");

        multiset<int> a;
        print_compare("1", a.empty());

        a.insert(1);
        print_compare("0", a.empty());

        // size
        continue_key("size");

        multiset<int> b{ 1,1,2,3,4,5,6,7,8,9,10 };
        print_compare("11", b.size());

        a.erase(1);
        print_compare("0", a.size());

        // max_size（不同平台有差异）
        continue_key("max_size");

        print_compare("214748364", a.max_size());
        print_compare("214748364", b.max_size());
    }
    if (index_int[3]) {  //修改器：clear/insert/emplace/erase/swap/extract/merge
        continue_key("第4组：修改器");

        // clear 
        continue_key("clear");

        multiset<int> a{ 1,1,2,3,4,5 };
        a.clear();
        print_compare("<None>", a);

        a.clear();  // 空集情况下clear
        print_compare("<None>", a);

        // insert
        continue_key("insert");

        a.insert(1);
        print_compare("1", a);

        a.insert(1);  //插入相同元素
        print_compare("1 1", a);

        multiset<int> b{ 2,3,4 };
        auto it = a.insert(5);
        a.insert(b.begin(), b.find(4));  // 迭代器插入
        print_compare("1 1 2 3 5", a);
        print_compare("5", *it);

        a.insert(b.find(3), b.end());  // 迭代器插入，含相同元素
        print_compare("1 1 2 3 3 4 5", a);

        a.insert({ 6,7,8 });  // initializer_list 插入
        print_compare("1 1 2 3 3 4 5 6 7 8", a);

        a.insert({ 8,9 });  // initializer_list 插入，含相同元素
        print_compare("1 1 2 3 3 4 5 6 7 8 8 9", a);

        // emplace 
        // 暂不要求

        // erase
        continue_key("erase");

        a.erase(a.find(5));
        print_compare("1 1 2 3 3 4 6 7 8 8 9", a);

        a.erase(a.find(1));  //擦除重复元素
        print_compare("1 2 3 3 4 6 7 8 8 9", a);

        a.erase(a.begin(), a.find(3));  //erase 迭代器
        print_compare("3 3 4 6 7 8 8 9", a);

        a.erase(a.find(3), a.find(3));  //区间重合（区间首尾为重复元素）
        print_compare("3 3 4 6 7 8 8 9", a);

        a.erase(a.find(3), a.find(4));  //区间重合（区间包含重复元素）
        print_compare("4 6 7 8 8 9", a);

        a.erase(8);   //erase 重复的值
        print_compare("4 6 7 9", a);

        a.erase(10);  //erase 不存在的值
        print_compare("4 6 7 9", a);

        // swap 
        continue_key("swap");

        a.swap(b);
        print_compare("2 3 4", a);
        print_compare("4 6 7 9", b);

        // extract 
        // 暂不要求（c++17）

        // merge
        // 暂不要求（c++17）
    }
    if (index_int[4]) {  //查找：count/find/equal_range/lower_bound/upper_bound
        continue_key("第5组：查找");

        // count
        continue_key("count");

        multiset<int> a{ 1,1,2,3,4,5 };
        print_compare("2", a.count(1));
        print_compare("0", a.count(6));

        // find 
        continue_key("find");
        auto it = a.find(1);
        print_compare("1", (it != a.end() ? std::to_string(*it) : "Not Found"));

        it = a.find(0);
        print_compare("Not Found", (it != a.end() ? std::to_string(*it) : "Not Found"));

        // equal_range 
        continue_key("equal_range");
        auto res = a.equal_range(1);  // 值存在
        print_compare("1", *res.first);
        print_compare("2", *res.second);

        a.erase(3);
        res = a.equal_range(3);  // 值不存在
        print_compare("4", *res.first);
        print_compare("4", *res.second);

        // lower_bound 
        continue_key("lower_bound");
        it = a.lower_bound(2);
        print_compare("2", *it);

        it = a.lower_bound(3);
        print_compare("4", *it);

        // upper_bound
        continue_key("upper_bound");
        it = a.upper_bound(4);
        print_compare("5", *it);

        it = a.upper_bound(3);
        print_compare("4", *it);
    }
    if (index_int[5]) {  //比较运算符：==/!=/>/>=/</<=
        continue_key("第6组：比较运算符");

        multiset<int> a{ 1,1,2,3 }, b{ 6,7,8,9 }, c{ 1,2,3 };

        // ==
        continue_key("==");

        print_compare("false", a == b ? "true" : "false");  //不相等set
        print_compare("false", a == c ? "true" : "false");  //相等set

        // != 
        continue_key("!=");

        print_compare("true", a != b ? "true" : "false");  //不相等set
        print_compare("true", a != c ? "true" : "false");  //相等set

        // > 
        continue_key(">");

        print_compare("false", a > b ? "true" : "false");  //不相等set
        print_compare("false", a > c ? "true" : "false");  //相等set

        // >=
        continue_key(">=");

        print_compare("false", a >= b ? "true" : "false");  //不相等set
        print_compare("false", a >= c ? "true" : "false");  //相等set

        // < 
        continue_key("<");

        print_compare("true", a < b ? "true" : "false");  //不相等set
        print_compare("true", a < c ? "true" : "false");  //相等set

        // <=
        continue_key("<=");

        print_compare("true", a <= b ? "true" : "false");  //相等set
        print_compare("true", a <= c ? "true" : "false");  //不相等set
    }
#endif

#ifdef STRING
    continue_key("string数据类型");

    int index_str[4] = { 1, 1, 1, 1 };

    if (index_str[0]) {  //容量：empty/size/max_size
        continue_key("第1组：容量");

        // empty
        continue_key("empty");

        multiset<std::string> a;
        print_compare("1", a.empty());

        a.insert("bird");
        print_compare("0", a.empty());

        // size
        continue_key("size");

        multiset<std::string> b{ "bird","bird","cat","cow","dog","fox","horse","rabbit","sheep","wolf" };
        print_compare("10", b.size());

        a.erase("bird");
        print_compare("0", a.size());

        // max_size（不同平台有差异）
        continue_key("max_size");

        print_compare("97612893", a.max_size());
        print_compare("97612893", b.max_size());
    }
    if (index_str[1]) {  //修改器：clear/insert/emplace/erase/swap/extract/merge
        continue_key("第2组：修改器");

        // clear 
        continue_key("clear");

        multiset<std::string> a{ "bird","bird","cat","cow","dog","fox" };
        a.clear();
        print_compare("<None>", a);

        a.clear();  // 空集情况下clear
        print_compare("<None>", a);

        // insert
        continue_key("insert");

        a.insert("bird");
        print_compare("bird", a);

        a.insert("bird");  //插入相同元素
        print_compare("bird bird", a);

        multiset<std::string> b{ "cat","cow","dog" };
        auto it = a.insert("fox");
        a.insert(b.begin(), b.find("dog"));  // 迭代器插入
        print_compare("bird bird cat cow fox", a);
        print_compare("fox", *it);

        a.insert(b.find("cow"), b.end());  // 迭代器插入，含相同元素
        print_compare("bird bird cat cow cow dog fox", a);

        a.insert({ "horse","rabbit","sheep" });  // initializer_list 插入
        print_compare("bird bird cat cow cow dog fox horse rabbit sheep", a);

        a.insert({ "sheep","wolf" });  // initializer_list 插入，含相同元素
        print_compare("bird bird cat cow cow dog fox horse rabbit sheep sheep wolf", a);

        // emplace 
        // 暂不要求

        // erase
        continue_key("erase");

        a.erase(a.find("fox"));
        print_compare("bird bird cat cow cow dog horse rabbit sheep sheep wolf", a);

        a.erase(a.find("bird"));  //擦除重复元素
        print_compare("bird cat cow cow dog horse rabbit sheep sheep wolf", a);

        a.erase(a.begin(), a.find("cow"));  //erase 迭代器
        print_compare("cow cow dog horse rabbit sheep sheep wolf", a);

        a.erase(a.find("cow"), a.find("cow"));  //区间重合（区间首尾为重复元素）
        print_compare("cow cow dog horse rabbit sheep sheep wolf", a);

        a.erase(a.find("cow"), a.find("dog"));  //区间重合（区间包含重复元素）
        print_compare("dog horse rabbit sheep sheep wolf", a);

        a.erase("sheep");   //erase 重复的值
        print_compare("dog horse rabbit wolf", a);

        a.erase("apple");  //erase 不存在的值
        print_compare("dog horse rabbit wolf", a);

        // swap 
        continue_key("swap");

        a.swap(b);
        print_compare("cat cow dog", a);
        print_compare("dog horse rabbit wolf", b);

        // extract 
        // 暂不要求（c++17）

        // merge
        // 暂不要求（c++17）
    }
    if (index_str[2]) {  //查找：count/find/equal_range/lower_bound/upper_bound
        continue_key("第3组：查找");

        // count
        continue_key("count");

        multiset<int> a{ 1,1,2,3,4,5 };
        print_compare("2", a.count(1));
        print_compare("0", a.count(6));

        // find 
        continue_key("find");
        auto it = a.find(1);
        print_compare("1", (it != a.end() ? std::to_string(*it) : "Not Found"));

        it = a.find(0);
        print_compare("Not Found", (it != a.end() ? std::to_string(*it) : "Not Found"));

        // equal_range 
        continue_key("equal_range");
        auto res = a.equal_range(1);  // 值存在
        print_compare("1", *res.first);
        print_compare("2", *res.second);

        a.erase(3);
        res = a.equal_range(3);  // 值不存在
        print_compare("4", *res.first);
        print_compare("4", *res.second);

        // lower_bound 
        continue_key("lower_bound");
        it = a.lower_bound(2);
        print_compare("2", *it);

        it = a.lower_bound(3);
        print_compare("4", *it);

        // upper_bound
        continue_key("upper_bound");
        it = a.upper_bound(4);
        print_compare("5", *it);

        it = a.upper_bound(3);
        print_compare("4", *it);
    }
    if (index_str[3]) {  //比较运算符：==/!=/>/>=/</<=
        continue_key("第4组：比较运算符");

        multiset<std::string> a{ "bird","bird","cat","cow" }, b{ "horse","rabbit","sheep","wolf" }, c{ "bird","cat","cow" };

        // ==
        continue_key("==");

        print_compare("false", a == b ? "true" : "false");  //不相等set
        print_compare("false", a == c ? "true" : "false");  //相等set

        // != 
        continue_key("!=");

        print_compare("true", a != b ? "true" : "false");  //不相等set
        print_compare("true", a != c ? "true" : "false");  //相等set

        // > 
        continue_key(">");

        print_compare("false", a > b ? "true" : "false");  //不相等set
        print_compare("false", a > c ? "true" : "false");  //相等set

        // >=
        continue_key(">=");

        print_compare("false", a >= b ? "true" : "false");  //不相等set
        print_compare("false", a >= c ? "true" : "false");  //相等set

        // < 
        continue_key("<");

        print_compare("true", a < b ? "true" : "false");  //不相等set
        print_compare("true", a < c ? "true" : "false");  //相等set

        // <=
        continue_key("<=");

        print_compare("true", a <= b ? "true" : "false");  //相等set
        print_compare("true", a <= c ? "true" : "false");  //不相等set
    }
#endif

#ifdef STUDENT
    continue_key("student数据类型");

    int index_stu[4] = { 1, 1, 1, 1 };

    const student stu[] = {
       {1850001,"张三",'m',(float)(83.5),"山东"},
       {1850002,"李四",'f',(float)(89.5),"山西"},
       {1850003,"王五",'m',(float)(91),"河北"},
       {1850004,"赵六",'f',(float)(95),"河南"},
       {1850005,"李华",'m',(float)(96),"中国"},
       {1850006,"Jenny",'f',(float)(98.5),"US"},
       {1850007,"Danny",'m',(float)(100),"UK"}
    };

    const std::string str[] = {
       "1850001 张三 m 83.5 山东",
       "1850002 李四 f 89.5 山西",
       "1850003 王五 m 91 河北",
       "1850004 赵六 f 95 河南",
       "1850005 李华 m 96 中国",
       "1850006 Jenny f 98.5 US",
       "1850007 Danny m 100 UK"
    };

    if (index_stu[0]) {  //容量：empty/size/max_size
        continue_key("第1组：容量");

        // empty
        continue_key("empty");

        multiset<student> a;
        print_compare("1", a.empty());

        a.insert(stu[0]);
        print_compare("0", a.empty());

        // size
        continue_key("size");

        multiset<student> b{ stu[0],stu[0],stu[1],stu[2],stu[3],stu[4],stu[5],stu[6] };
        print_compare("8", b.size());

        a.erase(stu[0]);
        print_compare("0", a.size());

        // max_size（不同平台有差异）
        continue_key("max_size");

        print_compare("63161283", a.max_size());
        print_compare("63161283", b.max_size());
    }
    if (index_stu[1]) {  //修改器：clear/insert/emplace/erase/swap/extract/merge
        continue_key("第2组：修改器");

        // clear 
        continue_key("clear");

        multiset<student> a{ stu[0],stu[0],stu[1],stu[2],stu[3],stu[4] };
        a.clear();
        print_compare("<None>", a);

        a.clear();  // 空集情况下clear
        print_compare("<None>", a);

        // insert
        continue_key("insert");

        a.insert(stu[0]);
        print_compare_stu(str[0], a);

        a.insert(stu[0]);  //插入相同元素
        print_compare_stu(str[0] + "\n" + str[0], a);

        multiset<student> b{ stu[1],stu[2],stu[3] };
        auto it = a.insert(stu[4]);
        a.insert(b.begin(), b.find(stu[3]));  // 迭代器插入
        print_compare_stu(str[0] + "\n" + str[0] + "\n" + str[1] + "\n" + str[2] + "\n" + str[4], a);
        print_compare(str[4], *it);

        a.insert(b.find(stu[2]), b.end());  // 迭代器插入，含相同元素
        print_compare_stu(str[0] + "\n" + str[0] + "\n" + str[1] + "\n" + str[2] + "\n" + str[2]
            + "\n" + str[3] + "\n" + str[4], a);

        a.insert({ stu[5] });  // initializer_list 插入
        print_compare_stu(str[0] + "\n" + str[0] + "\n" + str[1] + "\n" + str[2] + "\n" + str[2]
            + "\n" + str[3] + "\n" + str[4] + "\n" + str[5], a);

        a.insert({ stu[5],stu[6] });  // initializer_list 插入，含相同元素
        print_compare_stu(str[0] + "\n" + str[0] + "\n" + str[1] + "\n" + str[2] + "\n" + str[2]
            + "\n" + str[3] + "\n" + str[4] + "\n" + str[5] + "\n" + str[5] + "\n" + str[6], a);

        // emplace 
        // 暂不要求

        // erase
        continue_key("erase");

        a.erase(a.find(stu[4]));
        print_compare_stu(str[0] + "\n" + str[0] + "\n" + str[1] + "\n" + str[2] + "\n" + str[2]
            + "\n" + str[3] + "\n" + str[5] + "\n" + str[5] + "\n" + str[6], a);

        a.erase(a.find(stu[0]));  //擦除重复元素
        print_compare_stu(str[0] + "\n" + str[1] + "\n" + str[2] + "\n" + str[2]
            + "\n" + str[3] + "\n" + str[5] + "\n" + str[5] + "\n" + str[6], a);

        a.erase(a.begin(), a.find(stu[2]));  //erase 迭代器
        print_compare_stu(str[2] + "\n" + str[2] + "\n" + str[3] + "\n" + str[5] + "\n" + str[5] + "\n" + str[6], a);

        a.erase(a.find(stu[2]), a.find(stu[2]));  //区间重合（区间首尾为重复元素）
        print_compare_stu(str[2] + "\n" + str[2] + "\n" + str[3] + "\n" + str[5] + "\n" + str[5] + "\n" + str[6], a);

        a.erase(a.find(stu[2]), a.find(stu[3]));  //区间重合（区间包含重复元素）
        print_compare_stu(str[3] + "\n" + str[5] + "\n" + str[5] + "\n" + str[6], a);

        a.erase(stu[5]);   //erase 重复的值
        print_compare_stu(str[3] + "\n" + str[6], a);

        a.erase({ 1859999, "Mars", 'm', (float)(100), "火星" });  //erase 不存在的值
        print_compare_stu(str[3] + "\n" + str[6], a);

        // swap 
        continue_key("swap");

        a.swap(b);
        print_compare_stu(str[1] + "\n" + str[2] + "\n" + str[3], a);
        print_compare_stu(str[3] + "\n" + str[6], b);

        // extract 
        // 暂不要求（c++17）

        // merge
        // 暂不要求（c++17）
    }
    if (index_stu[2]) {  //查找：count/find/equal_range/lower_bound/upper_bound
        continue_key("第3组：查找");

        // count
        continue_key("count");

        multiset<student> a{ stu[0],stu[0],stu[1],stu[2],stu[3],stu[4] };
        print_compare("2", a.count(stu[0]));
        print_compare("0", a.count(stu[5]));

        // find 
        continue_key("find");
        auto it = a.find(stu[0]);
        if (it != a.end())
            print_compare("1", *it);
        else
            print_compare("1", "Not Found");

        it = a.find({ 1859999, "Mars", 'm', (float)(100), "火星" });
        if (it != a.end())
            print_compare("Not Found", *it);
        else
            print_compare("Not Found", "Not Found");

        // equal_range 
        continue_key("equal_range");
        auto res = a.equal_range(stu[0]);  // 值存在
        print_compare(str[0], *res.first);
        print_compare(str[1], *res.second);

        a.erase(stu[2]);
        res = a.equal_range(stu[2]);  // 值不存在
        print_compare(str[3], *res.first);
        print_compare(str[3], *res.second);

        // lower_bound 
        continue_key("lower_bound");
        it = a.lower_bound(stu[1]);
        print_compare(str[1], *it);

        it = a.lower_bound(stu[2]);
        print_compare(str[3], *it);

        // upper_bound
        continue_key("upper_bound");
        it = a.upper_bound(stu[3]);
        print_compare(str[4], *it);

        it = a.upper_bound(stu[2]);
        print_compare(str[3], *it);
    }
    if (index_stu[3]) {  //比较运算符：==/!=/>/>=/</<=
        continue_key("第4组：比较运算符");

        multiset<student> a{ stu[0],stu[0],stu[1],stu[2] },
            b{ stu[3],stu[4],stu[5],stu[6] },
            c{ stu[0],stu[1],stu[2] };

        // ==
        continue_key("==");

        print_compare("false", a == b ? "true" : "false");  //不相等set
        print_compare("false", a == c ? "true" : "false");  //相等set

        // != 
        continue_key("!=");

        print_compare("true", a != b ? "true" : "false");  //不相等set
        print_compare("true", a != c ? "true" : "false");  //相等set

        // > 
        continue_key(">");

        print_compare("false", a > b ? "true" : "false");  //不相等set
        print_compare("false", a > c ? "true" : "false");  //相等set

        // >=
        continue_key(">=");

        print_compare("false", a >= b ? "true" : "false");  //不相等set
        print_compare("false", a >= c ? "true" : "false");  //相等set

        // < 
        continue_key("<");

        print_compare("true", a < b ? "true" : "false");  //不相等set
        print_compare("true", a < c ? "true" : "false");  //相等set

        // <=
        continue_key("<=");

        print_compare("true", a <= b ? "true" : "false");  //相等set
        print_compare("true", a <= c ? "true" : "false");  //不相等set
    }
#endif
}

// 任意键继续
void continue_key(std::string prompt, int num)
{
    std::cout << std::endl << "开始" << prompt << "测试，按任意键继续...";
    if (0) {
        for (int i = 0; i < num; i++)
            char c = getchar();
    }
    std::cout << std::endl;
}

// 打印标准输出与实际输出的比较（集合）
template <class K, class C>
void print_compare(std::string std_out, multiset<K, C>& s)
{
    std::cout << "应输出：" << std_out << std::endl;
    std::cout << "实际是：";
    if (s.size() != 0) {
        for (auto& ele : s)
            std::cout << ele << " ";
    }
    else {
        std::cout << "<None>";
    }
    std::cout << "\n" << std::endl;
}

// 打印标准输出与实际输出的比较（student）
template <class K, class C>
void print_compare_stu(std::string std_out, multiset<K, C>& s)
{
    std::cout << "应输出：\n" << std_out << std::endl;
    std::cout << "实际是：\n";
    if (s.size() != 0) {
        for (auto& ele : s)
            std::cout << ele << std::endl;
    }
    else {
        std::cout << "<None>";
    }
    std::cout << std::endl;
}

// 打印标准输出与实际输出的比较（元素）
template <class T>
void print_compare(std::string std_out, T ele)
{
    std::cout << "应输出：" << std_out << std::endl;
    std::cout << "实际是：" << ele << "\n" << std::endl;
}
