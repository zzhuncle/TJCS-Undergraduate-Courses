
#include <iostream>
#include <string>
#include<set>
#include "tj_student.h"
#include "tj_multiset.cpp"
using namespace std;

/* 这句宏定义注释掉，就是系统set的测试，
   打开宏定义，则测试自己的 tj_set */
#define	multiset	tj_multiset


void press_key(int num, const char *prompt)
{
	int i;
	cout << endl << prompt << "，按任意键继续...";
	for (i = 0; i < num; i++)
		getchar();
	cout << endl;
}

string tmp = "test";
int main()
{
	//int
	if (1)
	{
		press_key(1, "\n key=int,value=int测试：\n");
		int test_int[] = { 1,1,1,1,1,1,1,1 }; //每个1对应1组if测试
		// 第1组测试 
		if (test_int[0]) {
			press_key(1, "第1组测试：\n size()和empty()函数");
			cout << "创建空map" << endl;
			multiset<int>m1;
			cout << "empty()函数: " << endl;
			cout << "应输出：1" << endl;
			cout << "实际是：" << m1.empty() << endl;
			cout << "size()函数: " << endl;
			cout << "应输出：0" << endl;
			cout << "实际是：" << m1.size() << endl;
		}
		if (test_int[1]) {
			press_key(1, "第2组测试：\n 几种输入方式");
			multiset<int>m1;
			cout << "map中赋值" << endl;
			m1.insert(1);
			cout << "insert(   int   )方式插入成功" << endl;
			cout << "empty()函数: " << endl;
			cout << "应输出：0" << endl;
			cout << "实际是：" << m1.empty() << endl;
			cout << "size()函数: " << endl;
			cout << "应输出：1" << endl;
			cout << "实际是：" << m1.size() << endl;

			m1.emplace(2);
			cout << "emplace方式插入成功" << endl;
			cout << "empty()函数: " << endl;
			cout << "应输出：0" << endl;
			cout << "实际是：" << m1.empty() << endl;
			cout << "size()函数: " << endl;
			cout << "应输出：2" << endl;
			cout << "实际是：" << m1.size() << endl;

			for (auto p = m1.begin(); p != m1.end(); p++) {
				cout << (*p) << endl;
			}

			cout << endl << "复制" << endl;
			cout << "应输出：1 2" << endl;
			cout << "实际是：";
			multiset<int>m2 = m1;
			for (auto p = m1.begin(); p != m1.end(); p++) {
				cout << (*p) << " ";
			}

			cout << endl << "赋值" << endl;
			multiset<int>m3;
			m3 = m1;
			cout << "应输出：1 2" << endl;
			cout << "实际是：";
			for (auto p = m1.begin(); p != m1.end(); p++) {
				cout << (*p) << " ";
			}
			cout << endl;
		}
		// 第2组测试 
		if (test_int[2]) {
			press_key(1, "第3组测试：\n begin/end/及rbegin/rend/函数");

			multiset<int>m1;
			for (int i = 1; i <= 5; i++)
				m1.insert(i);
			cout << "遍历(正向): " << endl;
			cout << "应输出：1 2 3 4 5" << endl;
			cout << "实际是：";
			for (auto it = m1.begin(); it != m1.end(); it++) {
				cout << (*it) << " ";
			}
			cout << endl;

			cout << endl;

			cout << "遍历(反向): " << endl;
			cout << "应输出：5 4 3 2 1 " << endl;
			cout << "实际是：";
			for (auto it = m1.rbegin(); it != m1.rend(); ++it) {
				cout << (*it) << " ";
			}
			cout << endl;

			cout << endl;

			multiset<int>m2;
			cout << "测试当map大小为0时begin和end是否相等" << endl;
			cout << "应输出：1" << endl;
			cout << "实际是：" << (m2.begin() == m2.end()) << endl;

			cout << endl;

			cout << "测试当map大小为0时rbegin和rend是否相等" << endl;
			cout << "应输出：1" << endl;
			cout << "实际是：" << (m2.rbegin() == m2.rend());

			cout << endl << endl;
		}

		// 第4组测试 
		if (test_int[3]) {
			press_key(1, "第4组测试：\n erase和insert");
			cout << "insert的测试";
			multiset<int>m1;
			for (int i = 1; i <= 5; i++)
				m1.emplace(i);
			multiset<int>m2;
			for (int i = 5; i <= 10; i++)
				m2.insert(i);
			auto q = m2.begin();
			q++; q++;
			m1.insert(m2.begin(), q);
			cout << "结果应该是:\n1 2 3 4 5 5 6 " << endl << "实际是: \n";
			for (auto p = m1.begin(); p != m1.end(); p++)
				cout << (*p) << " ";
			cout << endl;
			cout << endl;

			cout << "erase的测试";
			q = m1.end();
			q = m1.erase(--q);
			cout << "测试erase的返回值 ，结果应该是 1,  " << "实际是: ";
			cout << (q == m1.end()) << endl;
			cout << "删除一个元素" << endl;
			cout << "结果应该是:\n1 2 3 4 5 5 " << endl << "实际是: \n";
			for (auto p = m1.begin(); p != m1.end(); p++)
				cout << (*p) << " ";
			cout << endl;
			cout << endl;
			q = m1.begin();
			q++;
			auto q_e = m1.end();
			m1.erase(q, q_e);
			cout << "删除多个元素" << endl;
			cout << "结果应该是: 1" << endl << "实际是: ";
			for (auto p = m1.begin(); p != m1.end(); p++)
				cout << (*p) << "  ";
			cout << endl;
		}

		// 第5组测试 
		if (test_int[4]) {
			press_key(1, "第5组测试：\n find()/lower_bound()/upper_bound()/count()");
			multiset<int>m1;
			for (int i = 1; i <= 5; i++)
				m1.emplace(i);
			for (int i = 1; i <= 3; i++)
				m1.emplace(2);
			auto n1 = m1.find(2);
			cout << "find()的测试,应该是 2 ，实际是";
			cout << (*n1) << endl;
			auto n2 = m1.lower_bound(2);
			cout << "lower_bound()的测试,应该是 2 ，实际是 ";
			cout << (*n2) << endl;
			n2 = m1.upper_bound(2);
			cout << "upper_bound()的测试,应该是3 ，实际是";
			cout << (*n2) << endl;

			cout << "count()的测试,应该是4，实际是";
			cout << m1.count(2) << endl;
			cout << "count()的测试,应该是0，实际是";
			cout << m1.count(16) << endl;
			cout << endl << "find失败的情况" << endl;

			auto p1 = m1.find(100);
			cout << "find()找不存在的值的测试,应该1，实际是  ";
			cout << (p1 == m1.end()) << endl;
			auto p2 = m1.lower_bound(100);
			cout << "lower_bound()找不存在的值的测试,应该1，实际是  ";
			cout << (p2 == m1.end()) << endl;
			auto p3 = m1.upper_bound(100);
			cout << "upper_bound()找不存在的值的测试,应该1，实际是  ";
			cout << (p3 == m1.end()) << endl;
		}

		// 第6组测试 
		if (test_int[5]) {
			press_key(1, "第6组测试：\n key_comp");
			multiset<int>m1;
			bool r;
			r = m1.key_comp()(3, 5);
			cout << "key_comp()的测试，结果应该是：1，实际是";
			cout << r << endl;
			r = m1.key_comp()(3, 3);
			cout << "key_comp()的测试，结果应该是：0，实际是";
			cout << r << endl;
			r = m1.key_comp()(5, 3);
			cout << "key_comp()的测试，结果应该是：0，实际是";
			cout << r << endl;
		}

		// 第7组测试 
		if (test_int[6]) {

			press_key(1, "第7组测试：\n swap()  ");
			multiset<int>m1;
			multiset<int>m2;
			for (int i = 1; i <= 3; i++)
				m1.emplace(i);
			for (int i = 8; i <= 9; i++)
				m2.emplace(i);

			cout << "swap()的测试" << endl;
			m1.swap(m2);
			cout << "m1 应该是 8 9，实际是";
			for (auto p = m1.begin(); p != m1.end(); p++)
				cout << (*p) << "  ";
			cout << endl;
			cout << "m2 应该是1 2 3，实际是";
			for (auto p = m2.begin(); p != m2.end(); p++)
				cout << (*p) << "  ";
			cout << endl;
			cout << "再次交换的测试" << endl;
			m1.swap(m2);
			cout << "m1 应该是1 2 3，实际是";
			for (auto p = m1.begin(); p != m1.end(); p++)
				cout << (*p) << "  ";
			cout << endl;
			cout << "m2 应该是8 9，实际是";
			for (auto p = m2.begin(); p != m2.end(); p++)
				cout << (*p) << "  ";
			cout << endl;
		}
		// 第8组测试 
		if (test_int[7]) {

			press_key(1, "第8组测试：重载测试,clear测试 ");
			multiset<int>m1;
			multiset<int>m2;
			for (int i = 1; i <= 5; i++)
				m1.emplace(i);
			for (int i = 1; i <= 3; i++)
				m2.emplace(i);
			for (int i = 8; i <= 12; i++)
				m2.emplace(i);
			cout << "<号测试" << "应输出  1，实际输出" << (m1 < m2) << endl;
			cout << ">号测试" << "应输出  0，实际输出" << (m1 > m2) << endl;
			cout << "=号测试" << "应输出  0，实际输出" << (m1 == m2) << endl;

			m1.clear();
			m2.clear();
			cout << "clear m1,应输出 1，实际输出" << m1.empty() << endl;
			cout << "clear m2,应输出 1，实际输出" << m2.empty() << endl;
			cout << "<号测试" << "应输出  0，实际输出" << (m1 < m2) << endl;
			cout << ">号测试" << "应输出  0，实际输出" << (m1 > m2) << endl;
			cout << "=号测试" << "应输出  1，实际输出" << (m1 == m2) << endl;
		}
	}
	//string
	if (1) {
		press_key(1, "\n string：\n");
		int test_int[] = { 1,1,1,1,1,1,1,1,1 }; //每个1对应1组if测试
		// 第1组测试 
		if (test_int[0]) {
			press_key(1, "第1组测试：\n size()和empty()函数");
			cout << "创建空map" << endl;
			multiset<string>m1;
			cout << "empty()函数: " << endl;
			cout << "应输出：1" << endl;
			cout << "实际是：" << m1.empty() << endl;
			cout << "size()函数: " << endl;
			cout << "应输出：0" << endl;
			cout << "实际是：" << m1.size() << endl;
		}
		if (test_int[1]) {
			press_key(1, "第2组测试：\n 几种输入方式");
			multiset<string>m1;
			cout << "map中赋值" << endl;
			m1.insert("test1");
			cout << "insert(   string   )方式插入成功" << endl;
			cout << "empty()函数: " << endl;
			cout << "应输出：0" << endl;
			cout << "实际是：" << m1.empty() << endl;
			cout << "size()函数: " << endl;
			cout << "应输出：1" << endl;
			cout << "实际是：" << m1.size() << endl;

			m1.emplace("test2");
			cout << "emplace方式插入成功" << endl;
			cout << "empty()函数: " << endl;
			cout << "应输出：0" << endl;
			cout << "实际是：" << m1.empty() << endl;
			cout << "size()函数: " << endl;
			cout << "应输出：2" << endl;
			cout << "实际是：" << m1.size() << endl;

			for (auto p = m1.begin(); p != m1.end(); p++) {
				cout << (*p) << endl;
			}

			cout << endl << "复制" << endl;
			cout << "应输出：test1 test2" << endl;
			cout << "实际是：";
			multiset<string>m2 = m1;
			for (auto p = m1.begin(); p != m1.end(); p++) {
				cout << (*p) << " ";
			}

			cout << endl << "赋值" << endl;
			multiset<string>m3;
			m3 = m1;
			cout << "应输出：test1 test2" << endl;
			cout << "实际是：";
			for (auto p = m1.begin(); p != m1.end(); p++) {
				cout << (*p) << " ";
			}
			cout << endl;
		}
		// 第2组测试 
		if (test_int[2]) {
			press_key(1, "第3组测试：\n begin/end/及rbegin/rend/函数");
			multiset<string>m1;
			for (int i = 1; i <= 5; i++)
				m1.emplace(tmp + char('0' + i));
			cout << "遍历(正向): " << endl;
			cout << "应输出：test1 test2 test3 test4 test5" << endl;
			cout << "实际是：";
			for (auto it = m1.begin(); it != m1.end(); it++) {
				cout << (*it) << " ";
			}
			cout << endl;

			cout << endl;




			cout << "遍历(反向): " << endl;
			cout << "应输出：test5 test4 test3 test2 test1" << endl;
			cout << "实际是：";
			for (auto it = m1.rbegin(); it != m1.rend(); ++it) {
				cout << (*it) << " ";
			}
			cout << endl;

			cout << endl;

			multiset<int>m2;
			cout << "测试当map大小为0时begin和end是否相等" << endl;
			cout << "应输出：1" << endl;
			cout << "实际是：" << (m2.begin() == m2.end()) << endl;

			cout << endl;

			cout << "测试当map大小为0时rbegin和rend是否相等" << endl;
			cout << "应输出：1" << endl;
			cout << "实际是：" << (m2.rbegin() == m2.rend());

			cout << endl << endl;
		}

		// 第4组测试 
		if (test_int[3]) {
			press_key(1, "第4组测试：\n erase和insert");
			cout << "insert的测试";
			multiset<string>m1;
			for (int i = 1; i <= 5; i++)
				m1.emplace(tmp + char('0' + i));
			multiset<string>m2;
			for (int i = 5; i <= 9; i++)
				m2.insert(tmp + char('0' + i));
			auto q = m2.begin();
			q++; q++;
			m1.insert(m2.begin(), q);
			cout << "结果应该是:\ntest1 test2 test3 test4 test5 test5 test6 " << endl << "实际是: \n";
			for (auto p = m1.begin(); p != m1.end(); p++)
				cout << (*p) << " ";
			cout << endl;
			cout << endl;

			cout << "erase的测试";
			q = m1.end();
			q = m1.erase(--q);
			cout << "测试erase的返回值 ，结果应该是 1,  " << "实际是: ";
			cout << (q == m1.end()) << endl;
			cout << "删除一个元素" << endl;
			cout << "结果应该是:\n test1 test2 test3 test4 test5 test5 " << endl << "实际是: \n";
			for (auto p = m1.begin(); p != m1.end(); p++)
				cout << (*p) << " ";
			cout << endl;
			cout << endl;
			q = m1.begin();
			q++;
			auto q_e = m1.end();
			m1.erase(q, q_e);
			cout << "删除多个元素" << endl;
			cout << "结果应该是: test1" << endl << "实际是: ";
			for (auto p = m1.begin(); p != m1.end(); p++)
				cout << (*p) << "  ";
			cout << endl;
		}

		// 第5组测试 
		if (test_int[4]) {
			press_key(1, "第5组测试：\n find()/lower_bound()/upper_bound()/count()");
			multiset<string>m1;
			for (int i = 1; i <= 5; i++)
				m1.emplace(tmp + char('0' + i));
			for (int i = 1; i <= 3; i++)
				m1.emplace("test2");
			auto n1 = m1.find("test2");
			cout << "find()的测试,应该是test2，实际是";
			cout << (*n1) << endl;
			auto n2 = m1.lower_bound("test2");
			cout << "lower_bound()的测试,应该是test2，实际是";
			cout << (*n2) << endl;
			n2 = m1.upper_bound("test2");
			cout << "upper_bound()的测试,应该是test3，实际是";
			cout << (*n2) << endl;

			cout << "count()的测试,应该是4，实际是";
			cout << m1.count("test2") << endl;

			cout << endl << "find失败的情况" << endl;

			auto p1 = m1.find("test200");
			cout << "find()找不存在的值的测试,应该1，实际是  ";
			cout << (p1 == m1.end()) << endl;
			auto p2 = m1.lower_bound("test200");
			cout << "lower_bound()找不存在的值的测试,应该0，实际是  ";
			cout << (p2 == m1.end()) << endl;
			auto p3 = m1.upper_bound("test200");
			cout << "upper_bound()找不存在的值的测试,应该0，实际是  ";
			cout << (p3 == m1.end()) << endl;
		}

		// 第6组测试 
		if (test_int[5]) {
			press_key(1, "第6组测试：\n key_comp");
			multiset<string>m1;
			bool r;
			r = m1.key_comp()("test200", "test300");
			cout << "key_comp()的测试，结果应该是：1，实际是";
			cout << r << endl;
			r = m1.key_comp()("test300", "test300");
			cout << "key_comp()的测试，结果应该是：0，实际是";
			cout << r << endl;
			r = m1.key_comp()("test300", "test200");
			cout << "key_comp()的测试，结果应该是：0，实际是";
			cout << r << endl;
		}

		// 第7组测试 
		if (test_int[6]) {

			press_key(1, "第7组测试：\n swap()  ");
			multiset<string>m1;
			multiset<string>m2;
			for (int i = 1; i <= 3; i++)
				m1.emplace(tmp + char('0' + i));
			for (int i = 8; i <= 9; i++)
				m2.emplace(tmp + char('0' + i));

			cout << "swap()的测试" << endl;
			m1.swap(m2);
			cout << "m1 应该是 test8 test9，实际是";
			for (auto p = m1.begin(); p != m1.end(); p++)
				cout << (*p) << "  ";
			cout << endl;
			cout << "m2 应该是test1 test2 test3，实际是";
			for (auto p = m2.begin(); p != m2.end(); p++)
				cout << (*p) << "  ";
			cout << endl;
			cout << "再次交换的测试" << endl;
			m1.swap(m2);
			cout << "m1 应该是test1 test2 test3，实际是";
			for (auto p = m1.begin(); p != m1.end(); p++)
				cout << (*p) << "  ";
			cout << endl;
			cout << "m2 应该是test8 test9，实际是";
			for (auto p = m2.begin(); p != m2.end(); p++)
				cout << (*p) << "  ";
			cout << endl;
		}
		// 第8组测试 
		if (test_int[7]) {

			press_key(1, "第8组测试：重载测试,clear测试 ");
			multiset<string>m1;
			multiset<string>m2;
			for (int i = 1; i <= 5; i++)
				m1.emplace(tmp + char('0' + i));
			for (int i = 1; i <= 3; i++)
				m2.emplace(tmp + char('0' + i));
			for (int i = 8; i <= 9; i++)
				m2.emplace(tmp + char('0' + i));
			cout << "<号测试" << "应输出  1，实际输出" << (m1 < m2) << endl;
			cout << ">号测试" << "应输出  0，实际输出" << (m1 > m2) << endl;
			cout << "=号测试" << "应输出  0，实际输出" << (m1 == m2) << endl;

			m1.clear();
			m2.clear();
			cout << "clear m1,应输出 1，实际输出" << m1.empty() << endl;
			cout << "clear m2,应输出 1，实际输出" << m2.empty() << endl;
			cout << "<号测试" << "应输出  0，实际输出" << (m1 < m2) << endl;
			cout << ">号测试" << "应输出  0，实际输出" << (m1 > m2) << endl;
			cout << "=号测试" << "应输出  1，实际输出" << (m1 == m2) << endl;
		}

		// 第9组测试 
		if (test_int[9])
		{
			press_key(1, "第5组测试：相同键的测试 ");
			multiset<string>m1;
			for (int i = 1; i <= 5; i++)
				m1.emplace(tmp + char('1'));
			cout << "应输出：" << endl;
			cout << "test1" << endl;
			cout << "test1" << endl;
			cout << "test1" << endl;
			cout << "test1" << endl;
			cout << "test1" << endl;
			cout << "实际是：" << endl;
			for (auto p = m1.begin(); p != m1.end(); p++)
				cout << (*p) << endl;
			cout << endl;
			cout << endl;
		}
	}

	if (1) {
		press_key(1, "\n student测试：\n");
		int test_int[] = { 1,1,1,1,1 }; //每个1对应1组if测试
		student t[4] = { {1001,"t1",'M',81,"test1" },{1002,"t2",'F',82,"test2" },
								{1003,"t3",'M',83,"test3" } ,{1004,"t4",'F',84,"test4" } };

		// 第1组测试 
		if (test_int[0]) {
			press_key(1, "第1组测试：\n size()和empty()函数");
			cout << "创建空map" << endl;
			multiset<student>m1;
			cout << "empty()函数: " << endl;
			cout << "应输出：1" << endl;
			cout << "实际是：" << m1.empty() << endl;
			cout << "size()函数: " << endl;
			cout << "应输出：0" << endl;
			cout << "实际是：" << m1.size() << endl;
		}
		if (test_int[1]) {
			press_key(1, "第2组测试：\n 几种输入方式");
			multiset<student>m1;
			cout << "map中赋值" << endl;
			m1.insert(t[0]);
			cout << "insert(   student   )方式插入成功" << endl;
			cout << "empty()函数: " << endl;
			cout << "应输出：0" << endl;
			cout << "实际是：" << m1.empty() << endl;
			cout << "size()函数: " << endl;
			cout << "应输出：1" << endl;
			cout << "实际是：" << m1.size() << endl;

			m1.emplace(t[1]);
			cout << "emplace方式插入成功" << endl;
			cout << "empty()函数: " << endl;
			cout << "应输出：0" << endl;
			cout << "实际是：" << m1.empty() << endl;
			cout << "size()函数: " << endl;
			cout << "应输出：2" << endl;
			cout << "实际是：" << m1.size() << endl;

			for (auto p = m1.begin(); p != m1.end(); p++) {
				cout << (*p) << endl;
			}

			cout << endl << "复制" << endl;
			cout << "应输出：1001 t1 M 81 test1        1002 t2 F 82 test2" << endl;
			cout << "实际是：";
			multiset<student>m2 = m1;
			for (auto p = m1.begin(); p != m1.end(); p++) {
				cout << (*p) << "        ";
			}

			cout << endl << "赋值" << endl;
			multiset<student>m3;
			m3 = m1;
			cout << "应输出：1001 t1 M 81 test1        1002 t2 F 82 test2" << endl;
			cout << "实际是：";
			for (auto p = m1.begin(); p != m1.end(); p++) {
				cout << (*p) << "        ";
			}
			cout << endl;
		}
		// 第2组测试 
		if (test_int[2]) {
			press_key(1, "第3组测试：\n begin/end/及rbegin/rend/函数");
			multiset<student>m1;
			for (int i = 2; i >= 0; i--)
				m1.emplace(t[i]);
			cout << "遍历(正向): " << endl;
			cout << "应输出：\n" << endl;
			cout << "1001 t1 M 81 test1" << endl;
			cout << "1002 t2 F 82 test2" << endl;
			cout << "1003 t3 M 83 test3" << endl;
			cout << "实际是：\n";
			for (auto it = m1.begin(); it != m1.end(); it++) {
				cout << (*it) << endl;
			}
			cout << endl;

			cout << endl;

			cout << "遍历(反向): " << endl;
			cout << "应输出：\n" << endl;
			cout << "1003 t3 M 83 test3" << endl;
			cout << "1002 t2 F 82 test2" << endl;
			cout << "1001 t1 M 81 test1" << endl;

			cout << "实际是：\n";
			for (auto it = m1.rbegin(); it != m1.rend(); ++it) {
				cout << (*it) << endl;
			}
			cout << endl;

			cout << endl;

			multiset<student> m2;
			cout << "测试当map大小为0时begin和end是否相等" << endl;
			cout << "应输出：1" << endl;
			cout << "实际是：" << (m2.begin() == m2.end()) << endl;

			cout << endl;

			cout << "测试当map大小为0时rbegin和rend是否相等" << endl;
			cout << "应输出：1" << endl;
			cout << "实际是：" << (m2.rbegin() == m2.rend());

			cout << endl << endl;
		}

		// 第4组测试 
		if (test_int[3]) {
			press_key(1, "第4组测试：\n erase和insert");
			cout << "insert的测试";
			multiset<student>m1;
			for (int i = 0; i <= 1; i++)
				m1.emplace(t[i]);
			multiset<student>m2;
			for (int i = 0; i <= 3; i++)
				m2.insert(t[i]);
			auto q = m2.begin();
			q++; q++;
			m1.insert(m2.begin(), q);
			cout << "应输出：" << endl;
			cout << "1001 t1 M 81 test1" << endl;
			cout << "1001 t1 M 81 test1" << endl;
			cout << "1002 t2 F 82 test2" << endl;
			cout << "1002 t2 F 82 test2" << endl;
			cout << "实际是：" << endl;
			for (auto p = m1.begin(); p != m1.end(); p++)
				cout << (*p) << endl;
			cout << endl;
			cout << endl;

			cout << "erase的测试";
			q = m1.end();
			q = m1.erase(--q);
			cout << "测试erase的返回值 ，结果应该是 1,  " << "实际是: ";
			cout << (q == m1.end()) << endl;
			cout << "删除一个元素" << endl;
			cout << "应输出：" << endl;
			cout << "1001 t1 M 81 test1" << endl;
			cout << "1001 t1 M 81 test1" << endl;
			cout << "1002 t2 F 82 test2" << endl;
			cout << "实际是：" << endl;
			for (auto p = m1.begin(); p != m1.end(); p++)
				cout << (*p) << endl;
			cout << endl;
			cout << endl;
		}

		// 第5组测试 
		if (test_int[4]) {

			press_key(1, "第5组测试：相同键的测试 ");
			multiset<student>m1;
			for (int i = 0; i <= 3; i++)
				m1.emplace(t[0]);
			cout << "应输出：" << endl;
			cout << "1001 t1 M 81 test1" << endl;
			cout << "1001 t1 M 81 test1" << endl;
			cout << "1001 t1 M 81 test1" << endl;
			cout << "1001 t1 M 81 test1" << endl;
			cout << "实际是：" << endl;
			for (auto p = m1.begin(); p != m1.end(); p++)
				cout << (*p) << endl;
			cout << endl;
			cout << endl;

		}
	}
	return 0;
}
