import numpy as np

from common import *


def end(data):
    """
    判断是否结束
    :return: 如果未结束则返回None，否则返回获胜方的编码：R=-1， B=1， 和局0
    """

    # 判断军旗是否被吃
    if not (data == FLAG).any():
        return R
    if not (data == -FLAG).any():
        return B

    # 判断是否存在可以移动的力量
    live_R = live_B = False
    for i in range(3, 13):
        if (data == i).any():
            live_B = True
        if (data == -i).any():
            live_R = True
    if not live_B and not live_R:
        return 0  # 和局
    if live_B and live_R:
        return None  # 未结束

    return B if live_B else R


class attack_error(RuntimeError):
    def __init__(self):
        self.args = '非法的攻击者'


def attack(src, dst):
    """
    :param src: 攻击者
    :param dst: 攻击目标
    :return: 同归于尽返回0，攻击成功返回1，攻击失败返回-1
    """
    if abs(src) == DI or abs(src) == FLAG or src * dst >= 0:  # 攻击者不能是地雷或军旗或空白，双方不能是同一颜色，否则报错
        raise attack_error
    src = abs(src)
    dst = abs(dst)
    if dst == ZHA or src == ZHA:  # 炸弹必然同归于尽
        return 0
    if dst == DI:  # 工兵排地雷，否则同归于尽
        return 1 if src == GONG else 0
    if src == dst:  # 相同则同归于尽
        return 0
    if dst == FLAG:  # 军旗最弱
        return 1
    return 1 if src > dst else -1


class formation_error(RuntimeError):
    def __init__(self):
        self.args = '非法阵型'


class move_error(RuntimeError):
    def __init__(self):
        self.args = '非法移动'


def check_formation(formation):
    """
    布阵正确的条件：棋子种类和棋子数正确，双方颜色正确（上红下黑），炸弹不在第一排，地雷在最后两排，军旗在大本营里，行营前线和山界没有棋子
    :param formation: 布阵信息 13*5的array
    :return: 布阵正确返回True，否则返回False
    """
    # 检查颜色
    if (formation[6] != 0).any() or \
            (formation[:6] > 0).any() or \
            (formation[7:] < 0).any():
        return False
    # 检查炸弹位置
    if (abs(formation[5:8]) == ZHA).any():
        return False
    # 检查地雷位置
    if (abs(formation[2:11]) == DI).any():
        return False
    # 检查军旗位置
    if (formation[0, 1] != R * FLAG and formation[0, 3] != R * FLAG) or \
            (formation[12, 1] != B * FLAG and formation[12, 3] != B * FLAG):
        return False
    # 检查种类和数量
    for i in range(1, 13):
        if (formation == i).sum() != CHESS[i].N or (formation == -i).sum() != CHESS[i].N:
            return False
    return True


# Land Battle Chess 陆军棋
class LBChess:
    """
    LBChess类：
    维护棋盘信息
    确保行子和布阵的正确性
    确保攻击的正确性
    但不负责控制游戏流程
    """

    def __init__(self, data=None):
        self.data = np.zeros([13, 5], dtype=np.int) if data is None else data

    def copy(self):
        temp = LBChess()
        temp.data = self.data.copy()
        return temp

    def start(self, black, red):
        """
        初始化棋盘信息，严格检查布阵的正确性
        :param black: 黑方的布阵信息,6*5的array
        :param red: 红方的布阵信息，6*5的array
        :return: None
        """
        self.data *= 0
        self.data[7:, :] = black
        for i in range(6):
            for j in range(5):
                self.data[i, j] = red[5 - i, 4 - j]
        if not check_formation(self.data):
            raise formation_error

    def reset(self, state):
        """
        直接给棋盘信息赋值，不检查正确性
        :param state: 13*5的array
        :return: None
        """
        self.data = state.copy()

    def move(self, start, end):
        """
        移动棋子，行棋或攻击。非法移动会报错。
        :param start: 起点坐标,(r,c)
        :param end: 终点坐标,(r,c)
        :return: None 如果出现攻击则返回阵亡棋子的编号x, (r1, c1)
        """
        start_chess = self.data[start[0], start[1]]
        end_chess = self.data[end[0], end[1]]

        if tuple(end) not in self.get(start):
            raise move_error

        self.data[start[0], start[1]] = BLANK
        if end_chess == BLANK:
            self.data[end[0], end[1]] = start_chess
            return None
        else:
            re = attack(start_chess, end_chess)
            if re == 0:  # 同归于尽
                self.data[end[0], end[1]] = BLANK
                return start_chess, end_chess
            elif re == 1:  # 攻击成功
                self.data[end[0], end[1]] = start_chess
                return end_chess
            else:  # 攻击失败
                return start_chess

    def move_unsafe(self, start, end):
        """
        不安全的移动，如果百分百确定移动合法，则调用此函数，减小计算开销。可以配合get函数使用。建议仅用于引擎中的算法实现。
        :param start: 与move函数相同
        :param end: 与move函数相同
        :return: 与move函数相同
        """
        start_chess = self.data[start[0], start[1]]
        end_chess = self.data[end[0], end[1]]
        self.data[start[0], start[1]] = BLANK
        if end_chess == BLANK:
            self.data[end[0], end[1]] = start_chess
            return None
        else:
            re = attack(start_chess, end_chess)
            if re == 0:  # 同归于尽
                self.data[end[0], end[1]] = BLANK
                return start_chess, end_chess
            elif re == 1:  # 攻击成功
                self.data[end[0], end[1]] = start_chess
                return end_chess
            else:  # 攻击失败
                return start_chess

    def get(self, pos):
        """
        :param pos: 坐标(r,c)，调用者保证坐标范围合法
        :return: 返回该坐标的棋子的所有可达地点{(x1,y1),(x2,y2),...}
        """
        result = []
        no_move = [BLANK, DI, FLAG]
        if abs(self.data[pos[0], pos[1]]) in no_move or tuple(pos) in SHANJIE:  # [大本营无法移动：暗棋规则]
            return result
        # 公路线，考虑周围八个位置
        for i in range(-1, 2):
            for j in range(-1, 2):
                if i == 0 and j == 0: continue
                new_row = pos[0] + i
                new_col = pos[1] + j
                if new_row < 0 or new_row > 12 or new_col < 0 or new_col > 4:  # 不可以移动到界外
                    continue
                if (new_row, new_col) in SHANJIE:  # 不可以移动到山界
                    continue
                if i != 0 and j != 0 and \
                        pos not in XINGYING and \
                        (new_row, new_col) not in XINGYING:  # 斜方向的移动必须有一个在行营
                    continue
                if (new_row, new_col) in XINGYING and self.data[new_row, new_col] != BLANK:  # 如果移动到行营，行营不可以有棋子
                    continue
                if self.data[new_row, new_col] * self.data[pos[0], pos[1]] > 0:  # 同色棋子
                    continue
                result.append((new_row, new_col))

        # 铁路线，考虑直线路径，如果是工兵，使用工兵的寻路算法
        tie = lambda x: x[0] >= 0 and x[1] >= 0 and x[0] <= 12 and x[1] <= 4 and LBChess._tie[
            x[0], x[1]] == 1  # 是否在铁路线上
        if not tie(pos):
            return result
        if abs(self.data[pos[0], pos[1]]) == GONG:
            result += self._get_gong(pos)
        else:
            pos = np.array(pos)
            direct = [(0, 1), (1, 0), (0, -1), (-1, 0)]
            direct = [np.array(i) for i in direct]
            for i in range(4):
                new_pos = pos + direct[i]
                if tie(new_pos) and self.data[new_pos[0], new_pos[1]] == BLANK:
                    while True:
                        new_pos += direct[i]
                        if not tie(new_pos):  # 非铁路坐标
                            break
                        if self.data[new_pos[0], new_pos[1]] * self.data[pos[0], pos[1]] > 0:  # 同色遮挡
                            break
                        result.append(tuple(new_pos))
                        if self.data[new_pos[0], new_pos[1]] * self.data[pos[0], pos[1]] < 0:  # 攻击
                            break
        return list(set(result))

    _tie = np.array([  # railway
        [0, 0, 0, 0, 0],
        [1, 1, 1, 1, 1],
        [1, 0, 0, 0, 1],
        [1, 0, 0, 0, 1],
        [1, 0, 0, 0, 1],
        [1, 1, 1, 1, 1],
        [1, 0, 1, 0, 1],
        [1, 1, 1, 1, 1],
        [1, 0, 0, 0, 1],
        [1, 0, 0, 0, 1],
        [1, 0, 0, 0, 1],
        [1, 1, 1, 1, 1],
        [0, 0, 0, 0, 0]
    ])

    def _get_gong(self, pos):
        """
        计算工兵的可达地点
        :param pos: 工兵的坐标，并检查该处是否有工兵
        :return: [(r1,c1),(r2,c2)...]
        """
        pos = tuple(pos)
        if LBChess._tie[pos[0], pos[1]] == 0 or abs(self.data[pos[0], pos[1]]) != GONG:  # 非铁路线或者没有工兵则返回
            return []
        r = 1 if self.data[pos[0], pos[1]] > 0 else -1
        temp = LBChess._tie.copy()
        temp[self.data * r < 0] = 2  # 敌人置为2
        temp[self.data * r > 0] = -1  # 自己人置为-1
        temp[LBChess._tie == 0] = 0
        result = []
        queue = []
        queue.append(pos)
        while len(queue) > 0:
            current = queue.pop(0)
            result.append(current)
            temp[current[0], current[1]] = -1  # 标记已访问过
            if current[0] + 1 < 13:
                if temp[current[0] + 1, current[1]] == 1:
                    queue.append((current[0] + 1, current[1]))
                elif temp[current[0] + 1, current[1]] == 2:
                    result.append((current[0] + 1, current[1]))
            if current[1] + 1 < 5:
                if temp[current[0], current[1] + 1] == 1:
                    queue.append((current[0], current[1] + 1))
                elif temp[current[0], current[1] + 1] == 2:
                    result.append((current[0], current[1] + 1))
            if current[0] - 1 >= 0:
                if temp[current[0] - 1, current[1]] == 1:
                    queue.append((current[0] - 1, current[1]))
                elif temp[current[0] - 1, current[1]] == 2:
                    result.append((current[0] - 1, current[1]))
            if current[1] - 1 >= 0:
                if temp[current[0], current[1] - 1] == 1:
                    queue.append((current[0], current[1] - 1))
                elif temp[current[0], current[1] - 1] == 2:
                    result.append((current[0], current[1] - 1))
        result.pop(0)
        return result
