#include"AB_agent.h"
#include"common.h"
#include<fstream>
#pragma GCC optimize(3, "ofast","inline")
const int MAX = 20000000;

//4、5层结合搜索
string AB_agent::runable(bool flagtw) {
	bool compelete_4 = true, compelete_5 = true;
	int Depth;

	ofstream o("output_ab.txt", ios::app);

	int startTime4 = clock();
	LB.flag_find();//确定军旗位置

	vector<int> pre_positions; vector<vector<int>> res;
	LB.get_all(player, pre_positions, res);//找到所有可走位置

	int num = 0;
	for (int i = 0; i < pre_positions.size(); i++)
		num += res[i].size();

	vector<int>values4(num, MAX);//4层value值
	vector<int>values5(num, MAX);//5层value值
	array<array<int, 2>, 2> arr;

	//4层AB搜索
	Depth = 3;
	int k = 0;
	bool flag = false;
	for (int i = 0; i < pre_positions.size(); i++) {
		for (int j = 0; j < res[i].size(); j++, k++) {
			arr[0][0] = pre_positions[i] / 5;
			arr[0][1] = pre_positions[i] % 5;
			arr[1][0] = res[i][j] / 5;
			arr[1][1] = res[i][j] % 5;
			int start_value, end_value;
			int sit = LB.change_board(arr, start_value, end_value);//得到走步后棋盘
			if (sit != -1)//判断是否送人头
				values4[k] = alphaBeta(Depth, -player, -MAX, MAX);
			LB.recover_board(start_value, end_value, arr);
			if (clock() - startTime4 > time_limitation) {
				o << "4层超时" << endl;
				flag = true;//超出时间即停止
				compelete_4 = false;
				compelete_5 = false;
				break;
			}
		}
		if (flag == true)
			break;
	}

	int startTime5 = clock();
	o << "4层用时" << startTime5 - startTime4 << endl;

	if (compelete_4) {//如果4完成，尝试进行5
		//4层AB搜索
		Depth = 4;
		k = 0;
		flag = false;
		for (int i = 0; i < pre_positions.size(); i++) {
			for (int j = 0; j < res[i].size(); j++, k++) {
				arr[0][0] = pre_positions[i] / 5;
				arr[0][1] = pre_positions[i] % 5;
				arr[1][0] = res[i][j] / 5;
				arr[1][1] = res[i][j] % 5;
				int start_value, end_value;
				int sit = LB.change_board(arr, start_value, end_value);//得到走步后棋盘
				if (sit != -1)//判断是否送人头
					values5[k] = alphaBeta(Depth, -player, -MAX, MAX);
				LB.recover_board(start_value, end_value, arr);
				if (clock() - startTime4 > time_limitation) {
					o << "5层超时" << endl;
					flag = true;//超出时间即停止
					compelete_5 = false;
					break;
				}
			}
			if (flag == true)
				break;
		}
	}
	o << "5层用时" << clock() - startTime5 << endl;

	int firstx = 0, firsty = 0;
	int secondx = 0, secondy = 0;
	int firstmin = MAX, secondmin = MAX;
	int indx = 0, indy = 0;
	if (compelete_5) {
		//找出最小和次小值
		for (int i = 0, k = 0; i < pre_positions.size(); i++)
			for (int j = 0; j < res[i].size(); j++, k++) {
				if (values5[k] < firstmin) { //小于最小的元素 更新1和2
					secondmin = firstmin;
					secondx = firstx;
					secondy = firsty;

					firstmin = values5[k];
					firstx = i;
					firsty = j;
				}
				else if (values5[k] < secondmin) {//小于倒数二的 更新2
					secondmin = values5[k];
					secondx = i;
					secondy = j;
				}
			}

		if (!flagtw) {
			indx = firstx;
			indy = firsty;
		}
		else {
			srand((unsigned)time(NULL));
			int r = rand() % 3 < 1 ? 0 : 1; //P(0)=1/3, P(1)=2/3
			indx = r ? firstx : secondx;
			indy = r ? firsty : secondy;
		}

	}
	else {
		//找出最小和次小值
		for (int i = 0, k = 0; i < pre_positions.size(); i++)
			for (int j = 0; j < res[i].size(); j++, k++) {
				if (values4[k] < firstmin) { //小于最小的元素 更新1和2
					secondmin = firstmin;
					secondx = firstx;
					secondy = firsty;

					firstmin = values4[k];
					firstx = i;
					firsty = j;
				}
				else if (values4[k] < secondmin) {//小于倒数二的 更新2
					secondmin = values4[k];
					secondx = i;
					secondy = j;
				}
			}

		if (!flagtw) {
			indx = firstx;
			indy = firsty;
		}
		else {
			srand((unsigned)time(NULL));
			int r = rand() % 3 < 1 ? 0 : 1; //P(0)=1/3, P(1)=2/3
			indx = r ? firstx : secondx;
			indy = r ? firsty : secondy;
		}
	}
	arr[0][0] = pre_positions[indx] / 5;
	arr[0][1] = pre_positions[indx] % 5;
	arr[1][0] = res[indx][indy] / 5;
	arr[1][1] = res[indx][indy] % 5;

	string ss = move_to_ncn(arr);
	return ss;
}

int AB_agent::alphaBeta(int depth, int player, int alpha, int beta) {
	int lb_end = LB.END();
	if ((lb_end == 1 && player < 0) || (lb_end == -1 && player > 0))
		return ((-MAX / 2) - depth * (MAX / 2));

	if (depth == 0 || (lb_end == 1 && player > 0) || (lb_end == -1 && player < 0)) {
		//深度为0，或到达终止点，调用评估函数
		return LB.evalution(player);
	}

	vector<int> pre_positions;
	vector<vector<int>> res;
	LB.get_all(player, pre_positions, res);

	//For each possible move
	//AB搜索
	for (int i = 0; i < pre_positions.size(); i++)
		for (int j = 0; j < res[i].size(); j++) {
			array<array<int, 2>, 2> arr;
			arr[0][0] = pre_positions[i] / 5;
			arr[0][1] = pre_positions[i] % 5;
			arr[1][0] = res[i][j] / 5;
			arr[1][1] = res[i][j] % 5;
			int start_value, end_value;
			LB.change_board(arr, start_value, end_value);
			int value = -alphaBeta(depth - 1, -player, -beta, -alpha);//切换属性
			LB.recover_board(start_value, end_value, arr);
			if (value >= alpha)
				alpha = value;
			if (alpha >= beta)
				return alpha;
		}
	return alpha;
}