#include <iostream>
#include <set>
#include <string>
#include <initializer_list>
#include "tj_multiset.cpp"
#include "tj_student.h"
using namespace std;


//测试声明：
//1.测试的数据类型为int、string和student(6种关系运算符均定义过)，三种数据类型下的测试内容完全相同，仅改变了数据类型
//2.测试覆盖了multiset除get_allcator和emplace_hint之外的全部函数、运算符，对于每个函数的重载情况也测试了绝大部分
//3.测试仅仅测试了正确的使用方式，没有对非法的错误使用进行测试
//4.如果测试有问题，请及时反馈


#define multiset tj_multiset
//这句宏定义注释掉，就是系统multiset的测试；打开宏定义，则测试自己的 tj_multiset


/* press key */
void wait_for_next()
{
	cout << endl << "请按回车键继续..." << endl;
	while (getchar() != '\n')
		;
}


int main()
{
	//int
	if (1)
	{
		int test[] = { 1,1,1,1,1,1,1 };

		//第0组测试：insert+迭代器测试
		if (test[0])
		{
			cout << "第0组测试：insert/begin/end/cbegin/cend/rbegin/rend/crbegin/crend" << endl << endl;

			multiset<int> myset1;
			initializer_list<int> L = { 1,2,3 };
			myset1.insert(L);
			myset1.insert(4);
			myset1.insert(3);
			myset1.insert(6);
			myset1.insert(5);
			myset1.insert(6);

			cout << "应输出：1 2 3 3 4 5 6 6" << endl;
			cout << "实际是：";
			for (auto it = myset1.begin(); it != myset1.end(); it++)
				cout << *it << " ";
			cout << endl << endl;


			cout << "应输出：1 2 3 3 4 5 6 6" << endl;
			cout << "实际是：";
			for (auto it = myset1.cbegin(); it != myset1.cend(); it++)
				cout << *it << " ";
			cout << endl << endl;


			cout << "应输出：6 6 5 4 3 3 2 1" << endl;
			cout << "实际是：";
			for (auto it = myset1.rbegin(); it != myset1.rend(); it++)
				cout << *it << " ";
			cout << endl << endl;


			cout << "应输出：6 6 5 4 3 3 2 1" << endl;
			cout << "实际是：";
			for (auto it = myset1.crbegin(); it != myset1.crend(); it++)
				cout << *it << " ";
			cout << endl << endl;


			auto first = myset1.begin();
			auto last = myset1.end();
			multiset<int> myset2(first, last);
			myset2.insert(7);
			myset2.insert(6);
			cout << "应输出：7 6 6 6 5 4 3 3 2 1" << endl;
			cout << "实际是：";
			for (auto it = myset2.crbegin(); it != myset2.crend(); it++)
				cout << *it << " ";
			cout << endl;

			wait_for_next();
		}

		//第1组测试：构造相关
		if (test[1])
		{
			cout << "第1组测试：构造函数/=重载" << endl << endl;


			initializer_list<int> L = { 1,3 };
			multiset<int> myset1(L);
			myset1.insert(2);
			cout << "应输出：1 2 3" << endl;
			cout << "实际是：";
			for (auto it = myset1.begin(); it != myset1.end(); it++)
				cout << *it << " ";
			cout << endl << endl;


			multiset<int> myset2(myset1);
			cout << "应输出：1 2 3" << endl;
			cout << "实际是：";
			for (auto it = myset2.begin(); it != myset2.end(); it++)
				cout << *it << " ";
			cout << endl << endl;


			multiset<int> myset3(myset1.begin(), myset1.end());
			cout << "应输出：1 2 3" << endl;
			cout << "实际是：";
			for (auto it = myset3.begin(); it != myset3.end(); it++)
				cout << *it << " ";
			cout << endl << endl;


			multiset<int> myset4 = myset1;
			cout << "应输出：1 2 3" << endl;
			cout << "实际是：";
			for (auto it = myset4.begin(); it != myset4.end(); it++)
				cout << *it << " ";
			cout << endl;

			wait_for_next();
		}

		//第2组测试：容量相关
		if (test[2])
		{
			cout << "第2组测试：size/max_size/empty/count" << endl << endl;


			multiset<int> myset;
			cout << "mymap为空，应输出：1" << endl;
			cout << "mymap为空，实际是：" << myset.empty();
			cout << endl << endl;


			myset.insert(4);
			myset.insert(3);
			myset.insert(2);
			myset.insert(1);
			myset.insert(3);
			myset.insert(5);
			cout << "mymap不为空，有6个元素，应输出：0 6" << endl;
			cout << "mymap不为空，有6个元素，实际是：" << myset.empty() << " " << myset.size();
			cout << endl << endl;


			cout << "max_size()，输出可以不同(214748364)" << endl;
			cout << "实际为：" << myset.max_size();
			cout << endl << endl;


			cout << "应输出：1 2 1 0 0 0" << endl;
			cout << "实际是：" << myset.count(1) << " " << myset.count(3) << " " << myset.count(5) << " ";
			cout << myset.count(33) << " " << myset.count(6) << " " << myset.count(11) << " ";
			cout << endl;

			wait_for_next();
		}

		//第3组测试：元素操作
		if (test[3])
		{
			cout << "第3组测试：insert/erase/emplace/find" << endl << endl;


			multiset<int> myset;
			myset.insert(1);
			myset.insert(2);
			myset.insert(2);
			myset.insert(3);
			myset.insert(4);
			myset.insert(5);
			myset.insert(5);
			cout << "应输出：1 2 2 3 4 5 5" << endl;
			cout << "实际是：";
			for (auto it = myset.begin(); it != myset.end(); it++)
				cout << *it << " ";
			cout << endl << endl;


			myset.erase(myset.begin());
			myset.erase(myset.begin(), ++myset.begin());
			cout << "应输出：2 3 4 5 5" << endl;
			cout << "实际是：";
			for (auto it = myset.begin(); it != myset.end(); it++)
				cout << *it << " ";
			cout << endl << endl;


			myset.erase(5);
			cout << "应输出：2 3 4" << endl;
			cout << "实际是：";
			for (auto it = myset.begin(); it != myset.end(); it++)
				cout << *it << " ";
			cout << endl << endl;


			myset.emplace(1);
			myset.emplace(2);
			myset.emplace(3);
			myset.emplace(6);
			cout << "应输出：1 2 2 3 3 4 6" << endl;
			cout << "实际是：";
			for (auto it = myset.begin(); it != myset.end(); it++)
				cout << *it << " ";
			cout << endl << endl;



			cout << "应输出：2 6" << endl;
			cout << "实际是：";
			auto it = myset.find(1);
			cout << *++it << " ";
			it = myset.find(10);
			cout << *--it << " ";
			cout << endl;

			wait_for_next();
		}

		//第4组测试：整体操作+范围操作
		if (test[4])
		{
			cout << "第4组测试：clear/swap/equal_range/upper_bound/lower_bound" << endl << endl;


			multiset<int> myset1;
			myset1.insert(1);
			myset1.insert(2);
			myset1.insert(3);
			multiset<int> myset2 = myset1;
			myset2.emplace(4);
			myset2.emplace(5);
			myset2.emplace(4);
			myset1.swap(myset2);
			cout << "应输出：6 3" << endl;
			cout << "实际是：";
			cout << myset1.size() << " " << myset2.size();
			cout << endl << endl;


			myset2.clear();
			myset1.swap(myset1);
			swap(myset1, myset2);
			cout << "应输出：0 6" << endl;
			cout << "实际是：";
			cout << myset1.size() << " " << myset2.size();
			cout << endl << endl;


			auto range = myset2.equal_range(4);
			cout << "应输出：4 5" << endl;
			cout << "实际是：";
			cout << *range.first << " " << *range.second;
			cout << endl << endl;


			auto lower = myset2.lower_bound(4);
			auto upper = myset2.upper_bound(4);
			cout << "应输出：4 5" << endl;
			cout << "实际是：";
			cout << *lower << " " << *upper;
			cout << endl;

			wait_for_next();
		}

		//第5组测试：其他函数
		if (test[5])
		{
			cout << "第5组测试：key_comp/value_comp" << endl << endl;


			multiset<int> mymap;
			auto key_compare = mymap.key_comp();
			cout << "应输出：0 1 1" << endl;
			cout << "实际是：";
			cout << key_compare(1, 1) << " " << key_compare(1, 2) << " " << key_compare(0, 1);
			cout << endl << endl;


			auto value_compare = mymap.value_comp();
			cout << "应输出：0 1 1" << endl;
			cout << "实际是：";
			cout << value_compare(1, 1) << " " << value_compare(1, 2) << " " << value_compare(0, 1);
			cout << endl;

			wait_for_next();
		}

		//第6组测试：关系运算符
		if (test[6])
		{
			cout << "第6组测试：6个关系运算符" << endl << endl;


			multiset<int> myset1;
			myset1.emplace(1);
			myset1.emplace(2);
			myset1.emplace(3);
			myset1.emplace(4);
			myset1.emplace(5);
			multiset<int> myset2 = myset1;
			cout << "应输出：1 0 1 1 0 0" << endl;
			cout << "实际是：";
			cout << (myset1 == myset2) << " " << (myset1 != myset2) << " " << (myset1 >= myset2) << " ";
			cout << (myset1 <= myset2) << " " << (myset1 < myset2) << " " << (myset1 > myset2) << " ";
			cout << endl << endl;


			myset2.erase(5);
			cout << "应输出：0 1 1 0 0 1" << endl;
			cout << "实际是：";
			cout << (myset1 == myset2) << " " << (myset1 != myset2) << " " << (myset1 >= myset2) << " ";
			cout << (myset1 <= myset2) << " " << (myset1 < myset2) << " " << (myset1 > myset2) << " ";
			cout << endl << endl;


			myset2.emplace(0);
			cout << "应输出：0 1 1 0 0 1" << endl;
			cout << "实际是：";
			cout << (myset1 == myset2) << " " << (myset1 != myset2) << " " << (myset1 >= myset2) << " ";
			cout << (myset1 <= myset2) << " " << (myset1 < myset2) << " " << (myset1 > myset2) << " ";
			cout << endl << endl;


			myset2.erase(0);
			myset2.emplace(6);
			cout << "应输出：0 1 0 1 1 0" << endl;
			cout << "实际是：";
			cout << (myset1 == myset2) << " " << (myset1 != myset2) << " " << (myset1 >= myset2) << " ";
			cout << (myset1 <= myset2) << " " << (myset1 < myset2) << " " << (myset1 > myset2) << " ";
			cout << endl;

			wait_for_next();
		}

	}

	//string
	if (1)
	{
		int test[] = { 1,1,1,1,1,1,1 };

		//第0组测试：insert+迭代器测试
		if (test[0])
		{
			cout << "第0组测试：insert/begin/end/cbegin/cend/rbegin/rend/crbegin/crend" << endl << endl;

			multiset<string> myset1;
			initializer_list<string> L = { "11","22","33" };
			myset1.insert(L);
			myset1.insert("44");
			myset1.insert("33");
			myset1.insert("66");
			myset1.insert("55");
			myset1.insert("66");

			cout << "应输出：11 22 33 33 44 55 66 66" << endl;
			cout << "实际是：";
			for (auto it = myset1.begin(); it != myset1.end(); it++)
				cout << *it << " ";
			cout << endl << endl;


			cout << "应输出：11 22 33 33 44 55 66 66" << endl;
			cout << "实际是：";
			for (auto it = myset1.cbegin(); it != myset1.cend(); it++)
				cout << *it << " ";
			cout << endl << endl;


			cout << "应输出：66 66 55 44 33 33 22 11" << endl;
			cout << "实际是：";
			for (auto it = myset1.rbegin(); it != myset1.rend(); it++)
				cout << *it << " ";
			cout << endl << endl;


			cout << "应输出：66 66 55 44 33 33 22 11" << endl;
			cout << "实际是：";
			for (auto it = myset1.crbegin(); it != myset1.crend(); it++)
				cout << *it << " ";
			cout << endl << endl;


			auto first = myset1.begin();
			auto last = myset1.end();
			multiset<string> myset2(first, last);
			myset2.insert("77");
			myset2.insert("66");
			cout << "应输出：77 66 66 66 55 44 33 33 22 11" << endl;
			cout << "实际是：";
			for (auto it = myset2.crbegin(); it != myset2.crend(); it++)
				cout << *it << " ";
			cout << endl;

			wait_for_next();
		}

		//第1组测试：构造相关
		if (test[1])
		{
			cout << "第1组测试：构造函数/=重载" << endl << endl;


			initializer_list<string> L = { "11","33" };
			multiset<string> myset1(L);
			myset1.insert("22");
			cout << "应输出：11 22 33" << endl;
			cout << "实际是：";
			for (auto it = myset1.begin(); it != myset1.end(); it++)
				cout << *it << " ";
			cout << endl << endl;


			multiset<string> myset2(myset1);
			cout << "应输出：11 22 33" << endl;
			cout << "实际是：";
			for (auto it = myset2.begin(); it != myset2.end(); it++)
				cout << *it << " ";
			cout << endl << endl;


			multiset<string> myset3(myset1.begin(), myset1.end());
			cout << "应输出：11 22 33" << endl;
			cout << "实际是：";
			for (auto it = myset3.begin(); it != myset3.end(); it++)
				cout << *it << " ";
			cout << endl << endl;


			multiset<string> myset4 = myset1;
			cout << "应输出：11 22 33" << endl;
			cout << "实际是：";
			for (auto it = myset4.begin(); it != myset4.end(); it++)
				cout << *it << " ";
			cout << endl;

			wait_for_next();
		}

		//第2组测试：容量相关
		if (test[2])
		{
			cout << "第2组测试：size/max_size/empty/count" << endl << endl;


			multiset<string> myset;
			cout << "mymap为空，应输出：1" << endl;
			cout << "mymap为空，实际是：" << myset.empty();
			cout << endl << endl;


			myset.insert("44");
			myset.insert("33");
			myset.insert("22");
			myset.insert("11");
			myset.insert("33");
			myset.insert("55");
			cout << "mymap不为空，有6个元素，应输出：0 6" << endl;
			cout << "mymap不为空，有6个元素，实际是：" << myset.empty() << " " << myset.size();
			cout << endl << endl;


			cout << "max_size()，输出可以不同(214748364)" << endl;
			cout << "实际为：" << myset.max_size();
			cout << endl << endl;


			cout << "应输出：1 2 1 0 0 0" << endl;
			cout << "实际是：" << myset.count("11") << " " << myset.count("33") << " " << myset.count("55") << " ";
			cout << myset.count("23") << " " << myset.count("66") << " " << myset.count("111") << " ";
			cout << endl;

			wait_for_next();
		}

		//第3组测试：元素操作
		if (test[3])
		{
			cout << "第3组测试：insert/erase/emplace/find" << endl << endl;


			multiset<string> myset;
			myset.insert("11");
			myset.insert("22");
			myset.insert("22");
			myset.insert("33");
			myset.insert("44");
			myset.insert("55");
			myset.insert("55");
			cout << "应输出：11 22 22 33 44 55 55" << endl;
			cout << "实际是：";
			for (auto it = myset.begin(); it != myset.end(); it++)
				cout << *it << " ";
			cout << endl << endl;


			myset.erase(myset.begin());
			myset.erase(myset.begin(), ++myset.begin());
			cout << "应输出：22 33 44 55 55" << endl;
			cout << "实际是：";
			for (auto it = myset.begin(); it != myset.end(); it++)
				cout << *it << " ";
			cout << endl << endl;


			myset.erase("55");
			cout << "应输出：22 33 44" << endl;
			cout << "实际是：";
			for (auto it = myset.begin(); it != myset.end(); it++)
				cout << *it << " ";
			cout << endl << endl;


			myset.emplace("11");
			myset.emplace("22");
			myset.emplace("33");
			myset.emplace("66");
			cout << "应输出：11 22 22 33 33 44 66" << endl;
			cout << "实际是：";
			for (auto it = myset.begin(); it != myset.end(); it++)
				cout << *it << " ";
			cout << endl << endl;



			cout << "应输出：22 66" << endl;
			cout << "实际是：";
			auto it = myset.find("11");
			cout << *++it << " ";
			it = myset.find("10");
			cout << *--it << " ";
			cout << endl;

			wait_for_next();
		}

		//第4组测试：整体操作+范围操作
		if (test[4])
		{
			cout << "第4组测试：clear/swap/equal_range/upper_bound/lower_bound" << endl << endl;


			multiset<string> myset1;
			myset1.insert("11");
			myset1.insert("22");
			myset1.insert("33");
			multiset<string> myset2 = myset1;
			myset2.emplace("44");
			myset2.emplace("55");
			myset2.emplace("44");
			myset1.swap(myset2);
			cout << "应输出：6 3" << endl;
			cout << "实际是：";
			cout << myset1.size() << " " << myset2.size();
			cout << endl << endl;


			myset2.clear();
			myset1.swap(myset1);
			swap(myset1, myset2);
			cout << "应输出：0 6" << endl;
			cout << "实际是：";
			cout << myset1.size() << " " << myset2.size();
			cout << endl << endl;


			auto range = myset2.equal_range("44");
			cout << "应输出：44 55" << endl;
			cout << "实际是：";
			cout << *range.first << " " << *range.second;
			cout << endl << endl;


			auto lower = myset2.lower_bound("44");
			auto upper = myset2.upper_bound("44");
			cout << "应输出：44 55" << endl;
			cout << "实际是：";
			cout << *lower << " " << *upper;
			cout << endl;

			wait_for_next();
		}

		//第5组测试：其他函数
		if (test[5])
		{
			cout << "第5组测试：key_comp/value_comp" << endl << endl;


			multiset<string> mymap;
			auto key_compare = mymap.key_comp();
			cout << "应输出：0 1 1" << endl;
			cout << "实际是：";
			cout << key_compare("11", "11") << " " << key_compare("1", "2") << " " << key_compare("00", "11");
			cout << endl << endl;


			auto value_compare = mymap.value_comp();
			cout << "应输出：0 1 1" << endl;
			cout << "实际是：";
			cout << value_compare("11", "11") << " " << value_compare("11", "22") << " " << value_compare("0", "1");
			cout << endl;

			wait_for_next();
		}

		//第6组测试：关系运算符
		if (test[6])
		{
			cout << "第6组测试：6个关系运算符" << endl << endl;


			multiset<string> myset1;
			myset1.emplace("11");
			myset1.emplace("22");
			myset1.emplace("33");
			myset1.emplace("44");
			myset1.emplace("55");
			multiset<string> myset2 = myset1;
			cout << "应输出：1 0 1 1 0 0" << endl;
			cout << "实际是：";
			cout << (myset1 == myset2) << " " << (myset1 != myset2) << " " << (myset1 >= myset2) << " ";
			cout << (myset1 <= myset2) << " " << (myset1 < myset2) << " " << (myset1 > myset2) << " ";
			cout << endl << endl;


			myset2.erase("55");
			cout << "应输出：0 1 1 0 0 1" << endl;
			cout << "实际是：";
			cout << (myset1 == myset2) << " " << (myset1 != myset2) << " " << (myset1 >= myset2) << " ";
			cout << (myset1 <= myset2) << " " << (myset1 < myset2) << " " << (myset1 > myset2) << " ";
			cout << endl << endl;


			myset2.emplace("00");
			cout << "应输出：0 1 1 0 0 1" << endl;
			cout << "实际是：";
			cout << (myset1 == myset2) << " " << (myset1 != myset2) << " " << (myset1 >= myset2) << " ";
			cout << (myset1 <= myset2) << " " << (myset1 < myset2) << " " << (myset1 > myset2) << " ";
			cout << endl << endl;


			myset2.erase("00");
			myset2.emplace("66");
			cout << "应输出：0 1 0 1 1 0" << endl;
			cout << "实际是：";
			cout << (myset1 == myset2) << " " << (myset1 != myset2) << " " << (myset1 >= myset2) << " ";
			cout << (myset1 <= myset2) << " " << (myset1 < myset2) << " " << (myset1 > myset2) << " ";
			cout << endl;

			wait_for_next();
		}

	}

	//student
	if (1)
	{
		const student S[] = {
	   {1851000,"小王",'f',(float)(93.2),"陕西"},
	   {1852000,"小李",'m',(float)(91.6),"山东"},
	   {1853000,"老张",'m',(float)(96.3),"河南"},
	   {1854000,"小赵",'f',(float)(93.7),"四川"},
	   {1855000,"老周",'m',(float)(97.2),"广西"},
	   {1856000,"老吴",'f',(float)(98.5),"广东"},
	   {1857000,"小包",'f',(float)(89.5),"云南"}
		};

		int test[] = { 1,1,1,1,1,1,1 };

		//第0组测试：insert+迭代器测试
		if (test[0])
		{
			cout << "第0组测试：insert/begin/end/cbegin/cend/rbegin/rend/crbegin/crend" << endl << endl;

			multiset<student> myset1;
			initializer_list<student> L = { S[0],S[1],S[2] };
			myset1.insert(L);
			myset1.insert(S[3]);
			myset1.insert(S[2]);
			myset1.insert(S[5]);
			myset1.insert(S[4]);
			myset1.insert(S[5]);

			cout << "应输出：f m m m f m f f" << endl;
			cout << "实际是：";
			for (auto it = myset1.begin(); it != myset1.end(); it++)
				cout << it->sex << " ";
			cout << endl << endl;


			cout << "应输出：93.2 91.6 96.3 96.3 93.7 97.2 98.5 98.5" << endl;
			cout << "实际是：";
			for (auto it = myset1.cbegin(); it != myset1.cend(); it++)
				cout << it->score << " ";
			cout << endl << endl;


			cout << "应输出：老吴 老吴 老周 小赵 老张 老张 小李 小王" << endl;
			cout << "实际是：";
			for (auto it = myset1.rbegin(); it != myset1.rend(); it++)
				cout << it->name << " ";
			cout << endl << endl;


			cout << "应输出：广东 广东 广西 四川 河南 河南 山东 陕西" << endl;
			cout << "实际是：";
			for (auto it = myset1.crbegin(); it != myset1.crend(); it++)
				cout << it->addr << " ";
			cout << endl << endl;


			auto first = myset1.begin();
			auto last = myset1.end();
			multiset<student> myset2(first, last);
			myset2.insert(S[6]);
			myset2.insert(S[5]);
			cout << "应输出：f f f f m f m m m f" << endl;
			cout << "实际是：";
			for (auto it = myset2.crbegin(); it != myset2.crend(); it++)
				cout << it->sex << " ";
			cout << endl;

			wait_for_next();
		}

		//第1组测试：构造相关
		if (test[1])
		{
			cout << "第1组测试：构造函数/=重载" << endl << endl;


			initializer_list<student> L = { S[0],S[2] };
			multiset<student> myset1(L);
			myset1.insert(S[1]);
			cout << "应输出：f m m" << endl;
			cout << "实际是：";
			for (auto it = myset1.begin(); it != myset1.end(); it++)
				cout << it->sex << " ";
			cout << endl << endl;


			multiset<student> myset2(myset1);
			cout << "应输出：f m m" << endl;
			cout << "实际是：";
			for (auto it = myset1.begin(); it != myset1.end(); it++)
				cout << it->sex << " ";
			cout << endl << endl;


			multiset<student> myset3(myset1.begin(), myset1.end());
			cout << "应输出：f m m" << endl;
			cout << "实际是：";
			for (auto it = myset1.begin(); it != myset1.end(); it++)
				cout << it->sex << " ";
			cout << endl << endl;


			multiset<student> myset4 = myset1;
			cout << "应输出：f m m" << endl;
			cout << "实际是：";
			for (auto it = myset1.begin(); it != myset1.end(); it++)
				cout << it->sex << " ";
			cout << endl;

			wait_for_next();
		}

		//第2组测试：容量相关
		if (test[2])
		{
			cout << "第2组测试：size/max_size/empty/count" << endl << endl;


			multiset<student> myset;
			cout << "mymap为空，应输出：1" << endl;
			cout << "mymap为空，实际是：" << myset.empty();
			cout << endl << endl;


			myset.insert(S[3]);
			myset.insert(S[2]);
			myset.insert(S[1]);
			myset.insert(S[0]);
			myset.insert(S[2]);
			myset.insert(S[4]);
			cout << "mymap不为空，有6个元素，应输出：0 6" << endl;
			cout << "mymap不为空，有6个元素，实际是：" << myset.empty() << " " << myset.size();
			cout << endl << endl;


			cout << "max_size()，输出可以不同(63161283)" << endl;
			cout << "实际为：" << myset.max_size();
			cout << endl << endl;


			cout << "应输出：1 2 1 0 0 0" << endl;
			cout << "实际是：" << myset.count(S[0]) << " " << myset.count(S[2]) << " " << myset.count(S[4]) << " ";
			cout << myset.count(S[6]) << " " << myset.count(S[5]) << " " << myset.count(S[6]) << " ";
			cout << endl;

			wait_for_next();
		}

		//第3组测试：元素操作
		if (test[3])
		{
			cout << "第3组测试：insert/erase/emplace/find" << endl << endl;


			multiset<student> myset;
			myset.insert(S[0]);
			myset.insert(S[1]);
			myset.insert(S[1]);
			myset.insert(S[2]);
			myset.insert(S[3]);
			myset.insert(S[4]);
			myset.insert(S[4]);
			cout << "应输出：f m m m f m m" << endl;
			cout << "实际是：";
			for (auto it = myset.begin(); it != myset.end(); it++)
				cout << it->sex << " ";
			cout << endl << endl;


			myset.erase(myset.begin());
			myset.erase(myset.begin(), ++myset.begin());
			cout << "应输出：m m f m m" << endl;
			cout << "实际是：";
			for (auto it = myset.begin(); it != myset.end(); it++)
				cout << it->sex << " ";
			cout << endl << endl;


			myset.erase(S[4]);
			cout << "应输出：m m f" << endl;
			cout << "实际是：";
			for (auto it = myset.begin(); it != myset.end(); it++)
				cout << it->sex << " ";
			cout << endl << endl;


			myset.emplace(S[0]);
			myset.emplace(S[1]);
			myset.emplace(S[2]);
			myset.emplace(S[5]);
			cout << "应输出：f m m m m f f" << endl;
			cout << "实际是：";
			for (auto it = myset.begin(); it != myset.end(); it++)
				cout << it->sex << " ";
			cout << endl << endl;



			cout << "应输出：m f" << endl;
			cout << "实际是：";
			auto it = myset.find(S[0]);
			cout << (++it)->sex << " ";
			it = myset.find(S[6]);
			cout << (--it)->sex << " ";
			cout << endl;

			wait_for_next();
		}

		//第4组测试：整体操作+范围操作
		if (test[4])
		{
			cout << "第4组测试：clear/swap/equal_range/upper_bound/lower_bound" << endl << endl;


			multiset<student> myset1;
			myset1.insert(S[0]);
			myset1.insert(S[1]);
			myset1.insert(S[2]);
			multiset<student> myset2 = myset1;
			myset2.emplace(S[3]);
			myset2.emplace(S[4]);
			myset2.emplace(S[3]);
			myset1.swap(myset2);
			cout << "应输出：6 3" << endl;
			cout << "实际是：";
			cout << myset1.size() << " " << myset2.size();
			cout << endl << endl;


			myset2.clear();
			myset1.swap(myset1);
			swap(myset1, myset2);
			cout << "应输出：0 6" << endl;
			cout << "实际是：";
			cout << myset1.size() << " " << myset2.size();
			cout << endl << endl;


			auto range = myset2.equal_range(S[3]);
			cout << "应输出：小赵 老周" << endl;
			cout << "实际是：";
			cout << range.first->name << " " << range.second->name;
			cout << endl << endl;


			auto lower = myset2.lower_bound(S[3]);
			auto upper = myset2.upper_bound(S[3]);
			cout << "应输出：小赵 老周" << endl;
			cout << "实际是：";
			cout << lower->name << " " << upper->name;
			cout << endl;

			wait_for_next();
		}

		//第5组测试：其他函数
		if (test[5])
		{
			cout << "第5组测试：key_comp/value_comp" << endl << endl;


			multiset<student> mymap;
			auto key_compare = mymap.key_comp();
			cout << "应输出：0 1 1" << endl;
			cout << "实际是：";
			cout << key_compare(S[1], S[1]) << " " << key_compare(S[1], S[2]) << " " << key_compare(S[0], S[1]);
			cout << endl << endl;


			auto value_compare = mymap.value_comp();
			cout << "应输出：0 1 1" << endl;
			cout << "实际是：";
			cout << value_compare(S[1], S[1]) << " " << value_compare(S[1], S[2]) << " " << value_compare(S[0], S[1]);
			cout << endl;

			wait_for_next();
		}

		//第6组测试：关系运算符
		if (test[6])
		{
			cout << "第6组测试：6个关系运算符" << endl << endl;


			multiset<student> myset1;
			myset1.emplace(S[1]);
			myset1.emplace(S[2]);
			myset1.emplace(S[3]);
			myset1.emplace(S[4]);
			myset1.emplace(S[5]);
			multiset<student> myset2 = myset1;
			cout << "应输出：1 0 1 1 0 0" << endl;
			cout << "实际是：";
			cout << (myset1 == myset2) << " " << (myset1 != myset2) << " " << (myset1 >= myset2) << " ";
			cout << (myset1 <= myset2) << " " << (myset1 < myset2) << " " << (myset1 > myset2) << " ";
			cout << endl << endl;


			myset2.erase(S[5]);
			cout << "应输出：0 1 1 0 0 1" << endl;
			cout << "实际是：";
			cout << (myset1 == myset2) << " " << (myset1 != myset2) << " " << (myset1 >= myset2) << " ";
			cout << (myset1 <= myset2) << " " << (myset1 < myset2) << " " << (myset1 > myset2) << " ";
			cout << endl << endl;


			myset2.emplace(S[0]);
			cout << "应输出：0 1 1 0 0 1" << endl;
			cout << "实际是：";
			cout << (myset1 == myset2) << " " << (myset1 != myset2) << " " << (myset1 >= myset2) << " ";
			cout << (myset1 <= myset2) << " " << (myset1 < myset2) << " " << (myset1 > myset2) << " ";
			cout << endl << endl;


			myset2.erase(S[0]);
			myset2.emplace(S[6]);
			cout << "应输出：0 1 0 1 1 0" << endl;
			cout << "实际是：";
			cout << (myset1 == myset2) << " " << (myset1 != myset2) << " " << (myset1 >= myset2) << " ";
			cout << (myset1 <= myset2) << " " << (myset1 < myset2) << " " << (myset1 > myset2) << " ";
			cout << endl;

			wait_for_next();
		}

	}


}
