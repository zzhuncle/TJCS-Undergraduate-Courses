#include<cstdio>
#include<iostream>
#include<fstream>
#include<iomanip>
#include <algorithm> //for find(vector)
#include <sstream> //for istringstream

#include"AB_agent.h"
#include"common.h"
#include"ncn.h"
#include"tools.h"
using namespace std;
/**
* @file			test.cpp
* @brief        测试ncn和棋盘转化
* @param[in]    Ncn串
* @param[out]   打印棋盘状态，没有子的位置为0，有子的位置显示内部数组的值，行营用*标出
* @author       ssc
* @date			2020.04.18
* @version		0.0.1
*/
void test_ncn(const char s[])
{
	array<array<int, 5>, 13> board = {};
	int a = 0, b = 0;
	parse_ncn(board, s, a, b);
	LBChess t(board);
	t.seetheboard();
}

/**
* @file			test.cpp
* @brief        测试棋子可以达到的位置
* @param[in]    Ncn串
* @param[out]   打印棋盘状态，棋子现在所在的位置用*标出，可以走的位置置1，不能到的位置置0
* @author       ssc
* @date			2020.04.20
* @version		0.0.1
*/
void test_move(const int row, const int col, const char s[])
{
	vector<int>res;
	array<array<int, 5>, 13> board = {};
	int a = 0, b = 0;
	parse_ncn(board, s, a, b);//输入ncn，转化成棋盘
	LBChess t(board);
	t.seetheboard();//显示棋局样式
	cout << endl << endl;
	t.get(position2int(row, col), res);//得到可以到的位置，存入res
	char r[13][5];
	int i, j;
	for (i = 0; i < 13; i++)
		for (j = 0; j < 5; j++)
			r[i][j] = '0';
	r[row][col] = '*';//设置一个二维数组，初始全是0，然后当前位置置*

	for (int i = 0; i < res.size(); i++)
		r[res[i] / 5][res[i] % 5] = '1';//返回的vector中对应的位置，在二维数组中置1
	//打印二维数组
	for (i = 0; i < 13; i++) {
		for (j = 0; j < 5; j++)
			cout << r[i][j] << "  ";
		cout << endl;
	}
}

const int NCN_MAXSIZE = 100;	//ncn串最大长度

static inline void get_option(char optchar[], string& optstr)
{
	cin.getline(optchar, NCN_MAXSIZE);	//接受来自界面的指令
	cin.clear();						//good置1
	cin.sync();							//清空缓存区
	optstr = string(optchar);			//转成string类
}

static inline void post_option(const string opt)
{
	cout << opt << endl;			//向界面发送指令，endl自动清空缓存区
}

static inline void parse_option(const string opt, string& ncn, int& cnt, int& total_cnt)
{
	istringstream iss(opt);
	iss >> ncn >> cnt >> total_cnt; //将opt分割 xxx/.../xxx/ 0 1
}

static inline void print_log(ofstream& fout, string movement, array<array<int, 5>, 13> board)
{
	//输出当前棋盘到日志文件
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 5; j++) {
			fout << board[i][j] << " ";
		}
		fout << endl;
	}
	fout << endl;
	//输出移动指令到日志文件
	fout << movement << endl;
}

int time_limitation = (60 - 3) * 1000;

void test_agent(int role, int time)
{
	ofstream fout("time_per_step.txt", ios::app);
	time_limitation = (time - 3) * 1000;

	array<array<int, 5>, 13> board = { 0 }; //棋盘，初始值=全0
	char buf[NCN_MAXSIZE]; //存指令的字符数组
	string opt, ncn; //整条指令、ncn串 
	int cnt = 0, total_cnt = 0; //距离上一次吃子步数、总步数

	post_option("ready"); //向界面发送ready信息

	if (role == 1) { //黑方特殊处理，会提前接受到初始棋盘的指令
		get_option(buf, opt); //获取初始棋局
		//opt.replace(0, 5, ""); //去掉开头的"init"
		//parse_option(opt, ncn, cnt, total_cnt); //讲命令解析成ncn串和步数
	}

	get_option(buf, opt); //获取初始棋局
	array<array<int, 5>, 13> last_board2 = { 0 };
	array<array<int, 5>, 13> last_board1 = { 0 };
	bool flag;
	while (opt != "end") { //若接受到棋局结束的命令，则退出
	 //parse_option(opt, ncn, cnt, total_cnt); //将命令解析成ncn串和步数
	 //board = zero_board; //初始化board=全0，会影响效率，待改进
		parse_ncn(board, opt, cnt, total_cnt); //更新棋盘
		// Node* node = t.UCTSearch(role, board, (total_cnt / 2 == 0 ? true : false)); //UCT搜索最佳走子方案
		//post_option(node->get_move()); //向界面发送移动指令
		flag = (last_board2 == board);
		last_board2 = last_board1;
		last_board1 = board;
		AB_agent ab_agent(role, board);
		int startTime = clock();
		string sss = ab_agent.runable(flag);
		fout << clock() - startTime << endl;
		post_option(sss);
		get_option(buf, opt); //接受来自界面的指令

#if 0 //将此处改为1则输出日志文件
  //这一回合的棋盘状态（未移动前）、移动
		print_log(fout, sss, board);
#endif
	}

	fout.close();
}

#if 1
int main(int args, char* argv[])
{
	vector<string> argpaser; //参数解析器
	int role = 1; //参数role：-1表示红方、1表示黑方，默认为黑方1
	int time = 60; //参数time：表示单步决策时间，单位s

	// 将所有参数装入解析器
	for (int i = 0; i < args; i++) {
		string arg(argv[i]);
		argpaser.push_back(arg);
	}

	auto it = find(argpaser.begin(), argpaser.end(), "--role");
	if (it != argpaser.end())
		role = stoi(*(it + 1));

	it = find(argpaser.begin(), argpaser.end(), "--time");
	if (it != argpaser.end())
		time = stoi(*(it + 1));

	string inti_ncn = "ppdfd/plldz/z1g1y/yt1ty/s1g1s/jvgva/5/jvgva/s1g1s/yt1ty/z1g1y/plldz/ppdfd";

	//test_move(2, 1, "5/1pg2/1gg11/212/s1g11/jvg11/5/jvgva/s1g1s/yt1ty/z1g1y/plldz/ppdfd");
	test_agent(role, time);
	return 0;
}
#else
int main()
{
	string init_ncn = "init 2dfd/3d1/p4/5/5/2a2/5/5/A4/2Y2/2L1Z/3D1/PLDFD 0 0";
	init_ncn.replace(0, 5, "");
	cout << init_ncn << endl;
	return 0;
	ofstream fout("log.txt", ios::out); //日志文件
	UCT t;

	string ncn = "2dfd/3d1/p4/5/5/2a2/5/5/A4/2Y2/2L1Z/3D1/PLDFD";
	int cnt = 0, total_cnt = 0;
	array<array<int, 5>, 13> board = { 0 };
	parse_ncn(board, ncn, cnt, total_cnt);
	Node* node = t.UCTSearch(-1, board, (total_cnt / 2 == 0 ? true : false));
	cout << node->get_move() << endl;

	//输出当前棋盘到日志文件
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 5; j++) {
			cout << board[i][j] << " ";
		}
		cout << endl;
	}
	fout.close();
	return 0;
}
#endif