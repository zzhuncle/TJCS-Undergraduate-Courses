#include"common.h"
/**
* @file			LBChess.cpp
* @brief        判断是否结束
* @param[in]    棋盘数组
* @param[out]   如果和局返回0，否则返回赢家R代表-1，B代表1，如果没有结束返回2
* @author       zzh
* @date			2020.04.13
* @version		0.0.1
*/
const unordered_map<int, int>TYPE = { {DI,39},{GONG,60} ,{PAI,20},{LIAN,40},{ZHA,130},{YING,70},{TUAN,90},{LV,120},{SHI,170},{JUN,260},{SI,350} };
int LBChess::END()
{
	//判断军旗是否被吃
	bool FlagR = false, FlagB = false;
	//判断是否存在可以移动的力量
	bool live_R = false, live_B = false;
	for (int i = 0; i < 13; i++)
		for (int j = 0; j < 5; j++) {
			if (board[i][j] == FLAG)//B是正数，R是负数
				FlagB = true;
			else if (board[i][j] == -FLAG)
				FlagR = true;
			else if (board[i][j] >= 3 && board[i][j] <= 12)
				live_B = true;
			else if (board[i][j] <= -3 && board[i][j] >= -12)
				live_R = true;
		}
	if (!FlagB)  return R;
	if (!FlagR)  return B;
	if (!live_B && !live_R)//和局
		return 0;
	if (live_B && live_R)//未结束
		return 2;
	return live_B ? B : R;
}

/**
* @file			LBChess.cpp
* @brief        根据棋子等级判断攻击情况
* @param[in]    src : 攻击者 dst : 攻击目标
* @param[out]   同归于尽返回0，攻击成功返回1，攻击失败返回 -1，非法攻击返回2
* @author       zzh
* @date			2020.04.13
* @version		0.0.1
*/
int attack(int src, int dst)
{
	if (abs(src) == DI || abs(src) == FLAG || src * dst > 0) // 攻击者不能是地雷或军旗或空白，双方不能是同一颜色，否则报错
		return 2;
	src = abs(src); dst = abs(dst);
	if (dst == ZHA or src == ZHA)  // 炸弹必然同归于尽
		return 0;
	if (dst == DI)  // 工兵排地雷，否则同归于尽
		return src == GONG ? 1 : 0;
	if (src == dst)  // 相同则同归于尽
		return 0;
	if (dst == FLAG)  // 军旗最弱
		return 1;
	return src > dst ? 1 : -1;
}

/**
* @file			LBChess.cpp
* @brief        根据攻击情况改变棋局
* @param[in]    board : 原始棋局 arr : 攻击起始末尾位置
* @param[out]   res : 更新后的棋局
* @author       zzh
* @date			2020.04.26
* @version		0.0.1
* @date         2020.05.10
* @version      0.0.2
*/
int  LBChess::change_board(const array<array<int, 2>, 2>& arr, int &start_value, int &end_value)
{
	int start_x = arr[0][0], start_y = arr[0][1];
	int end_x = arr[1][0], end_y = arr[1][1];

	start_value = board[start_x][start_y];
	end_value = board[end_x][end_y];

	//array<array<int, 5>, 13> res = board;
	if (board[end_x][end_y] == BLANK) {//要去的地方是空白，直接覆盖空白并返回
		board[end_x][end_y] = board[start_x][start_y];
		board[start_x][start_y] = BLANK;
		return 1;
	}
	int situation = attack(board[start_x][start_y], board[end_x][end_y]);//进行攻击判断
	if (situation == 0)//同归于尽
		board[start_x][start_y] = board[end_x][end_y] = BLANK;
	else if (situation == 1) {//攻击成功
		board[end_x][end_y] = board[start_x][start_y];
		board[start_x][start_y] = BLANK;
	}
	else//攻击失败
		board[start_x][start_y] = BLANK;
	return situation;
}

//棋盘恢复
void LBChess::recover_board(int start_value,int end_value, const array<array<int, 2>, 2>& arr) {
	
	board[arr[0][0]][arr[0][1]] = start_value;
	board[arr[1][0]][arr[1][1]] = end_value;
}

bool LBChess::tie(int x, int y) {
	return(x >= 0 && y >= 0 && x <= 12 && y <= 4 && _tie[x][y] == 1);
}
void  LBChess::flag_find()
{

	if (-FLAG == board[0][1])//判断是否是军旗
		Nflag_location = make_pair(0, 1);
	else
		Nflag_location = make_pair(0, 3);

	if (FLAG == board[12][1])//判断是否是军旗
		Sflag_location = make_pair(12, 1);
	else
		Sflag_location = make_pair(12, 3);

}

/**
* @file       LBChess.cpp
* @brief      棋局评估
* @author     zjx
* @date        2020  5.4
* @param[in]   player   当前操作者
* @param[out]  value    评估值
* @par history
		zjx于2020.5.4创建\n
*/
int LBChess::evalution(int  player) {
	int value = 0;
	int size_board = board.size();
	int size_board_0 = board[0].size();

	//修改 炸弹的子力变成当前最大子力的0.4 因为初始值130差不多是司令350的0.4左右
	int max_ChessPower_B = -1;
	int max_ChessPower_R = -1;
	int ZHA_ChessPower_B = 0;
	int ZHA_ChessPower_R = 0;
	for (int i = 0; i < size_board; i++)
		for (int j = 0; j < size_board_0; j++)
		{
			if (abs(board[i][j]) != BLANK && abs(board[i][j]) != FLAG)
			{
				if (board[i][j] > 0 && board[i][j] != ZHA && TYPE.find(board[i][j])->second > max_ChessPower_B)
					max_ChessPower_B = TYPE.find(board[i][j])->second;
				if (board[i][j] < 0 && abs(board[i][j]) != ZHA && TYPE.find(abs(board[i][j]))->second > max_ChessPower_R)
					max_ChessPower_R = TYPE.find(abs(board[i][j]))->second;
			}
		}
	ZHA_ChessPower_B = int(max_ChessPower_B * 0.35);
	ZHA_ChessPower_R = int(max_ChessPower_R * 0.35);
	//TYPE.find(ZHA)->second = int(max_ChessPower * 0.4);

	for (int i = 0; i < size_board; i++)
		for (int j = 0; j < size_board_0; j++)
		{
			if (abs(board[i][j]) == BLANK && abs(board[i][j]) == FLAG)
				continue;
			//子力加和分数
			if (board[i][j] > 0 && !(i == Sflag_location.first && j == Sflag_location.second))
			{
				//对炸弹进行专门处理
				if (board[i][j] != ZHA)
					value += TYPE.find(board[i][j])->second;
				else
					value += ZHA_ChessPower_R;//需要增加的是对方的最大的值的子力

				/*if (board[i][j] == DI)
					value += 39;
				else if (board[i][j] == GONG)
					value += 60;
				else if (board[i][j] == PAI)
					value += 20;
				else if (board[i][j] == LIAN)
					value += 40;
				else if (board[i][j] == ZHA)
					value += 130;
				else if (board[i][j] == YING)
					value += 70;
				else if (board[i][j] == TUAN)
					value += 90;
				else if (board[i][j] == LV)
					value += 120;
				else if (board[i][j] == SHI)
					value += 170;
				else  if (board[i][j] == JUN)
					value += 260;
				else
					value += 350;*/
			}
			if (board[i][j] < 0 && !(i == Nflag_location.first && j == Nflag_location.second)) {
				//对炸弹进行专门处理
				if (abs(board[i][j]) != ZHA)
					value -= TYPE.find(abs(board[i][j]))->second;
				else
					value -= ZHA_ChessPower_B;//需要增加的是对方的最大的值的子力
				/*if (board[i][j] == -DI)
					value -= 39;
				else if (board[i][j] == -GONG)
					value -= 60;
				else if (board[i][j] == -PAI)
					value -= 20;
				else if (board[i][j] == -LIAN)
					value -= 40;
				else if (board[i][j] == -ZHA)
					value -= 130;
				else if (board[i][j] == -YING)
					value -= 70;
				else if (board[i][j] == -TUAN)
					value -= 90;
				else if (board[i][j] == -LV)
					value -= 120;
				else if (board[i][j] == -SHI)
					value -= 170;
				else  if (board[i][j] == -JUN)
					value -= 260;
				else
					value -= 350;*/
			}

			//攻占对方底线加分（鼓励进攻与加强防守）
			if (i == 11 && board[i][j] < 0)
				value -= 20;
			if (i == 1 && board[i][j] > 0)
				value += 20;
			//占有利位置


			//其他行营加分
			if (XINGYING.count(i * 5 + j) != 0 && board[i][j] > 0)
				value += 5;
			if (XINGYING.count(i * 5 + j) != 0 && board[i][j] < 0)
				value -= 5;

		}

	//旗附近的位置是killer法，主要弥补搜索深度不足
	if (abs(board[Nflag_location.first][Nflag_location.second]) == FLAG)
		value -= FLAG_VALUE;
	if (abs(board[Sflag_location.first][Sflag_location.second]) == FLAG)
		value += FLAG_VALUE;
	if (board[Sflag_location.first][Sflag_location.second + 1] < 0 || board[Sflag_location.first][Sflag_location.second - 1] < 0)
		value -= FLAG_VALUE / 10;
	if ((board[Sflag_location.first - 1][Sflag_location.second]) < 0)
		value -= FLAG_VALUE / 10;
	/*if (board[Sflag_location.first - 1][Sflag_location.second-1]< 0|| board[Sflag_location.first - 1][Sflag_location.second +1]<0)
		value -= FLAG_VALUE / 20;
	if (board[Nflag_location.first + 1][Nflag_location.second - 1] > 0 || board[Nflag_location.first + 1][Nflag_location.second + 1] > 0)
		value += FLAG_VALUE / 20;*/
	if ((board[Nflag_location.first + 1][Nflag_location.second]) > 0)
		value += FLAG_VALUE / 10;
	if (board[Nflag_location.first][Nflag_location.second + 1] > 0 || board[Nflag_location.first][Nflag_location.second - 1] > 0)
		value += FLAG_VALUE / 10;
	//要破三角雷
	if ((board[Sflag_location.first - 1][Sflag_location.second]) == DI && board[Sflag_location.first][Sflag_location.second + 1] == DI && board[Sflag_location.first][Sflag_location.second - 1] == DI)
		value += 300;
	if ((board[Nflag_location.first + 1][Nflag_location.second]) == -DI && board[Nflag_location.first][Nflag_location.second + 1] == -DI && board[Nflag_location.first][Nflag_location.second - 1] == -DI)
		value -= 300;
	//占对方旗上的行营，位置重要
	if (board[Sflag_location.first - 2][Sflag_location.second] < 0)
		value -= 45;
	if (board[Sflag_location.first - 2][Sflag_location.second] > 0)
		value += 45;
	if (board[Nflag_location.first + 2][Nflag_location.second] > 0)
		value += 45;
	if (board[Nflag_location.first + 2][Nflag_location.second] < 0)
		value -= 45;
	//负值最大搜索。对当前一方而言，如果占优则返回正数，否则返回负数
	if (player < 0)
		value = -value;
	return value;
}


/**
* @file       LBChess.cpp
* @brief      得到所有位置棋子可以走的所有位置
* @author     zzh
* @date       2020.4.14
* @param[in]
* @param[out] result   所有可到达位置
* @par history
		zzh于2020.4.25创建\n
*/

void LBChess::get_all(int chessman, vector<int>& pre_positions, vector<vector<int>>& res)
{
	int index = 0;
	for (int i = 0; i < 13; i++)
		for (int j = 0; j < 5; j++)
			if (chessman * board[i][j] > 0) {//只能移动本方棋子
				pre_positions.push_back(i * 5 + j);
				vector<int>move = {};
				res.push_back(move);
				get(i * 5 + j, res[index]);
				if (!res[index].size())
				{
					res.pop_back();
					pre_positions.pop_back();
					//continue;
				}
				else
					index++;
			}
	return;
}

/**
* @file       LBChess.cpp
* @brief      得到某一位置棋子可以走的所有位置
* @author     zjx
* @date        2020.4.14
* @param[in]   pos    位置坐标
* @param[out]  result   所有可到达位置
* @par history
		zjx于2020.4.14创建\n
*/
//优先向前走
int r_tomove[8][2] = { {1,0},{1,1},{1,-1},{0,1},{0,-1},{-1,1}, {-1,-1},{-1,0} };
int b_tomove[8][2] = { {-1,0},{-1,1},{-1,-1},{0,1},{0,-1},{1,1}, {1,-1},{1,0} };
int r_tomove_tie[4][2] = { {1,0},{0,1},{0,-1},{-1,0} };
int b_tomove_tie[4][2] = { {-1,0},{0,1},{0,-1}, {1,0} };
void LBChess::get(int pos, vector<int>& result) {
	int pos_first = pos / 5, pos_second = pos % 5;
	if (abs(board[pos_first][pos_second]) == BLANK || abs(board[pos_first][pos_second]) == FLAG || abs(board[pos_first][pos_second]) == DI ||
		SHANJIE.count(pos_first * 5 + pos_second)) //[大本营无法移动：暗棋规则]
		return;
	//公路线，考虑周围八个位置
	int which_ = board[pos_first][pos_second];//取出要判断的子的数值
	int(*p)[2];
	if (which_*R > 0)//说明是红方
		p = r_tomove;
	else
		p = b_tomove;
	for (int k = 0; k < 8; k++) {

		int i = p[k][0];
		int j = p[k][1];
		int new_row = pos_first + i;
		int new_col = pos_second + j;
		if (new_row < 0 || new_row>12 || new_col < 0 || new_col>4)    //不可移动到界外
			continue;
		if (SHANJIE.count(new_row * 5 + new_col))   //不可移动到山界
			continue;
		if (i != 0 && j != 0 && !XINGYING.count(pos_first * 5 + pos_second) &&
			!XINGYING.count(new_row * 5 + new_col))//斜方向上移动必须有一个在行营
			continue;
		if (XINGYING.count(new_row * 5 + new_col) && board[new_row][new_col] != BLANK)//如果移动到行营，行营不可以有棋子
			continue;
		if (board[new_row][new_col] != 0 && board[pos_first][pos_second] != 0 && board[new_row][new_col] * board[pos_first][pos_second] > 0)
			continue;
		result.push_back(new_row * 5 + new_col);
	}

	//铁路线，考虑直线路径，如果是工兵，使用工兵的寻路算法
	if (!tie(pos_first, pos_second))
		return;
	if (abs(board[pos_first][pos_second]) == GONG)
		_get_gong(pos, result);
	else
	{
		int(*direct)[2];
		if (which_*R > 0)//说明是红方
			direct = r_tomove_tie;
		else
			direct = b_tomove_tie;
		for (int i = 0; i < 4; i++) {
			int  new_pos_first = pos_first + direct[i][0], new_pos_second = pos_second + direct[i][1];
			if (tie(new_pos_first, new_pos_second) && board[new_pos_first][new_pos_second] == BLANK)
				while (true) {
					new_pos_first = new_pos_first + direct[i][0];
					new_pos_second = new_pos_second + direct[i][1];
					if (!tie(new_pos_first, new_pos_second))  //非铁路坐标
						break;
					if (board[new_pos_first][new_pos_second] * board[pos_first][pos_second] > 0)  //同色遮挡
						break;
					result.push_back(new_pos_first * 5 + new_pos_second);
					if (board[new_pos_first][new_pos_second] * board[pos_first][pos_second] < 0)
						break;
				}
		}
	}
	return;

}

/**
* @file			LBChess.cpp
* @brief        返回铁路线上，工兵可以到达的所有位置
* @param[in]    工兵位置pair<int, int>, 可以到达位置的vector，引用传入
* @param[out]   无(可以到达位置的返回由引用的vector带出
* @author       ssc
* @date			2020.04.14
* @version		0.0.3
* @history      zzh于2020.5.1复制添加
*/
void LBChess::_get_gong(int pos, vector<int>& res)
{
	/*函数思路，使用BFS进行寻路，如果下一个位置是己方或者已经探索或者是公路线，则停止探索
	 如果是铁路线，则加入队列，等待搜索
	 从队列中出队列，进行上下左右搜索
	*/
	//cout <<"testget_gong   "<< pos.first << "  " << pos.second << endl;
	int group;//工兵是哪方阵营
	int pos_first = pos / 5, pos_second = pos % 5;
	//if (RAILWAY.count(position2int(pos.first, pos.second)) == 0 || abs(board[pos.first][pos.second]) != GONG) //非铁路线或者没有工兵则返回
		//return;
	if (board[pos_first][pos_second] > 0)
		group = 1;
	else
		group = -1;//保证己方的字*group>0,对方的*group<0
	int temp[13][5] = {
		{0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1},
		{1, 0, 0, 0, 1},
		{1, 0, 0, 0, 1},
		{1, 0, 0, 0, 1},
		{1, 1, 1, 1, 1},
		{1, 0, 1, 0, 1},
		{1, 1, 1, 1, 1},
		{1, 0, 0, 0, 1},
		{1, 0, 0, 0, 1},
		{1, 0, 0, 0, 1},
		{1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0} };//内置铁路线地图，为了加快运行速度
	for (int i = 0; i < 13; i++)
		for (int j = 0; j < 5; j++)
		{
			if (temp[i][j] == 0)
				continue;
			if (board[i][j] * group > 0)
				temp[i][j] = -1;//自己人置-1
			if (board[i][j] * group < 0)
				temp[i][j] = 2;//敌人置2
		}//重置地图，公路线位置是0，铁路线空白地方为0，自己位置为1，敌人位置为2

	vector<int>result;
	/*cout << "testget_gong   " << endl;
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 5; j++)
			cout << temp[i][j]<<"  ";
		cout << endl;
	}*/
	queue<int> Q;
	Q.push(pos);
	int current_first, current_second, a;//临时变量，记录现在的位置
	while (Q.size() > 0)
	{
		current_first = Q.front() / 5;
		current_second = Q.front() % 5;
		Q.pop();//出队列，然后记录
		//cout << "testget_gong   " << current.first << "  " << current.second << endl;
		if (temp[current_first][current_second] != -1)
			result.push_back(current_first * 5 + current_second);//进入result中，代表可以走到
		temp[current_first][current_second] = -1;//代表已访问过
		if (current_first + 1 < 13) {
			if (temp[current_first + 1][current_second] == 1)
				Q.push((current_first + 1) * 5 + current_second);//如果是铁路，加入队列，等着下次访问
			else if (temp[current_first + 1][current_second] == 2)
				result.push_back((current_first + 1) * 5 + current_second);//如果是敌人，就可以走到这个位置，不能继续
			//如果是公路，己方，或者已经探索，则结束
		}
		if (current_second + 1 < 5) {
			if (temp[current_first][current_second + 1] == 1)
				Q.push(current_first * 5 + current_second + 1);
			else if (temp[current_first][current_second + 1] == 2)
				result.push_back(current_first * 5 + current_second + 1);
		}
		if (current_first - 1 >= 0) {
			if (temp[current_first - 1][current_second] == 1)
				Q.push((current_first - 1) * 5 + current_second);
			else if (temp[current_first - 1][current_second] == 2)
				result.push_back((current_first - 1) * 5 + current_second);
		}
		if (current_second - 1 >= 0) {
			if (temp[current_first][current_second - 1] == 1)
				Q.push(current_first * 5 + current_second - 1);
			else if (temp[current_first][current_second - 1] == 2)
				result.push_back(current_first * 5 + current_second - 1);
		}
	}
	//result.erase(result.begin());//第一个元素是传入的pos,移除
	vector<int> res_tmp = res;
	res.clear();//res结果赋给新的vector，并清空
	for (int i = 0; i < res_tmp.size(); i++)
		if (_tie[res_tmp[i] / 5][res_tmp[i] % 5] == 0)
			res.push_back(res_tmp[i]);
	res.insert(res.end(), result.begin(), result.end());//把在铁路线可到达位置进入，
	//cout << "testget_gong   " << result.size() << endl;
}

/**
* @file			LBChess.cpp
* @brief        查看棋盘当前状态
* @param[in]    /
* @param[out]   打印棋盘状态，没有子的位置为0，有子的位置显示内部数组的值，行营用*标出
* @author       ssc
* @date			2020.04.18
* @version		0.0.1
*/
void LBChess::seetheboard()
{
	int i, j;
	for (i = 0; i < 13; i++) {
		for (j = 0; j < 5; j++)
		{
			if (XINGYING.count(position2int(i, j)) > 0)
				cout << setw(4) << "*" << board[i][j] << "*";
			else
				cout << setw(5) << board[i][j];
		}
		cout << endl;
	}
}